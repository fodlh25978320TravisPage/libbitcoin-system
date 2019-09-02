/**
 * Copyright (c) 2011-2019 libbitcoin developers (see AUTHORS)
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

// Sponsored in part by Digital Contract Design, LLC

#include <bitcoin/system/chain/basic_compact_filter.hpp>

//#include <initializer_list>
//#include <bitcoin/system/math/limits.hpp>
//#include <bitcoin/system/message/messages.hpp>
//#include <bitcoin/system/message/version.hpp>
//#include <bitcoin/system/utility/container_sink.hpp>
//#include <bitcoin/system/utility/container_source.hpp>
//#include <bitcoin/system/utility/istream_reader.hpp>
//#include <bitcoin/system/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

const uint8_t basic_compact_filter::basic_filter_type = 0u;

basic_compact_filter basic_compact_filter::factory(const data_chunk& data)
{
	basic_compact_filter instance;
    instance.from_data(data);
    return instance;
}

basic_compact_filter basic_compact_filter::factory(std::istream& stream)
{
	basic_compact_filter instance;
    instance.from_data(stream);
    return instance;
}

basic_compact_filter basic_compact_filter::factory(reader& source)
{
	basic_compact_filter instance;
    instance.from_data(source);
    return instance;
}

basic_compact_filter::basic_compact_filter()
  : compact_filter()
{
}

basic_compact_filter::basic_compact_filter(const hash_digest& block_hash,
	const data_chunk& filter)
  : compact_filter(basic_compact_filter::basic_filter_type, block_hash,
        filter)
{
}

basic_compact_filter::basic_compact_filter(hash_digest&& block_hash,
	data_chunk&& filter)
  : compact_filter(basic_compact_filter::basic_filter_type,
      std::move(block_hash), std::move(filter))
{
}

basic_compact_filter::basic_compact_filter(const basic_compact_filter& other)
  : compact_filter(other)
{
}

basic_compact_filter::basic_compact_filter(basic_compact_filter&& other)
  : compact_filter(std::move(other))
{
}

basic_compact_filter::basic_compact_filter(const compact_filter& other)
  : compact_filter(other)
{
    if (filter_type() != basic_compact_filter::basic_filter_type)
        reset();
}

basic_compact_filter::basic_compact_filter(compact_filter&& other)
  : compact_filter(std::move(other))
{
    if (filter_type() != basic_compact_filter::basic_filter_type)
        reset();
}

bool basic_compact_filter::is_valid() const
{
    return compact_filter::is_valid()
        && (filter_type() == basic_compact_filter::basic_filter_type);
}

void basic_compact_filter::reset()
{
    compact_filter::reset();
    set_filter_type(basic_compact_filter::basic_filter_type);
}

basic_compact_filter& basic_compact_filter::operator=(
    basic_compact_filter&& other)
{
    compact_filter::operator=(std::move(other));
    return *this;
}

basic_compact_filter& basic_compact_filter::operator=(compact_filter&& other)
{
    compact_filter::operator=(std::move(other));
    return *this;
}

bool basic_compact_filter::operator==(const basic_compact_filter& other) const
{
    return compact_filter::operator==(other);
}

bool basic_compact_filter::operator==(const compact_filter& other) const
{
    return compact_filter::operator==(other);
}

bool basic_compact_filter::operator!=(const basic_compact_filter& other) const
{
    return !(*this == other);
}

bool basic_compact_filter::operator!=(const compact_filter& other) const
{
    return !(*this == other);
}

bool basic_compact_filter::match(
    const wallet::payment_address::list /*addresses*/) const
{
    return false;
}

bool basic_compact_filter::populate(const block /*validated_block*/)
{
    return true;
}

} // namespace chain
} // namespace system
} // namespace libbitcoin
