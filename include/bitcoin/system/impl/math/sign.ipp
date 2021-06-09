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

#include <bitcoin/system/type_constraints.hpp>

namespace libbitcoin {
namespace system {

template <typename Integer, if_signed_integer<Integer>>
inline Integer absolute(Integer value)
{
    // std::abs is limited to signed types.
    return is_negative(value) ? -value : value;
}

template <typename Integer, if_unsigned_integer<Integer>>
inline Integer absolute(Integer value)
{
    return value;
}

template <typename Integer, if_signed_integer<Integer>>
inline bool is_negative(Integer value)
{
    // std::signbit is limited to floating point types.
    return value < 0;
}

template <typename Integer, if_unsigned_integer<Integer>>
inline bool is_negative(Integer)
{
    return false;
}

} // namespace system
} // namespace libbitcoin

#endif
