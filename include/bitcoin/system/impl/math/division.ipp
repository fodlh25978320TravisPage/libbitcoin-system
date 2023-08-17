/**
 * Copyright (c) 2011-2023 libbitcoin developers (see AUTHORS)
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
#ifndef LIBBITCOIN_SYSTEM_MATH_DIVISION_IPP
#define LIBBITCOIN_SYSTEM_MATH_DIVISION_IPP

#include <bitcoin/system/define.hpp>
#include <bitcoin/system/math/sign.hpp>

namespace libbitcoin {
namespace system {

// unpublished
// ----------------------------------------------------------------------------

template <typename Factor1, typename Factor2,
    if_integer<Factor1> = true, if_integer<Factor2> = true>
constexpr bool is_negative_(Factor1 factor1, Factor2 factor2) NOEXCEPT
{
    return is_negative(factor1) != is_negative(factor2);
}

template <typename Dividend, typename Divisor,
    if_integer<Dividend> = true, if_integer<Divisor> = true>
constexpr bool no_remainder_(Dividend dividend, Divisor divisor) NOEXCEPT
{
    return is_zero(dividend % divisor);
}

template <typename Dividend, typename Divisor,
    if_integer<Dividend> = true, if_integer<Divisor> = true>
constexpr bool is_ceilinged_(Dividend dividend, Divisor divisor) NOEXCEPT
{
    return is_negative_(dividend, divisor) || no_remainder_(dividend, divisor);
}

template <typename Dividend, typename Divisor,
    if_integer<Dividend> = true, if_integer<Divisor> = true>
constexpr bool is_floored_(Dividend dividend, Divisor divisor) NOEXCEPT
{
    return !is_negative_(dividend, divisor) || no_remainder_(dividend, divisor);
}

// published
// ----------------------------------------------------------------------------

template <typename Dividend, typename Divisor,
    if_integer<Dividend>, if_integer<Divisor>>
constexpr to_common_type<Dividend, Divisor>
ceilinged_divide(Dividend dividend, Divisor divisor) NOEXCEPT
{
    return truncated_divide(dividend, divisor) + 
        (is_ceilinged_(dividend, divisor) ? 0 : 1);
}

// The native % operator is truncated and the common type is unsigned.
// But ceilinged modulo is logically negative, so type is changed to signed.
template <typename Dividend, typename Divisor,
    if_integer<Dividend>, if_integer<Divisor>>
constexpr to_signed_type<to_common_type<Dividend, Divisor>>
ceilinged_modulo(Dividend dividend, Divisor divisor) NOEXCEPT
{
    return truncated_modulo(dividend, divisor) -
        (is_ceilinged_(dividend, divisor) ? 0 : divisor);
}

template <typename Dividend, typename Divisor,
    if_integer<Dividend>, if_integer<Divisor>>
constexpr to_common_type<Dividend, Divisor>
floored_divide(Dividend dividend, Divisor divisor) NOEXCEPT
{
    return truncated_divide(dividend, divisor) -
        (is_floored_(dividend, divisor) ? 0 : 1);
}

// Always positive result.
template <typename Dividend, typename Divisor,
    if_integer<Dividend>, if_integer<Divisor>>
constexpr to_common_type<Dividend, Divisor>
floored_modulo(Dividend dividend, Divisor divisor) NOEXCEPT
{
    return truncated_modulo(dividend, divisor) +
        (is_floored_(dividend, divisor) ? 0 : divisor);
}

template <typename Dividend, typename Divisor,
    if_integer<Dividend>, if_integer<Divisor>>
constexpr to_common_type<Dividend, Divisor>
truncated_divide(Dividend dividend, Divisor divisor) NOEXCEPT
{
    return dividend / divisor;
}

template <typename Dividend, typename Divisor,
    if_integer<Dividend>, if_integer<Divisor>>
constexpr to_common_type<Dividend, Divisor>
truncated_modulo(Dividend dividend, Divisor divisor) NOEXCEPT
{
    return dividend % divisor;
}

} // namespace system
} // namespace libbitcoin

#endif
