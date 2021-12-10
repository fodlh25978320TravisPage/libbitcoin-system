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
#include "../test.hpp"

BOOST_AUTO_TEST_SUITE(operation_tests)

using namespace bc::system::chain;

const auto op_data = base16_chunk("0900ff11ee22bb33aa44");
const auto expected_op = operation(op_data);
const auto op_data_underflow = base16_chunk("0900ff11ee22bb33");
const auto push_data_32 = base16_chunk(
    "000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");

// constructors
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(operation__constructor__default__empty_invalid)
{
    const operation instance;
    BOOST_REQUIRE(instance.data().empty());
    BOOST_REQUIRE(instance.is_invalid());
}

BOOST_AUTO_TEST_CASE(operation__constructor__move__expected)
{
    const operation expected(push_data_32, false);
    auto copy = expected;
    const operation instance(std::move(copy));
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(operation__constructor__copy__expected)
{
    const operation expected(push_data_32, false);
    const operation instance(expected);
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(operation__constructor__opcode_checklocktimeverify__empty_valid)
{
    const operation instance(opcode::checklocktimeverify);
    BOOST_REQUIRE(instance.data().empty());
    BOOST_REQUIRE(!instance.is_invalid());
}

BOOST_AUTO_TEST_CASE(operation__constructor__opcode_xor__empty_invalid)
{
    const operation instance(opcode::op_xor);
    BOOST_REQUIRE(instance.data().empty());
    BOOST_REQUIRE(instance.is_invalid());
}

BOOST_AUTO_TEST_CASE(operation__constructor__opcode_push_two_size___empty)
{
    const operation instance(opcode::push_two_size);

    // If code is push data the data member will be inconsistent (empty).
    BOOST_REQUIRE(instance.data().empty());
    BOOST_REQUIRE(!instance.is_invalid());
}

BOOST_AUTO_TEST_CASE(operation__constructor__push_data_move__expected)
{
    auto copy = push_data_32;
    const operation instance(std::move(copy), false);
    BOOST_REQUIRE(instance.code() == opcode::push_size_32);
    BOOST_REQUIRE_EQUAL(instance.data(), push_data_32);
}

BOOST_AUTO_TEST_CASE(operation__constructor__push_data_move_minimal__expected)
{
    const operation instance(data_chunk{ 0x01 }, true);

    // minimal: push_positive_1 <= 0x51 (numbers::positive_1) on the stack.
    BOOST_REQUIRE(instance.code() == opcode::push_positive_1);
    BOOST_REQUIRE(instance.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__constructor__push_data_move_non_minimal__expected)
{
    const data_chunk expected{ 0x51 };
    auto copy = expected;
    const operation instance(data_chunk{ 0x51 }, false);

    // non-minimal: [1.01] <= 0x51 on the stack.
    BOOST_REQUIRE(instance.code() == opcode::push_size_1);
    BOOST_REQUIRE_EQUAL(instance.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__constructor__push_data_copy__expected)
{
    const operation instance(push_data_32, false);
    BOOST_REQUIRE(instance.code() == opcode::push_size_32);
    BOOST_REQUIRE_EQUAL(instance.data(), push_data_32);
}

BOOST_AUTO_TEST_CASE(operation__constructor__push_data_copy_minimal__expected)
{
    const data_chunk expected{ 0x01 };
    const operation instance(expected, true);

    // minimal: push_positive_1 <= 0x51 (numbers::positive_1) on the stack.
    BOOST_REQUIRE(instance.code() == opcode::push_positive_1);
    BOOST_REQUIRE(instance.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__constructor__push_data_copy_non_minimal__expected)
{
    const data_chunk expected{ 0x51 };
    const operation instance(expected, false);

    // non-minimal: [1.51] <= 0x51 on the stack.
    BOOST_REQUIRE(instance.code() == opcode::push_size_1);
    BOOST_REQUIRE_EQUAL(instance.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__constructor__op_data__expected)
{
    const operation instance(op_data);
    BOOST_REQUIRE_EQUAL(instance.to_data(), op_data);
}

BOOST_AUTO_TEST_CASE(operation__constructor__stream__expected)
{
    stream::in::copy istream(op_data);
    const operation instance(istream);
    BOOST_REQUIRE_EQUAL(instance.to_data(), op_data);
}

BOOST_AUTO_TEST_CASE(operation__constructor__reader__expected)
{
    read::bytes::copy source(op_data);
    const operation instance(source);
    BOOST_REQUIRE_EQUAL(instance.to_data(), op_data);
}

// operators
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(operation__assign__move__expected)
{
    const operation alpha(op_data);
    const auto beta = operation(op_data);
    BOOST_REQUIRE(beta == alpha);
}

BOOST_AUTO_TEST_CASE(operation__assign__copy__expected)
{
    const operation alpha(op_data);
    const auto beta = alpha;
    BOOST_REQUIRE(beta == alpha);
}

BOOST_AUTO_TEST_CASE(operation__equality__same__expected)
{
    const operation alpha(op_data);
    const operation beta(op_data);
    BOOST_REQUIRE(alpha == beta);
}

BOOST_AUTO_TEST_CASE(operation__equality__different__expected)
{
    const operation alpha;
    const operation beta(op_data);
    BOOST_REQUIRE(!(alpha == beta));
}

BOOST_AUTO_TEST_CASE(operation__inequality__same__expected)
{
    const operation alpha(op_data);
    const operation beta(op_data);
    BOOST_REQUIRE(!(alpha != beta));
}

BOOST_AUTO_TEST_CASE(operation__inequality__different__expected)
{
    const operation alpha;
    const operation beta(op_data);
    BOOST_REQUIRE(alpha != beta);
}

// from_data
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(operation__from_data__empty__underflow)
{
    operation instance;
    BOOST_REQUIRE(instance.from_data({}));
    BOOST_REQUIRE(instance.is_underflow());
}

BOOST_AUTO_TEST_CASE(operation__from_data__push_size_0__expected)
{
    const auto data0 = base16_chunk("");
    const auto data = base16_chunk("00");
    operation instance;

    BOOST_REQUIRE(instance.from_data(data));
    BOOST_REQUIRE_EQUAL(instance.to_data(), data);

    operation duplicate;
    BOOST_REQUIRE(duplicate.from_data(instance.to_data()));
    BOOST_REQUIRE(instance == duplicate);

    BOOST_REQUIRE(instance.code() == opcode::push_size_0);
    BOOST_REQUIRE_EQUAL(instance.data(), data0);
}

BOOST_AUTO_TEST_CASE(operation__from_data__push_size_75__expected)
{
    const auto data75 = data_chunk(75, '.');
    const auto data = splice(base16_array("4b"), data75);
    operation instance;

    BOOST_REQUIRE(instance.from_data(data));
    BOOST_REQUIRE_EQUAL(instance.to_data(), data);

    operation duplicate;
    BOOST_REQUIRE(duplicate.from_data(instance.to_data()));
    BOOST_REQUIRE(instance == duplicate);

    BOOST_REQUIRE(instance.code() == opcode::push_size_75);
    BOOST_REQUIRE(instance.data() == data75);
}

BOOST_AUTO_TEST_CASE(operation__from_data__push_negative_1__expected)
{
    const auto op_79 = static_cast<uint8_t>(opcode::push_negative_1);
    const auto data = data_chunk{ op_79 };
    operation instance;

    BOOST_REQUIRE(instance.from_data(data));
    BOOST_REQUIRE_EQUAL(instance.to_data(), data);

    operation duplicate;
    BOOST_REQUIRE(duplicate.from_data(instance.to_data()));
    BOOST_REQUIRE(instance == duplicate);

    // The code is the data for numeric push codes.
    BOOST_REQUIRE(instance.code() == opcode::push_negative_1);
    BOOST_REQUIRE(instance.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_data__push_positive_1__expected)
{
    const auto op_81 = static_cast<uint8_t>(opcode::push_positive_1);
    const auto data = data_chunk{ op_81 };
    operation instance;

    // This is read as an encoded operation, not as data.
    // Constructors read (unencoded) data and can select minimal encoding.
    BOOST_REQUIRE(instance.from_data(data));
    BOOST_REQUIRE_EQUAL(instance.to_data(), data);

    operation duplicate;
    BOOST_REQUIRE(duplicate.from_data(instance.to_data()));
    BOOST_REQUIRE(instance == duplicate);

    // The code is the data for numeric push codes.
    BOOST_REQUIRE(instance.code() == opcode::push_positive_1);
    BOOST_REQUIRE(instance.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_data__push_positive_16__expected)
{
    const auto op_96 = static_cast<uint8_t>(opcode::push_positive_16);
    const auto data = data_chunk{ op_96 };
    operation instance;

    // This is read as an encoded operation, not as data.
    // Constructors read (unencoded) data and can select minimal encoding.
    BOOST_REQUIRE(instance.from_data(data));
    BOOST_REQUIRE_EQUAL(instance.to_data(), data);

    operation duplicate;
    BOOST_REQUIRE(duplicate.from_data(instance.to_data()));
    BOOST_REQUIRE(instance == duplicate);

    // The code is the data for numeric push codes.
    BOOST_REQUIRE(instance.code() == opcode::push_positive_16);
    BOOST_REQUIRE(instance.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_data__push_one_size__expected)
{
    const auto data255 = data_chunk(255, '.');
    const auto data = splice(base16_array("4c" "ff"), data255);
    operation instance;

    BOOST_REQUIRE(instance.from_data(data));
    BOOST_REQUIRE_EQUAL(instance.to_data(), data);

    operation duplicate;
    BOOST_REQUIRE(duplicate.from_data(instance.to_data()));
    BOOST_REQUIRE(instance == duplicate);

    BOOST_REQUIRE(instance.code() == opcode::push_one_size);
    BOOST_REQUIRE_EQUAL(instance.data(), data255);
}

BOOST_AUTO_TEST_CASE(operation__from_data__push_two_size__expected)
{
    const auto data520 = data_chunk(520, '.');
    const auto data = splice(base16_array("4d" "0802"), data520);
    operation instance;

    BOOST_REQUIRE(instance.from_data(data));
    BOOST_REQUIRE_EQUAL(instance.to_data(), data);

    operation duplicate;
    BOOST_REQUIRE(duplicate.from_data(instance.to_data()));
    BOOST_REQUIRE(instance == duplicate);

    BOOST_REQUIRE(instance.code() == opcode::push_two_size);
    BOOST_REQUIRE_EQUAL(instance.data(), data520);
}

BOOST_AUTO_TEST_CASE(operation__from_data__push_four_size__expected)
{
    const auto data520 = data_chunk(520, '.');
    const auto data = splice(base16_array("4e" "08020000"), data520);
    operation instance;

    BOOST_REQUIRE(instance.from_data(data));
    BOOST_REQUIRE_EQUAL(instance.to_data(), data);

    operation duplicate;
    BOOST_REQUIRE(duplicate.from_data(instance.to_data()));
    BOOST_REQUIRE(instance == duplicate);

    BOOST_REQUIRE(instance.code() == opcode::push_four_size);
    BOOST_REQUIRE_EQUAL(instance.data(), data520);
}

BOOST_AUTO_TEST_CASE(operation__from_data__data__valid)
{
    operation instance;
    BOOST_REQUIRE(instance.from_data(op_data));
    BOOST_REQUIRE(instance == expected_op);
}

BOOST_AUTO_TEST_CASE(operation__from_data__stream__valid)
{
    operation instance;
    stream::in::copy stream(op_data);
    BOOST_REQUIRE(instance.from_data(stream));
    BOOST_REQUIRE(instance == expected_op);
}

BOOST_AUTO_TEST_CASE(operation__from_data__reader__valid)
{
    operation instance;
    read::bytes::copy source(op_data);
    BOOST_REQUIRE(instance.from_data(source));
    BOOST_REQUIRE(instance == expected_op);
}

// to_data
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(operation__to_data__empty__underflow)
{
    const operation instance(data_chunk{});
    BOOST_REQUIRE(instance.is_invalid());
    BOOST_REQUIRE(instance.is_underflow());
    BOOST_REQUIRE(instance.data().empty());
    BOOST_REQUIRE(instance.to_data().empty());
}

BOOST_AUTO_TEST_CASE(input__to_data__data__expected)
{
    const auto size = expected_op.to_data().size();
    BOOST_REQUIRE_EQUAL(size, expected_op.serialized_size());
}

BOOST_AUTO_TEST_CASE(input__to_data__stream__expected)
{
    // Write input to stream.
    std::stringstream iostream;
    expected_op.to_data(iostream);
    BOOST_REQUIRE(iostream);

    // Verify stream contents.
    const operation copy(iostream);
    BOOST_REQUIRE(iostream);
    BOOST_REQUIRE(!copy.is_invalid());
    BOOST_REQUIRE(!copy.is_underflow());
    BOOST_REQUIRE(copy == expected_op);
}

BOOST_AUTO_TEST_CASE(input__to_data__writer__expected)
{
    // Write input to stream.
    std::stringstream iostream;
    write::bytes::ostream out(iostream);
    expected_op.to_data(out);
    BOOST_REQUIRE(iostream);

    // Verify stream contents.
    const operation copy(iostream);
    BOOST_REQUIRE(iostream);
    BOOST_REQUIRE(!copy.is_invalid());
    BOOST_REQUIRE(!copy.is_underflow());
    BOOST_REQUIRE(copy == expected_op);
}

// from_string
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(operation__from_string__negative_1__push_negative_1_empty)
{
    operation value;
    BOOST_REQUIRE(value.from_string("-1"));
    BOOST_REQUIRE(value.code() == opcode::push_negative_1);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__0__push_size_0_empty)
{
    operation value;
    BOOST_REQUIRE(value.from_string("0"));
    BOOST_REQUIRE(value.code() == opcode::push_size_0);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__1__push_positive_1_empty)
{
    operation value;
    BOOST_REQUIRE(value.from_string("1"));
    BOOST_REQUIRE(value.code() == opcode::push_positive_1);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__16__push_positive_16_empty)
{
    operation value;
    BOOST_REQUIRE(value.from_string("16"));
    BOOST_REQUIRE(value.code() == opcode::push_positive_16);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__17__push_size_1_expected)
{
    const data_chunk expected{ 0x11 };
    operation value;
    BOOST_REQUIRE(value.from_string("17"));
    BOOST_REQUIRE(value.code() == opcode::push_size_1);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__negative_2__push_size_1_expected)
{
    const data_chunk expected{ 0x82 };
    operation value;
    BOOST_REQUIRE(value.from_string("-2"));
    BOOST_REQUIRE(value.code() == opcode::push_size_1);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__9223372036854775807__push_size_8_expected)
{
    const data_chunk expected{ { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f } };
    operation value;
    BOOST_REQUIRE(value.from_string("9223372036854775807"));
    BOOST_REQUIRE(value.code() == opcode::push_size_8);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__negative_9223372036854775807__push_size_8_expected)
{
    const data_chunk expected{ { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff } };
    operation value;
    BOOST_REQUIRE(value.from_string("-9223372036854775807"));
    BOOST_REQUIRE(value.code() == opcode::push_size_8);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__string_empty__push_size_0_empty)
{
    const data_chunk expected{ 0x61 };
    operation value;
    BOOST_REQUIRE(value.from_string("''"));
    BOOST_REQUIRE(value.code() == opcode::push_size_0);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__string_a__push_size_1_expected_byte)
{
    const data_chunk expected{ 0x61 };
    operation value;
    BOOST_REQUIRE(value.from_string("'a'"));
    BOOST_REQUIRE(value.code() == opcode::push_size_1);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__string_abc__push_size_3_expected_byte_order)
{
    const data_chunk expected{ { 0x61, 0x62, 0x63 } };
    operation value;
    BOOST_REQUIRE(value.from_string("'abc'"));
    BOOST_REQUIRE(value.code() == opcode::push_size_3);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__negative_1_character__push_size_1_nominal_encoding)
{
    const data_chunk expected{ 0x4f };
    operation value;
    BOOST_REQUIRE(value.from_string("'O'"));
    BOOST_REQUIRE(value.code() == opcode::push_size_1);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__empty_underflow__expected)
{
    operation value;
    BOOST_REQUIRE(value.from_string("<>"));
    BOOST_REQUIRE(value.is_invalid());
    BOOST_REQUIRE(value.is_underflow());
    BOOST_REQUIRE(value.data().empty());
    BOOST_REQUIRE(value.to_data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__non_empty_underflow__expected)
{
    const data_chunk expected{ { 0x61, 0x62, 0x63 } };
    operation value;
    BOOST_REQUIRE(value.from_string("<616263>"));
    BOOST_REQUIRE(value.is_invalid());
    BOOST_REQUIRE(value.is_underflow());
    BOOST_REQUIRE_EQUAL(value.data(), expected);
    BOOST_REQUIRE_EQUAL(value.to_data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__push_0__push_size_0)
{
    operation value;
    BOOST_REQUIRE(value.from_string("push_0"));
    BOOST_REQUIRE(value.code() == opcode::push_size_0);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__push_1__empty)
{
    operation value;
    BOOST_REQUIRE(value.from_string("push_1"));
    BOOST_REQUIRE(value.code() == opcode::push_size_1);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__push_75__empty)
{
    operation value;
    BOOST_REQUIRE(value.from_string("push_75"));
    BOOST_REQUIRE(value.code() == opcode::push_size_75);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__push_one__push_one_size_empty)
{
    operation value;
    BOOST_REQUIRE(value.from_string("push_one"));
    BOOST_REQUIRE(value.code() == opcode::push_one_size);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__push_two__push_two_size_empty)
{
    operation value;
    BOOST_REQUIRE(value.from_string("push_two"));
    BOOST_REQUIRE(value.code() == opcode::push_two_size);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__push_four__push_four_size_empty)
{
    operation value;
    BOOST_REQUIRE(value.from_string("push_four"));
    BOOST_REQUIRE(value.code() == opcode::push_four_size);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__7__push_positive_7)
{
    operation value;
    BOOST_REQUIRE(value.from_string("7"));
    BOOST_REQUIRE(value.code() == opcode::push_positive_7);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__empty__push_size_0)
{
    operation value;
    BOOST_REQUIRE(value.from_string("[]"));
    BOOST_REQUIRE(value.code() == opcode::push_size_0);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__0x07__push_size_1)
{
    const data_chunk expected{ 0x07 };
    operation value;
    BOOST_REQUIRE(value.from_string("[07]"));
    BOOST_REQUIRE(value.code() == opcode::push_size_1);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__0x42__push_size_1)
{
    const data_chunk expected{ 0x42 };
    operation value;
    BOOST_REQUIRE(value.from_string("[42]"));
    BOOST_REQUIRE(value.code() == opcode::push_size_1);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__0x112233__push_size_3)
{
    const data_chunk expected{ { 0x11, 0x22, 0x33 } };
    operation value;
    BOOST_REQUIRE(value.from_string("[112233]"));
    BOOST_REQUIRE(value.code() == opcode::push_size_3);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__0_0x112233__push_size_3)
{
    const data_chunk expected{ { 0x11, 0x22, 0x33 } };
    operation value;
    BOOST_REQUIRE(value.from_string("[0.112233]"));
    BOOST_REQUIRE(value.code() == opcode::push_size_3);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__1_0x112233__push_one_size)
{
    const data_chunk expected{ { 0x11, 0x22, 0x33 } };
    operation value;
    BOOST_REQUIRE(value.from_string("[1.112233]"));
    BOOST_REQUIRE(value.code() == opcode::push_one_size);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__2_0x112233__push_two_size)
{
    const data_chunk expected{ { 0x11, 0x22, 0x33 } };
    operation value;
    BOOST_REQUIRE(value.from_string("[2.112233]"));
    BOOST_REQUIRE(value.code() == opcode::push_two_size);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__4_0x112233__push_four_size)
{
    const data_chunk expected{ { 0x11, 0x22, 0x33 } };
    operation value;
    BOOST_REQUIRE(value.from_string("[4.112233]"));
    BOOST_REQUIRE(value.code() == opcode::push_four_size);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__5_0x112233__false)
{
    operation value;
    BOOST_REQUIRE(!value.from_string("[5.112233]"));
}

BOOST_AUTO_TEST_CASE(operation__from_string__empty_0x112233__false)
{
    operation value;
    BOOST_REQUIRE(!value.from_string("[.112233]"));
}

BOOST_AUTO_TEST_CASE(operation__from_string__nop2__nop2_checklocktimeverify)
{
    operation value;
    BOOST_REQUIRE(value.from_string("nop2"));
    BOOST_REQUIRE(value.code() == opcode::nop2);
    BOOST_REQUIRE(value.code() == opcode::checklocktimeverify);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__checklocktimeverify__nop2_checklocktimeverify)
{
    operation value;
    BOOST_REQUIRE(value.from_string("checklocktimeverify"));
    BOOST_REQUIRE(value.code() == opcode::nop2);
    BOOST_REQUIRE(value.code() == opcode::checklocktimeverify);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__nop3__nop3_checksequenceverify)
{
    operation value;
    BOOST_REQUIRE(value.from_string("nop3"));
    BOOST_REQUIRE(value.code() == opcode::nop3);
    BOOST_REQUIRE(value.code() == opcode::checksequenceverify);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__checklocktimeverify__nop3_checksequenceverify)
{
    operation value;
    BOOST_REQUIRE(value.from_string("checksequenceverify"));
    BOOST_REQUIRE(value.code() == opcode::nop3);
    BOOST_REQUIRE(value.code() == opcode::checksequenceverify);
    BOOST_REQUIRE(value.data().empty());
}

// to_string
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(operation__to_string__push_size_0__zero)
{
    const operation value(opcode::push_size_0);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "zero");
}

BOOST_AUTO_TEST_CASE(operation__to_string__push_size_75__push_75)
{
    // Empty data allows the push code to serialize as an op code.
    const operation value(opcode::push_size_75);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "push_75");
}

BOOST_AUTO_TEST_CASE(operation__to_string__push_positive_7__7)
{
    const operation value(opcode::push_positive_7);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "7");
}

BOOST_AUTO_TEST_CASE(operation__to_string_minimal__0x07__7)
{
    const operation value(data_chunk{ 0x07 }, true);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "7");
}

BOOST_AUTO_TEST_CASE(operation__to_string_nominal__empty__zero)
{
    const operation value(data_chunk{}, false);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "zero");
}

BOOST_AUTO_TEST_CASE(operation__to_string_nominal__0x07__0x07)
{
    const operation value(data_chunk{ 0x07 }, false);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[07]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__0x42__0x42)
{
    const operation value(data_chunk{ 0x42 }, true);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[42]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__0x112233__0x112233)
{
    const operation value(data_chunk{ 0x11, 0x22, 0x33 }, true);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[112233]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__push_size_3__0x112233__0x112233)
{
    const data_chunk encoded{ { 0x03, 0x11, 0x22, 0x33 } };
    operation value;
    value.from_data(encoded);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[112233]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__push_one_size_0x112233__1_0x112233)
{
    const data_chunk encoded{ { 0x4c, 0x03, 0x11, 0x22, 0x33 } };
    operation value;
    value.from_data(encoded);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[1.112233]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__push_two_size_0x112233__2_0x112233)
{
    const data_chunk encoded{ { 0x4d, 0x03, 0x00, 0x11, 0x22, 0x33 } };
    operation value;
    value.from_data(encoded);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[2.112233]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__push_four_size_0x112233__4_0x112233)
{
    const data_chunk encoded{ { 0x4e, 0x03, 0x00, 0x00, 0x00, 0x11, 0x22, 0x33 } };
    operation value;
    value.from_data(encoded);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[4.112233]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__empty_underflow__empty)
{
    operation value;
    value.from_data({});
    BOOST_REQUIRE(value.is_invalid());
    BOOST_REQUIRE(value.is_underflow());
    BOOST_REQUIRE_EQUAL(value.to_string(0), "<>");
}

BOOST_AUTO_TEST_CASE(operation__to_string__non_empty_underflow__empty)
{
    // Should be two bytes of push data.
    const data_chunk encoded{ { 0x02, 0xff } };
    operation value;
    value.from_data(encoded);
    BOOST_REQUIRE(value.is_invalid());
    BOOST_REQUIRE(value.is_underflow());
    BOOST_REQUIRE_EQUAL(value.to_string(0), "<02ff>");
}

BOOST_AUTO_TEST_CASE(operation__to_string__nop2_no_rules__nop2)
{
    const operation value(opcode::nop2);
    BOOST_REQUIRE_EQUAL(value.to_string(chain::forks::no_rules), "nop2");
}

BOOST_AUTO_TEST_CASE(operation__to_string__nop2_bip65_rule__checklocktimeverify)
{
    const operation value(opcode::nop2);
    BOOST_REQUIRE_EQUAL(value.to_string(chain::forks::bip65_rule), "checklocktimeverify");
}

BOOST_AUTO_TEST_CASE(operation__to_string__nop3_no_rules__nop3)
{
    const operation value(opcode::nop3);
    BOOST_REQUIRE_EQUAL(value.to_string(chain::forks::no_rules), "nop3");
}

BOOST_AUTO_TEST_CASE(operation__to_string__nop3_bip112_rule__checksequenceverify)
{
    const operation value(opcode::nop3);
    BOOST_REQUIRE_EQUAL(value.to_string(chain::forks::bip112_rule), "checksequenceverify");
}

// utilities (static)
// ----------------------------------------------------------------------------

////size_t serialized_size() const
////opcode opcode_from_size(size_t size)
////opcode minimal_opcode_from_data(const data_chunk& data);
////opcode nominal_opcode_from_data(const data_chunk& data)
////opcode opcode_from_version(uint8_t value)
////opcode opcode_from_positive(uint8_t value)
////uint8_t opcode_to_positive(opcode code)

////static bool is_push(opcode code);
////static bool is_payload(opcode code);
////static bool is_counted(opcode code);
////static bool is_version(opcode code);
////static bool is_numeric(opcode code);
////static bool is_positive(opcode code);
////static bool is_invalid(opcode code);
////static bool is_reserved(opcode code);
////static bool is_conditional(opcode code);
////static bool is_relaxed_push(opcode code);

// utilities (member)
// ----------------------------------------------------------------------------

////bool is_push() const;
////bool is_payload() const;
////bool is_counted() const;
////bool is_version() const;
////bool is_numeric() const;
////bool is_positive() const;
////bool is_invalid() const;
////bool is_reserved() const;
////bool is_conditional() const;
////bool is_relaxed_push() const;
////bool is_oversized() const;
////bool is_minimal_push() const;
////bool is_nominal_push() const;
////bool is_underflow() const;

BOOST_AUTO_TEST_SUITE_END()
