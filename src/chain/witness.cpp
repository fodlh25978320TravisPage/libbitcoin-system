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
#include <bitcoin/system/chain/witness.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <numeric>
#include <string>
#include <utility>
#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/chain/enums/magic_numbers.hpp>
#include <bitcoin/system/chain/operation.hpp>
#include <bitcoin/system/chain/script.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/error/error.hpp>
#include <bitcoin/system/machine/machine.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

using namespace system::machine;
static const auto checksig_script = script{ { opcode::checksig } };

// Constructors.
// ----------------------------------------------------------------------------

witness::witness()
  : witness({}, false)
{
}

witness::witness(witness&& other)
  : witness(other)
{
}

witness::witness(const witness& other)
  : witness(other.stack_, other.valid_)
{
}

witness::witness(data_stack&& stack)
  : witness(std::move(stack), true)
{
}

witness::witness(const data_stack& stack)
  : witness(stack, true)
{
}

witness::witness(const data_slice& data, bool prefix)
  : witness(stream::in::copy(data), prefix)
{
}

witness::witness(std::istream& stream, bool prefix)
  : witness(read::bytes::istream(stream), prefix)
{
}

witness::witness(reader& source, bool prefix)
  : witness(from_data(source, prefix))
{
}

witness::witness(const std::string& mnemonic)
  : witness(from_string(mnemonic))
{
}

// protected
witness::witness(data_stack&& stack, bool valid)
  : stack_(std::move(stack)), valid_(valid)
{
}

// protected
witness::witness(const data_stack& stack, bool valid)
  : stack_(stack), valid_(valid)
{
}

// Operators.
// ----------------------------------------------------------------------------

witness& witness::operator=(witness&& other)
{
    stack_ = std::move(other.stack_);
    valid_ = other.valid_;
    return *this;
}

witness& witness::operator=(const witness& other)
{
    stack_ = other.stack_;
    valid_ = other.valid_;
    return *this;
}

bool witness::operator==(const witness& other) const
{
    // TODO: after changing to vector(data_ptr) compare membership, not ptrs.
    return (stack_ == other.stack_);
}

bool witness::operator!=(const witness& other) const
{
    return !(*this == other);
}

// Deserialization.
// ----------------------------------------------------------------------------

static data_chunk read_element(reader& source)
{
    // Each witness encoded as variable integer prefixed byte array (bip144).
    return source.read_bytes(source.read_size(max_block_weight));
}

// static/private
witness witness::from_data(reader& source, bool prefix)
{
    data_stack stack;

    if (prefix)
    {
        // Each witness is prefixed with number of elements (bip144).
        // Witness prefix is an element count, not byte length.
        stack.reserve(source.read_size(max_block_weight));

        for (size_t element = 0; element < stack.capacity(); ++element)
            stack.emplace_back(read_element(source));
    }
    else
    {
        while (!source.is_exhausted())
            stack.emplace_back(read_element(source));
    }

    return { stack, source };
}

inline bool is_push_token(const std::string& token)
{
    return token.size() > one && token.front() == '[' && token.back() == ']';
}

inline std::string remove_token_delimiters(const std::string& token)
{
    BITCOIN_ASSERT(token.size() > one);
    return std::string(std::next(token.begin()), std::prev(token.end()));
}

// static/private
witness witness::from_string(const std::string& mnemonic)
{
    // There is always one data element per non-empty string token.
    auto tokens = split(mnemonic);

    // Split always returns at least one token, and when trimming it will be
    // empty only if there was nothing but whitespace in the mnemonic.
    if (tokens.front().empty())
        tokens.clear();

    data_chunk data;
    data_stack stack;
    stack.reserve(tokens.size());

    // Create data stack from the split tokens.
    for (const auto& token: tokens)
    {
        if (is_push_token(token) && decode_base16(data,
            remove_token_delimiters(token)))
            stack.push_back(data);
        else
            return {};
    }

    return { std::move(stack) };
}

// Serialization.
// ----------------------------------------------------------------------------

data_chunk witness::to_data(bool prefix) const
{
    data_chunk data(no_fill_byte_allocator);
    data.resize(serialized_size(prefix));
    stream::out::copy ostream(data);
    to_data(ostream, prefix);
    return data;
}

void witness::to_data(std::ostream& stream, bool prefix) const
{
    write::bytes::ostream out(stream);
    to_data(out, prefix);
}

void witness::to_data(writer& sink, bool prefix) const
{
    DEBUG_ONLY(const auto bytes = serialized_size(prefix);)
    DEBUG_ONLY(const auto start = sink.get_position();)

    // Witness prefix is an element count, not byte length (unlike script).
    if (prefix)
        sink.write_variable(stack_.size());

    // Tokens encoded as variable integer prefixed byte array (bip144).
    for (const auto& element: stack_)
    {
        sink.write_variable(element.size());
        sink.write_bytes(element);
    }

    BITCOIN_ASSERT(sink && sink.get_position() - start == bytes);
}

std::string witness::to_string() const
{
    if (!valid_)
        return "(?)";

    std::string text;
    for (const auto& element: stack_)
        text += "[" + encode_base16(element) + "] ";

    trim_right(text);
    return text;
}

// Properties.
// ----------------------------------------------------------------------------

bool witness::is_valid() const
{
    return valid_;
}

const data_stack& witness::stack() const
{
    return stack_;
}

// private
size_t witness::serialized_size() const
{
    const auto sum = [](size_t total, const data_chunk& element)
    {
        // Tokens encoded as variable integer prefixed byte array (bip144).
        const auto size = element.size();
        return total + variable_size(size) + size;
    };

    return std::accumulate(stack_.begin(), stack_.end(), zero, sum);
}

size_t witness::serialized_size(bool prefix) const
{
    // Witness prefix is an element count, not a byte length (unlike script).
    return (prefix ? variable_size(stack_.size()) : zero) + serialized_size();
}

// Utilities.
// ----------------------------------------------------------------------------

// static
bool witness::is_push_size(const data_stack& stack)
{
    const auto push_size = [](const data_chunk& element)
    {
        return element.size() <= max_push_data_size;
    };

    return std::all_of(stack.begin(), stack.end(), push_size);
}

// static
// The (only) coinbase witness must be (arbitrary) 32-byte value (bip141).
bool witness::is_reserved_pattern(const data_stack& stack)
{
    return stack.size() == 1 &&
        stack[0].size() == hash_size;
}

// This is an internal optimization over using script::to_pay_key_hash_pattern.
inline operations to_pay_key_hash(data_chunk&& program)
{
    BITCOIN_ASSERT(program.size() == short_hash_size);

    return operations
    {
        { opcode::dup },
        { opcode::hash160 },
        { std::move(program), true },
        { opcode::equalverify },
        { opcode::checksig }
    };
}

// The return script is only useful only for sigop counting.
bool witness::extract_sigop_script(script& out_script,
    const script& program_script) const
{
    // Caller may recycle script parameter.
    out_script = {};

    switch (program_script.version())
    {
        case script_version::zero:
        {
            switch (program_script.witness_program().size())
            {
                // Each p2wkh input is counted as 1 sigop (bip141).
                case short_hash_size:
                    out_script = checksig_script;
                    return true;

                // p2wsh sigops are counted as before for p2sh (bip141).
                case hash_size:
                    if (!stack_.empty())
                        out_script = { stack_.back(), false };

                    return true;

                // Undefined v0 witness script, will not validate.
                default:
                    return true;
            }
        }

        // These versions are reserved for future extensions (bip141).
        case script_version::reserved:
            return true;

        // Return false only if this is not a witness script.
        case script_version::unversioned:
        default:
            return false;
    }
}

// Extract script and initial execution stack.
bool witness::extract_script(script& out_script,
    data_stack& out_stack, const script& program_script) const
{
    auto program = program_script.witness_program();
    out_stack = stack_;

    switch (program_script.version())
    {
        case script_version::zero:
        {
            switch (program.size())
            {
                // p2wkh
                // witness stack : <signature> <public-key>
                // input script  : (empty)
                // output script : <0> <20-byte-hash-of-public-key>
                case short_hash_size:
                {
                    // Stack must be 2 elements (bip141).
                    if (out_stack.size() != 2)
                        return false;

                    // Create a pay-to-key-hash input script from the program.
                    // The hash160 of public key must match program (bip141).
                    out_script = { to_pay_key_hash(std::move(program)) };
                    return true;
                }

                // p2wsh
                // witness stack : <script> [stack-elements]
                // input script  : (empty)
                // output script : <0> <32-byte-hash-of-script>
                case hash_size:
                {
                    // The stack must consist of at least 1 element (bip141).
                    if (out_stack.empty())
                        return false;

                    // Input script is popped from the stack (bip141).
                    out_script = { pop(out_stack), false };

                    // The sha256 of popped script must match program (bip141).
                    return std::equal(program.begin(), program.end(),
                        sha256_hash(out_script.to_data(false)).begin());
                }

                // The witness extraction is invalid for v0.
                default:
                    return false;
            }
        }

        // These versions are reserved for future extensions (bip141).
        case script_version::reserved:
            return true;

        // The witness version is undefined.
        case script_version::unversioned:
        default:
            return false;
    }
}

// Validation.
// ----------------------------------------------------------------------------

// static
// The program script is either a prevout script or an embedded script.
// It validates this witness, from which the witness script is derived.
code witness::verify(const transaction& tx, uint32_t input_index,
    uint32_t forks, const script& program_script, uint64_t value) const
{
    code ec;
    script script;
    data_stack stack;
    const auto version = program_script.version();

    // Versions (and version 0) are defined by bip141.
    switch (version)
    {
        case script_version::zero:
        {
            if (!extract_script(script, stack, program_script))
                return error::invalid_witness;

            // A defined version indicates bip141 is active (not bip143).
            program witness(script, tx, input_index, forks, std::move(stack),
                value, version);

            if ((ec = witness.evaluate()))
                return ec;

            // A v0 script must succeed with a clean true stack (bip141).
            return witness.stack_result(true) ? error::script_success :
                error::stack_false;
        }

        // These versions are reserved for future extensions (bip141).
        case script_version::reserved:
            return error::success;

        case script_version::unversioned:
        default:
            return error::unversioned_script;
    }
}

} // namespace chain
} // namespace system
} // namespace libbitcoin
