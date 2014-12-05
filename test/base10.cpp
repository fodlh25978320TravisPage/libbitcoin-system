/*
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <boost/test/unit_test.hpp>
#include <bitcoin/bitcoin.hpp>

using namespace bc;

BOOST_AUTO_TEST_SUITE(parse_amount_tests)

#define TEST(NAME, EXPECTED, ...) \
    BOOST_AUTO_TEST_CASE(parse_amount_##NAME##_test) \
    { \
        uint64_t result, expected = EXPECTED; \
        BOOST_REQUIRE(decode_base10(result, __VA_ARGS__)); \
        BOOST_REQUIRE_EQUAL(result, expected); \
    }

#define TEST_ERROR(NAME, ...) \
    BOOST_AUTO_TEST_CASE(parse_amount_##NAME##_test) \
    { \
        uint64_t result; \
        BOOST_REQUIRE(!decode_base10(result, __VA_ARGS__)); \
    }

// Limits:
TEST(zero,                      0, "0")
TEST(max_uint64,                max_uint64, "18446744073709551615")

// Max money:
TEST(max_money,
    max_money(), "20999999.9769", btc_decimal_places)
TEST(overflow_max_money,
    max_money() + 1, "20999999.97690001", btc_decimal_places)

// Decimal points:
TEST(pure_integer,              42, "42.0",         0)
TEST(no_decimal,                1000000, "10",      mbtc_decimal_places)
TEST(normal_decimal,            420000, "4.2",      mbtc_decimal_places)
TEST(leading_decimal,           50000, ".5",        mbtc_decimal_places)
TEST(trailing_decial,           500000, "5.",       mbtc_decimal_places)
TEST(extra_zeros,               1002000, "010.020", mbtc_decimal_places)
TEST(harmless_zeros,            1, "0.0000100",     mbtc_decimal_places)
TEST(decimal_point_only,        0, ".")

// Rounding:
TEST(pure_integer_rounding,     1, ".1", 0, false)
TEST(rounding,                  11, "0.101",   ubtc_decimal_places, false)
TEST(rounding_carry,            1000, "9.991", ubtc_decimal_places, false)
TEST(zero_past_max,             max_uint64, "18446744073709551615.0")

// Format errors:
TEST_ERROR(lexical_cast_fail,   "0.-1")
TEST_ERROR(extra_decimal,       "0.0.0")
TEST_ERROR(bad_characters,      "0x0ff")

// Numeric errors:
TEST_ERROR(overflow,            "18446744073709551616")
TEST_ERROR(rounding_overflow,   "18446744073709551615.1", 0, false)
TEST_ERROR(fractional_amount,   "0.999999999", btc_decimal_places)

#undef TEST
#undef TEST_ERROR

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(format_amount_tests)

#define TEST(NAME, EXPECTED, ...) \
    BOOST_AUTO_TEST_CASE(format_amount_##NAME##_test) \
    { \
        std::string expected = EXPECTED; \
        std::string result = encode_base10(__VA_ARGS__); \
        BOOST_REQUIRE_EQUAL(result, expected); \
    }

// Limits:
TEST(zero,                      "0", 0)
TEST(max_uint64,                "18446744073709551615", max_uint64)

// Max money:
TEST(max_money,
    "20999999.9769", max_money(), btc_decimal_places)
TEST(overflow_max_money,
    "20999999.97690001", max_money() + 1, btc_decimal_places)

// Decimal points:
TEST(pure_integer,              "42", 42, 0)
TEST(no_decimal,                "10", 1000000, mbtc_decimal_places)
TEST(normal_decimal,            "4.2", 420000, mbtc_decimal_places)
TEST(leading_zero,              "0.42", 42000, mbtc_decimal_places)
TEST(internal_leading_zero,     "0.042", 4200, mbtc_decimal_places)

#undef TEST

BOOST_AUTO_TEST_SUITE_END()
