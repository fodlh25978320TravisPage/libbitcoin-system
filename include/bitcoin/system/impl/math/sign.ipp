/**
 * Copyright (c) 2011-2021 libbitcoin developers (see AUTHORS)
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
#ifndef LIBBITCOIN_SYSTEM_SIGN_IPP
#define LIBBITCOIN_SYSTEM_SIGN_IPP

#include <type_traits>
#include <bitcoin/system/type_constraints.hpp>

namespace libbitcoin {
namespace system {

// to_unsigned
    
template <typename Integer, typename Unsigned, if_integer<Integer>>
inline Unsigned to_unsigned(Integer value) noexcept
{
    // C++14: this can be made constexpr.
    return static_cast<Unsigned>(value);
}

// absolute

template <typename Integer, typename Absolute, if_signed_integer<Integer>>
constexpr Absolute absolute(Integer value) noexcept
{
    // std::abs is limited to signed types.
    return is_negative(value) ? -value : value;
}

template <typename Integer, typename Absolute, if_unsigned_integer<Integer>>
constexpr Absolute absolute(Integer value) noexcept
{
    return value;
}

// is_negative

template <typename Integer, if_signed_integer<Integer>>
constexpr bool is_negative(Integer value) noexcept
{
    // std::signbit is limited to floating point types.
    return value < 0;
}

template <typename Integer, if_unsigned_integer<Integer>>
constexpr bool is_negative(Integer) noexcept
{
    return false;
}

// is_greater

template <typename Left, typename Right, if_same_signed_integer<Left, Right>>
constexpr bool is_greater(Left left, Right right) noexcept
{
    return left > right;
}

template <typename Left, typename Right,
    if_unsigned_integer<Left>, if_signed_integer<Right>>
inline bool is_greater(Left left, Right right) noexcept
{
    // C++14: this can be made constexpr (to_unsigned casts).
    return is_negative(right) || (left > to_unsigned(right));
}

template <typename Left, typename Right,
    if_signed_integer<Left>, if_unsigned_integer<Right>>
inline bool is_greater(Left left, Right right) noexcept
{
    // C++14: this can be made constexpr (to_unsigned casts).
     return !is_negative(left) && (right < to_unsigned(left));
}

// is_lesser

template <typename Left, typename Right, if_same_signed_integer<Left, Right>>
constexpr bool is_lesser(Left left, Right right) noexcept
{
    return left < right;
}

template <typename Left, typename Right,
    if_signed_integer<Left>, if_unsigned_integer<Right>>
inline bool is_lesser(Left left, Right right) noexcept
{
    // C++14: this can be made constexpr (to_unsigned casts).
    return is_negative(left) || (to_unsigned(left) < right);
}

template <typename Left, typename Right,
    if_unsigned_integer<Left>, if_signed_integer<Right>>
inline bool is_lesser(Left left, Right right) noexcept
{
    // C++14: this can be made constexpr (to_unsigned casts).
    return !is_negative(right) && (to_unsigned(right) > left);
}

// greater

template<typename Result, typename Left, typename Right,
    if_integer<Left>, if_integer<Right>>
inline Result greater(Left left, Right right) noexcept
{
    // C++14: this can be made constexpr.
    return static_cast<Result>(is_greater(left, right) ? left : right);
}

// lesser

template<typename Result, typename Left, typename Right,
    if_integer<Left>, if_integer<Right>>
inline Result lesser(Left left, Right right) noexcept
{
    // C++14: this can be made constexpr.
    return static_cast<Result>(is_lesser(left, right) ? left : right);
}

} // namespace system
} // namespace libbitcoin

#endif
