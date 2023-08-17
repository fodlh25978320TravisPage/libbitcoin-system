/**
 * Copyright (c) 2011-2023 libbitcoin developers (see AUTHORS)
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

// negate (signedness and [absolue] value do not change, sign to inverted)
static_assert(negate(0_i32)   == 0_i32);
static_assert(negate(0_u32)   == 0_u32);
static_assert(negate(1_i32)   == 1_ni32);
static_assert(negate(1_u32)   == 1_nu32);
static_assert(negate(1_ni32)  == 1_i32);
static_assert(negate(42_i32)  == 42_ni32);
static_assert(negate(42_u32)  == 42_nu32);
static_assert(negate(42_ni32) == 42_i32);
static_assert(is_same_type<decltype(negate(-1)), signed>);
static_assert(is_same_type<decltype(negate(0)),  signed>);
static_assert(is_same_type<decltype(negate(0u)), unsigned>);
static_assert(is_same_type<decltype(negate(1u)), unsigned>);

// negate(minimum<integral>) overflows the integral domain.
// Cannot be represented in 64 bit constexpr.
static_assert(to_unsigned(negate(min_int8))  == add1(to_unsigned(max_int8)));
static_assert(to_unsigned(negate(min_int16)) == add1(to_unsigned(max_int16)));
static_assert(to_unsigned(negate(min_int32)) == add1(to_unsigned(max_int32)));
////static_assert(to_unsigned(negate(min_int64)) == add1(to_unsigned(max_int64)));

// absolute ([absolue] value does not change, signedness to unsigned, sign to positive)
static_assert(absolute(0_i32)   == 0_u32);
static_assert(absolute(0_u32)   == 0_u32);
static_assert(absolute(1_i32)   == 1_u32);
static_assert(absolute(1_u32)   == 1_u32);
static_assert(absolute(1_ni32)  == 1_u32);
static_assert(absolute(42_i32)  == 42_u32);
static_assert(absolute(42_u32)  == 42_u32);
static_assert(absolute(42_ni32) == 42_u32);
static_assert(absolute(min_uint32) == zero);
static_assert(absolute(max_uint32) == max_uint32);
static_assert(absolute(max_int32) == max_int32);
static_assert(absolute(add1(min_int32)) == max_int32);
static_assert(is_same_type<decltype(absolute(-1)), unsigned>);
static_assert(is_same_type<decltype(absolute(0)),  unsigned>);
static_assert(is_same_type<decltype(absolute(0u)), unsigned>);
static_assert(is_same_type<decltype(absolute(1u)), unsigned>);

// absolute(minimum<integral>) overflows the integral domain.
// Cannot be represented in 64 bit constexpr.
static_assert(absolute(min_int8)  == add1(to_unsigned(max_int8)));
static_assert(absolute(min_int16) == add1(to_unsigned(max_int16)));
static_assert(absolute(min_int32) == add1(to_unsigned(max_int32)));
////static_assert(absolute(min_int64) == add1(to_unsigned(max_int64)));

// ones_complement
// alias for bit_not (~n)
static_assert(ones_complement(-4) == 3);
static_assert(ones_complement(-3) == 2);
static_assert(ones_complement(-2) == 1);
static_assert(ones_complement(-1) == 0);
static_assert(ones_complement(0) == -1);
static_assert(ones_complement(1) == -2);
static_assert(ones_complement(2) == -3);
static_assert(ones_complement(3) == -4);
static_assert(ones_complement(4) == -5);
static_assert(ones_complement<int8_t>(-4) == 3);
static_assert(ones_complement<int8_t>(-3) == 2);
static_assert(ones_complement<int8_t>(-2) == 1);
static_assert(ones_complement<int8_t>(-1) == 0);
static_assert(ones_complement<uint8_t>(0x00) == 0xffu);
static_assert(ones_complement<uint8_t>(0xff) == 0u);
static_assert(ones_complement<uint8_t>(0xfe) == 1u);
static_assert(ones_complement<uint8_t>(0xfd) == 2u);
static_assert(ones_complement<uint8_t>(0xfc) == 3u);
static_assert(ones_complement(ones_complement(0x12345678)) == 0x12345678);
static_assert(is_same_type<decltype(ones_complement<int8_t>(0)), int8_t>);

// twos_complement
// similar to but different than absolute (~n+1)
static_assert(twos_complement(4_ni32) == 4_i32);
static_assert(twos_complement(3_ni32) == 3_i32);
static_assert(twos_complement(2_ni32) == 2_i32);
static_assert(twos_complement(1_ni32) == 1_i32);
static_assert(twos_complement(0_i16) == 0_i16);
static_assert(twos_complement(1_i16) == 1_ni16);
static_assert(twos_complement(2_i16) == 2_ni16);
static_assert(twos_complement(3_i16) == 3_ni16);
static_assert(twos_complement(4_i16) == 4_ni16);
static_assert(twos_complement(4_ni8) == 4_i8);
static_assert(twos_complement(3_ni8) == 3_i8);
static_assert(twos_complement(2_ni8) == 2_i8);
static_assert(twos_complement(1_ni8) == 1_i8);
static_assert(twos_complement(0x00_u8) == 0_u8);
static_assert(twos_complement(0xffff_i16) == 1_i16);
static_assert(twos_complement(0xfffffffe_u32) == 2_u32);
static_assert(twos_complement(0xfffffffffffffffd_u64) == 3_u64);
static_assert(twos_complement(0xfffffffffffffffc_u64) == 4_u64);
static_assert(twos_complement(twos_complement(0x12345678)) == 0x12345678);
static_assert(is_same_type<decltype(twos_complement<int8_t>(0)), int8_t>);

// signed
static_assert(negate(max_int8)  == -max_int8);          // rhs size/sign promotion (expected result)
static_assert(negate(max_int16) == -max_int16);         // rhs size/sign promotion (expected result)
static_assert(negate(max_int32) == -max_int32);         // rhs sign promotion (expected result)
static_assert(negate(max_int64) == -max_int64);         // rhs sign promotion (expected result)

// unsigned
static_assert(negate(max_uint8)  != -max_uint8);        // rhs size/sign promotion (unexpected result)
static_assert(negate(max_uint16) != -max_uint16);       // rhs size/sign promotion (unexpected result)

// instead... always avoid promoting operators.
static_assert(negate(0)          == twos_complement(0));
static_assert(negate(0u)         == twos_complement(0u));
static_assert(negate(1)          == twos_complement(1));
static_assert(negate(1u)         == twos_complement(1u));
static_assert(negate(-1)         == twos_complement(-1));
static_assert(negate(-42)        == twos_complement(-42));
static_assert(negate(42u)        == twos_complement(42u));
static_assert(negate(max_uint8)  == twos_complement(max_uint8));
static_assert(negate(max_uint16) == twos_complement(max_uint16));
static_assert(negate(max_uint32) == twos_complement(max_uint32));
static_assert(negate(max_uint64) == twos_complement(max_uint64));
static_assert(negate(max_uint8)  == add1(min_uint8));
static_assert(negate(max_uint16) == add1(min_uint16));
static_assert(negate(max_uint32) == add1(min_uint32));
static_assert(negate(max_uint64) == add1(min_uint64));

// -minimum<integral> is undefined (cannot add1 to the domain maximum and get a negative).
////static_assert(negate(min_int32) == add1(max_int8));
////static_assert(negate(min_int64) == add1(max_int8));
////static_assert(negate(min_int8)  == add1(max_int8));
////static_assert(negate(min_int16) == add1(max_int16));

// These can be mitigated using explicit domain promotion (except for uint64_t).
static_assert(negate<int16_t>(min_int8)  == add1<int16_t>(max_int8));
static_assert(negate<int32_t>(min_int16) == add1<int32_t>(max_int16));
static_assert(negate<int64_t>(min_int32) == add1<int64_t>(max_int32));

// uint32 requires explicit promotion before negation.
static_assert(negate<int16_t>(min_int8)  == -min_int8);
static_assert(negate<int32_t>(min_int16) == -min_int16);
static_assert(negate<int64_t>(min_int32) == -int64_t(min_int32));
////static_assert(negate<int64_t>(min_int64) == -min_int64);

// is_negative
static_assert(is_negative(1_ni32));
static_assert(is_negative(42_ni16));
static_assert(is_negative(0x80_i8));
static_assert(is_negative(0xff_i8));
static_assert(is_negative(min_int32));
static_assert(!is_negative(0_u16));
static_assert(!is_negative(1_u32));
static_assert(!is_negative(42_u64));
static_assert(!is_negative(0x00_i8));
static_assert(!is_negative(0x01_u8));
static_assert(!is_negative(0x80_u8));
static_assert(!is_negative(0xff_u8));
static_assert(!is_negative(max_int32));
static_assert(!is_negative(max_uint32));

// to_unsigned
static_assert(is_greater(1, 0));
static_assert(is_greater(1u, 0));
static_assert(is_greater(1u, 0u));
static_assert(is_greater(0, -1));
static_assert(is_greater(0u, -1));
static_assert(!is_greater(-1, -1));
static_assert(!is_greater(0u, 0));
static_assert(!is_greater(0u, 0u));
static_assert(!is_greater(0, 0u));
static_assert(!is_greater(1, 1));
static_assert(!is_greater(1u, 1));
static_assert(!is_greater(1u, 1u));
static_assert(!is_greater(1, 1u));
static_assert(!is_greater(0, 1));
static_assert(!is_greater(-1, 0));

// to_unsigned
static_assert(!is_lesser(1, 0));
static_assert(!is_lesser(1u, 0));
static_assert(!is_lesser(1u, 0u));
static_assert(!is_lesser(0, -1));
static_assert(!is_lesser(0u, -1));
static_assert(!is_lesser(-1, -1));
static_assert(!is_lesser(0u, 0));
static_assert(!is_lesser(0u, 0u));
static_assert(!is_lesser(0, 0u));
static_assert(!is_lesser(1, 1));
static_assert(!is_lesser(1u, 1));
static_assert(!is_lesser(1u, 1u));
static_assert(!is_lesser(1, 1u));
static_assert(is_lesser(0, 1));
static_assert(is_lesser(-1, 0));

// greater

static_assert(greater(1, 0) == 1);
static_assert(greater(0, 1) == 1);
static_assert(greater(1u, 0u) == 1u);
static_assert(greater(0u, 1u) == 1u);
static_assert(greater<signed>(1, 0u) == 1);
static_assert(greater<signed>(0, 1u) == 1);
static_assert(greater<signed>(1u, 0) == 1);
static_assert(greater<signed>(0u, 1) == 1);

static_assert(greater(0, 0) == 0);
static_assert(greater(1, 1) == 1);
static_assert(greater(0u, 0u) == 0u);
static_assert(greater(1u, 1u) == 1u);
static_assert(greater<signed>(0u, 0) == 0);
static_assert(greater<signed>(1u, 1) == 1);
static_assert(greater<signed>(0, 0u) == 0);
static_assert(greater<signed>(1, 1u) == 1);

static_assert(greater(1, 0) == 1u);
static_assert(greater(0, 1) == 1u);
static_assert(greater(1u, 0u) == 1u);
static_assert(greater(0u, 1u) == 1u);
static_assert(greater<unsigned>(1, 0u) == 1u);
static_assert(greater<unsigned>(0, 1u) == 1u);
static_assert(greater<unsigned>(1u, 0) == 1u);
static_assert(greater<unsigned>(0u, 1) == 1u);

static_assert(greater(0, 0) == 0u);
static_assert(greater(1, 1) == 1u);
static_assert(greater(0u, 0u) == 0u);
static_assert(greater(1u, 1u) == 1u);
static_assert(greater<unsigned>(0u, 0) == 0u);
static_assert(greater<unsigned>(1u, 1) == 1u);
static_assert(greater<unsigned>(0, 0u) == 0u);
static_assert(greater<unsigned>(1, 1u) == 1u);

// lesser

static_assert(lesser(1, 0) == 0);
static_assert(lesser(0, 1) == 0);
static_assert(lesser(1u, 0u) == 0u);
static_assert(lesser(0u, 1u) == 0u);
static_assert(lesser<signed>(1, 0u) == 0);
static_assert(lesser<signed>(0, 1u) == 0);
static_assert(lesser<signed>(1u, 0) == 0);
static_assert(lesser<signed>(0u, 1) == 0);

static_assert(lesser(0, 0) == 0);
static_assert(lesser(1, 1) == 1);
static_assert(lesser(0u, 0u) == 0u);
static_assert(lesser(1u, 1u) == 1u);
static_assert(lesser<signed>(0u, 0) == 0);
static_assert(lesser<signed>(1u, 1) == 1);
static_assert(lesser<signed>(0, 0u) == 0);
static_assert(lesser<signed>(1, 1u) == 1);

static_assert(lesser(1, 0) == 0);
static_assert(lesser(0, 1) == 0);
static_assert(lesser(1u, 0u) == 0u);
static_assert(lesser(0u, 1u) == 0u);
static_assert(lesser<unsigned>(1, 0u) == 0u);
static_assert(lesser<unsigned>(0, 1u) == 0u);
static_assert(lesser<unsigned>(1u, 0) == 0u);
static_assert(lesser<unsigned>(0u, 1) == 0u);

static_assert(lesser(1, 0) == 0);
static_assert(lesser(1, 1) == 1);
static_assert(lesser(0u, 0u) == 0u);
static_assert(lesser(1u, 1u) == 1u);
static_assert(lesser<unsigned>(0u, 0) == 0u);
static_assert(lesser<unsigned>(1u, 1) == 1u);
static_assert(lesser<unsigned>(0, 0u) == 0u);
static_assert(lesser<unsigned>(1, 1u) == 1u);
