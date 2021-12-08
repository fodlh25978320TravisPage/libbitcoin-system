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
#include "../test.hpp"

BOOST_AUTO_TEST_SUITE(input_tests)

using namespace bc::system::chain;

const auto valid_raw_input = to_chunk(base16_array(
    "54b755c39207d443fd96a8d12c94446a1c6f66e39c95e894c23418d7501f681b01000"
    "0006b48304502203267910f55f2297360198fff57a3631be850965344370f732950b4"
    "7795737875022100f7da90b82d24e6e957264b17d3e5042bab8946ee5fc676d15d915"
    "da450151d36012103893d5a06201d5cf61400e96fa4a7514fc12ab45166ace618d68b"
    "8066c9c585f9ffffffff"));

BOOST_AUTO_TEST_CASE(input__constructor_1__always__default_initialized)
{
    input instance;
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(input__constructor_2__valid_input__input_initialized)
{
    const point previous_output{ null_hash, 5434u };
    script script;
    BOOST_REQUIRE(script.from_data(base16_chunk("ece424a6bb6ddf4db592c0faed60685047a361b1"), false));

    uint32_t sequence = 4568656u;

    input instance(previous_output, script, sequence);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(previous_output == instance.point());
    BOOST_REQUIRE(script == instance.script());
    BOOST_REQUIRE_EQUAL(sequence, instance.sequence());
}

BOOST_AUTO_TEST_CASE(input__constructor_3__valid_input__input_initialized)
{
    const point previous_output{ null_hash, 5434u };
    script script;
    BOOST_REQUIRE(script.from_data(base16_chunk("ece424a6bb6ddf4db592c0faed60685047a361b1"), false));

    uint32_t sequence = 4568656u;

    auto dup_previous_output = previous_output;
    auto dup_script = script;
    input instance(std::move(dup_previous_output), std::move(dup_script), sequence);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(previous_output == instance.point());
    BOOST_REQUIRE(script == instance.script());
    BOOST_REQUIRE_EQUAL(sequence, instance.sequence());
}

BOOST_AUTO_TEST_CASE(input__constructor_4__valid_input__input_initialized)
{
    input expected;
    BOOST_REQUIRE(expected.from_data(valid_raw_input));

    input instance(expected);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(input__constructor_5__valid_input__input_initialized)
{
    input expected;
    BOOST_REQUIRE(expected.from_data(valid_raw_input));

    input instance(std::move(expected));
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(input__from_data__insufficient_data__failure)
{
    data_chunk data(2);
    input instance;
    BOOST_REQUIRE(!instance.from_data(data));
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(input__from_data__valid_data__success)
{
    const auto junk = base16_array("000000000000005739943a9c29a1955dfae2b3f37de547005bfb9535192e5fb0000000000000005739943a9c29a1955dfae2b3f37de547005bfb9535192e5fb0");
    input instance;
    read::bytes::copy reader(junk);
    BOOST_REQUIRE(instance.from_data(reader));
}

BOOST_AUTO_TEST_CASE(input__factory_1__valid_input__success)
{
    const input instance(valid_raw_input);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(instance.serialized_size(false), valid_raw_input.size());

    // Re-save and compare against original.
    const auto resave = instance.to_data();
    BOOST_REQUIRE_EQUAL(resave.size(), valid_raw_input.size());
    BOOST_REQUIRE(resave == valid_raw_input);
}

BOOST_AUTO_TEST_CASE(input__factory_2__valid_input__success)
{
    stream::in::copy stream(valid_raw_input);
    const input instance(stream);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(instance.serialized_size(false), valid_raw_input.size());

    // Re-save and compare against original.
    const auto resave = instance.to_data();
    BOOST_REQUIRE_EQUAL(resave.size(), valid_raw_input.size());
    BOOST_REQUIRE(resave == valid_raw_input);
}

BOOST_AUTO_TEST_CASE(input__factory_3__valid_input__success)
{
    read::bytes::copy source(valid_raw_input);
    const input instance(valid_raw_input);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(instance.serialized_size(false), valid_raw_input.size());

    // Re-save and compare against original.
    const auto resave = instance.to_data();
    BOOST_REQUIRE_EQUAL(resave.size(), valid_raw_input.size());
    BOOST_REQUIRE(resave == valid_raw_input);
}

BOOST_AUTO_TEST_CASE(input__is_final__max_input_sequence__true)
{
    const input instance({}, {}, max_input_sequence);
    BOOST_REQUIRE(instance.is_final());
}

BOOST_AUTO_TEST_CASE(input__is_final__sequence_zero__false)
{
    const input instance({}, {}, 0);
    BOOST_REQUIRE(!instance.is_final());
}

BOOST_AUTO_TEST_CASE(input__is_locked__enabled_block_type_sequence_age_equals_minimum__false)
{
    static const auto age = 7u;
    static const auto sequence_enabled_block_type_minimum = age;
    input instance({}, {}, sequence_enabled_block_type_minimum);
    auto& prevout = instance.prevout;
    prevout.height = 42;
    BOOST_REQUIRE(!instance.is_locked(prevout.height + age, 0));
}

BOOST_AUTO_TEST_CASE(input__is_locked__enabled_block_type_sequence_age_above_minimum__false)
{
    static const auto age = 7u;
    static const auto sequence_enabled_block_type_minimum = age - 1;
    input instance({}, {}, sequence_enabled_block_type_minimum);
    auto& prevout = instance.prevout;
    prevout.height = 42;
    BOOST_REQUIRE(!instance.is_locked(prevout.height + age, 0));
}

BOOST_AUTO_TEST_CASE(input__is_locked__enabled_block_type_sequence_age_below_minimum__true)
{
    static const auto age = 7u;
    static const auto sequence_enabled_block_type_minimum = age + 1;
    input instance({}, {}, sequence_enabled_block_type_minimum);
    auto& prevout = instance.prevout;
    prevout.height = 42;
    BOOST_REQUIRE(instance.is_locked(prevout.height + age, 0));
}

BOOST_AUTO_TEST_CASE(input__is_locked__disabled_block_type_sequence_age_below_minimum__false)
{
    static const auto age = 7u;
    static const auto sequence_disabled_block_type_minimum = bit_right<uint32_t>(relative_locktime_disabled_bit) | add1(age);
    input instance({}, {}, sequence_disabled_block_type_minimum);
    auto& prevout = instance.prevout;
    prevout.height = 42;
    BOOST_REQUIRE(!instance.is_locked(prevout.height + age, 0));
}

BOOST_AUTO_TEST_CASE(input__is_locked__enabled_time_type_sequence_age_equals_minimum__false)
{
    static const auto age = 7u;
    static const auto age_seconds = 7u << relative_locktime_seconds_shift_left;
    static const auto sequence_enabled_time_type_minimum = bit_right<uint32_t>(relative_locktime_time_locked_bit) | age;
    input instance({}, {}, sequence_enabled_time_type_minimum);
    auto& prevout = instance.prevout;
    prevout.median_time_past = 42;
    BOOST_REQUIRE(!instance.is_locked(0, prevout.median_time_past + age_seconds));
}

BOOST_AUTO_TEST_CASE(input__is_locked__enabled_time_type_sequence_age_above_minimum__false)
{
    static const auto age = 7u;
    static const auto age_seconds = 7u << relative_locktime_seconds_shift_left;
    static const auto sequence_enabled_time_type_minimum = bit_right<uint32_t>(relative_locktime_time_locked_bit) | sub1(age);
    input instance({}, {}, sequence_enabled_time_type_minimum);
    auto& prevout = instance.prevout;
    prevout.median_time_past = 42;
    BOOST_REQUIRE(!instance.is_locked(0, prevout.median_time_past + age_seconds));
}

BOOST_AUTO_TEST_CASE(input__is_locked__enabled_time_type_sequence_age_below_minimum__true)
{
    static const auto age = 7u;
    static const auto age_seconds = 7u << relative_locktime_seconds_shift_left;
    static const auto sequence_enabled_time_type_minimum = bit_right<uint32_t>(relative_locktime_time_locked_bit) | add1(age);
    input instance({}, {}, sequence_enabled_time_type_minimum);
    auto& prevout = instance.prevout;
    prevout.median_time_past = 42;
    BOOST_REQUIRE(instance.is_locked(0, prevout.median_time_past + age_seconds));
}

BOOST_AUTO_TEST_CASE(input__is_locked__disabled_time_type_sequence_age_below_minimum__false)
{
    static const auto age = 7u;
    static const auto age_seconds = 7u << relative_locktime_seconds_shift_left;
    static const auto sequence_disabled_time_type_minimum = bit_right<uint32_t>(relative_locktime_disabled_bit) | bit_right<uint32_t>(relative_locktime_time_locked_bit) | add1(age);
    input instance({}, {}, sequence_disabled_time_type_minimum);
    auto& prevout = instance.prevout;
    prevout.median_time_past = 42;
    BOOST_REQUIRE(!instance.is_locked(0, prevout.median_time_past + age_seconds));
}

BOOST_AUTO_TEST_CASE(input__signature_operations__bip16_inactive__script_sigops)
{
    const auto raw_script = base16_chunk("02acad");
    script script;
    BOOST_REQUIRE(script.from_data(raw_script, true));
    input instance{ {}, script, chain::max_input_sequence };
    BOOST_REQUIRE_EQUAL(script.sigops(false), instance.signature_operations(false, false));
}

BOOST_AUTO_TEST_CASE(input__signature_operations__bip16_active_cache_empty__script_sigops)
{
    const auto raw_script = base16_chunk("02acad");
    script script;
    BOOST_REQUIRE(script.from_data(raw_script, true));
    input instance{ {}, script, chain::max_input_sequence };
    BOOST_REQUIRE_EQUAL(script.sigops(false), instance.signature_operations(true, false));
}

BOOST_AUTO_TEST_CASE(input__operator_assign_equals_1__always__matches_equivalent)
{
    input expected;
    BOOST_REQUIRE(expected.from_data(valid_raw_input));
    const input instance(valid_raw_input);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(input__operator_assign_equals_2__always__matches_equivalent)
{
    input expected;
    BOOST_REQUIRE(expected.from_data(valid_raw_input));
    input instance;
    instance = expected;
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(input__operator_boolean_equals__duplicates__true)
{
    input alpha;
    input beta;
    BOOST_REQUIRE(alpha.from_data(valid_raw_input));
    BOOST_REQUIRE(beta.from_data(valid_raw_input));
    BOOST_REQUIRE(alpha == beta);
}

BOOST_AUTO_TEST_CASE(input__operator_boolean_equals__differs__false)
{
    input alpha;
    input beta;
    BOOST_REQUIRE(alpha.from_data(valid_raw_input));
    BOOST_REQUIRE(!(alpha == beta));
}

BOOST_AUTO_TEST_CASE(input__operator_boolean_not_equals__duplicates__false)
{
    input alpha;
    input beta;
    BOOST_REQUIRE(alpha.from_data(valid_raw_input));
    BOOST_REQUIRE(beta.from_data(valid_raw_input));
    BOOST_REQUIRE(!(alpha != beta));
}

BOOST_AUTO_TEST_CASE(input__operator_boolean_not_equals__differs__true)
{
    input alpha;
    input beta;
    BOOST_REQUIRE(alpha.from_data(valid_raw_input));
    BOOST_REQUIRE(alpha != beta);
}

BOOST_AUTO_TEST_SUITE_END()
