/**
 * Copyright (c) 2011-2013 libbitcoin developers (see AUTHORS)
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
#include <boost/iostreams/stream.hpp>
#include <boost/test/unit_test.hpp>
#include <bitcoin/bitcoin.hpp>

using namespace bc;

BOOST_AUTO_TEST_SUITE(block_tests)

BOOST_AUTO_TEST_SUITE(block_serialization_tests)

BOOST_AUTO_TEST_CASE(from_data_fails)
{
    data_chunk data(10);
    chain::block instance;

    BOOST_REQUIRE_EQUAL(false, instance.from_data(data));
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(block__genesis__mainnet__valid_structure)
{
    const auto genesis = bc::chain::block::genesis_mainnet();
    BOOST_REQUIRE(genesis.is_valid());
    BOOST_REQUIRE_EQUAL(genesis.transactions.size(), 1u);
    BOOST_REQUIRE(genesis.header.merkle == genesis.generate_merkle_root());
}

BOOST_AUTO_TEST_CASE(block__genesis__testnet__valid_structure)
{
    const auto genesis = bc::chain::block::genesis_testnet();
    BOOST_REQUIRE(genesis.is_valid());
    BOOST_REQUIRE_EQUAL(genesis.transactions.size(), 1u);
    BOOST_REQUIRE(genesis.header.merkle == genesis.generate_merkle_root());
}

BOOST_AUTO_TEST_CASE(roundtrip_mainnet_genesis_block_serialization_factory_data_chunk)
{
    const auto genesis = bc::chain::block::genesis_mainnet();
    BOOST_REQUIRE_EQUAL(genesis.serialized_size(), 285u);
    BOOST_REQUIRE_EQUAL(genesis.header.serialized_size(false), 80u);

    // Save genesis block.
    auto raw_block = genesis.to_data();
    BOOST_REQUIRE_EQUAL(std::distance(raw_block.begin(), raw_block.end()), 285u);
    BOOST_REQUIRE_EQUAL(raw_block.size(), 285u);

    // Reload genesis block.
    const auto block = chain::block::factory_from_data(raw_block);

    BOOST_REQUIRE(block.is_valid());
    BOOST_REQUIRE(genesis.header.version == block.header.version);
    BOOST_REQUIRE(genesis.header.previous_block_hash == block.header.previous_block_hash);
    BOOST_REQUIRE(genesis.header.merkle == block.header.merkle);
    BOOST_REQUIRE(genesis.header.timestamp == block.header.timestamp);
    BOOST_REQUIRE(genesis.header.bits == block.header.bits);
    BOOST_REQUIRE(genesis.header.nonce == block.header.nonce);
    BOOST_REQUIRE(genesis.header == block.header);

    // Verify merkle root from transactions.
    BOOST_REQUIRE(genesis.header.merkle == block.generate_merkle_root());
}

BOOST_AUTO_TEST_CASE(roundtrip_mainnet_genesis_block_serialization_factory_stream)
{
    const auto genesis = bc::chain::block::genesis_mainnet();
    BOOST_REQUIRE_EQUAL(genesis.serialized_size(), 285u);
    BOOST_REQUIRE_EQUAL(genesis.header.serialized_size(false), 80u);

    // Save genesis block.
    auto raw_block = genesis.to_data();
    BOOST_REQUIRE_EQUAL(std::distance(raw_block.begin(), raw_block.end()), 285u);
    BOOST_REQUIRE_EQUAL(raw_block.size(), 285u);

    // Reload genesis block.
    data_source stream(raw_block);
    const auto block = chain::block::factory_from_data(stream);

    BOOST_REQUIRE(block.is_valid());
    BOOST_REQUIRE(genesis.header.version == block.header.version);
    BOOST_REQUIRE(genesis.header.previous_block_hash == block.header.previous_block_hash);
    BOOST_REQUIRE(genesis.header.merkle == block.header.merkle);
    BOOST_REQUIRE(genesis.header.timestamp == block.header.timestamp);
    BOOST_REQUIRE(genesis.header.bits == block.header.bits);
    BOOST_REQUIRE(genesis.header.nonce == block.header.nonce);
    BOOST_REQUIRE(genesis.header == block.header);

    // Verify merkle root from transactions.
    BOOST_REQUIRE(genesis.header.merkle == block.generate_merkle_root());
}

BOOST_AUTO_TEST_CASE(roundtrip_mainnet_genesis_block_serialization_factory_reader)
{
    const auto genesis = bc::chain::block::genesis_mainnet();
    BOOST_REQUIRE_EQUAL(genesis.serialized_size(), 285u);
    BOOST_REQUIRE_EQUAL(genesis.header.serialized_size(false), 80u);

    // Save genesis block.
    data_chunk raw_block = genesis.to_data();
    BOOST_REQUIRE_EQUAL(std::distance(raw_block.begin(), raw_block.end()), 285u);
    BOOST_REQUIRE_EQUAL(raw_block.size(), 285u);

    // Reload genesis block.
    data_source stream(raw_block);
    istream_reader reader(stream);
    const auto block = chain::block::factory_from_data(reader);

    BOOST_REQUIRE(block.is_valid());
    BOOST_REQUIRE(genesis.header.version == block.header.version);
    BOOST_REQUIRE(genesis.header.previous_block_hash == block.header.previous_block_hash);
    BOOST_REQUIRE(genesis.header.merkle == block.header.merkle);
    BOOST_REQUIRE(genesis.header.timestamp == block.header.timestamp);
    BOOST_REQUIRE(genesis.header.bits == block.header.bits);
    BOOST_REQUIRE(genesis.header.nonce == block.header.nonce);
    BOOST_REQUIRE(genesis.header == block.header);

    // Verify merkle root from transactions.
    BOOST_REQUIRE(genesis.header.merkle == block.generate_merkle_root());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(block_generate_merkle_root_tests)

BOOST_AUTO_TEST_CASE(generate_merkle_root_block_with_zero_transactions_matches_null_hash)
{
    chain::block empty;
    BOOST_REQUIRE(empty.generate_merkle_root() == null_hash);
}

BOOST_AUTO_TEST_CASE(generate_merkle_root_block_with_multiple_transactions_matches_historic_data)
{
    // encodes the 100,000 block data.
    chain::block block100k;
    block100k.header.version = 1u;
    block100k.header.previous_block_hash = hash_literal("00000000000ae3b3a73a5ffd5963e56376ad29364a75f006eeee2d053106117f");
    block100k.header.merkle = hash_literal("448211dcf4e9bf254a427e96fabce05f1744ae7b6deba8472171d9276899555f");
    block100k.header.timestamp = 1287094230;
    block100k.header.bits = 456101533;
    block100k.header.nonce = 719871722;

    block100k.transactions.emplace_back();
    block100k.transactions.back().from_data(to_chunk(base16_literal(
        "010000000100000000000000000000000000000000000000000000000000000000000"
        "00000ffffffff07049d8e2f1b0114ffffffff0100f2052a0100000043410437b36a72"
        "21bc977dce712728a954e3b5d88643ed5aef46660ddcfeeec132724cd950c1fdd008a"
        "d4a2dfd354d6af0ff155fc17c1ee9ef802062feb07ef1d065f0ac00000000")));

    block100k.transactions.emplace_back();
    block100k.transactions.back().from_data(to_chunk(base16_literal(
        "0100000001260fd102fab456d6b169f6af4595965c03c2296ecf25bfd8790e7aa29b4"
        "04eff010000008c493046022100c56ad717e07229eb93ecef2a32a42ad041832ffe66"
        "bd2e1485dc6758073e40af022100e4ba0559a4cebbc7ccb5d14d1312634664bac46f3"
        "6ddd35761edaae20cefb16f01410417e418ba79380f462a60d8dd12dcef8ebfd7ab17"
        "41c5c907525a69a8743465f063c1d9182eea27746aeb9f1f52583040b1bc341b31ca0"
        "388139f2f323fd59f8effffffff0200ffb2081d0000001976a914fc7b44566256621a"
        "ffb1541cc9d59f08336d276b88ac80f0fa02000000001976a914617f0609c9fabb545"
        "105f7898f36b84ec583350d88ac00000000")));

    block100k.transactions.emplace_back();
    block100k.transactions.back().from_data(to_chunk(base16_literal(
        "010000000122cd6da26eef232381b1a670aa08f4513e9f91a9fd129d912081a3dd138"
        "cb013010000008c4930460221009339c11b83f234b6c03ebbc4729c2633cbc8cbd0d1"
        "5774594bfedc45c4f99e2f022100ae0135094a7d651801539df110a028d65459d24bc"
        "752d7512bc8a9f78b4ab368014104a2e06c38dc72c4414564f190478e3b0d01260f09"
        "b8520b196c2f6ec3d06239861e49507f09b7568189efe8d327c3384a4e488f8c53448"
        "4835f8020b3669e5aebffffffff0200ac23fc060000001976a914b9a2c9700ff95195"
        "16b21af338d28d53ddf5349388ac00743ba40b0000001976a914eb675c349c474bec8"
        "dea2d79d12cff6f330ab48788ac00000000")));

    BOOST_REQUIRE(block100k.is_valid());

    for (const auto& tx: block100k.transactions)
    {
        BOOST_REQUIRE(tx.is_valid());

        for (const auto& input: tx.inputs)
        {
            BOOST_REQUIRE(input.is_valid());
            BOOST_REQUIRE(input.script.is_valid());
        }

        for (const auto& output: tx.outputs)
        {
            BOOST_REQUIRE(output.is_valid());
            BOOST_REQUIRE(output.script.is_valid());
        }
    }

    BOOST_REQUIRE(block100k.generate_merkle_root() == block100k.header.merkle);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(block_is_distinct_transaction_set_tests)

BOOST_AUTO_TEST_CASE(block__distinct_transactions__empty__true)
{
    chain::block value;
    BOOST_REQUIRE(value.is_distinct_transaction_set());
}

BOOST_AUTO_TEST_CASE(validate_block__is_distinct_tx_set__single__true)
{
    chain::block value;
    value.transactions.push_back({ 1, 0, {}, {} });
    BOOST_REQUIRE(value.is_distinct_transaction_set());
}

BOOST_AUTO_TEST_CASE(validate_block__is_distinct_tx_set__duplicate__false)
{
    chain::block value;
    value.transactions.push_back({ 1, 0, {}, {} });
    value.transactions.push_back({ 1, 0, {}, {} });
    BOOST_REQUIRE(!value.is_distinct_transaction_set());
}

BOOST_AUTO_TEST_CASE(validate_block__is_distinct_tx_set__distinct_by_version__true)
{
    chain::block value;
    value.transactions.push_back({ 1, 0, {}, {} });
    value.transactions.push_back({ 2, 0, {}, {} });
    value.transactions.push_back({ 3, 0, {}, {} });
    BOOST_REQUIRE(value.is_distinct_transaction_set());
}

BOOST_AUTO_TEST_CASE(validate_block__is_distinct_tx_set__partialy_distinct_by_version__false)
{
    chain::block value;
    value.transactions.push_back({ 1, 0, {}, {} });
    value.transactions.push_back({ 2, 0, {}, {} });
    value.transactions.push_back({ 2, 0, {}, {} });
    BOOST_REQUIRE(!value.is_distinct_transaction_set());
}

BOOST_AUTO_TEST_CASE(validate_block__is_distinct_tx_set__partialy_distinct_not_adjacent_by_version__false)
{
    chain::block value;
    value.transactions.push_back({ 1, 0, {}, {} });
    value.transactions.push_back({ 2, 0, {}, {} });
    value.transactions.push_back({ 1, 0, {}, {} });
    BOOST_REQUIRE(!value.is_distinct_transaction_set());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
