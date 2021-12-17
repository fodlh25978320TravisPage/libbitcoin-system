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
#ifndef LIBBITCOIN_SYSTEM_DATA_DATA_CHUNK_HPP
#define LIBBITCOIN_SYSTEM_DATA_DATA_CHUNK_HPP

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include <bitcoin/system/data/data_slice.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

/// Byte storage types.
typedef std::vector<uint8_t> data_chunk;
typedef std::vector<data_chunk> data_stack;
typedef std::shared_ptr<const data_chunk> chunk_ptr;
typedef std::shared_ptr<const data_stack> stack_ptr;

/// Create a single byte data_chunk with given element value.
BC_API data_chunk to_chunk(uint8_t byte) noexcept;

/// Create a data chunk from data slice.
/// to_chunk(to_string(data)) == data.
BC_API data_chunk to_chunk(const data_slice& bytes) noexcept;

/// Concatenate several data slices into a single data_chunk.
/// extra_reserve reserves but does not allocate additional bytes.
BC_API data_chunk build_chunk(const data_loaf& slices,
    size_t extra_reserve=0) noexcept;

/// Concatenate two data_slices into a single data chunk.
BC_API data_chunk splice(const data_slice& left,
    const data_slice& right) noexcept;

} // namespace system
} // namespace libbitcoin

#endif
