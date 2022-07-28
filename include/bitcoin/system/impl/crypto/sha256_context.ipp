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
#ifndef LIBBITCOIN_SYSTEM_CRYPTO_SHA256_CONTEXT_IPP
#define LIBBITCOIN_SYSTEM_CRYPTO_SHA256_CONTEXT_IPP

#include <algorithm>
#include <bitcoin/system/crypto/sha256.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/endian/endian.hpp>
#include <bitcoin/system/math/math.hpp>

// Copy/array are guarded, buffer initialization is not required.
BC_PUSH_WARNING(NO_UNSAFE_COPY_N)
BC_PUSH_WARNING(NO_ARRAY_INDEXING)
BC_PUSH_WARNING(NO_DYNAMIC_ARRAY_INDEXING)
BC_PUSH_WARNING(NO_UNINITIALZIED_MEMBER)

namespace libbitcoin {
namespace system {
namespace sha256 {

constexpr context::context() NOEXCEPT
{
}

constexpr void context::clear() NOEXCEPT
{
    size_ = zero;
}

constexpr void context::reset() NOEXCEPT
{
    size_ = zero;
    bits_ = 0_u64;
    state_ = initial;
}

constexpr bool context::increment(size_t blocks) NOEXCEPT
{
    const auto bytes = blocks * block_size;
    if (is_buffer_overflow(bytes))
        return false;

    // This is always a no-op (zero).
    ////size_ += bytes % block_size;
    bits_ += to_bits(bytes);
    return true;
}

constexpr bool context::is_full() NOEXCEPT
{
    return is_zero(gap());
}

constexpr size_t context::pad_size() const NOEXCEPT
{
    BC_ASSERT_MSG(!is_limited(size_, block_size), "unexpected size");

    constexpr auto singled = block_size - counter_size;
    constexpr auto doubled = block_size + singled;
    return size_ < singled ? singled - size_ : doubled - size_;
}

constexpr size_t context::add_data(size_t bytes, const uint8_t* data) NOEXCEPT
{
    BC_ASSERT_MSG(!is_buffer_overflow(bytes), "hash function overflow");

    // No bytes accepted on overflow or uncleared buffer.
    if (is_buffer_overflow(bytes) || is_zero(gap()))
        return zero;

    const auto accepted = std::min(gap(), bytes);
    std::copy_n(data, accepted, &buffer_[size_]);
    size_ += accepted;
    bits_ += to_bits(accepted);
    return accepted;
}

constexpr sha256::state& context::state() NOEXCEPT
{
    return state_;
}

inline const sha256::block& context::buffer() const NOEXCEPT
{
    return buffer_;
}

inline context::counter context::serialize_counter() const NOEXCEPT
{
    return to_big_endian(bits_);
}

inline void context::serialize_state(digest& out) const NOEXCEPT
{
    to_big_endians(array_cast<uint32_t>(out), state_);
}

// private
constexpr size_t context::gap() const NOEXCEPT
{
    BC_ASSERT_MSG(!is_subtract_overflow(block_size, size_), "unexpected size");
    return block_size - size_;
}

// private
constexpr bool context::is_buffer_overflow(size_t bytes) NOEXCEPT
{
    // bits is always a multiple of 8 (set only by to_bits(accepted)).
    BC_ASSERT_MSG(is_zero(bits_ % byte_bits), "unexpected bit count");

    const auto used = to_floored_bytes(bits_);
    return is_add_overflow<uint64_t>(used, bytes) ||
        is_add_overflow<uint64_t>(counter_size, add(used, bytes));
}

// sha256::accumulator.write()
inline bool update(context& context, size_t size, const uint8_t* in) NOEXCEPT
{
    // Fill gap if possible and update counter.
    const auto accepted = context.add_data(size, in);

    // Buffer overflow, no bytes were accepted (or size was zero).
    // If accepted is non-zero then the full size value is acceptable.
    if (is_zero(accepted))
        return is_zero(size);

    // No transformation on this update unless buffer is full.
    if (!context.is_full())
        return true;

    // Transform and clear the buffer.
    transform(context.state(), one, context.buffer().data());
    std::advance(in, accepted);
    context.clear();

    // No more bytes to process.
    if (is_zero((size -= accepted)))
        return true;

    // Transform all whole blocks and save remainder to empty buffer.
    const auto blocks = size / block_size;
    const auto remain = size % block_size;

    transform(context.state(), blocks, in);
    std::advance(in, size - remain);
    context.increment(blocks);

    // Add the remaining partial block to the empty block buffer.
    context.add_data(remain, in);
    return true;
}

// sha256::accumulator.flush()
inline void finalize(context& context, uint8_t* out32) NOEXCEPT
{
    const auto counter = context.serialize_counter();

    update(context, context.pad_size(), pad_stream.data());
    update(context, context::counter_size, counter.data());
    context.serialize_state(unsafe_array_cast<uint8_t, digest_size>(out32));
}

} // namespace sha256
} // namespace system
} // namespace libbitcoin

BC_POP_WARNING()
BC_POP_WARNING()
BC_POP_WARNING()
BC_POP_WARNING()

#endif
