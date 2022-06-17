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
#ifndef LIBBITCOIN_SYSTEM_SERIAL_ENDIAN_IPP
#define LIBBITCOIN_SYSTEM_SERIAL_ENDIAN_IPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <boost/range/adaptor/reversed.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/math/math.hpp>

#define BUFFERED_STREAM

namespace libbitcoin {
namespace system {

// These conversions are efficient, performing no buffer copies or reversals.
// array/uintx_t sizes are inferred by type, and vector/uintx by value.
// High order bits are padded when read (from) is insufficient.
// High order bits are ignored when write (to) is insufficient.
// uintx return can handle data of arbitrary size.
// Limit sizeof to integral integers.

// stream <=> integral
// ----------------------------------------------------------------------------

#ifdef BUFFERED_STREAM

template <typename Integer, if_integral_integer<Integer>>
Integer from_big_endian(std::istream& stream) noexcept
{
    std::vector<char> buffer(sizeof(Integer));
    stream.read(buffer.data(), sizeof(Integer));
    buffer.resize(possible_narrow_and_sign_cast<size_t>(stream.gcount()));
    return from_big_endian<Integer>(buffer);
}

template <typename Integer, if_integral_integer<Integer>>
Integer from_little_endian(std::istream& stream) noexcept
{
    std::vector<char> buffer(sizeof(Integer));
    stream.read(buffer.data(), sizeof(Integer));
    buffer.resize(possible_narrow_and_sign_cast<size_t>(stream.gcount()));
    return from_little_endian<Integer>(buffer);
}

template <typename Integer, if_integral_integer<Integer>>
void to_big_endian(std::ostream& stream, Integer value) noexcept
{
    stream.write(reinterpret_cast<const char*>(
        to_big_endian(value).data()), sizeof(Integer));
}

template <typename Integer, if_integral_integer<Integer>>
void to_little_endian(std::ostream& stream, Integer value) noexcept
{
    stream.write(reinterpret_cast<const char*>(
        to_little_endian(value).data()), sizeof(Integer));
}

#else // BUFFERED_STREAM

template <typename Integer, if_integral_integer<Integer>>
inline Integer from_big_endian(std::istream& stream) noexcept
{
    Integer value(0);
    constexpr auto bytes = sizeof(Integer);

    // TODO: flatten loop using if constexpr (or rely on compiler to do it).
    for (size_t byte = 0; byte < bytes; ++byte)
    {
        value <<= byte_bits;

        BC_PUSH_WARNING(NO_THROW_IN_NOEXCEPT)
        value |= possible_narrow_and_sign_cast<Integer>(stream.get());
        BC_POP_WARNING()
    }

    // Stream invalidated if read past end (and Integer is unspecified).
    return value;
}

template <typename Integer, if_integral_integer<Integer>>
inline Integer from_little_endian(std::istream& stream) noexcept
{
    // TODO: byte_swap_intrinsic (when fully implemented).
    // Stream invalidated if read past end (and Integer is unspecified).
    return byteswap(from_big_endian<Integer>(stream));
}

template <typename Integer, if_integral_integer<Integer>>
inline void to_big_endian(std::ostream& stream, Integer value) noexcept
{
    constexpr auto bytes = sizeof(Integer);

    // TODO: flatten loop using if constexpr (or rely on compiler to do it).
    for (size_t byte = 0; byte < bytes; ++byte)
    {
        BC_PUSH_WARNING(NO_THROW_IN_NOEXCEPT)
        stream.put(possible_sign_narrow_cast<char>(value));
        BC_POP_WARNING()

        value >>= byte_bits;
    }
}

template <typename Integer, if_integral_integer<Integer>>
inline void to_little_endian(std::ostream& stream, Integer value) noexcept
{
    // TODO: byte_swap_intrinsic (when fully implemented).
    to_big_endian(stream, byteswap(value));
}

#endif // BUFFERED_STREAM

// byte convertors
// ----------------------------------------------------------------------------
// C++ standard: "The [shift] behavior is undefined if ... greater than or
// equal to the width of the promoted left operand." So we must be careful to
// not shift bytes by byte_bits (8). For this reason we specialize on byte-size
// integers. This also presents a performance optimization for byte conversion,
// which is close to a no-op. Empty vector returns zero and zero returns empty.

template <typename Integer, if_byte<Integer> = true>
constexpr Integer from_big(size_t, const data_slice& data) noexcept
{
    return data.empty() ? 0 : data.front();
}

template <typename Integer, if_byte<Integer> = true>
constexpr Integer from_little(size_t, const data_slice& data) noexcept
{
    return data.empty() ? 0 : data.front();
}

template <typename Data, typename Integer, if_byte<Integer> = true>
constexpr Data to_big(Data&& bytes, Integer value) noexcept
{
    if (!bytes.empty())
        bytes.front() = possible_sign_cast<uint8_t>(value);

    return std::move(bytes);
}

template <typename Data, typename Integer, if_byte<Integer> = true>
constexpr Data to_little(Data&& bytes, Integer value) noexcept
{
    if (!bytes.empty())
        bytes.front() = possible_sign_cast<uint8_t>(value);

    return std::move(bytes);
}

// integer convertors
// ----------------------------------------------------------------------------
// C++ standard: "Right-shift on signed integral types is an arithmetic right
// shift, which performs sign-extension". In other words, repeatedly shifting
// -1 of any integer width will produce "1" bits, indefinitely.

template <typename Integer, if_bytes<Integer> = true>
constexpr Integer from_big(size_t size, const data_slice& data) noexcept
{
    // read msb (forward), shift in the byte (no shift on first)
    // data[0] is most significant
    // { 0x01, 0x02 } => 0x0102
    // 0x0000 << 8 => 0x0000
    // 0x0001 << 8 => 0x0100
    // 0x0000 |= 0x01[0] => 0x0001
    // 0x0100 |= 0x02[1] => 0x0102

    Integer value(0);
    const auto bytes = std::min(size, data.size());

    // TODO: flatten loop using if constexpr (or rely on compiler to do it).
    for (size_t byte = 0; byte < bytes; ++byte)
    {
        value <<= byte_bits;

        BC_PUSH_WARNING(USE_GSL_AT)
        value |= possible_sign_cast<Integer>(data[byte]);
        BC_POP_WARNING()
    }

    return value;
}

template <typename Integer, if_bytes<Integer> = true>
constexpr Integer from_little(size_t size, const data_slice& data) noexcept
{
    // read msb (reverse), shift in the byte (no shift on first)
    // data[0] is least significant
    // { 0x01, 0x02 } => 0x0201
    // 0x0000 << 8 => 0x0000
    // 0x0002 << 8 => 0x0200
    // 0x0000 |= 0x02[1] => 0x0002
    // 0x0200 |= 0x01[0] => 0x0201

    Integer value(0);
    const auto bytes = std::min(size, data.size());

    // TODO: flatten loop using if constexpr (or rely on compiler to do it).
    for (auto byte = bytes; byte > 0; --byte)
    {
        value <<= byte_bits;

        BC_PUSH_WARNING(USE_GSL_AT)
        value |= possible_sign_cast<Integer>(data[sub1(byte)]);
        BC_POP_WARNING()
    }

    return value;
}

template <typename Data, typename Integer, if_bytes<Integer> = true>
constexpr Data to_big(Data&& bytes, Integer value) noexcept
{
    // read and shift out lsb, set byte in reverse order
    // data[0] is most significant
    // 0x0102 => { 0x01, 0x02 }
    // (uint8_t)0x0102 => 0x02[1]
    // (uint8_t)0x0001 => 0x01[0]
    // 0x0102 >> 8 => 0x0001
    // 0x0001 >> 8 => 0x0000

    // TODO: flatten loop using if constexpr (or rely on compiler to do it).
    // std::ranges::reverse_view does not supprt std::array.
    BC_PUSH_WARNING(NO_THROW_IN_NOEXCEPT)
    for (auto& byte: boost::adaptors::reverse(bytes))
    BC_POP_WARNING()
    {
        byte = possible_sign_narrow_cast<uint8_t>(value);
        value >>= byte_bits;
    }

    return std::move(bytes);
}

template <typename Data, typename Integer, if_bytes<Integer> = true>
constexpr Data to_little(Data&& bytes, Integer value) noexcept
{
    // read and shift out lsb, set byte in forward order
    // data[0] is least significant
    // 0x0102 => { 0x02, 0x01 }
    // (uint8_t)0x0102 => 0x02[0]
    // (uint8_t)0x0001 => 0x01[1]
    // 0x0102 >> 8 => 0x0001
    // 0x0001 >> 8 => 0x0000

    // TODO: flatten loop using if constexpr (or rely on compiler to do it).
    for (auto& byte: bytes)
    {
        byte = possible_sign_narrow_cast<uint8_t>(value);
        value >>= byte_bits;
    }

    return std::move(bytes);
}

// integer => data (value-sized) or array (explicit size)
// ----------------------------------------------------------------------------
// These allocate the to-endian outgoing buffer and forward the call.

template <size_t Size, typename Integer, if_integer<Integer>>
constexpr data_array<Size> to_big_endian_array(Integer value) noexcept
{
    return to_big(data_array<Size>{}, value);
}

template <size_t Size, typename Integer, if_integer<Integer>>
constexpr data_array<Size> to_little_endian_array(Integer value) noexcept
{
    return to_little(data_array<Size>{}, value);
}

template <typename Integer, if_integer<Integer>>
constexpr data_chunk to_big_endian_chunk(Integer value, size_t excess) noexcept
{
    // Zero value returns empty chunk.
    const auto size = byte_width(value);
    data_chunk chunk(size + excess, no_fill_byte_allocator);

    // Vector capacity is never reduced when resizing to smaller size.
    chunk.resize(size);
    return to_big(std::move(chunk), value);
}

template <typename Integer, if_integer<Integer>>
constexpr data_chunk to_little_endian_chunk(Integer value, size_t excess) noexcept
{
    // Zero value returns empty chunk.
    const auto size = byte_width(value);
    data_chunk chunk(size + excess, no_fill_byte_allocator);

    // Vector capacity is never reduced when resizing to smaller size.
    chunk.resize(size);
    return to_little(std::move(chunk), value);
}

// data => integral, integral => byte_array
// ----------------------------------------------------------------------------

template <typename Integer, if_integral_integer<Integer>>
constexpr Integer from_big_endian(const data_slice& data) noexcept
{
    return from_big<Integer>(sizeof(Integer), data);
}

template <typename Integer, if_integral_integer<Integer>>
constexpr Integer from_little_endian(const data_slice& data) noexcept
{
    return from_little<Integer>(sizeof(Integer), data);
}

template <typename Integer, if_integral_integer<Integer>>
constexpr data_array<sizeof(Integer)> to_big_endian(Integer value) noexcept
{
    return to_big_endian_array<sizeof(Integer)>(value);
}

template <typename Integer, if_integral_integer<Integer>>
constexpr data_array<sizeof(Integer)> to_little_endian(Integer value) noexcept
{
    return to_little_endian_array<sizeof(Integer)>(value);
}

// data => uintx
// uintx => data_chunk
// ----------------------------------------------------------------------------
// Overload for uintx, as from_big_endian<0> reads zero bytes and uintx is a
// signed type (though otherwise would be declared as uintx_t<0>).
// data_chunk size determined by uintx (vs. type).

template <typename Integer, if_base_of<Integer, uintx>>
constexpr Integer from_big_endian(const data_slice& data) noexcept
{
    return from_big<uintx>(data.size(), data);
}

template <typename Integer, if_base_of<Integer, uintx>>
constexpr  Integer from_little_endian(const data_slice& data) noexcept
{
    return from_little<uintx>(data.size(), data);
}

template <typename Integer, if_base_of<Integer, uintx>>
constexpr data_chunk to_big_endian(const Integer& value) noexcept
{
    return to_big_endian_chunk(value);
}

template <typename Integer, if_base_of<Integer, uintx>>
constexpr data_chunk to_little_endian(const Integer& value) noexcept
{
    return to_little_endian_chunk(value);
}

// data => uintx_t<to_bits(Bytes)>
// uintx_t (or other integer) => data_array
// ----------------------------------------------------------------------------

template <size_t Bytes>
constexpr uintx_t<to_bits(Bytes)> from_big_endian(const data_slice& data) noexcept
{
    return from_big<uintx_t<to_bits(Bytes)>>(Bytes, data);
}

template <size_t Bytes>
constexpr uintx_t<to_bits(Bytes)> from_little_endian(const data_slice& data) noexcept
{
    return from_little<uintx_t<to_bits(Bytes)>>(Bytes, data);
}

template <size_t Bytes, typename Integer, if_integer<Integer>>
constexpr data_array<Bytes> to_big_endian(const Integer& value) noexcept
{
    return to_big_endian_array<Bytes>(value);
}

template <size_t Bytes, typename Integer, if_integer<Integer>>
constexpr data_array<Bytes> to_little_endian(const Integer& value) noexcept
{
    return to_little_endian_array<Bytes>(value);
}
// iterator => integral
// ----------------------------------------------------------------------------
// size is unguarded.

template <typename Integer, typename Iterator, if_integral_integer<Integer>>
constexpr Integer from_big_endian_unchecked(const Iterator& data) noexcept
{
    return from_big_endian<Integer>({ data, std::next(data, sizeof(Integer)) });
}

template <typename Integer, typename Iterator, if_integral_integer<Integer>>
constexpr Integer from_little_endian_unchecked(const Iterator& data) noexcept
{
    return from_little_endian<Integer>({ data, std::next(data, sizeof(Integer)) });
}

// data[] <=> integral[]
/// ---------------------------------------------------------------------------
// Hack: pointer_cast fools transform into accepting a C-style array. This is
// safe because std::array must have only the C-style array non-static member.

template <size_t Count, typename Integer, if_integral_integer<Integer>>
constexpr void from_big_endian(Integer to[Count],
    const uint8_t from[Count * sizeof(Integer)]) noexcept
{
    typedef data_array<sizeof(Integer)> data;
    const auto in = pointer_cast<const data>(from);

    /// C++17: Parallel policy for std::transform.
    std::transform(in, std::next(in, Count), to, [](const data& value) noexcept
    {
        return from_big_endian<Integer>(value);
    });
}

template <size_t Count, typename Integer, if_integral_integer<Integer>>
constexpr void from_little_endian(Integer to[Count],
    const uint8_t from[Count * sizeof(Integer)]) noexcept
{
    typedef data_array<sizeof(Integer)> data;
    const auto in = pointer_cast<const data>(from);

    /// C++17: Parallel policy for std::transform.
    std::transform(in, std::next(in, Count), to, [](const data& value) noexcept
    {
        return from_little_endian<Integer>(value);
    });
}

template <size_t Count, typename Integer, if_integral_integer<Integer>>
constexpr void to_big_endian(uint8_t to[Count * sizeof(Integer)],
    const Integer from[Count]) noexcept
{
    typedef data_array<sizeof(Integer)> data;
    const auto out = pointer_cast<data>(to);

    /// C++17: Parallel policy for std::transform.
    std::transform(from, std::next(from, Count), out, [](Integer value) noexcept
    {
        return to_big_endian<Integer>(value);
    });
}

template <size_t Count, typename Integer, if_integral_integer<Integer>>
constexpr void to_little_endian(uint8_t to[Count * sizeof(Integer)],
    const Integer from[Count]) noexcept
{
    typedef data_array<sizeof(Integer)> data;
    const auto out = pointer_cast<data>(to);

    /// C++17: Parallel policy for std::transform.
    std::transform(from, std::next(from, Count), out, [](Integer value) noexcept
    {
        return to_little_endian<Integer>(value);
    });
}

} // namespace system
} // namespace libbitcoin

#endif
