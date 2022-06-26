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
#ifndef LIBBITCOIN_SYSTEM_MATH_POWER_HPP
#define LIBBITCOIN_SYSTEM_MATH_POWER_HPP

#include <cstddef>
#include <bitcoin/system/constraints.hpp>

namespace libbitcoin {
namespace system {

/// Division by Zero warning
/// log(0, n) is undefined, and given inlining of these functions, a compiler
/// may warn of a "possible" (but actually unreachable) division by zero when
/// the base is const 0. This occurs because the compiler is inlining
/// "value / 0". The resolution is to not make this call in production code
/// (the warning is beneficial) and to use a non-const base 0 in tests.

/// All operations below support signed and unsigned template parameters.

/// Ceilinged logarithms.

/// Obtain the ceilinged (rounded up) integer logarithm of given value and base.
/// Returns 0 for undefined (base < 2 or value < 1).
template <typename Exponent = size_t, typename Base, typename Value,
    if_integer<Exponent> = true, if_integer<Base> = true,
    if_integer<Value> = true>
constexpr Exponent ceilinged_log(Base base, Value value) noexcept;
template <size_t Base, typename Exponent = size_t, typename Value>
constexpr Exponent ceilinged_log(Value value) noexcept;

/// Optimizations for ceilinged_log(2, Value).
template <typename Exponent = size_t, typename Value,
    if_integer<Exponent> = true, if_integral_integer<Value> = true>
constexpr Exponent ceilinged_log2(Value value) noexcept;
template <typename Exponent = size_t, typename Value,
    if_integer<Exponent> = true, if_non_integral_integer<Value> = true>
constexpr Exponent ceilinged_log2(Value value) noexcept;

/// Optimizations for ceilinged_log(256, integral).
template <typename Exponent = size_t, typename Value,
    if_integer<Exponent> = true, if_integral_integer<Value> = true>
constexpr size_t ceilinged_log256(Value value) noexcept;
template <typename Exponent = size_t, typename Value,
    if_not_one_byte<Exponent> = true, if_integer<Exponent> = true,
    if_non_integral_integer<Value> = true>
constexpr size_t ceilinged_log256(Value value) noexcept;

/// Floored logarithms.

/// Obtain the floored (rounded down) integer logarithm of given value and base.
/// Returns 0 for undefined (base < 2 or value < 1).
template <typename Exponent = size_t, typename Base, typename Value,
    if_integer<Exponent> = true, if_integer<Base> = true,
    if_integer<Value> = true>
constexpr Exponent floored_log(Base base, Value value) noexcept;
template <size_t Base, typename Exponent = size_t, typename Value>
constexpr Exponent floored_log(Value value) noexcept;

/// Optimizations for floored_log(2, Value).
template <typename Exponent = size_t, typename Value,
    if_integer<Exponent> = true, if_integral_integer<Value> = true>
constexpr Exponent floored_log2(Value value) noexcept;
template <typename Exponent = size_t, typename Value,
    if_integer<Exponent> = true, if_non_integral_integer<Value> = true>
constexpr Exponent floored_log2(Value value) noexcept;

/// Optimizations for floored_log(256, Value).
template <typename Exponent = size_t, typename Value,
    if_integer<Exponent> = true, if_integral_integer<Value> = true>
constexpr Exponent floored_log256(Value value) noexcept;
template <typename Exponent = size_t, typename Value,
    if_not_one_byte<Exponent>, if_integer<Exponent> = true,
    if_non_integral_integer<Value> = true>
constexpr Exponent floored_log256(Value value) noexcept;

/// Powers.

/// Obtain the integer power of given base for given exponent.
/// Returns zero if both operands are zero (undefined).
template <typename Value = size_t, typename Base, typename Exponent,
    if_integer<Value> = true, if_integer<Base> = true,
    if_integer<Exponent> = true>
constexpr Value power(Base base, Exponent exponent) noexcept;
template <size_t Base, typename Value = size_t, typename Exponent>
constexpr Value power(Exponent exponent) noexcept;

/// Optimizations for power(2, Exponent).
template <typename Value = size_t, typename Exponent,
    if_integral_integer<Value> = true, if_integer<Exponent> = true>
constexpr Value power2(Exponent exponent) noexcept;
template <typename Value = size_t, typename Exponent,
    if_non_integral_integer<Value> = true, if_integer<Exponent> = true>
constexpr Value power2(Exponent exponent) noexcept;

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/math/power.ipp>

#endif
