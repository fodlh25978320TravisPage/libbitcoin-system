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
#include <bitcoin/system/stream/flippers/bit_flipper.hpp>
#include <bitcoin/system/stream/flippers/byte_flipper.hpp>
#include <bitcoin/system/stream/interfaces/bitflipper.hpp>
#include <bitcoin/system/stream/interfaces/byteflipper.hpp>
#include <bitcoin/system/stream/interfaces/bitreader.hpp>
#include <bitcoin/system/stream/interfaces/bytereader.hpp>
#include <bitcoin/system/stream/interfaces/bitwriter.hpp>
#include <bitcoin/system/stream/interfaces/bytewriter.hpp>
#include <bitcoin/system/stream/makers/make_stream.hpp>
#include <bitcoin/system/stream/makers/make_streamer.hpp>
#include <bitcoin/system/stream/sinks/copy_sink.hpp>
#include <bitcoin/system/stream/sinks/flip_sink.hpp>
#include <bitcoin/system/stream/sinks/push_sink.hpp>
#include <bitcoin/system/stream/sources/copy_source.hpp>
#include <bitcoin/system/stream/sources/move_source.hpp>
#include <bitcoin/system/stream/readers/bit_reader.hpp>
#include <bitcoin/system/stream/readers/byte_reader.hpp>
#include <bitcoin/system/stream/writers/bit_writer.hpp>
#include <bitcoin/system/stream/writers/byte_writer.hpp>

namespace libbitcoin {
namespace system {
    
// Streams are not copyable, so factory cannot be used to infer container type.
// Always copy read from a container reference and move read from a slab.
// Always copy write to a slab and push write to an insertable container.
// A data_slice cannot be used for read reference because it accepts any
// movable data type but requires a long lifetime as a class member.
// std::string and data_chunk are the only insertable types, so there is no
// need for template construction to obtain the container type for pushes.

namespace stream
{
    namespace in
    {
        /// An input stream that moves data from a data_slab.
        using move = make_stream<move_source<data_slab>>;

        /// An input stream that copies data from a Container reference.
        template <typename Container>
        using from = make_stream<copy_source<Container>>;
        using copy = make_stream<copy_source<data_chunk>>;
    }

    namespace out
    {
        /// An output stream that copies data to a data_slab.
        using slab = make_stream<copy_sink<data_slab>>;

        /// An output stream that inserts data to a container.
        /// Container.reserve() may improve performance for expectations > 1kb.
        /// push_sink is buffered/indirect (inefficient) and requires flush.
        /// For better performance, size the buffer and use a copy_sink.
        using text = make_stream<push_sink<std::string>>;
        using push = make_stream<push_sink<data_chunk>>;
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

        /// A byte reader that moves data from a data_slab.
        using move = make_streamer<move_source<data_slab>, byte_reader>;

        /// A byte reader that copies data from a Container reference.
        template <typename Container>
        using from = make_streamer<copy_source<Container>, byte_reader>;
        using copy = make_streamer<copy_source<data_chunk>, byte_reader>;
    }

    namespace bits
    {
        /// A bit reader that reads data from a std::istream.
        using istream = bit_reader<std::istream>;

        /// A bit reader that moves data from a data_slab.
        using move = make_streamer<move_source<data_slab>, bit_reader>;

        /// A bit reader that copies data from a Container reference.
        template <typename Container>
        using from = make_streamer<copy_source<Container>, bit_reader>;
        using copy = make_streamer<copy_source<data_chunk>, bit_reader>;
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

        /// A byte reader/writer of a data_slab.
        using flip = make_streamer<flip_sink<data_slab>, byte_flipper>;

        /// A byte writer that writes data to a data_slab.
        using slab = make_streamer<copy_sink<data_slab>, byte_writer>;

        /// A byte writer that inserts data into a container.
        /// Container.reserve() may improve performance for expectations > 1kb.
        /// push_sink is buffered/indirect (inefficient) and requires flush.
        /// For better performance, size the buffer and use a copy_sink.
        using text = make_streamer<push_sink<std::string>, byte_writer>;
        using push = make_streamer<push_sink<data_chunk>, byte_writer>;
    }

    namespace bits
    {
        /// A bit writer that writes data to a std::ostream.
        using ostream = bit_writer<std::ostream>;

        /// A bit reader/writer of a std::iostream.
        using iostream = bit_flipper<std::iostream>;

        /// A bit reader/writer of a data_slab.
        using flip = make_streamer<flip_sink<data_slab>, bit_flipper>;

        /// A bit writer that writes data to a data_slab.
        using slab = make_streamer<copy_sink<data_slab>, bit_writer>;

        /// A bit writer that inserts data into a container.
        /// Container.reserve() may improve performance for expectations > 1kb.
        /// push_sink is buffered/indirect (inefficient) and requires flush.
        /// For better performance, size the buffer and use a copy_sink.
        using text = make_streamer<push_sink<std::string>, bit_writer>;
        using push = make_streamer<push_sink<data_chunk>, bit_writer>;
    }
}

/// Interface byte aliases.
using flipper = byteflipper;
using reader = bytereader;
using writer = bytewriter;

} // namespace system
} // namespace libbitcoin

#endif
