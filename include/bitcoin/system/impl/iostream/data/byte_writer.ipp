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
#ifndef LIBBITCOIN_SYSTEM_IOSTREAM_DATA_BYTE_WRITER_IPP
#define LIBBITCOIN_SYSTEM_IOSTREAM_DATA_BYTE_WRITER_IPP

#include <utility>
#include <bitcoin/system/serialization/endian.hpp>
#include <bitcoin/system/type_constraints.hpp>

namespace libbitcoin {
namespace system {
    
// Call into virtual writer (vs. stream) so derived class can reuse.
template <typename Integer, if_integer<Integer>>
void byte_writer::write_big_endian(Integer value)
{
    // TODO: reimplement on derived classes and change to this here:
    ////to_big_endian<Integer>(stream_, value);

    // This creates a copy that could be avoided by using stream overload.
    write_bytes(to_big_endian<Integer>(value));
}

// Call into virtual writer (vs. stream) so derived class can reuse.
template <typename Integer, if_integer<Integer>>
void byte_writer::write_little_endian(Integer value)
{
    // TODO: reimplement on derived classes and change to this here:
    ////to_little_endian<Integer>(stream_, value);

    // This creates a copy that could be avoided by using stream overload.
    write_bytes(to_little_endian<Integer>(value));
}

} // namespace system
} // namespace libbitcoin

#endif
