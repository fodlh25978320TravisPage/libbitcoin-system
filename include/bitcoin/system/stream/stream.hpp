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
#ifndef LIBBITCOIN_SYSTEM_STREAM_STREAM_HPP
#define LIBBITCOIN_SYSTEM_STREAM_STREAM_HPP

#include <bitcoin/system/stream/binary.hpp>
#include <bitcoin/system/stream/device.hpp>
#include <bitcoin/system/stream/devices/copy_sink.hpp>
#include <bitcoin/system/stream/devices/copy_source.hpp>
#include <bitcoin/system/stream/devices/flip_sink.hpp>
#include <bitcoin/system/stream/devices/push_sink.hpp>
#include <bitcoin/system/stream/make_stream.hpp>
#include <bitcoin/system/stream/make_streamer.hpp>
#include <bitcoin/system/stream/iostream/iostream.hpp>
#include <bitcoin/system/stream/iostream/istream.hpp>
#include <bitcoin/system/stream/iostream/ostream.hpp>
#include <bitcoin/system/stream/streamers/bit_flipper.hpp>
#include <bitcoin/system/stream/streamers/bit_reader.hpp>
#include <bitcoin/system/stream/streamers/bit_writer.hpp>
#include <bitcoin/system/stream/streamers/byte_flipper.hpp>
#include <bitcoin/system/stream/streamers/byte_reader.hpp>
#include <bitcoin/system/stream/streamers/byte_writer.hpp>
#include <bitcoin/system/stream/streamers/interfaces/bitflipper.hpp>
#include <bitcoin/system/stream/streamers/interfaces/byteflipper.hpp>
#include <bitcoin/system/stream/streamers/interfaces/bitreader.hpp>
#include <bitcoin/system/stream/streamers/interfaces/bytereader.hpp>
#include <bitcoin/system/stream/streamers/interfaces/bitwriter.hpp>
#include <bitcoin/system/stream/streamers/interfaces/bytewriter.hpp>
#include <bitcoin/system/stream/streamers/sha256_writer.hpp>
#include <bitcoin/system/stream/streamers.hpp>
#include <bitcoin/system/stream/streams.hpp>
#include <bitcoin/system/stream/stream_result.hpp>

//*****************************************************************************
// CONSENSUS: Serialization must be consensus-normalized for hashing headers
// and transactions, otherwise valid transactions will seen as invalid.
// Deserialization is neither a consensus nor a DoS consideration. It does not
// matter in what form an object is obtained, just that hash serialization is
// consensus-normalized. The 'write_variable()' method writes variable integers
// in consensus-normal form, and all other byte writer methods are explicit.
//*****************************************************************************

#endif
