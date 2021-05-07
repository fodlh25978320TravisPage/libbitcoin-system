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
#include <bitcoin/system/wallet/witness_address.hpp>

#include <algorithm>
#include <cstdint>
#include <string>
#include <utility>
#include <bitcoin/system/chain/script.hpp>
#include <bitcoin/system/formats/base_32.hpp>
#include <bitcoin/system/math/checksum.hpp>
#include <bitcoin/system/math/elliptic_curve.hpp>
#include <bitcoin/system/math/hash.hpp>
#include <bitcoin/system/utility/data.hpp>
#include <bitcoin/system/utility/string.hpp>
#include <bitcoin/system/wallet/ec_private.hpp>
#include <bitcoin/system/wallet/ec_public.hpp>
#include <bitcoin/system/utility/exceptions.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

using namespace bc::system::machine;

constexpr char separator = '1';
constexpr uint8_t version_0 = 0;
constexpr uint8_t version_maximum = 16;
constexpr uint8_t version_invalid = max_uint8;

// The prefix is factored into the checksum but not the base32 encoding.
const std::string witness_address::mainnet = "bc";
const std::string witness_address::testnet = "tb";

const char witness_address::prefix_minimum_character = 33;
const char witness_address::prefix_maximum_character = 126;
const size_t witness_address::prefix_minimum_length = 1;
const size_t witness_address::address_maximum_length = 90;
const size_t witness_address::program_minimum_size = 2;
const size_t witness_address::program_maximum_size = 40;
const size_t witness_address::version_0_p2kh_program_size = 20;
const size_t witness_address::version_0_p2sh_program_size = 32;
const size_t witness_address::checksum_length = 6;

// Contructors.
// ----------------------------------------------------------------------------

// invalid
witness_address::witness_address()
  : program_(),
    prefix_(),
    version_(version_invalid)
{
}

// move
witness_address::witness_address(witness_address&& other)
  : program_(std::move(other.program_)),
    prefix_(other.prefix_),
    version_(other.version_)
{
}

// copy
witness_address::witness_address(const witness_address& other)
  : program_(other.program_),
    prefix_(other.prefix_),
    version_(other.version_)
{
}

// fully specified by address
witness_address::witness_address(const std::string& address, bool strict)
  : witness_address(from_address(address, strict))
{
}

// version_0_p2kh
witness_address::witness_address(const short_hash& public_key_hash,
    const std::string& prefix)
  : witness_address(from_short(public_key_hash, prefix))
{
}

// version_0_p2kh
witness_address::witness_address(const ec_private& secret,
    const std::string& prefix)
  : witness_address(from_private(secret, prefix))
{
}

// version_0_p2kh
witness_address::witness_address(const ec_public& point,
    const std::string& prefix)
  : witness_address(from_public(point, prefix))
{
}

// version_0_p2sh
witness_address::witness_address(const hash_digest& script_hash,
    const std::string& prefix)
  : witness_address(from_long(script_hash, prefix))
{
}

// version_0_p2sh
witness_address::witness_address(const chain::script& script,
    const std::string& prefix)
  : witness_address(from_script(script, prefix))
{
}

// private
witness_address::witness_address(data_chunk&& program,
    const std::string& prefix, uint8_t version)
  : program_(std::move(program)),
    prefix_(prefix),
    version_(version)
{
}

// Helpers.
// ----------------------------------------------------------------------------

// local
inline bool is_invalid_version(uint8_t version)
{
    return version > version_maximum;
}

// local
static bool has_invalid_prefix_character(const std::string& prefix)
{
    return (std::any_of(prefix.begin(), prefix.end(), [](const char character)
    {
        return character < witness_address::prefix_minimum_character ||
            character > witness_address::prefix_maximum_character;
    }));
}

// Lower case required for creation but not validation.
witness_address::parse_result witness_address::parse_prefix(
    const std::string& prefix)
{
    if (!is_ascii(prefix))
        return parse_result::prefix_not_ascii;

    // Require all lower case (unless part of all upper case address).
    if (ascii_to_lower(prefix) != prefix)
        return parse_result::prefix_not_lower_case;

    if (prefix.length() < prefix_minimum_length)
        return parse_result::prefix_too_short;

    // A separator, version, and checksum are also required.
    if (prefix.length() > (address_maximum_length - sizeof(separator) -
        sizeof(version_) - checksum_length))
        return parse_result::prefix_too_long;

    if (has_invalid_prefix_character(prefix))
        return parse_result::prefix_invalid_character;
        
    return parse_result::valid;
}

// Version is exactly 1 char but encoded into other program and checksum data.
// Limits: [[prefix:1-][separator:1]([version:1][program:0-][checksum:6])]:-90.
witness_address::parse_result witness_address::parse_address(
    std::string& out_prefix, uint8_t& out_version, data_chunk& out_program,
    const std::string& address, bool strict)
{
    if (!is_ascii(address))
        return parse_result::address_not_ascii;

    if (has_mixed_ascii_case(address))
        return parse_result::address_mixed_case;

    if (address.length() > address_maximum_length)
        return parse_result::address_too_long;

    // Allow all upper case.
    const auto lowered = ascii_to_lower(address);

    const auto split = lowered.rfind(separator);
    if (split == lowered.npos)
        return parse_result::missing_prefix;

    // Split the parts and discard the separator character.
    out_prefix = lowered.substr(0, split - sizeof(separator));
    const auto payload = lowered.substr(split + sizeof(separator));

    // Parsing lowered prefix, so all upper will pass as all lower.
    const auto parse = parse_prefix(out_prefix);
    if (parse != parse_result::valid)
        return parse;

    // The payload must contain at least the checksum and version byte.
    if (payload.length() < checksum_length + sizeof(version_))
        return parse_result::payload_too_short;

    base32_chunk checked;
    if (!decode_base32(checked, payload))
        return parse_result::payload_not_base32;

    // Verify the bech32 checksum and extract version and program.
    if (!bech32_verify_checked(out_version, out_program, out_prefix, checked))
        return parse_result::checksum_invalid;

    if (is_invalid_version(out_version))
        return parse_result::version_invalid;

    // Rejects invalid known programs even if strict is false.
    const auto type = parse_program(out_version, out_program);
    if (type == program_type::invalid)
        return parse_result::program_invalid;

    // Rejects unknown script programs if strict is true.
    if (strict && type == program_type::unknown)
        return parse_result::program_invalid;
    
    return parse_result::valid;
}

witness_address::program_type witness_address::parse_program(uint8_t version,
    const data_slice& program)
{
    if (is_invalid_version(version))
        return program_type::invalid;

    switch (version)
    {
        case version_0:
        {
            switch (program.size())
            {
                case version_0_p2kh_program_size:
                    return program_type::version_0_p2kh;
                case version_0_p2sh_program_size:
                    return program_type::version_0_p2sh;
                default:
                    return program_type::invalid;
            }
        }
        default:
        {
            return program_type::unknown;
        }
    }
}

// Factories.
// ----------------------------------------------------------------------------

// fully specified by address
witness_address witness_address::from_address(const std::string& address,
    bool strict)
{
    std::string prefix;
    uint8_t version;
    data_chunk program;

    // Unknown programs are valid but output_script will return empty.
    if (parse_address(prefix, version, program, address, strict) !=
        parse_result::valid)
        return {};

    // Address is the only way to construct unknown program type addresses.
    return { std::move(program), prefix, version };
}

// version_0_p2kh
witness_address witness_address::from_short(const short_hash& hash,
    const std::string& prefix)
{
    if (parse_prefix(prefix) != parse_result::valid)
        return {};

    // github.com/bitcoin/bips/blob/master/bip-0141.mediawiki
    // If the version byte is 0, and the witness program is 20 bytes it is
    // interpreted as a pay-to-witness-public-key-hash (P2WPKH) program.
    return { hash, prefix, version_0 };
}

// version_0_p2kh
witness_address witness_address::from_private(const ec_private& secret,
    const std::string& prefix)
{
    if (!secret)
        return {};

    // Contruction from secret utilizes the public key.
    return from_public(secret, prefix);
}

// version_0_p2kh
witness_address witness_address::from_public(const ec_public& point,
    const std::string& prefix)
{
    if (!point)
        return {};

    // .point() is always the compressed public key, as required.
    return from_short(bitcoin_short_hash(point.point()), prefix);
}

// version_0_p2sh
witness_address witness_address::from_long(const hash_digest& hash,
    const std::string& prefix)
{
    if (parse_prefix(prefix) != parse_result::valid)
        return {};

    // github.com/bitcoin/bips/blob/master/bip-0141.mediawiki
    // If the version byte is 0, and the witness program is 32 bytes it is
    // interpreted as a pay-to-witness-script-hash (P2WSH) program.
    return { hash, prefix, version_0 };
}

// version_0_p2sh
witness_address witness_address::from_script(const chain::script& script,
    const std::string& prefix)
{
    // Disallow construction of a valid address for an invalid script.
    if (!script.is_valid())
        return {};

    // .to_payments_key() is the sha256 hash of the serialized script.
    return from_long(script.to_payments_key(), prefix);
}

// Cast operators.
// ----------------------------------------------------------------------------

witness_address::operator bool() const
{
    // The program may be unknown, in which case output_script returns empty.
    // Yet all other properties are valid and conformant, so unknown is valid.
    return !is_invalid_version(version_);
}

// Serializer.
// ----------------------------------------------------------------------------

std::string witness_address::encoded() const
{
    const auto checked = bech32_build_checked(version_, prefix_, program_);
    return prefix_ + separator + encode_base32(checked);
}

// Properties.
// ----------------------------------------------------------------------------

const std::string& witness_address::prefix() const
{
    return prefix_;
}

// Version is simple numeric [0-16], not an opcode represenation.
uint8_t witness_address::version() const
{
    return version_;
}

const data_chunk& witness_address::program() const
{
    return program_;
}

witness_address::program_type witness_address::identifier() const
{
    return parse_program(version_, program_);
}

// Return constructed output script for known program types.
chain::script witness_address::output_script() const
{
    switch (identifier())
    {
        case program_type::version_0_p2kh:
            return chain::script::to_pay_witness_key_hash_pattern(
                to_array<short_hash_size>(program_));
        case program_type::version_0_p2sh:
            return chain::script::to_pay_witness_script_hash_pattern(
                to_array<hash_size>(program_));
        case program_type::unknown:
        case program_type::invalid:
        default:
            return {};
    }
}

// Operators.
// ----------------------------------------------------------------------------

witness_address& witness_address::operator=(witness_address&& other)
{
    program_ = std::move(other.program_);
    prefix_ = other.prefix_;
    version_ = other.version_;
    return *this;
}

witness_address& witness_address::operator=(const witness_address& other)
{
    program_ = other.program_;
    prefix_ = other.prefix_;
    version_ = other.version_;
    return *this;
}

bool witness_address::operator<(const witness_address& other) const
{
    return encoded() < other.encoded();
}

bool operator==(const witness_address& left, const witness_address& right)
{
    return left.encoded() == right.encoded();
}

bool operator!=(const witness_address& left, const witness_address& right)
{
    return !(left == right);
}

std::istream& operator>>(std::istream& in, witness_address& to)
{
    std::string value;
    in >> value;
    to = witness_address(value);

    if (!to)
        throw istream_exception(value);

    return in;
}

std::ostream& operator<<(std::ostream& out, const witness_address& of)
{
    out << of.encoded();
    return out;
}

} // namespace wallet
} // namespace system
} // namespace libbitcoin
