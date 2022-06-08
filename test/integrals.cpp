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
#include "test.hpp"

 /// Signed.
constexpr int64_t min_int64 = std::numeric_limits<int64_t>::min();
constexpr int64_t max_int64 = std::numeric_limits<int64_t>::max();
constexpr int32_t min_int32 = std::numeric_limits<int32_t>::min();
constexpr int32_t max_int32 = std::numeric_limits<int32_t>::max();
constexpr int16_t min_int16 = std::numeric_limits<int16_t>::min();
constexpr int16_t max_int16 = std::numeric_limits<int16_t>::max();

/// Unsigned.
constexpr size_t max_size_t = std::numeric_limits<size_t>::max();
constexpr uint64_t max_uint64 = std::numeric_limits<uint64_t>::max();
constexpr uint32_t max_uint32 = std::numeric_limits<uint32_t>::max();
constexpr uint16_t max_uint16 = std::numeric_limits<uint16_t>::max();
constexpr uint8_t max_uint8 = std::numeric_limits<uint8_t>::max();

static_assert(zero == 0u, "");
static_assert(one == 1u, "");
static_assert(two == 2u, "");
static_assert(byte_bits == 8u, "");
static_assert(negative_one == -1, "");
