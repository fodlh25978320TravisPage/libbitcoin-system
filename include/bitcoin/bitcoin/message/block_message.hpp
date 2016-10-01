/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_MESSAGE_BLOCK_MESSAGE_HPP
#define LIBBITCOIN_MESSAGE_BLOCK_MESSAGE_HPP

#include <cstdint>
#include <cstddef>
#include <istream>
#include <memory>
#include <bitcoin/bitcoin/chain/block.hpp>
#include <bitcoin/bitcoin/chain/header.hpp>
#include <bitcoin/bitcoin/chain/transaction.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/bitcoin/utility/data.hpp>
#include <bitcoin/bitcoin/utility/reader.hpp>

namespace libbitcoin {
namespace message {

class BC_API block_message
  : public chain::block
{
public:
    typedef std::shared_ptr<block_message> ptr;
    typedef std::shared_ptr<const block_message> const_ptr;
    typedef std::vector<ptr> ptr_list;
    typedef std::vector<const_ptr> const_ptr_list;

    static block_message factory_from_data(uint32_t version,
        const data_chunk& data);
    static block_message factory_from_data(uint32_t version,
        std::istream& stream);
    static block_message factory_from_data(uint32_t version,
        reader& source);

    block_message();
    block_message(const chain::header& header,
        const chain::transaction::list& transactions);
    block_message(chain::header&& header,
        chain::transaction::list&& transactions);
    block_message(const chain::block& other);
    block_message(chain::block&& other);
    block_message(const block_message& other);
    block_message(block_message&& other);

    uint64_t originator() const;

    // HACK: The fact that this is const makes it unsafe.
    void set_originator(uint64_t value) const;

    bool from_data(uint32_t version, const data_chunk& data);
    bool from_data(uint32_t version, std::istream& stream);
    bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, std::ostream& stream) const;
    void to_data(uint32_t version, writer& sink) const;
    uint64_t serialized_size(uint32_t version) const;

    block_message& operator=(chain::block&& other);

    // This class is move assignable but not copy assignable.
    block_message& operator=(block_message&& other);
    void operator=(const block_message&) = delete;

    bool operator==(const chain::block& other) const;
    bool operator!=(const chain::block& other) const;

    bool operator==(const block_message& other) const;
    bool operator!=(const block_message& other) const;

    static const std::string command;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;

private:
    mutable uint64_t originator_;
};

} // namespace message
} // namespace libbitcoin

#endif
