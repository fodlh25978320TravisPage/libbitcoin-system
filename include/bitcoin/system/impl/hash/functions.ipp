/**
 * Copyright (c) 2011-2022 libbitcoin developers (see AUTHORS)
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
#ifndef LIBBITCOIN_SYSTEM_HASH_HASH_IPP
#define LIBBITCOIN_SYSTEM_HASH_HASH_IPP

#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/hash/accumulator.hpp>
#include <bitcoin/system/hash/algorithms.hpp>
#include <bitcoin/system/hash/scrypt.hpp>
#include <bitcoin/system/math/math.hpp>

namespace libbitcoin {
namespace system {
    
// Hash table keying.
// ----------------------------------------------------------------------------

// Value will vary with sizeof(size_t).
// A DJB variation uses [^ byte] vs. [+ byte].
// Objectives: deterministic, uniform distribution, efficient computation.
INLINE constexpr size_t djb2_hash(const data_slice& data) NOEXCEPT
{
    // Nothing special here except that it tested well against collisions.
    auto hash = 5381_size;

    // Efficient sum of ((hash * 33) + byte) for all bytes.
    for (const auto byte: data)
        hash = (shift_left(hash, 5_size) + hash) + byte;

    return hash;
}

// Combine hash values, such as a pair of djb2_hash outputs.
INLINE constexpr size_t hash_combine(size_t left, size_t right) NOEXCEPT
{
    return left ^ shift_left(right, one);
}

// Generalized cryptographic hash functions.
// ----------------------------------------------------------------------------

// ripemd128 (historical).
INLINE half_hash ripemd128_hash(const data_slice& data) NOEXCEPT
{
    return accumulator<rmd128>::hash_digest(data);
}
INLINE data_chunk ripemd128_chunk(const data_slice& data) NOEXCEPT
{
    return accumulator<rmd128>::hash_chunk(data);
}

// ripemd160 [script].
template <size_t Size>
INLINE short_hash ripemd160_hash(const data_array<Size>& data) NOEXCEPT
{
    return accumulator<rmd160>::hash(data);
}
INLINE short_hash ripemd160_hash(const data_chunk& data) NOEXCEPT
{
    return accumulator<rmd160>::hash(data);
}
////INLINE short_hash ripemd160_hash_slice(const data_slice& data) NOEXCEPT
////{
////    return accumulator<rmd160>::hash_digest(data);
////}
template <size_t Size>
INLINE data_chunk ripemd160_chunk(const data_array<Size>& data) NOEXCEPT
{
    return accumulator<rmd160>::hash_chunk(data);
}
INLINE data_chunk ripemd160_chunk(const data_chunk& data) NOEXCEPT
{
    return accumulator<rmd160>::hash_chunk(data);
}
////INLINE data_chunk ripemd160_chunk_slice(const data_slice& data) NOEXCEPT
////{
////    return accumulator<rmd160>::hash_chunk(data);
////}

// sha1 (sha160) [script].
template <size_t Size>
INLINE short_hash sha1_hash(const data_array<Size>& data) NOEXCEPT
{
    return accumulator<sha160>::hash(data);
}
INLINE short_hash sha1_hash(const data_chunk& data) NOEXCEPT
{
    return accumulator<sha160>::hash(data);
}
////INLINE short_hash sha1_hash_slice(const data_slice& data) NOEXCEPT
////{
////    return accumulator<sha160>::hash_digest(data);
////}
template <size_t Size>
INLINE data_chunk sha1_chunk(const data_array<Size>& data) NOEXCEPT
{
    return accumulator<sha160>::hash_chunk(data);
}
INLINE data_chunk sha1_chunk(const data_chunk& data) NOEXCEPT
{
    return accumulator<sha160>::hash_chunk(data);
}
////INLINE data_chunk sha1_chunk_slice(const data_slice& data) NOEXCEPT
////{
////    return accumulator<sha160>::hash_chunk(data);
////}

// sha256 [script, chain].
template <size_t Size>
INLINE hash_digest sha256_hash(const data_array<Size>& data) NOEXCEPT
{
    return accumulator<sha256>::hash(data);
}
INLINE hash_digest sha256_hash(const data_chunk& data) NOEXCEPT
{
    return accumulator<sha256>::hash(data);
}
INLINE hash_digest sha256_hash_slice(const data_slice& data) NOEXCEPT
{
    return accumulator<sha256>::hash_digest(data);
}
template <size_t Size>
INLINE data_chunk sha256_chunk(const data_array<Size>& data) NOEXCEPT
{
    return accumulator<sha256>::hash_chunk(data);
}
INLINE data_chunk sha256_chunk(const data_chunk& data) NOEXCEPT
{
    return accumulator<sha256>::hash_chunk(data);
}
INLINE data_chunk sha256_chunk_slice(const data_slice& data) NOEXCEPT
{
    return accumulator<sha256>::hash_chunk(data);
}

// sha512 (wallet).
INLINE long_hash sha512_hash(const data_slice& data) NOEXCEPT
{
    return accumulator<sha512>::hash_digest(data);
}
INLINE data_chunk sha512_chunk(const data_slice& data) NOEXCEPT
{
    return accumulator<sha512>::hash_chunk(data);
}

// Specialzied bitcoin cryptographic hash functions.
// ----------------------------------------------------------------------------

// Bitcoin short hash (ripemd160(sha256)) [script].
template <size_t Size>
INLINE short_hash bitcoin_short_hash(const data_array<Size>& data) NOEXCEPT
{
    return rmd160::hash(accumulator<sha256>::hash(data));
}
INLINE short_hash bitcoin_short_hash(const data_chunk& data) NOEXCEPT
{
    return rmd160::hash(accumulator<sha256>::hash(data));
}
////INLINE short_hash bitcoin_short_hash_slice(const data_slice& data) NOEXCEPT
////{
////    return rmd160::hash(accumulator<sha256>::hash_digest(data));
////}
template <size_t Size>
INLINE data_chunk bitcoin_short_chunk(const data_array<Size>& data) NOEXCEPT
{
    return accumulator<rmd160>::hash_chunk(accumulator<sha256>::hash(data));
}
INLINE data_chunk bitcoin_short_chunk(const data_chunk& data) NOEXCEPT
{
    return accumulator<rmd160>::hash_chunk(accumulator<sha256>::hash(data));
}
////INLINE data_chunk bitcoin_short_chunk_slice(const data_slice& data) NOEXCEPT
////{
////    return accumulator<rmd160>::hash_chunk(accumulator<sha256>::hash_digest(data));
////}

// Bitcoin hash (sha256(sha256)) [script, chain].
template <size_t Size>
INLINE hash_digest bitcoin_hash(const data_array<Size>& data) NOEXCEPT
{
    return sha256::hash(accumulator<sha256>::hash(data));
}
INLINE hash_digest bitcoin_hash(const data_chunk& data) NOEXCEPT
{
    return sha256::hash(accumulator<sha256>::hash(data));
}
INLINE hash_digest bitcoin_hash_slice(const data_slice& data) NOEXCEPT
{
    return sha256::hash(accumulator<sha256>::hash_digest(data));
}
template <size_t Size>
INLINE data_chunk bitcoin_chunk(const data_array<Size>& data) NOEXCEPT
{
    return accumulator<sha256>::hash_chunk(accumulator<sha256>::hash(data));
}
INLINE data_chunk bitcoin_chunk(const data_chunk& data) NOEXCEPT
{
    return accumulator<sha256>::hash_chunk(accumulator<sha256>::hash(data));
}
INLINE data_chunk bitcoin_chunk_slice(const data_slice& data) NOEXCEPT
{
    return accumulator<sha256>::hash_chunk(accumulator<sha256>::hash_digest(data));
}

// Bitcoin hash of concatenate(left, right) [witness commitment].
INLINE hash_digest bitcoin_hash(const hash_digest& left,
    const hash_digest& right) NOEXCEPT
{
    // Two assignments bypass more costly accumulator with same assignments.
    long_hash block{};
    array_cast<uint8_t, hash_size>(block) = left;
    array_cast<uint8_t, hash_size, hash_size>(block) = right;
    return sha256::hash(sha256::hash(block));
}

// Bitcoin hash set from an ordered set of ptrs [header commitment].
INLINE hashes bitcoin_hash(std_vector<uint8_t*>&&) NOEXCEPT
{
    // TODO: create std_vector<uint8_t*> parse_block(data_chunk&).
    // TODO: these are not arrays (arbitrary size) and vector cannot be cast.
    // TODO: holding an offset (index) would work, but requires base pointer.
    // TODO: these could be cast into a set of differently sized byte arrays, but
    // TODO: a vector cannot hold distinct types (though could hold arrays union).
    // TODO: cannot use shared pointer because data is unowned, so pointer it is.
    // TODO: requires specialized skip-parsing block reader to isolate txs.
    // TODO: merkle_root(bitcoin_hash(parse_block(data_chunk&))).
    // TODO: implement in sha::algorithm.
    // TODO: each element represents set of independent sha blocks/remainders.
    // TODO: concurrently hash blocks/remainders across set.
    // TODO: thread parallelize upon vectorize/sha-ni (as in merkle hashing).
    // TODO: individual tx hashing performed concurrently with tx parse.
    // TODO: block merkle root performed concurrently with block.txs parse.
    // TODO: these are two independent jobs both requiring the raw tx/s data.
    return {};
}

// Merkle root from a bitcoin_hash set [header commitment].
INLINE hash_digest merkle_root(hashes&& set) NOEXCEPT
{

    constexpr auto digest_size = array_count<sha256::digest_t>;
    constexpr auto block_size = array_count<sha256::block_t>;

    // TODO: move to sha::algorithm and optimize.
    const auto merkle_hash = [](uint8_t* out, size_t blocks,
        const uint8_t* in) NOEXCEPT
    {
        // TODO: this loop is inherently unordered (independent hash sets).
        while (to_bool(blocks--))
        {
            auto& to = unsafe_array_cast<uint8_t, digest_size>(out);
            auto& from = unsafe_array_cast<uint8_t, block_size>(in);
            to = sha256::double_hash(from);
            std::advance(out, hash_size);
            std::advance(in, long_hash_size);
        }
    };

    if (set.empty())
        return {};

    if (is_one(set.size()))
        return std::move(set.front());

    if (is_odd(set.size()))
        set.push_back(set.back());

    // TODO: move to sha::algorithm and optimize.
    // TODO: this loop is inherently ordered (dependent hash sets).
    while (is_even(set.size()))
    {
        // N blocks in, N/2 hashes out.
        const auto half = to_half(set.size());

        merkle_hash(
            pointer_cast<uint8_t>(set.data()), half,
            pointer_cast<const uint8_t>(set.data()));
        set.resize(half);
    }

    return std::move(set.front());
}

// sha256 hash of concatenate(left, right) [electrum seed stretch].
INLINE hash_digest sha256_hash(const data_slice& left,
    const data_slice& right) NOEXCEPT
{
    accumulator<sha256> context{};
    context.write(left.size(), left.data());
    context.write(right.size(), right.data());
    return context.flush();
}

// Litecoin scrypt hash [pow].
INLINE hash_digest scrypt_hash(const data_slice& data) NOEXCEPT
{
    // Litecoin parameters, with concurrency enabled.
    return scrypt<1024, 1, 1, true>::hash<hash_size>(data, data);
}

} // namespace system
} // namespace libbitcoin

#endif
