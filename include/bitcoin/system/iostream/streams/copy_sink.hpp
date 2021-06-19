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
#ifndef LIBBITCOIN_SYSTEM_IOSTREAM_STREAMS_COPY_SINK_HPP
#define LIBBITCOIN_SYSTEM_IOSTREAM_STREAMS_COPY_SINK_HPP

#include <bitcoin/system/iostream/streams/base_sink.hpp>

namespace libbitcoin {
namespace system {

/// Sink for boost::iostreams::stream, copies bytes to Container.
template <typename Container>
class copy_sink
  : base_sink<Container>
{
public:
    copy_sink(typename Container::iterator& begin,
        typename Container::iterator& end) noexcept;

protected:
    virtual size_type do_write(const value_type* from,
        size_type size) noexcept;

private:
    typename Container::iterator to_;
};

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/iostream/streams/copy_sink.ipp>

#endif
