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
#ifndef LIBBITCOIN_SYSTEM_STREAM_STREAM_HPP
#define LIBBITCOIN_SYSTEM_STREAM_STREAM_HPP

#include <iostream>
#include <string>
#include <boost/iostreams/stream.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/stream/device.hpp>
#include <bitcoin/system/stream/devices/copy_sink.hpp>
#include <bitcoin/system/stream/devices/copy_source.hpp>
#include <bitcoin/system/stream/devices/flip_sink.hpp>
#include <bitcoin/system/stream/devices/push_sink.hpp>
#include <bitcoin/system/stream/make_stream.hpp>
#include <bitcoin/system/stream/make_streamer.hpp>
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

namespace libbitcoin {
namespace system {
    
/// Streams are not copyable, so factory cannot be used for type inference.
/// All sources and sinks (devices) accpt only const and non-const references.
/// Insert requires the container object and therefore type (only two).
/// Container.reserve() may improve performance for expectations > 1kb.
/// push_sink is buffered/indirect (inefficient) and requires flush.
/// For better performance, size the buffer and use a copy_sink.
namespace stream
{
    namespace in
    {
        /// An input stream that moves data from a data_reference.
        using copy = make_stream<copy_source<data_reference>>;
    }

    namespace out
    {
        /// An output stream that copies data to a data_slab.
        using copy = make_stream<copy_sink<data_slab>>;

        // TODO: change to push, text, data.
        /// An output stream that inserts data to a container.
        template <typename Container>
        using into = make_stream<push_sink<Container>>;
        using text = into<std::string>;
        using push = into<data_chunk>;
    }

    /// An input/output stream that copies data to a data_slab.
    using flip = make_stream<flip_sink<data_slab>>;
}

namespace read
{
    namespace bytes
    {
        /// A byte reader that reads data from a std::istream.
        using istream = byte_reader<std::istream>;

        /// A byte reader that copies data from a data_reference.
        using copy = make_streamer<copy_source<data_reference>, byte_reader>;
    }

    namespace bits
    {
        /// A bit reader that reads data from a std::istream.
        using istream = bit_reader<std::istream>;

        /// A bit reader that moves data from a data_reference.
        using copy = make_streamer<copy_source<data_reference>, bit_reader>;
    }
}

namespace write
{
    namespace bytes
    {
        /// A byte writer that writes data to a std::ostream.
        using ostream = byte_writer<std::ostream>;

        /// A byte reader/writer of a std::iostream.
        using iostream = byte_flipper<std::iostream>;

        /// A byte reader/writer of a data_slab (no push and requires own sink).
        using flip = make_streamer<flip_sink<data_slab>, byte_flipper>;

        /// A byte writer that copies data to a data_slab.
        using copy = make_streamer<copy_sink<data_slab>, byte_writer>;

        // TODO: change to push, text, data.
        /// A byte writer that inserts data into a container.
        template <typename Container>
        using into = make_streamer<push_sink<Container>, byte_writer>;
        using text = into<std::string>;
        using push = into<data_chunk>;
    }

    namespace bits
    {
        /// A bit writer that writes data to a std::ostream.
        using ostream = bit_writer<std::ostream>;

        /// A bit reader/writer of a std::iostream.
        using iostream = bit_flipper<std::iostream>;

        /// A bit reader/writer of a data_slab (no push and requires own sink).
        using flip = make_streamer<flip_sink<data_slab>, bit_flipper>;

        /// A bit writer that copies data to a data_slab.
        using copy = make_streamer<copy_sink<data_slab>, bit_writer>;

        // TODO: change to push, text, data.
        /// A bit writer that inserts data into a container.
        template <typename Container>
        using into = make_streamer<push_sink<Container>, bit_writer>;
        using text = into<std::string>;
        using push = into<data_chunk>;
    }
}

/// Interface byte aliases.
using flipper = byteflipper;
using reader = bytereader;
using writer = bytewriter;

} // namespace system
} // namespace libbitcoin

#endif
