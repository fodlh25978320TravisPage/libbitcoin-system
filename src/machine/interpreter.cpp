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
#include <bitcoin/system/machine/interpreter.hpp>

#include <cstdint>
#include <utility>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/chain/operation.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/error/error.hpp>
#include <bitcoin/system/machine/number.hpp>
#include <bitcoin/system/machine/program.hpp>
#include <bitcoin/system/math/math.hpp>

namespace libbitcoin {
namespace system {
namespace machine {

using namespace system::chain;

// Operations (shared).
// ----------------------------------------------------------------------------

interpreter::result interpreter::op_unevaluated(opcode code) noexcept
{
    return operation::is_invalid(code) ? error::op_invalid : error::op_reserved;
}

// Codes op_nop1..op_nop10 promoted from reserved by [0.3.6] hard fork.
interpreter::result interpreter::op_nop(program& program, opcode) noexcept
{
    if (program.is_enabled(forks::nops_rule))
        return error::op_success;

    // TODO: nops_rule *must* be enabled in test cases and default config.
    // TODO: cats_rule should be enabled in test cases and default config.
    return error::op_success;
    ////return op_unevaluated(code);
}

interpreter::result interpreter::op_push_number(program& program,
    uint8_t value) noexcept
{
    program.push_move({ value });
    return error::op_success;
}

interpreter::result interpreter::op_push_size(program& program,
    const operation& op) noexcept
{
    static constexpr auto op_75 = static_cast<uint8_t>(opcode::push_size_75);

    if (op.data().size() > op_75)
        return error::op_push_size;

    program.push_ref(op.data_ptr());
    return error::op_success;
}

interpreter::result interpreter::op_push_data(program& program,
    chunk_ptr&& data, uint32_t size_limit) noexcept
{
    if (data->size() > size_limit)
        return error::op_push_data;

    program.push_ref(std::move(data));
    return error::op_success;
}

// Operations (not shared).
// ----------------------------------------------------------------------------
// All index parameters are zero-based and relative to stack top.

interpreter::result interpreter::op_nop(opcode) noexcept
{
    return error::op_success;
}

interpreter::result interpreter::op_ver(program& program) noexcept
{
    if (program.is_enabled(forks::nops_rule))
        return op_unevaluated(opcode::op_ver);

    return error::op_not_implemented;
}

interpreter::result interpreter::op_if(program& program) noexcept
{
    auto value = false;

    if (program.succeeded())
    {
        if (program.empty())
            return error::op_if;

        value = program.stack_true(false);
        program.pop();
    }

    program.open(value);
    return error::op_success;
}

interpreter::result interpreter::op_notif(program& program) noexcept
{
    auto value = false;

    if (program.succeeded())
    {
        if (program.empty())
            return error::op_notif;

        value = !program.stack_true(false);
        program.pop();
    }

    program.open(value);
    return error::op_success;
}

interpreter::result interpreter::op_verif(program& program) noexcept
{
    if (program.is_enabled(forks::nops_rule))
        return op_unevaluated(opcode::op_verif);

    return error::op_not_implemented;
}

interpreter::result interpreter::op_vernotif(program& program) noexcept
{
    if (program.is_enabled(forks::nops_rule))
        return op_unevaluated(opcode::op_vernotif);

    return error::op_not_implemented;
}

interpreter::result interpreter::op_else(program& program) noexcept
{
    if (program.closed())
        return error::op_else;

    program.negate();
    return error::op_success;
}

interpreter::result interpreter::op_endif(program& program) noexcept
{
    if (program.closed())
        return error::op_endif;

    program.close();
    return error::op_success;
}

interpreter::result interpreter::op_verify(program& program) noexcept
{
    if (program.empty())
        return error::op_verify1;

    if (!program.stack_true(false))
        return error::op_verify2;

    program.pop();
    return error::op_success;
}

interpreter::result interpreter::op_return(program& program) noexcept
{
    if (program.is_enabled(forks::nops_rule))
        return op_unevaluated(opcode::op_return);
        
    return error::op_not_implemented;
}

interpreter::result interpreter::op_to_alt_stack(program& program) noexcept
{
    if (program.empty())
        return error::op_to_alt_stack;

    program.push_alternate(program.pop_ref());
    return error::op_success;
}

interpreter::result interpreter::op_from_alt_stack(program& program) noexcept
{
    if (program.empty_alternate())
        return error::op_from_alt_stack;

    program.push_ref(program.pop_alternate());
    return error::op_success;
}

interpreter::result interpreter::op_drop2(program& program) noexcept
{
    if (program.size() < 2)
        return error::op_drop2;

    program.pop();
    program.pop();
    return error::op_success;
}

interpreter::result interpreter::op_dup2(program& program) noexcept
{
    if (program.size() < 2)
        return error::op_dup2;

    auto item1 = program.item(1);
    auto item0 = program.item(0);

    program.push_ref(std::move(item1));
    program.push_ref(std::move(item0));
    return error::op_success;
}

interpreter::result interpreter::op_dup3(program& program) noexcept
{
    if (program.size() < 3)
        return error::op_dup3;

    auto item2 = program.item(2);
    auto item1 = program.item(1);
    auto item0 = program.item(0);

    program.push_ref(std::move(item2));
    program.push_ref(std::move(item1));
    program.push_ref(std::move(item0));
    return error::op_success;
}

interpreter::result interpreter::op_over2(program& program) noexcept
{
    if (program.size() < 4)
        return error::op_over2;

    auto item3 = program.item(3);
    auto item2 = program.item(2);

    program.push_ref(std::move(item3));
    program.push_ref(std::move(item2));
    return error::op_success;
}

interpreter::result interpreter::op_rot2(program& program) noexcept
{
    if (program.size() < 6)
        return error::op_rot2;

    const auto position5 = program.position(5);
    const auto position4 = program.position(4);

    auto item5 = *position5;
    auto item4 = *position4;

    program.erase(position5, add1(position4));
    program.push_ref(std::move(item5));
    program.push_ref(std::move(item4));
    return error::op_success;
}

interpreter::result interpreter::op_swap2(program& program) noexcept
{
    if (program.size() < 4)
        return error::op_swap2;

    program.swap(3, 1);
    program.swap(2, 0);
    return error::op_success;
}

interpreter::result interpreter::op_if_dup(program& program) noexcept
{
    if (program.empty())
        return error::op_if_dup;

    if (program.stack_true(false))
        program.duplicate(0);

    return error::op_success;
}

interpreter::result interpreter::op_depth(program& program) noexcept
{
    program.push_move(number(program.size()).data());
    return error::op_success;
}

interpreter::result interpreter::op_drop(program& program) noexcept
{
    if (program.empty())
        return error::op_drop;

    program.pop();
    return error::op_success;
}

interpreter::result interpreter::op_dup(program& program) noexcept
{
    if (program.empty())
        return error::op_dup;

    program.duplicate(0);
    return error::op_success;
}

interpreter::result interpreter::op_nip(program& program) noexcept
{
    if (program.size() < 2)
        return error::op_nip;

    program.erase(program.position(1));
    return error::op_success;
}

interpreter::result interpreter::op_over(program& program) noexcept
{
    if (program.size() < 2)
        return error::op_over;

    program.duplicate(1);
    return error::op_success;
}

interpreter::result interpreter::op_pick(program& program) noexcept
{
    program::stack_iterator position;
    if (!program.pop_position(position))
        return error::op_pick;

    program.push_ref(*position);
    return error::op_success;
}

interpreter::result interpreter::op_roll(program& program) noexcept
{
    program::stack_iterator position;
    if (!program.pop_position(position))
        return error::op_roll;

    auto copy = *position;
    program.erase(position);
    program.push_ref(std::move(copy));
    return error::op_success;
}

interpreter::result interpreter::op_rot(program& program) noexcept
{
    if (program.size() < 3)
        return error::op_rot;

    program.swap(2, 1);
    program.swap(1, 0);
    return error::op_success;
}

interpreter::result interpreter::op_swap(program& program) noexcept
{
    if (program.size() < 2)
        return error::op_swap;

    program.swap(1, 0);
    return error::op_success;
}

interpreter::result interpreter::op_tuck(program& program) noexcept
{
    if (program.size() < 2)
        return error::op_tuck;

    auto first = program.pop_ref();
    auto second = program.pop_ref();
    program.push_ref(first);
    program.push_ref(std::move(second));
    program.push_ref(std::move(first));
    return error::op_success;
}

interpreter::result interpreter::op_cat(program& program) noexcept
{
    if (program.is_enabled(forks::cats_rule))
        return op_unevaluated(opcode::op_cat);

    return error::op_not_implemented;
}

interpreter::result interpreter::op_substr(program& program) noexcept
{
    if (program.is_enabled(forks::cats_rule))
        return op_unevaluated(opcode::op_substr);

    return error::op_not_implemented;
}

interpreter::result interpreter::op_left(program& program) noexcept
{
    if (program.is_enabled(forks::cats_rule))
        return op_unevaluated(opcode::op_left);

    return error::op_not_implemented;
}

interpreter::result interpreter::op_right(program& program) noexcept
{
    if (program.is_enabled(forks::cats_rule))
        return op_unevaluated(opcode::op_right);

    return error::op_not_implemented;
}

interpreter::result interpreter::op_size(program& program) noexcept
{
    if (program.empty())
        return error::op_size;

    auto top = program.pop_ref();
    const auto size = top->size();
    program.push_ref(std::move(top));
    program.push_move(number(size).data());
    return error::op_success;
}

interpreter::result interpreter::op_invert(program& program) noexcept
{
    if (program.is_enabled(forks::cats_rule))
        return op_unevaluated(opcode::op_invert);

    return error::op_not_implemented;
}

interpreter::result interpreter::op_and(program& program) noexcept
{
    if (program.is_enabled(forks::cats_rule))
        return op_unevaluated(opcode::op_and);

    return error::op_not_implemented;
}

interpreter::result interpreter::op_or(program& program) noexcept
{
    if (program.is_enabled(forks::cats_rule))
        return op_unevaluated(opcode::op_or);

    return error::op_not_implemented;
}

interpreter::result interpreter::op_xor(program& program) noexcept
{
    if (program.is_enabled(forks::cats_rule))
        return op_unevaluated(opcode::op_xor);

    return error::op_not_implemented;
}

interpreter::result interpreter::op_equal(program& program) noexcept
{
    if (program.size() < 2)
        return error::op_equal;

    program.push(program.pop() == program.pop());
    return error::op_success;
}

interpreter::result interpreter::op_equal_verify(program& program) noexcept
{
    if (program.size() < 2)
        return error::op_equal_verify1;

    return (program.pop() == program.pop()) ? error::op_success :
        error::op_equal_verify2;
}

interpreter::result interpreter::op_add1(program& program) noexcept
{
    number number;
    if (!program.pop(number))
        return error::op_add1;

    number += 1;
    program.push_move(number.data());
    return error::op_success;
}

interpreter::result interpreter::op_sub1(program& program) noexcept
{
    number number;
    if (!program.pop(number))
        return error::op_sub1;

    number -= 1;
    program.push_move(number.data());
    return error::op_success;
}

interpreter::result interpreter::op_mul2(program& program) noexcept
{
    if (program.is_enabled(forks::cats_rule))
        return op_unevaluated(opcode::op_mul2);

    return error::op_not_implemented;
}

interpreter::result interpreter::op_div2(program& program) noexcept
{
    if (program.is_enabled(forks::cats_rule))
        return op_unevaluated(opcode::op_div2);

    return error::op_not_implemented;
}

interpreter::result interpreter::op_negate(program& program) noexcept
{
    number number;
    if (!program.pop(number))
        return error::op_negate;

    number = -number;
    program.push_move(number.data());
    return error::op_success;
}

interpreter::result interpreter::op_abs(program& program) noexcept
{
    number number;
    if (!program.pop(number))
        return error::op_abs;

    if (number < 0)
        number = -number;

    program.push_move(number.data());
    return error::op_success;
}

interpreter::result interpreter::op_not(program& program) noexcept
{
    number number;
    if (!program.pop(number))
        return error::op_not;

    program.push(number.is_false());
    return error::op_success;
}

interpreter::result interpreter::op_nonzero(program& program) noexcept
{
    number number;
    if (!program.pop(number))
        return error::op_nonzero;

    program.push(number.is_true());
    return error::op_success;
}

interpreter::result interpreter::op_add(program& program) noexcept
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_add;

    const auto result = first + second;
    program.push_move(result.data());
    return error::op_success;
}

interpreter::result interpreter::op_sub(program& program) noexcept
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_sub;

    const auto result = second - first;
    program.push_move(result.data());
    return error::op_success;
}

interpreter::result interpreter::op_mul(program& program) noexcept
{
    if (program.is_enabled(forks::cats_rule))
        return op_unevaluated(opcode::op_mul);

    return error::op_not_implemented;
}

interpreter::result interpreter::op_div(program& program) noexcept
{
    if (program.is_enabled(forks::cats_rule))
        return op_unevaluated(opcode::op_div);

    return error::op_not_implemented;
}

interpreter::result interpreter::op_mod(program& program) noexcept
{
    if (program.is_enabled(forks::cats_rule))
        return op_unevaluated(opcode::op_mod);

    return error::op_not_implemented;
}

interpreter::result interpreter::op_lshift(program& program) noexcept
{
    if (program.is_enabled(forks::cats_rule))
        return op_unevaluated(opcode::op_lshift);

    return error::op_not_implemented;
}

interpreter::result interpreter::op_rshift(program& program) noexcept
{
    if (program.is_enabled(forks::cats_rule))
        return op_unevaluated(opcode::op_rshift);

    return error::op_not_implemented;
}

interpreter::result interpreter::op_bool_and(program& program) noexcept
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_bool_and;

    program.push(first.is_true() && second.is_true());
    return error::op_success;
}

interpreter::result interpreter::op_bool_or(program& program) noexcept
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_bool_or;

    program.push(first.is_true() || second.is_true());
    return error::op_success;
}

interpreter::result interpreter::op_num_equal(program& program) noexcept
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_num_equal;

    program.push(first == second);
    return error::op_success;
}

interpreter::result interpreter::op_num_equal_verify(program& program) noexcept
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_num_equal_verify1;

    return (first == second) ? error::op_success :
        error::op_num_equal_verify2;
}

interpreter::result interpreter::op_num_not_equal(program& program) noexcept
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_num_not_equal;

    program.push(first != second);
    return error::op_success;
}

interpreter::result interpreter::op_less_than(program& program) noexcept
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_less_than;

    program.push(second < first);
    return error::op_success;
}

interpreter::result interpreter::op_greater_than(program& program) noexcept
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_greater_than;

    program.push(second > first);
    return error::op_success;
}

interpreter::result interpreter::op_less_than_or_equal(program& program) noexcept
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_less_than_or_equal;

    program.push(second <= first);
    return error::op_success;
}

interpreter::result interpreter::op_greater_than_or_equal(
    program& program) noexcept
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_greater_than_or_equal;

    program.push(second >= first);
    return error::op_success;
}

interpreter::result interpreter::op_min(program& program) noexcept
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_min;

    if (second < first)
        program.push_move(second.data());
    else
        program.push_move(first.data());

    return error::op_success;
}

interpreter::result interpreter::op_max(program& program) noexcept
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_max;

    if (second > first)
        program.push_move(second.data());
    else
        program.push_move(first.data());

    return error::op_success;
}

interpreter::result interpreter::op_within(program& program) noexcept
{
    number first, second, third;
    if (!program.pop_ternary(first, second, third))
        return error::op_within;

    program.push(second <= third && third < first);
    return error::op_success;
}

interpreter::result interpreter::op_ripemd160(program& program) noexcept
{
    if (program.empty())
        return error::op_ripemd160;

    program.push_move(ripemd160_hash_chunk(program.pop()));
    return error::op_success;
}

interpreter::result interpreter::op_sha1(program& program) noexcept
{
    if (program.empty())
        return error::op_sha1;

    program.push_move(sha1_hash_chunk(program.pop()));
    return error::op_success;
}

interpreter::result interpreter::op_sha256(program& program) noexcept
{
    if (program.empty())
        return error::op_sha256;

    program.push_move(sha256_hash_chunk(program.pop()));
    return error::op_success;
}

interpreter::result interpreter::op_hash160(program& program) noexcept
{
    if (program.empty())
        return error::op_hash160;

    program.push_move(ripemd160_hash_chunk(sha256_hash(program.pop())));
    return error::op_success;
}

interpreter::result interpreter::op_hash256(program& program) noexcept
{
    if (program.empty())
        return error::op_hash256;

    program.push_move(sha256_hash_chunk(sha256_hash(program.pop())));
    return error::op_success;
}

interpreter::result interpreter::op_codeseparator(program& program,
    const operation& op) noexcept
{
    return program.set_subscript(op) ? error::op_success :
        error::op_code_separator;
}

// In signing mode, prepare_signature converts key from a private key to
// a public key and generates the signature from key and hash. The signature is
// then verified against the key and hash as if obtained from the script.
interpreter::result interpreter::op_check_sig_verify(program& program) noexcept
{
    if (program.empty())
        return error::op_check_sig_verify1;

    const auto key = program.pop_ref();

    if (key->empty())
        return error::op_check_sig_verify2;

    if (program.empty())
        return error::op_check_sig_verify3;

    const auto endorsement = program.pop_ref();

    // error::op_check_sig_verify_parse causes op_check_sig fail.
    if (endorsement->empty())
        return error::op_check_sig_verify4;

    hash_digest hash;
    ec_signature sig;

    // Parse endorsement into DER signature into an EC signature.
    // Also generates signature hash from endorsement sighash flags.
    // Under bip66 op_check_sig fails if parsed endorsement is not strict DER.
    if (!program.prepare(sig, *key, hash, endorsement))
        return error::op_check_sig_verify_parse;

    // TODO: for signing mode - make key mutable and return above.
    return system::verify_signature(*key, hash, sig) ?
        error::op_success : error::op_check_sig_verify5;
}

interpreter::result interpreter::op_check_sig(program& program) noexcept
{
    auto verify = op_check_sig_verify(program);
    auto bip66 = program.is_enabled(forks::bip66_rule);

    // BIP66: invalid signature encoding fails the operation.
    if (bip66 && verify == error::op_check_sig_verify_parse)
        return error::op_check_sig;

    program.push(verify == error::op_success);
    return error::op_success;
}

interpreter::result interpreter::op_check_multisig_verify(
    program& program) noexcept
{
    auto bip147 = program.is_enabled(forks::bip147_rule);

    int32_t count;
    if (!program.pop(count))
        return error::op_check_multisig_verify1;

    if (!program.increment_op_count(count))
        return error::op_check_multisig_verify2;

    chunk_ptrs keys;
    if (!program.pop(keys, count))
        return error::op_check_multisig_verify3;

    if (!program.pop(count))
        return error::op_check_multisig_verify4;

    if (is_greater(count, keys.size()))
        return error::op_check_multisig_verify5;

    chunk_ptrs endorsements;
    if (!program.pop(endorsements, count))
        return error::op_check_multisig_verify6;

    if (program.empty())
        return error::op_check_multisig_verify7;

    //*************************************************************************
    // CONSENSUS: Satoshi bug, discard stack element, malleable until bip147.
    //*************************************************************************
    if (!program.pop().empty() && bip147)
        return error::op_check_multisig_verify8;

    uint8_t flags;
    ec_signature sig;
    program::hash_cache cache;

    // Subscript is the same for all signatures.
    const auto sub = program.subscript(endorsements);
    auto endorsement = endorsements.begin();

    // Keys may be empty, endorsements is an ordered subset of corresponding
    // keys, all endorsements must be verified against a key. Under bip66,
    // op_check_multisig fails if any parsed endorsement is not strict DER.
    for (const auto& key: keys)
    {
        // All signatures are valid (empty does not increment the iterator).
        if (endorsement == endorsements.end())
            break;

        // error::op_check_multisig_verify_parse causes op_check_multisig fail.
        if (!(*endorsement)->empty())
        {
            // Parse endorsement into DER signature into an EC signature.
            // Also generates signature hash from endorsement sighash flags.
            if (!program.prepare(sig, *key, cache, flags, **endorsement, *sub))
                return error::op_check_multisig_verify_parse;

            // TODO: for signing mode - make key mutable and return above.
            if (system::verify_signature(*key, cache.at(flags), sig))
                ++endorsement;
        }
    }

    return endorsement != endorsements.end() ?
        error::op_check_multisig_verify9 : error::op_success;
}

interpreter::result interpreter::op_check_multisig(program& program) noexcept
{
    auto verify = op_check_multisig_verify(program);
    auto bip66 = program.is_enabled(forks::bip66_rule);

    // BIP66: invalid signature encoding fails the operation.
    if (bip66 && verify == error::op_check_multisig_verify_parse)
        return error::op_check_multisig;

    program.push(verify == error::op_success);
    return error::op_success;
}

interpreter::result interpreter::op_check_locktime_verify(
    program& program) noexcept
{
    // BIP65: nop2 subsumed by checklocktimeverify when bip65 fork is active.
    if (!program.is_enabled(forks::bip65_rule))
        return op_nop(program, opcode::nop2);

    // BIP65: the tx sequence is 0xffffffff.
    if (program.input().is_final())
        return error::op_check_locktime_verify1;

    // BIP65: the stack is empty.
    // BIP65: extend the (signed) script number range to 5 bytes.
    number stack;
    if (!program.top(stack, max_check_locktime_verify_number_size))
        return error::op_check_locktime_verify2;

    // BIP65: the top stack item is negative.
    if (stack.is_negative())
        return error::op_check_locktime_verify3;

    // The top stack item is positive, so cast is safe.
    const auto locktime = static_cast<uint64_t>(stack.int64());
    const auto& tx_locktime = program.transaction().locktime();

    // BIP65: the stack locktime type differs from that of tx.
    if ((locktime < locktime_threshold) != (tx_locktime < locktime_threshold))
        return error::op_check_locktime_verify4;

    // BIP65: the stack locktime is greater than the tx locktime.
    return (locktime > tx_locktime) ? error::op_check_locktime_verify5 :
        error::op_success;
}

interpreter::result interpreter::op_check_sequence_verify(
    program& program) noexcept
{
    // BIP112: nop3 subsumed by checksequenceverify when bip112 fork is active.
    if (!program.is_enabled(forks::bip112_rule))
        return op_nop(program, opcode::nop3);

    // BIP112: the stack is empty.
    // BIP112: extend the (signed) script number range to 5 bytes.
    number stack;
    if (!program.top(stack, max_check_sequence_verify_number_size))
        return error::op_check_sequence_verify1;

    // BIP112: the top stack item is negative.
    if (stack.is_negative())
        return error::op_check_sequence_verify2;

    // The top stack item is positive, and only 32 bits are ever tested.
    const auto sequence = static_cast<uint32_t>(absolute(stack.int64()));
    const auto tx_sequence = program.input().sequence();

    // BIP112: the stack sequence is disabled, treat as nop3.
    if (get_right(sequence, relative_locktime_disabled_bit))
        return op_nop(program, opcode::nop3);

    // BIP112: the stack sequence is enabled and tx version less than 2.
    if (program.transaction().version() < relative_locktime_min_version)
        return error::op_check_sequence_verify3;

    // BIP112: the transaction sequence is disabled.
    if (get_right(tx_sequence, relative_locktime_disabled_bit))
        return error::op_check_sequence_verify4;

    // BIP112: the stack sequence type differs from that of tx input.
    if (get_right(sequence, relative_locktime_time_locked_bit) !=
        get_right(tx_sequence, relative_locktime_time_locked_bit))
        return error::op_check_sequence_verify5;

    // BIP112: the unmasked stack sequence is greater than that of tx sequence.
    return (mask_left(sequence, relative_locktime_mask_left)) >
        (mask_left(tx_sequence, relative_locktime_mask_left)) ?
        error::op_check_sequence_verify6 : error::op_success;
}

// private:
// It is expected that the compiler will produce a very efficient jump table.
interpreter::result interpreter::run_op(const operation& op,
    program& program) noexcept
{
    const auto code = op.code();

    switch (code)
    {
        case opcode::push_size_0:
        case opcode::push_size_1:
        case opcode::push_size_2:
        case opcode::push_size_3:
        case opcode::push_size_4:
        case opcode::push_size_5:
        case opcode::push_size_6:
        case opcode::push_size_7:
        case opcode::push_size_8:
        case opcode::push_size_9:
        case opcode::push_size_10:
        case opcode::push_size_11:
        case opcode::push_size_12:
        case opcode::push_size_13:
        case opcode::push_size_14:
        case opcode::push_size_15:
        case opcode::push_size_16:
        case opcode::push_size_17:
        case opcode::push_size_18:
        case opcode::push_size_19:
        case opcode::push_size_20:
        case opcode::push_size_21:
        case opcode::push_size_22:
        case opcode::push_size_23:
        case opcode::push_size_24:
        case opcode::push_size_25:
        case opcode::push_size_26:
        case opcode::push_size_27:
        case opcode::push_size_28:
        case opcode::push_size_29:
        case opcode::push_size_30:
        case opcode::push_size_31:
        case opcode::push_size_32:
        case opcode::push_size_33:
        case opcode::push_size_34:
        case opcode::push_size_35:
        case opcode::push_size_36:
        case opcode::push_size_37:
        case opcode::push_size_38:
        case opcode::push_size_39:
        case opcode::push_size_40:
        case opcode::push_size_41:
        case opcode::push_size_42:
        case opcode::push_size_43:
        case opcode::push_size_44:
        case opcode::push_size_45:
        case opcode::push_size_46:
        case opcode::push_size_47:
        case opcode::push_size_48:
        case opcode::push_size_49:
        case opcode::push_size_50:
        case opcode::push_size_51:
        case opcode::push_size_52:
        case opcode::push_size_53:
        case opcode::push_size_54:
        case opcode::push_size_55:
        case opcode::push_size_56:
        case opcode::push_size_57:
        case opcode::push_size_58:
        case opcode::push_size_59:
        case opcode::push_size_60:
        case opcode::push_size_61:
        case opcode::push_size_62:
        case opcode::push_size_63:
        case opcode::push_size_64:
        case opcode::push_size_65:
        case opcode::push_size_66:
        case opcode::push_size_67:
        case opcode::push_size_68:
        case opcode::push_size_69:
        case opcode::push_size_70:
        case opcode::push_size_71:
        case opcode::push_size_72:
        case opcode::push_size_73:
        case opcode::push_size_74:
        case opcode::push_size_75:
            return op_push_size(program, op);
        case opcode::push_one_size:
            return op_push_data(program, op.data_ptr(), max_uint8);
        case opcode::push_two_size:
            return op_push_data(program, op.data_ptr(), max_uint16);
        case opcode::push_four_size:
            return op_push_data(program, op.data_ptr(), max_uint32);
        case opcode::push_negative_1:
            return op_push_number(program, numbers::negative_1);
        case opcode::reserved_80:
            return op_unevaluated(code);
        case opcode::push_positive_1:
            return op_push_number(program, numbers::positive_1);
        case opcode::push_positive_2:
            return op_push_number(program, numbers::positive_2);
        case opcode::push_positive_3:
            return op_push_number(program, numbers::positive_3);
        case opcode::push_positive_4:
            return op_push_number(program, numbers::positive_4);
        case opcode::push_positive_5:
            return op_push_number(program, numbers::positive_5);
        case opcode::push_positive_6:
            return op_push_number(program, numbers::positive_6);
        case opcode::push_positive_7:
            return op_push_number(program, numbers::positive_7);
        case opcode::push_positive_8:
            return op_push_number(program, numbers::positive_8);
        case opcode::push_positive_9:
            return op_push_number(program, numbers::positive_9);
        case opcode::push_positive_10:
            return op_push_number(program, numbers::positive_10);
        case opcode::push_positive_11:
            return op_push_number(program, numbers::positive_11);
        case opcode::push_positive_12:
            return op_push_number(program, numbers::positive_12);
        case opcode::push_positive_13:
            return op_push_number(program, numbers::positive_13);
        case opcode::push_positive_14:
            return op_push_number(program, numbers::positive_14);
        case opcode::push_positive_15:
            return op_push_number(program, numbers::positive_15);
        case opcode::push_positive_16:
            return op_push_number(program, numbers::positive_16);
        case opcode::nop:
            return op_nop(code);
        case opcode::op_ver:
            return op_ver(program);
        case opcode::if_:
            return op_if(program);
        case opcode::notif:
            return op_notif(program);
        case opcode::op_verif:
            return op_verif(program);
        case opcode::op_vernotif:
            return op_vernotif(program);
        case opcode::else_:
            return op_else(program);
        case opcode::endif:
            return op_endif(program);
        case opcode::verify:
            return op_verify(program);
        case opcode::op_return:
            return op_return(program);
        case opcode::toaltstack:
            return op_to_alt_stack(program);
        case opcode::fromaltstack:
            return op_from_alt_stack(program);
        case opcode::drop2:
            return op_drop2(program);
        case opcode::dup2:
            return op_dup2(program);
        case opcode::dup3:
            return op_dup3(program);
        case opcode::over2:
            return op_over2(program);
        case opcode::rot2:
            return op_rot2(program);
        case opcode::swap2:
            return op_swap2(program);
        case opcode::ifdup:
            return op_if_dup(program);
        case opcode::depth:
            return op_depth(program);
        case opcode::drop:
            return op_drop(program);
        case opcode::dup:
            return op_dup(program);
        case opcode::nip:
            return op_nip(program);
        case opcode::over:
            return op_over(program);
        case opcode::pick:
            return op_pick(program);
        case opcode::roll:
            return op_roll(program);
        case opcode::rot:
            return op_rot(program);
        case opcode::swap:
            return op_swap(program);
        case opcode::tuck:
            return op_tuck(program);
        case opcode::op_cat:
            return op_cat(program);
        case opcode::op_substr:
            return op_substr(program);
        case opcode::op_left:
            return op_left(program);
        case opcode::op_right:
            return op_right(program);
        case opcode::size:
            return op_size(program);
        case opcode::op_invert:
            return op_invert(program);
        case opcode::op_and:
            return op_and(program);
        case opcode::op_or:
            return op_or(program);
        case opcode::op_xor:
            return op_xor(program);
        case opcode::equal:
            return op_equal(program);
        case opcode::equalverify:
            return op_equal_verify(program);
        case opcode::reserved_137:
            return op_unevaluated(code);
        case opcode::reserved_138:
            return op_unevaluated(code);
        case opcode::add1:
            return op_add1(program);
        case opcode::sub1:
            return op_sub1(program);
        case opcode::op_mul2:
            return op_mul2(program);
        case opcode::op_div2:
            return op_div2(program);
        case opcode::negate:
            return op_negate(program);
        case opcode::abs:
            return op_abs(program);
        case opcode::not_:
            return op_not(program);
        case opcode::nonzero:
            return op_nonzero(program);
        case opcode::add:
            return op_add(program);
        case opcode::sub:
            return op_sub(program);
        case opcode::op_mul:
            return op_mul(program);
        case opcode::op_div:
            return op_div(program);
        case opcode::op_mod:
            return op_mod(program);
        case opcode::op_lshift:
            return op_lshift(program);
        case opcode::op_rshift:
            return op_rshift(program);
        case opcode::booland:
            return op_bool_and(program);
        case opcode::boolor:
            return op_bool_or(program);
        case opcode::numequal:
            return op_num_equal(program);
        case opcode::numequalverify:
            return op_num_equal_verify(program);
        case opcode::numnotequal:
            return op_num_not_equal(program);
        case opcode::lessthan:
            return op_less_than(program);
        case opcode::greaterthan:
            return op_greater_than(program);
        case opcode::lessthanorequal:
            return op_less_than_or_equal(program);
        case opcode::greaterthanorequal:
            return op_greater_than_or_equal(program);
        case opcode::min:
            return op_min(program);
        case opcode::max:
            return op_max(program);
        case opcode::within:
            return op_within(program);
        case opcode::ripemd160:
            return op_ripemd160(program);
        case opcode::sha1:
            return op_sha1(program);
        case opcode::sha256:
            return op_sha256(program);
        case opcode::hash160:
            return op_hash160(program);
        case opcode::hash256:
            return op_hash256(program);
        case opcode::codeseparator:
            return op_codeseparator(program, op);
        case opcode::checksig:
            return op_check_sig(program);
        case opcode::checksigverify:
            return op_check_sig_verify(program);
        case opcode::checkmultisig:
            return op_check_multisig(program);
        case opcode::checkmultisigverify:
            return op_check_multisig_verify(program);
        case opcode::nop1:
            return op_nop(program, code);
        case opcode::checklocktimeverify:
            return op_check_locktime_verify(program);
        case opcode::checksequenceverify:
            return op_check_sequence_verify(program);
        case opcode::nop4:
        case opcode::nop5:
        case opcode::nop6:
        case opcode::nop7:
        case opcode::nop8:
        case opcode::nop9:
        case opcode::nop10:
            return op_nop(program, code);
        default:
            return op_unevaluated(code);
    }
}

code interpreter::run(program& program) noexcept
{
    result ec;

    // Enforce script size limit (10,000) [0.3.7+].
    // Enforce initial primary stack size limit (520) [bip141].
    if (program.is_invalid())
        return error::invalid_script;

    for (const auto& op: program)
    {
        // Enforce push data limit (520) [0.3.6+].
        if (op.is_oversized())
            return error::invalid_push_data_size;

        // Enforce unconditionally invalid opcodes ("disabled").
        if (op.is_invalid())
            return error::op_invalid;

        // Enforce opcode count limit (201).
        if (!program.increment_op_count(op))
            return error::invalid_operation_count;

        // Conditional evaluation scope.
        if (program.if_(op))
        {
            // Evaluate opcode (switch).
            if ((ec = run_op(op, program)))
                return ec;

            // Enforce combined stacks size limit (1,000).
            if (program.is_stack_overflow())
                return error::invalid_stack_size;
        }
    }

    // Guard against unclosed evaluation scope.
    return program.closed() ? error::script_success :
        error::invalid_stack_scope;
}

} // namespace machine
} // namespace system
} // namespace libbitcoin
