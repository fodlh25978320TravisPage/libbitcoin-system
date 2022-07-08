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
#ifndef LIBBITCOIN_SYSTEM_MATH_BYTES_IPP
#define LIBBITCOIN_SYSTEM_MATH_BYTES_IPP

#include <bitcoin/system/define.hpp>
#include <bitcoin/system/math/cast.hpp>
#include <bitcoin/system/math/external/byte_swap.hpp>
#include <bitcoin/system/math/logarithm.hpp>
#include <bitcoin/system/math/sign.hpp>

namespace libbitcoin {
namespace system {

// Byte widths.
// ----------------------------------------------------------------------------
// See also std::bit_width (C++20).

// Called by chain::compact (for validation).
template <typename Integral, if_unsigned_integer<Integral>>
constexpr size_t byte_width(Integral value) NOEXCEPT
{
    // (zero-based position of msb) + 7 / 8.
    // (bit_width(value) + 7) / 8
    // (ceilinged_log2(value) + 7) / 8
    return ceilinged_log256(value);
}

// Called by machine::number (for little-endian chunk sizing).
template <typename Integral, if_signed_integer<Integral>>
constexpr size_t byte_width(Integral value) NOEXCEPT
{
    return is_negative(value) ? sizeof(Integral) :
        byte_width(to_unsigned(value));
}

// Endianness.
// ----------------------------------------------------------------------------

template <typename Integral, if_integral_integer<Integral>>
constexpr Integral native_to_big_end(Integral big) NOEXCEPT
{
    if constexpr (is_little_endian)
        return byteswap(big);
    else
        return big;
}

template <typename Integral, if_integral_integer<Integral>>
constexpr Integral native_to_little_end(Integral little) NOEXCEPT
{
    if constexpr (is_big_endian)
        return byteswap(little);
    else
        return little;
}

template <typename Integral, if_integral_integer<Integral>>
constexpr Integral native_from_big_end(Integral big) NOEXCEPT
{
    return native_to_big_end(big);
}

template <typename Integral, if_integral_integer<Integral>>
constexpr Integral native_from_little_end(Integral little) NOEXCEPT
{
    return native_to_little_end(little);
}

// Byteswap (platform independent byte reversal).
// ----------------------------------------------------------------------------
// If wrong overload is selected (such as for a literal) result is unexpected.
// C++23 std::byteswap does not support signed integrals.

template <typename Integral,
    if_integral_integer<Integral>,
    if_size_of<Integral, sizeof(uint8_t)>,
    if_unique_object_representations<Integral>>
constexpr Integral byteswap(Integral value) NOEXCEPT
{
    // no-op for calling consistency across all integral integer types.
    return value;
}

template <typename Integral,
    if_integral_integer<Integral>,
    if_size_of<Integral, sizeof(uint16_t)>,
    if_unique_object_representations<Integral>>
constexpr Integral byteswap(Integral value) NOEXCEPT
{
    // Compiles away to direct API call for non-constexpr.
    return std::is_constant_evaluated() ?
        possible_sign_cast<Integral>(byte_swap16_native(to_unsigned(value))) :
        possible_sign_cast<Integral>(byte_swap16(to_unsigned(value)));
}

template <typename Integral,
    if_integral_integer<Integral>,
    if_size_of<Integral, sizeof(uint32_t)>,
    if_unique_object_representations<Integral>>
constexpr Integral byteswap(Integral value) NOEXCEPT
{
    // Compiles away to direct API call for non-constexpr.
    return std::is_constant_evaluated() ?
        possible_sign_cast<Integral>(byte_swap32_native(to_unsigned(value))) :
        possible_sign_cast<Integral>(byte_swap32(to_unsigned(value)));
}

template <typename Integral,
    if_integral_integer<Integral>,
    if_size_of<Integral, sizeof(uint64_t)>,
    if_unique_object_representations<Integral>>
constexpr Integral byteswap(Integral value) NOEXCEPT
{
    // Compiles away to direct API call for non-constexpr.
    return std::is_constant_evaluated() ?
        possible_sign_cast<Integral>(byte_swap64_native(to_unsigned(value))) :
        possible_sign_cast<Integral>(byte_swap64(to_unsigned(value)));
}

// Bits to bytes utilities.
// ----------------------------------------------------------------------------

template <typename Integer, if_unsigned_integer<Integer>>
constexpr Integer to_ceilinged_bytes(Integer bits) NOEXCEPT
{
    return ceilinged_divide(bits, byte_bits);
}

template <typename Integer, if_unsigned_integer<Integer>>
constexpr Integer to_floored_bytes(Integer bits) NOEXCEPT
{
    return floored_divide(bits, byte_bits);
}

// Byte Negation.
// ----------------------------------------------------------------------------
// ****************************************************************************
// CONSENSUS: byte negation is consensus behavior, see machine::number.
// ****************************************************************************

template <typename Integer, if_integer<Integer>>
constexpr bool is_negated(Integer value) NOEXCEPT
{
    // Guard precludes zero appearing negated: ((0 % 8) + 1) == 1.
    return is_nonzero(value) && add1(bit_width(value) % byte_bits) == one;
};

template <typename Integer, if_integer<Integer>>
constexpr Integer to_negated(Integer value) NOEXCEPT
{
    return set_left(value);
}

template <typename Integer, if_signed_integer<Integer>>
constexpr Integer to_unnegated(Integer value) NOEXCEPT
{
    // negate(minimum) is undefined behavior, but precluded by clearing a bit.
    return !is_negated(value) ? value : negate(
        set_right(value, sub1(bit_width(value)), false));
}

} // namespace system
} // namespace libbitcoin

#endif
