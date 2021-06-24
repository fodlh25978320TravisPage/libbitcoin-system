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
#ifndef LIBBITCOIN_SYSTEM_IOSTREAM_SOURCES_MOVE_SOURCE_HPP
#define LIBBITCOIN_SYSTEM_IOSTREAM_SOURCES_MOVE_SOURCE_HPP

#include <boost/iostreams/stream.hpp>
#include <bitcoin/system/iostream/sources/source.hpp>

namespace libbitcoin {
namespace system {

/// Source for boost::iostreams::stream, moves bytes from Container.
template <typename Container>
class move_source
  : public base_source<source::tag::move, Container>
{
public:
    move_source(Container& data) noexcept;

protected:
    virtual void do_read(value_type* to, size_type size) noexcept;

private:
    Container& source_;
    typename Container::iterator from_;
};

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/iostream/sources/move_source.ipp>

#endif
