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

#include <bitcoin/system/chain/enums/coverage.hpp>
#include <bitcoin/system/chain/enums/extension.hpp>
#include <bitcoin/system/chain/enums/key_version.hpp>
#include <bitcoin/system/chain/input.hpp>
#include <bitcoin/system/chain/output.hpp>
#include <bitcoin/system/chain/script.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/hash/hash.hpp>
#include <bitcoin/system/math/math.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

// Signature hashing (version 1 - taproot).
// ----------------------------------------------------------------------------

//*****************************************************************************
// CONSENSUS: if index exceeds outputs in signature hash, fail validation.
//*****************************************************************************
bool transaction::version1_output_hash(hash_digest& out,
    const input_iterator& input) const NOEXCEPT
{
    const auto index = input_index(input);
    if (index >= outputs_->size())
        return false;

    stream::out::fast stream{ out };
    hash::sha256::fast sink{ stream };
    outputs_->at(index)->to_data(sink);
    sink.flush();
    return true;
}

// ext_flags and annex flag are combined into one byte, who knows why.
uint8_t transaction::spend_type_v1(bool annex, bool tapscript) const NOEXCEPT
{
    const auto ext_flags = to_value(tapscript ?
        extension::tapscript : extension::taproot);

    return set_right(shift_left(ext_flags), zero, annex);
}

bool transaction::version1_sighash(hash_digest& out,
    const input_iterator& input, const script& script, uint64_t value,
    uint8_t sighash_flags) const NOEXCEPT
{
    // TODO: obtain.
    constexpr bool tapscript{};

    // Mask anyone_can_pay, and set hash_all by default.
    // sighash_flags previously verified (see schnorr_split).
    const auto flag = mask_sighash(sighash_flags);
    const auto anyone = is_anyone_can_pay(sighash_flags);
    const auto single = (flag == coverage::hash_single);
    const auto all = (flag == coverage::hash_all);

    const auto& in = **input;
    const auto& annex = in.witness().annex();

    // Create tagged hash writer.
    stream::out::fast stream{ out };
    hash::sha256t::fast<"TapSighash"> sink{ stream };

    sink.write_byte(0);
    sink.write_byte(sighash_flags);
    sink.write_4_bytes_little_endian(version_);
    sink.write_4_bytes_little_endian(locktime_);

    if (!anyone)
    {
        sink.write_bytes(single_hash_points());
        sink.write_bytes(single_hash_amounts());
        sink.write_bytes(single_hash_scripts());
        sink.write_bytes(single_hash_sequences());
    }

    if (all)
    {
        sink.write_bytes(single_hash_outputs());
    }

    sink.write_byte(spend_type_v1(annex, tapscript));

    if (anyone)
    {
        in.point().to_data(sink);
        sink.write_8_bytes_little_endian(value);
        script.to_data(sink, true);
        sink.write_4_bytes_little_endian(in.sequence());
    }
    else
    {
        sink.write_4_bytes_little_endian(input_index(input));
    }

    if (annex)
    {
        sink.write_variable(annex.size());
        sink.write_bytes(annex.hash());
    }

    if (single)
    {
        hash_digest hash_output{};
        if (!version1_output_hash(hash_output, input))
            return false;

        // TODO: measure this prevalence for caching benefit (rare).
        sink.write_bytes(hash_output);
    }

    // Additional for tapscript [bip342].
    if (tapscript)
    {
        // TODO: obtain.
        // The tapleaf hash as defined in BIP341.
        constexpr hash_digest tapleaf_hash{};

        // TODO: obtain.
        // The opcode position of last executed op_codeseparator before the
        // currently executing signature opcode. Because code_separator is the
        // last input to signature hash, the signature hash sha256 midstate
        // (above) can be cached for multiple op_codeseparators.
        constexpr uint32_t code_separator{ 0 };

        sink.write_bytes(tapleaf_hash);
        sink.write_byte(to_value(key_version::tapscript));
        sink.write_4_bytes_little_endian(code_separator);
    }

    // Total length at most 206 bytes (!anyone, no epoch/tapscript) [bip341].
    BC_ASSERT(!anyone && sink.get_write_position() == (add1(206u) + 37));
    BC_ASSERT( anyone && sink.get_write_position() == (add1(157u) + 37));

    sink.flush();
    return true;
}

} // namespace chain
} // namespace system
} // namespace libbitcoin
