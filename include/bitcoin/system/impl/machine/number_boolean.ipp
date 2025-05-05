/**
 * Copyright (c) 2011-2025 libbitcoin developers (see AUTHORS)
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
#ifndef LIBBITCOIN_SYSTEM_MACHINE_NUMBER_BOOLEAN_IPP
#define LIBBITCOIN_SYSTEM_MACHINE_NUMBER_BOOLEAN_IPP

#include <algorithm>
#include <iterator>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/machine/number.hpp>

namespace libbitcoin {
namespace system {
namespace machine {
namespace number {

template <size_t Size, if_not_greater<Size, sizeof(int64_t)>>
constexpr signed_type<Size> boolean::
to_integer(bool vary) NOEXCEPT
{
    // The cast can safely be ignored, which is why Size is defaulted.
    return bc::to_int<signed_type<Size>>(vary);
}

constexpr bool boolean::
from_chunk(const data_chunk& vary) NOEXCEPT
{
    // An optimization, also guards vector empty.
    if (strict_false(vary))
        return false;

    // Boolean is not overflow constrained (any length of bytes is valid).
    ////if (is_overflow(vary))
    ////    return false;

    if (!is_sign_byte(vary.back()))
        return true;

    // A logical zero is any +/- sequence of zero bytes (sign excluded above).
    return std::any_of(vary.begin(), std::prev(vary.end()), is_nonzero<uint8_t>);

    // Size constraint guards from_little_endian.
    // any_of optimizes by eliminating this conversion, allocation, and copy.
    ////return bc::to_bool(from_little_endian<uintx>(vary));
}

constexpr bool boolean::
strict_from_chunk(const data_chunk& vary) NOEXCEPT
{
    // True if not strictly false.
    return !strict_false(vary);
}

constexpr bool boolean::
to_bool(int64_t vary) NOEXCEPT
{
    // Boolean is not overflow constrained.
    return bc::to_bool(vary);
}

// protected
constexpr bool boolean::
strict_false(const data_chunk& vary) NOEXCEPT
{
    return vary.empty();
}

// protected
constexpr bool boolean::
is_sign_byte(uint8_t byte) NOEXCEPT
{
    return (byte == positive_sign_byte) || (byte == negative_sign_byte);
}

} // namespace number
} // namespace machine
} // namespace system
} // namespace libbitcoin

#endif
