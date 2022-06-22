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
#ifndef LIBBITCOIN_SYSTEM_MATH_LIMITS_HPP
#define LIBBITCOIN_SYSTEM_MATH_LIMITS_HPP

#include <cstddef>
#include <bitcoin/system/constraints.hpp>

namespace libbitcoin {
namespace system {

/// The value is outside of the type bounds.
template <typename By, typename Integer,
    if_integer<By> = true, if_integer<Integer> = true>
constexpr bool is_limited(Integer value) noexcept;

/// The value exceeds the type bound or upper limit.
template <typename By, typename Integer,
    if_integer<By> = true, if_integer<Integer> = true>
constexpr bool is_limited(Integer value, By maximum) noexcept;

/// The value exceeds the lower or upper limit.
template <typename By, typename Integer,
    if_integer<By> = true, if_integer<Integer> = true>
constexpr bool is_limited(Integer value, By minimum, By maximum) noexcept;

/// Cast a value to Result, constrained to the limits of both types.
template <typename Result, typename Integer,
    if_integer<Result> = true, if_integer<Integer> = true>
constexpr Result limit(Integer value) noexcept;

/// Cast a value to Result, constrained to the type and upper limit.
/// Casting positive to/from negative may change the sign of the result.
template <typename Result, typename Integer,
    if_integer<Result> = true, if_integer<Integer> = true>
constexpr Result limit(Integer value, Result maximum) noexcept;

/// Cast a value to Result, constrained to the specified limits.
/// Casting positive to/from negative may change the sign of the result.
template <typename Result, typename Integer,
    if_integer<Result> = true, if_integer<Integer> = true>
constexpr Result limit(Integer value, Result minimum, Result maximum) noexcept;

/// The minimum value of a integral integer type (std::numeric_limits).
template <typename Integer, if_integral_integer<Integer> = true>
constexpr Integer minimum() noexcept;

/// The maximum value of a integral integer type (std::numeric_limits).
template <typename Integer, if_integral_integer<Integer> = true>
constexpr Integer maximum() noexcept;

/// The minimum value of a type by byte size (1-8 bytes).
template <size_t Bytes, typename Return = signed_type<Bytes>>
constexpr Return minimum() noexcept;

/// The maximum value of a type by byte size (1-8 bytes).
template <size_t Bytes, typename Return = signed_type<Bytes>>
constexpr Return maximum() noexcept;

/// Bitcoin serialization imposes the following domain constraint on integers.
/// The domains are constrained by one less negative value than C++ integrals.
/// This is a consequence of a valid encoding for both negative zero (e.g 0x80)
/// and positive zero (e.g. 0x00). This is a side effect of the use of variable
/// lenth serialization (sighash/stack) encoding of integers as byte vectors.
/// 1 byte :[-2^07+1...2^07-1]
/// 2 bytes:[-2^15+1...2^15-1]
/// 3 bytes:[-2^23+1...2^23-1]
/// 4 bytes:[-2^31+1...2^31-1]
/// 5 bytes:[-2^39+1...2^39-1]
/// 6 bytes:[-2^47+1...2^47-1]
/// 7 bytes:[-2^55+1...2^55-1]
/// 8 bytes:[-2^63+1...2^63-1]

/// The minimum value of a bitcoin integer by byte size (1-8 bytes).
template <size_t Bytes, typename Return = signed_type<Bytes>>
constexpr Return bitcoin_min() noexcept;

/// The maximum value of a bitcoin integer by byte size (1-8 bytes).
template <size_t Bytes, typename Return = signed_type<Bytes>>
constexpr Return bitcoin_max() noexcept;

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/math/limits.ipp>

#endif
