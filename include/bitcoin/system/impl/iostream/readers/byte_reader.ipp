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
#ifndef LIBBITCOIN_SYSTEM_IOSTREAM_READERS_BYTE_READER_IPP
#define LIBBITCOIN_SYSTEM_IOSTREAM_READERS_BYTE_READER_IPP

#include <utility>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/serialization/endian.hpp>
#include <bitcoin/system/type_constraints.hpp>

namespace libbitcoin {
namespace system {

// Call into virtual reader (vs. stream) so derived class can reuse.
template <typename Integer, if_integer<Integer>>
Integer byte_reader::read_big_endian() noexcept
{
    return from_big_endian<Integer>(read_bytes(sizeof(Integer)));
}

// Call into virtual reader (vs. stream) so derived class can reuse.
template <typename Integer, if_integer<Integer>>
Integer byte_reader::read_little_endian() noexcept
{
    return from_little_endian<Integer>(read_bytes(sizeof(Integer)));
}

// Reader supports directly populating an array, this avoids a copy.
template <size_t Size>
data_array<Size> byte_reader::read_forward() noexcept
{
    data_array<Size> out;
    read_bytes(out.data(), Size);
    return std::move(out);
}

// Reader supports directly populating an array, this avoids a copy.
template <size_t Size>
data_array<Size> byte_reader::read_reverse() noexcept
{
    // The reverse move override performs no copy.
    return reverse(read_forward<Size>());
}

} // namespace system
} // namespace libbitcoin

#endif
