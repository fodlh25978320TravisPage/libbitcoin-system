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
#ifndef LIBBITCOIN_SYSTEM_ERROR_ERROR_CODE_HPP
#define LIBBITCOIN_SYSTEM_ERROR_ERROR_CODE_HPP

#include <string>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {
namespace error {

enum error_t
{
    // general
    success = 0,
    unknown,
    not_found,
    not_implemented,
    oversubscribed,
    file_system,

    // network
    listen_failed,
    accept_failed,
    resolve_failed,
    connect_failed,
    channel_timeout,
    channel_stopped,
    service_stopped,
    operation_canceled,
    operation_failed,
    address_in_use,
    bad_stream,
    address_blocked,
    peer_throttling,

    // database
    store_block_duplicate,
    store_block_invalid_height,
    store_block_missing_parent,
    store_incorrect_state,
    store_lock_failure,

    // blockchain
    duplicate_block,
    orphan_block,
    invalid_previous_block,
    insufficient_work,
    duplicate_transaction,
    orphan_transaction,
    transaction_version,
    insufficient_fee,
    stale_chain,
    dusty_transaction,

    // check header
    invalid_proof_of_work,
    futuristic_timestamp,

    // accept header
    checkpoints_failed,
    invalid_block_version,
    incorrect_proof_of_work,
    timestamp_too_early,

    // check block
    block_size_limit,
    empty_block,
    first_not_coinbase,
    extra_coinbases,
    internal_duplicate,
    block_internal_double_spend,
    forward_reference,
    merkle_mismatch,
    block_legacy_sigop_limit,

    // accept block
    block_non_final,
    coinbase_height_mismatch,
    coinbase_value_limit,
    block_embedded_sigop_limit,
    invalid_witness_commitment,
    block_weight_limit,
    temporary_hash_limit,
    unspent_coinbase_collision,

    // check transaction
    empty_transaction,
    previous_output_null,
    spend_overflow,
    invalid_coinbase_script_size,
    coinbase_transaction,
    transaction_internal_double_spend,
    transaction_size_limit,
    transaction_legacy_sigop_limit,

    // accept transaction
    transaction_non_final,
    premature_validation,
    unspent_duplicate,
    missing_previous_output,
    double_spend,
    coinbase_maturity,
    spend_exceeds_value,
    transaction_embedded_sigop_limit,
    sequence_locked,
    transaction_weight_limit,

    // connect input
    invalid_script,
    invalid_script_size,
    invalid_push_data_size,
    invalid_operation_count,
    invalid_stack_size,
    invalid_stack_scope,
    invalid_script_embed,
    invalid_signature_encoding,
    incorrect_signature,
    unexpected_witness,
    invalid_witness,
    dirty_witness,
    stack_false,

    // op eval
    op_invalid,
    op_reserved,
    op_push_size,
    op_push_data,
    op_if,
    op_notif,
    op_else,
    op_endif,
    op_verify1,
    op_verify2,
    op_return,
    op_to_alt_stack,
    op_from_alt_stack,
    op_drop2,
    op_dup2,
    op_dup3,
    op_over2,
    op_rot2,
    op_swap2,
    op_if_dup,
    op_drop,
    op_dup,
    op_nip,
    op_over,
    op_pick,
    op_roll,
    op_rot,
    op_swap,
    op_tuck,
    op_size,
    op_equal,
    op_equal_verify1,
    op_equal_verify2,
    op_add1,
    op_sub1,
    op_negate,
    op_abs,
    op_not,
    op_nonzero,
    op_add,
    op_sub,
    op_bool_and,
    op_bool_or,
    op_num_equal,
    op_num_equal_verify1,
    op_num_equal_verify2,
    op_num_not_equal,
    op_less_than,
    op_greater_than,
    op_less_than_or_equal,
    op_greater_than_or_equal,
    op_min,
    op_max,
    op_within,
    op_ripemd160,
    op_sha1,
    op_sha256,
    op_hash160,
    op_hash256,
    op_code_seperator,
    op_check_sig_verify1,
    op_check_sig,
    op_check_multisig_verify1,
    op_check_multisig_verify2,
    op_check_multisig_verify3,
    op_check_multisig_verify4,
    op_check_multisig_verify5,
    op_check_multisig_verify6,
    op_check_multisig_verify7,
    op_check_multisig,
    op_check_locktime_verify1,
    op_check_locktime_verify2,
    op_check_locktime_verify3,
    op_check_locktime_verify4,
    op_check_locktime_verify5,
    op_check_locktime_verify6,
    op_check_sequence_verify1,
    op_check_sequence_verify2,
    op_check_sequence_verify3,
    op_check_sequence_verify4,
    op_check_sequence_verify5,
    op_check_sequence_verify6,
    op_check_sequence_verify7,
    op_check_multisig_verify8,

    // http [TODO: isolate to http service]
    http_invalid_request,
    http_method_not_found,
    http_internal_error,

    ////// server [not implemented]
    ////unrecognized_filter_type,
    ////invalid_response_range,
    ////configuration_disabled,
    ////prevout_missing,

    maxumum_error_code
};

// The store retains codes in 1 byte fields.
static_assert(maxumum_error_code < 256, "error code capacity");

} // namespace error
} // namespace system
} // namespace libbitcoin

namespace std {

// Make eligible for std::error_code/std::error_condition automatic conversions.
template <>
struct is_error_code_enum<bc::system::error::error_t>
  : public true_type
{
};

// Required by is_error_code_enum, see: std::error_code source.
std::error_code make_error_code(bc::system::error::error_t value) noexcept;

} // namespace std

#endif
