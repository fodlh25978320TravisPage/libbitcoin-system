/**
 * Copyright (c) 2011-2025 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/system/chain/transaction.hpp>

#include <iterator>
#include <bitcoin/system/chain/enums/coverage.hpp>
#include <bitcoin/system/chain/input.hpp>
#include <bitcoin/system/chain/output.hpp>
#include <bitcoin/system/chain/script.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/endian/endian.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/error/error.hpp>
#include <bitcoin/system/hash/hash.hpp>
#include <bitcoin/system/math/math.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

BC_PUSH_WARNING(NO_THROW_IN_NOEXCEPT)

// All private methods pertaining to the validation of a signature.
// There are three versions of signature hashing and verification implemented.
// Version: (unversioned) original, (0) bip143/segwit, (1) bip341/taproot.

// Share fixed elements of signature hashing.
// ----------------------------------------------------------------------------

constexpr auto prefixed = true;

static const auto& null_output() NOEXCEPT
{
    static const auto null = output{}.to_data();
    return null;
}

static const auto& empty_script() NOEXCEPT
{
    static const auto empty = script{}.to_data(prefixed);
    return empty;
}

static const auto& zero_sequence() NOEXCEPT
{
    static const auto sequence = to_little_endian<uint32_t>(0);
    return sequence;
}

// Compute signature hash subcomponents.
// ----------------------------------------------------------------------------

hash_digest transaction::outputs_hash() const NOEXCEPT
{
    if (sighash_cache_)
        return sighash_cache_->outputs;

    hash_digest digest{};
    stream::out::fast stream{ digest };
    hash::sha256x2::fast sink{ stream };

    for (const auto& output: *outputs_)
        output->to_data(sink);

    sink.flush();
    return digest;
}

hash_digest transaction::points_hash() const NOEXCEPT
{
    if (sighash_cache_)
        return sighash_cache_->points;

    hash_digest digest{};
    stream::out::fast stream{ digest };
    hash::sha256x2::fast sink{ stream };

    for (const auto& input: *inputs_)
        input->point().to_data(sink);

    sink.flush();
    return digest;
}

hash_digest transaction::sequences_hash() const NOEXCEPT
{
    if (sighash_cache_)
        return sighash_cache_->sequences;

    hash_digest digest{};
    stream::out::fast stream{ digest };
    hash::sha256x2::fast sink{ stream };

    for (const auto& input: *inputs_)
        sink.write_4_bytes_little_endian(input->sequence());

    sink.flush();
    return digest;
}

// Signature hashing (unversioned).
// ----------------------------------------------------------------------------

transaction::input_iterator transaction::input_at(
    uint32_t index) const NOEXCEPT
{
    // Guarded by check_signature and create_endorsement.
    BC_ASSERT_MSG(index < inputs_->size(), "invalid input index");

    return std::next(inputs_->begin(), index);
}

uint32_t transaction::input_index(const input_iterator& input) const NOEXCEPT
{
    // Guarded by unversioned_signature_hash and output_hash.
    BC_ASSERT_MSG(inputs_->begin() != inputs_->end(), "invalid input iterator");

    return possible_narrow_and_sign_cast<uint32_t>(
        std::distance(inputs_->begin(), input));
}

// static
//*****************************************************************************
// CONSENSUS: Due to masking of bits 6/7 (8 is the anyone_can_pay flag),
// there are 4 possible 7 bit values that can set "single" and 4 others that
// can set none, and yet all other values set "all".
//*****************************************************************************
inline coverage transaction::mask_sighash(uint8_t sighash_flags) NOEXCEPT
{
    switch (sighash_flags & coverage::mask)
    {
        case coverage::hash_single:
            return coverage::hash_single;
        case coverage::hash_none:
            return coverage::hash_none;
        default:
            return coverage::hash_all;
    }
}

// ****************************************************************************
// CONSENSUS: sighash flags are carried in a single byte but are encoded as 4
// bytes in the signature hash preimage serialization.
// ****************************************************************************

void transaction::signature_hash_single(writer& sink,
    const input_iterator& input, const script& sub,
    uint8_t sighash_flags) const NOEXCEPT
{
    const auto write_inputs = [this, &input, &sub, sighash_flags](
        writer& sink) NOEXCEPT
    {
        const auto anyone = to_bool(sighash_flags & coverage::anyone_can_pay);
        input_cptrs::const_iterator in;

        sink.write_variable(anyone ? one : inputs_->size());

        for (in = inputs_->begin(); !anyone && in != input; ++in)
        {
            (*in)->point().to_data(sink);
            sink.write_bytes(empty_script());
            sink.write_bytes(zero_sequence());
        }

        (*input)->point().to_data(sink);
        sub.to_data(sink, prefixed);
        sink.write_4_bytes_little_endian((*input)->sequence());

        for (++in; !anyone && in != inputs_->end(); ++in)
        {
            (*in)->point().to_data(sink);
            sink.write_bytes(empty_script());
            sink.write_bytes(zero_sequence());
        }
    };

    const auto write_outputs = [this, &input](writer& sink) NOEXCEPT
    {
        const auto index = input_index(input);

        sink.write_variable(add1(index));

        for (size_t output = 0; output < index; ++output)
            sink.write_bytes(null_output());

        // Guarded by unversioned_signature_hash.
        outputs_->at(index)->to_data(sink);
    };

    sink.write_4_bytes_little_endian(version_);
    write_inputs(sink);
    write_outputs(sink);
    sink.write_4_bytes_little_endian(locktime_);
    sink.write_4_bytes_little_endian(sighash_flags);
}

void transaction::signature_hash_none(writer& sink,
    const input_iterator& input, const script& sub,
    uint8_t sighash_flags) const NOEXCEPT
{
    const auto write_inputs = [this, &input, &sub, sighash_flags](
        writer& sink) NOEXCEPT
    {
        const auto anyone = to_bool(sighash_flags & coverage::anyone_can_pay);
        input_cptrs::const_iterator in;

        sink.write_variable(anyone ? one : inputs_->size());

        for (in = inputs_->begin(); !anyone && in != input; ++in)
        {
            (*in)->point().to_data(sink);
            sink.write_bytes(empty_script());
            sink.write_bytes(zero_sequence());
        }

        (*input)->point().to_data(sink);
        sub.to_data(sink, prefixed);
        sink.write_4_bytes_little_endian((*input)->sequence());

        for (++in; !anyone && in != inputs_->end(); ++in)
        {
            (*in)->point().to_data(sink);
            sink.write_bytes(empty_script());
            sink.write_bytes(zero_sequence());
        }
    };

    sink.write_4_bytes_little_endian(version_);
    write_inputs(sink);
    sink.write_variable(zero);
    sink.write_4_bytes_little_endian(locktime_);
    sink.write_4_bytes_little_endian(sighash_flags);
}

void transaction::signature_hash_all(writer& sink,
    const input_iterator& input, const script& sub,
    uint8_t flags) const NOEXCEPT
{
    const auto write_inputs = [this, &input, &sub, flags](
        writer& sink) NOEXCEPT
    {
        const auto anyone = to_bool(flags & coverage::anyone_can_pay);
        input_cptrs::const_iterator in;

        sink.write_variable(anyone ? one : inputs_->size());

        for (in = inputs_->begin(); !anyone && in != input; ++in)
        {
            (*in)->point().to_data(sink);
            sink.write_bytes(empty_script());
            sink.write_4_bytes_little_endian((*in)->sequence());
        }

        (*input)->point().to_data(sink);
        sub.to_data(sink, prefixed);
        sink.write_4_bytes_little_endian((*input)->sequence());

        for (++in; !anyone && in != inputs_->end(); ++in)
        {
            (*in)->point().to_data(sink);
            sink.write_bytes(empty_script());
            sink.write_4_bytes_little_endian((*in)->sequence());
        }
    };

    const auto write_outputs = [this](writer& sink) NOEXCEPT
    {
        sink.write_variable(outputs_->size());
        for (const auto& output: *outputs_)
            output->to_data(sink);
    };

    sink.write_4_bytes_little_endian(version_);
    write_inputs(sink);
    write_outputs(sink);
    sink.write_4_bytes_little_endian(locktime_);
    sink.write_4_bytes_little_endian(flags);
}

// private
hash_digest transaction::unversioned_signature_hash(
    const input_iterator& input, const script& sub,
    uint8_t sighash_flags) const NOEXCEPT
{
    // Set options.
    const auto flag = mask_sighash(sighash_flags);

    // Create hash writer.
    hash_digest digest{};
    stream::out::fast stream{ digest };
    hash::sha256x2::fast sink{ stream };

    switch (flag)
    {
        case coverage::hash_single:
        {
            //*****************************************************************
            // CONSENSUS: return one_hash if index exceeds outputs in sighash.
            // Related Bug: bitcointalk.org/index.php?topic=260595
            // Exploit: joncave.co.uk/2014/08/bitcoin-sighash-single/
            //*****************************************************************
            if (input_index(input) >= outputs_->size())
                return one_hash;

            signature_hash_single(sink, input, sub, sighash_flags);
            break;
        }
        case coverage::hash_none:
        {
            signature_hash_none(sink, input, sub, sighash_flags);
            break;
        }
        default:
        case coverage::hash_all:
        {
            signature_hash_all(sink, input, sub, sighash_flags);
        }
    }

    sink.flush();
    return digest;
}

// Signature hashing (version 0 - segwit).
// ----------------------------------------------------------------------------

// TODO: taproot requires both single and double hash of each.
void transaction::initialize_sighash_cache() const NOEXCEPT
{
    if (!segregated_)
        return;

    // This overconstructs the cache (anyone or !all), however it is simple.
    sighash_cache_ =
    {
        outputs_hash(),
        points_hash(),
        sequences_hash()
    };
}

hash_digest transaction::output_hash(const input_iterator& input) const NOEXCEPT
{
    const auto index = input_index(input);

    //*************************************************************************
    // CONSENSUS: if index exceeds outputs in signature hash, return null_hash.
    //*************************************************************************
    if (index >= outputs_->size())
        return null_hash;

    hash_digest digest{};
    stream::out::fast stream{ digest };
    hash::sha256x2::fast sink{ stream };
    outputs_->at(index)->to_data(sink);
    sink.flush();
    return digest;
}

hash_digest transaction::version_0_signature_hash(const input_iterator& input,
    const script& sub, uint64_t value, uint8_t sighash_flags,
    bool bip143) const NOEXCEPT
{
    // bip143/v0: the way of serialization is changed.
    if (!bip143)
        return unversioned_signature_hash(input, sub, sighash_flags);

    // Set options.
    const auto anyone = to_bool(sighash_flags & coverage::anyone_can_pay);
    const auto flag = mask_sighash(sighash_flags);
    const auto all = (flag == coverage::hash_all);
    const auto single = (flag == coverage::hash_single);

    // Create hash writer.
    hash_digest digest{};
    stream::out::fast stream{ digest };
    hash::sha256x2::fast sink{ stream };

    // Create signature hash.
    sink.write_little_endian(version_);

    // Conditioning points, sequences, and outputs writes on cache_ instead of
    // conditionally passing them from methods avoids copying the cached hash.

    // points
    sink.write_bytes(!anyone ? points_hash() : null_hash);

    // sequences
    sink.write_bytes(!anyone && all ? sequences_hash() : null_hash);

    (*input)->point().to_data(sink);
    sub.to_data(sink, prefixed);
    sink.write_little_endian(value);
    sink.write_little_endian((*input)->sequence());

    // outputs
    if (single)
        sink.write_bytes(output_hash(input));
    else
        sink.write_bytes(all ? outputs_hash() : null_hash);

    sink.write_little_endian(locktime_);
    sink.write_4_bytes_little_endian(sighash_flags);

    sink.flush();
    return digest;
}

// Signature hashing (version 1 - taproot).
// ----------------------------------------------------------------------------

// TODO: tapscript.
// Because the codeseparator_position is the last input to the hash, the SHA256
// midstate can be efficiently cached for multiple OP_CODESEPARATOR in a script.

// static
// BIP341: Using any undefined hash_type causes validation failure if violated.
// defined types: 0x00, 0x01, 0x02, 0x03, 0x81, 0x82, or 0x83. [zero is the
// default and cannot be explicit, but is serialized for signature hashing.
inline bool transaction::is_sighash_valid(uint8_t sighash_flags) NOEXCEPT
{
    switch (sighash_flags)
    {
        case coverage::hash_default:
        case coverage::hash_all:
        case coverage::hash_none:
        case coverage::hash_single:
        case coverage::all_anyone_can_pay:
        case coverage::none_anyone_can_pay:
        case coverage::single_anyone_can_pay:
            return true;
        default:
            return false;
    }
}

} // namespace chain
} // namespace system
} // namespace libbitcoin
