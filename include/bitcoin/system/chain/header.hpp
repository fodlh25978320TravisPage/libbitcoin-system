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
#ifndef LIBBITCOIN_SYSTEM_CHAIN_HEADER_HPP
#define LIBBITCOIN_SYSTEM_CHAIN_HEADER_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <string>
#include <memory>
#include <vector>
#include <bitcoin/system/chain/chain_state.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/error/error.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {

class settings;

namespace chain {

class BC_API header
{
public:
    typedef std::vector<header> list;
    typedef std::shared_ptr<header> ptr;

    // Constructors.
    // ------------------------------------------------------------------------

    header();

    header(header&& other);
    header(const header& other);

    header(uint32_t version, const hash_digest& previous_block_hash,
        const hash_digest& merkle_root, uint32_t timestamp, uint32_t bits,
        uint32_t nonce);
    header(uint32_t version, hash_digest&& previous_block_hash,
        hash_digest&& merkle_root, uint32_t timestamp, uint32_t bits,
        uint32_t nonce);

    header(const data_slice& data);
    header(std::istream& stream);
    header(reader& source);

    // Operators.
    // ------------------------------------------------------------------------

    header& operator=(header&& other);
    header& operator=(const header& other);

    bool operator==(const header& other) const;
    bool operator!=(const header& other) const;

    // Deserialization.
    // ------------------------------------------------------------------------

    bool from_data(const data_slice& data);
    bool from_data(std::istream& stream);
    bool from_data(reader& source);

    // Deserialization result.
    bool is_valid() const;

    // Serialization.
    // ------------------------------------------------------------------------

    data_chunk to_data() const;
    void to_data(std::ostream& stream) const;
    void to_data(writer& sink) const;

    static size_t serialized_size();

    // Properties.
    // ------------------------------------------------------------------------

    /// Native properties.
    uint32_t version() const;
    const hash_digest& previous_block_hash() const;
    const hash_digest& merkle_root() const;
    uint32_t timestamp() const;
    uint32_t bits() const;
    uint32_t nonce() const;

    /// Computed properties.
    hash_digest hash() const;
    uint256_t difficulty() const;

    // Validation.
    // ------------------------------------------------------------------------

    code check(uint32_t timestamp_limit_seconds, uint32_t proof_of_work_limit,
        bool scrypt=false) const;

    code accept(const chain_state& state) const;

protected:
    // So block may reset its member.
    friend class block;

    header(uint32_t version, hash_digest&& previous_block_hash,
        hash_digest&& merkle_root, uint32_t timestamp, uint32_t bits,
        uint32_t nonce, bool valid);

    header(uint32_t version, const hash_digest& previous_block_hash,
        const hash_digest& merkle_root, uint32_t timestamp, uint32_t bits,
        uint32_t nonce, bool valid);

    void reset();

    // Check (context free).
    // ------------------------------------------------------------------------

    bool is_invalid_proof_of_work(uint32_t proof_of_work_limit,
        bool scrypt=false) const;
    bool is_invalid_timestamp(uint32_t timestamp_limit_seconds) const;

    // Accept (relative to chain_state).
    // ------------------------------------------------------------------------

    // error::checkpoints_failed
    // error::invalid_block_version
    // error::timestamp_too_early
    // error::incorrect_proof_of_work

private:
    static uint256_t difficulty(uint32_t bits);

    uint32_t version_;
    hash_digest previous_block_hash_;
    hash_digest merkle_root_;
    uint32_t timestamp_;
    uint32_t bits_;
    uint32_t nonce_;
    bool valid_;
};

} // namespace chain
} // namespace system
} // namespace libbitcoin

#endif
