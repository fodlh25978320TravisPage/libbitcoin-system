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
#include <bitcoin/system/crypto/hash.hpp>

#include <vector>
#include <bitcoin/system/crypto/aes256.hpp>
#include <bitcoin/system/crypto/hmac_sha256.hpp>
#include <bitcoin/system/crypto/pbkd_sha256.hpp>
#include <bitcoin/system/crypto/sha256.hpp>
#include <bitcoin/system/crypto/scrypt.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/math/math.hpp>

#include <bitcoin/system/crypto/external/hmac_sha512.hpp>
#include <bitcoin/system/crypto/external/pbkd_sha512.hpp>
#include <bitcoin/system/crypto/external/ripemd160.hpp>
#include <bitcoin/system/crypto/external/sha160.hpp>
#include <bitcoin/system/crypto/external/sha512.hpp>

namespace libbitcoin {
namespace system {

// Hash generators.
// ----------------------------------------------------------------------------

hash_digest bitcoin_hash(const data_slice& data) NOEXCEPT
{
    return sha256_hash(sha256_hash(data));
}

hash_digest bitcoin_hash(const data_slice& left,
    const data_slice& right) NOEXCEPT
{
    return sha256_hash(sha256_hash(left, right));
}

short_hash bitcoin_short_hash(const data_slice& data) NOEXCEPT
{
    return ripemd160_hash(sha256_hash(data));
}

short_hash ripemd160_hash(const data_slice& data) NOEXCEPT
{
    short_hash hash;
    RMD160(data.data(), data.size(), hash.data());
    return hash;
}

data_chunk ripemd160_chunk(const data_slice& data) NOEXCEPT
{
    data_chunk hash(short_hash_size, no_fill_byte_allocator);
    RMD160(data.data(), data.size(), hash.data());
    return hash;
}

short_hash sha1_hash(const data_slice& data) NOEXCEPT
{
    short_hash hash;
    SHA1(data.data(), data.size(), hash.data());
    return hash;
}

data_chunk sha1_chunk(const data_slice& data) NOEXCEPT
{
    data_chunk hash(short_hash_size, no_fill_byte_allocator);
    SHA1(data.data(), data.size(), hash.data());
    return hash;
}

// TODO: non-streaming hash optimizations:
// TODO: sha256 hashes of arrays can be optimized by computing the padding
// TODO: bytes (including count value) at compile time using constexpr.
// TODO: overloads for common sizes, such as 32 and 64 bytes, can do the same.
// TODO: this affects all double hashes (including that in the sha256x2 writer)
// TODO: as the second hash is always 32 bytes. See comments in sha256.cpp.

hash_digest sha256_hash(const data_slice& data) NOEXCEPT
{
    hash_digest hash;
    sha256::hash(hash.data(), data.size(), data.data());
    return hash;
}

data_chunk sha256_chunk(const data_slice& data) NOEXCEPT
{
    data_chunk hash(hash_size, no_fill_byte_allocator);
    sha256::hash(hash.data(), data.size(), data.data());
    return hash;
}

// This overload precludes the need to concatenate left + right.
hash_digest sha256_hash(const data_slice& left,
    const data_slice& right) NOEXCEPT
{
    hash_digest hash;
    sha256::context context;
    context.write(left.size(), left.data());
    context.write(right.size(), right.data());
    context.flush(hash.data());
    return hash;
}

hash_digest hmac_sha256(const data_slice& data,
    const data_slice& key) NOEXCEPT
{
    hash_digest hash;
    hmac::sha256::hash(data.data(), data.size(), key.data(), key.size(),
        hash.data());
    return hash;
}

data_chunk pbkd_sha256(const data_slice& passphrase,
    const data_slice& salt, size_t iterations, size_t length) NOEXCEPT
{
    data_chunk hash(length, no_fill_byte_allocator);
    pbkd::sha256::hash(passphrase.data(), passphrase.size(), salt.data(),
        salt.size(), iterations, hash.data(), length);
    return hash;
}

long_hash sha512_hash(const data_slice& data) NOEXCEPT
{
    long_hash hash;
    SHA512(data.data(), data.size(), hash.data());
    return hash;
}

long_hash hmac_sha512(const data_slice& data,
    const data_slice& key) NOEXCEPT
{
    long_hash hash;
    HMACSHA512(data.data(), data.size(), key.data(), key.size(), hash.data());
    return hash;
}

long_hash pbkd_sha512(const data_slice& passphrase,
    const data_slice& salt, size_t iterations) NOEXCEPT
{
    long_hash hash{};
    PBKDSHA512(passphrase.data(), passphrase.size(),
        salt.data(), salt.size(), hash.data(), hash.size(), iterations);
    return hash;
}

hash_digest scrypt_hash(const data_slice& data) NOEXCEPT
{
    return scrypt<1024, 1, 1, true>::hash<hash_size>(data, data);
}

// Objectives: deterministic, uniform distribution, efficient computation.
size_t djb2_hash(const data_slice& data) NOEXCEPT
{
    // Nothing special here except that it tested well against collisions.
    auto hash = 5381_size;

    // Efficient sum of ((hash * 33) + byte) for all bytes.
    for (const auto byte: data)
        hash = (shift_left(hash, 5_size) + hash) + byte;

    return hash;
}

} // namespace system
} // namespace libbitcoin
