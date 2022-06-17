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
#ifndef LIBBITCOIN_SYSTEM_MATH_BYTES_HPP
#define LIBBITCOIN_SYSTEM_MATH_BYTES_HPP

#include <bit>
#include <array>
#include <cstdint>
#include <bitcoin/system/constraints.hpp>

namespace libbitcoin {
namespace system {

// C++23: byteswap.
template <typename Integer,
    if_integral_integer<Integer> = true,
    if_unique_object_representations<Integer> = true>
constexpr Integer byteswap(Integer value) noexcept;

////// Intrinsics (non-constexpr).
////template<typename Integer,
////    if_integral_integer<Integer> = true,
////    if_unique_object_representations<Integer> = true>,
////    if_not_greater<sizeof(Integer), sizeof(uint64_t)> = true>
////inline Integer byte_swap_intrinsic(Integer value) noexcept;

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/math/bytes.ipp>

#endif
