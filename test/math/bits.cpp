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

// TODO: missing signed/negative tests on shifts.

// bit_width (unsigned/positive)
// unsigned are always value width.
static_assert(bit_width(0u) == 0u);
static_assert(bit_width(1u) == 1u);
static_assert(bit_width(2u) == 2u);
static_assert(bit_width(3u) == 2u);
static_assert(bit_width(4u) == 3u);
static_assert(bit_width(5u) == 3u);
static_assert(bit_width(6u) == 3u);
static_assert(bit_width(7u) == 3u);
static_assert(bit_width(8u) == 4u);
static_assert(bit_width(9u) == 4u);
static_assert(bit_width(9u) == 4u);
static_assert(bit_width(0x80_u8) == 8u);
static_assert(bit_width(0x8000_u16) == 16u);
static_assert(bit_width(0x800100_u32) == 24u);
static_assert(bit_width(0x80000000_u32) == 32u);
static_assert(bit_width(0x8001000000_u64) == 40u);
static_assert(bit_width(0x800000000000_u64) == 48u);
static_assert(bit_width(0x80010000000000_u64) == 56u);
static_assert(bit_width(0x8000000000000000_u64) == 64u);

// bit_width (signed)
// positive operators changes promotes to int, but positive are always value width.
static_assert(bit_width(+0) == 0u);
static_assert(bit_width(+1) == 1u);
static_assert(bit_width(+2) == 2u);
static_assert(bit_width(+3) == 2u);
static_assert(bit_width(+4) == 3u);
static_assert(bit_width(+5) == 3u);
static_assert(bit_width(+6) == 3u);
static_assert(bit_width(+7) == 3u);
static_assert(bit_width(+8) == 4u);
static_assert(bit_width(+9) == 4u);
static_assert(bit_width(+9) == 4u);
static_assert(bit_width(0x80_i8) == 8u);
static_assert(bit_width(0x8000_i16) == 16u);
static_assert(bit_width(0x800100_i32) == 24u);
static_assert(bit_width(0x80000000_i32) == 32u);
static_assert(bit_width(0x8001000000_i64) == 40u);
static_assert(bit_width(0x800000000000_i64) == 48u);
static_assert(bit_width(0x80010000000000_i64) == 56u);
static_assert(bit_width(0x8000000000000000_i64) == 64u);

// positive are always value width, explicit type does not alter because there are no negatives.
static_assert(bit_width<int64_t>(0x80_u8) == 8u);
static_assert(bit_width<int64_t>(0x8000_u16) == 16u);
static_assert(bit_width<int64_t>(0x800100_u32) == 24u);
static_assert(bit_width<int64_t>(0x80000000_u32) == 32u);
static_assert(bit_width<int64_t>(0x8001000000_u64) == 40u);
static_assert(bit_width<int64_t>(0x800000000000_u64) == 48u);
static_assert(bit_width<int64_t>(0x80010000000000_u64) == 56u);
static_assert(bit_width<int64_t>(0x8000000000000000_u64) == 64u);

// negatives are always type width, but sign is explicitly inverted, making negatives positive.
static_assert(bit_width<int64_t>(0x80_i8) == 64u); // negative
static_assert(bit_width<int64_t>(0x8000_i16) == 64u); // negative
static_assert(bit_width<int64_t>(0x800100_i32) == 24u); // positive
static_assert(bit_width<int64_t>(0x80000000_i32) == 64u); // negative
static_assert(bit_width<int64_t>(0x8001000000_i64) == 40u); // positive
static_assert(bit_width<int64_t>(0x800000000000_i64) == 48u); // positive
static_assert(bit_width<int64_t>(0x80010000000000_i64) == 56u); // positive
static_assert(bit_width<int64_t>(0x8000000000000000_i64) == 64u); // negative

// bit_width (negated)
// negatives are always type width.
// type promoted to int by negation operator.
static_assert(bit_width(-1) == 32u); 
static_assert(bit_width(-2) == 32u);
static_assert(bit_width(-3) == 32u);
static_assert(bit_width(-4) == 32u);
static_assert(bit_width(-5) == 32u);
static_assert(bit_width(-6) == 32u);
static_assert(bit_width(-7) == 32u);
static_assert(bit_width(-8) == 32u);
static_assert(bit_width(-9) == 32u);
static_assert(bit_width(0x80_ni8) == 8u);
static_assert(bit_width(0x8000_ni16) == 16u);
static_assert(bit_width(0x80000000_ni32) == 32u);
static_assert(bit_width(0x8001000000_ni64) == 64u);
static_assert(bit_width(0x800000000000_ni64) == 64u);
static_assert(bit_width(0x80010000000000_ni64) == 64u);
static_assert(bit_width(0x8000000000000000_ni64) == 64u);
static_assert(bit_width<int64_t>(0x80_ni8) == 64u);
static_assert(bit_width<int64_t>(0x8000_ni16) == 64u);
static_assert(bit_width<int64_t>(0x00800100_ni32) == 64u);
static_assert(bit_width<int64_t>(0x8001000000_ni64) == 64u);
static_assert(bit_width<int64_t>(0x800000000000_ni64) == 64u);
static_assert(bit_width<int64_t>(0x80010000000000_ni64) == 64u);
static_assert(bit_width<int64_t>(0x8000000000000000_ni64) == 64u);

// ones_complement (NOT)
// inverts all bits (~n, !bool)
static_assert(bit_not(-4) == 3);
static_assert(bit_not(-3) == 2);
static_assert(bit_not(-2) == 1);
static_assert(bit_not(-1) == 0);
static_assert(bit_not(0) == -1);
static_assert(bit_not(1) == -2);
static_assert(bit_not(2) == -3);
static_assert(bit_not(3) == -4);
static_assert(bit_not(4) == -5);
static_assert(bit_not<int8_t>(-4) == 3);
static_assert(bit_not<int8_t>(-3) == 2);
static_assert(bit_not<int8_t>(-2) == 1);
static_assert(bit_not<int8_t>(-1) == 0);
static_assert(bit_not<uint8_t>(0x00) == 0xffu);
static_assert(bit_not<uint8_t>(0xff) == 0u);
static_assert(bit_not<uint8_t>(0xfe) == 1u);
static_assert(bit_not<uint8_t>(0xfd) == 2u);
static_assert(bit_not<uint8_t>(0xfc) == 3u);
static_assert(is_same_type<decltype(bit_not<int8_t>(0)), int8_t>);

// AND
static_assert(bit_and(0, 0) == 0);
static_assert(bit_and(1, 0) == 0);
static_assert(bit_and(0, 1) == 0);
static_assert(bit_and(1, 1) == 1);
static_assert(is_same_type<decltype(bit_and<int8_t>(0, 0)), int8_t>);

// OR
static_assert(bit_or(0, 0) == 0);
static_assert(bit_or(1, 0) == 1);
static_assert(bit_or(0, 1) == 1);
static_assert(bit_or(1, 1) == 1);
static_assert(is_same_type<decltype(bit_or<int8_t>(0, 0)), int8_t>);

// XOR
static_assert(bit_xor(0, 0) == 0);
static_assert(bit_xor(1, 0) == 1);
static_assert(bit_xor(0, 1) == 1);
static_assert(bit_xor(1, 1) == 0);
static_assert(is_same_type<decltype(bit_xor<int8_t>(0, 0)), int8_t>);

// bit_all
static_assert(is_zero(bit_all<signed char>   ^ 0xff_i8));
static_assert(is_zero(bit_all<unsigned char> ^ 0xff_u8));
static_assert(is_zero(bit_all<int8_t>        ^ 0xff_i8));
static_assert(is_zero(bit_all<uint8_t>       ^ 0xff_u8));
static_assert(is_zero(bit_all<int16_t>       ^ 0xffff_i16));
static_assert(is_zero(bit_all<uint16_t>      ^ 0xffff_u16));
static_assert(is_zero(bit_all<int32_t>       ^ 0xffffffff_i32));
static_assert(is_zero(bit_all<uint32_t>      ^ 0xffffffff_u32));
static_assert(is_zero(bit_all<int64_t>       ^ 0xffffffffffffffff_i64));
static_assert(is_zero(bit_all<uint64_t>      ^ 0xffffffffffffffff_u64));
static_assert(is_same_type<decltype(bit_all<int8_t>), const int8_t>);

// bit_lo
static_assert(is_zero(bit_lo<signed char>   ^ 0b1_i64));
static_assert(is_zero(bit_lo<unsigned char> ^ 0b1_u64));
static_assert(is_zero(bit_lo<int8_t>        ^ 0b1_i64));
static_assert(is_zero(bit_lo<uint8_t>       ^ 0b1_u64));
static_assert(is_zero(bit_lo<int16_t>       ^ 0b1_i64));
static_assert(is_zero(bit_lo<uint16_t>      ^ 0b1_u64));
static_assert(is_zero(bit_lo<int32_t>       ^ 0b1_i64));
static_assert(is_zero(bit_lo<uint32_t>      ^ 0b1_u64));
static_assert(is_zero(bit_lo<int64_t>       ^ 0b1_i64));
static_assert(is_zero(bit_lo<uint64_t>      ^ 0b1_u64));
static_assert(is_same_type<decltype(bit_lo<int32_t>), const int32_t>);

// bit_hi (signed overflow to unsigned)
static_assert(is_zero(bit_hi<unsigned char> ^ 0x80_u8));
static_assert(is_zero(bit_hi<int8_t>        ^ 0x80_i8));
static_assert(is_zero(bit_hi<uint8_t>       ^ 0x80_u8));
static_assert(is_zero(bit_hi<int16_t>       ^ 0x8000_i16));
static_assert(is_zero(bit_hi<uint16_t>      ^ 0x8000_u16));
static_assert(is_zero(bit_hi<int32_t>       ^ 0x80000000_i32));
static_assert(is_zero(bit_hi<uint32_t>      ^ 0x80000000_u32));
static_assert(is_zero(bit_hi<int64_t>       ^ 0x8000000000000000_i64));
static_assert(is_zero(bit_hi<uint64_t>      ^ 0x8000000000000000_u64));
static_assert(is_same_type<decltype(bit_hi<uint16_t>), const uint16_t>);

// TODO: fully test signed below.

// bit_left
static_assert(bit_left<uint8_t>() == 0x80u);
static_assert(bit_left<uint16_t>() == 0x8000u);
static_assert(bit_left<uint32_t>() == 0x80000000u);
static_assert(bit_left<uint64_t>() == 0x8000000000000000ul);
static_assert(bit_left<uint8_t>(0) == 0x80u);
static_assert(bit_left<uint8_t>(1) == 64u);
static_assert(bit_left<uint8_t>(2) == 32u);
static_assert(bit_left<uint8_t>(3) == 16u);
static_assert(bit_left<uint8_t>(4) == 8u);
static_assert(bit_left<uint8_t>(5) == 4u);
static_assert(bit_left<uint8_t>(6) == 2u);
static_assert(bit_left<uint8_t>(7) == 1u);
static_assert(bit_left<uint16_t>(15) == 1u);
static_assert(bit_left<uint32_t>(31) == 1u);
static_assert(bit_left<uint64_t>(63) == 1u);
static_assert(bit_left<int8_t>() == -128);
static_assert(bit_left<int8_t>(0) == -128);
static_assert(bit_left<int8_t>(1) == 64);
static_assert(bit_left<int8_t>(2) == 32);
static_assert(bit_left<int8_t>(3) == 16);
static_assert(bit_left<int8_t>(4) == 8);
static_assert(bit_left<int8_t>(5) == 4);
static_assert(bit_left<int8_t>(6) == 2);
static_assert(bit_left<int8_t>(7) == 1);
static_assert(bit_left<int16_t>(15) == 1);
static_assert(bit_left<int32_t>(31) == 1);
static_assert(bit_left<int64_t>(63) == 1);
static_assert(bit_left<int16_t>() == -32768);
static_assert(bit_left<int16_t>(0) == -32768);
static_assert(bit_left<int16_t>(1) == 0x4000);
static_assert(bit_left<int16_t>(2) == 0x2000);
static_assert(bit_left<int16_t>(3) == 0x1000);
static_assert(bit_left<int16_t>(4) == 0x0800);
static_assert(bit_left<int16_t>(5) == 0x0400);
static_assert(bit_left<int16_t>(6) == 0x0200);
static_assert(bit_left<int16_t>(7) == 0x0100);
static_assert(bit_left<int16_t>(15) == 0x0001);
static_assert(bit_left<uint32_t>(31) == 0x00000001ul);
static_assert(bit_left<uint64_t>(63) == 0x0000000000000001ull);
static_assert(is_same_type<decltype(bit_left<int8_t>(0)), int8_t>);

// bit_right
static_assert(bit_right<uint8_t>() == 0x01u);
static_assert(bit_right<uint16_t>() == 0x0001u);
static_assert(bit_right<uint32_t>() == 0x00000001ul);
static_assert(bit_right<uint64_t>() == 0x0000000000000001ull);
static_assert(bit_right<uint8_t>(0) == 1u);
static_assert(bit_right<uint8_t>(1) == 2u);
static_assert(bit_right<uint8_t>(2) == 4u);
static_assert(bit_right<uint8_t>(3) == 8u);
static_assert(bit_right<uint8_t>(4) == 16u);
static_assert(bit_right<uint8_t>(5) == 32u);
static_assert(bit_right<uint8_t>(6) == 64u);
static_assert(bit_right<uint8_t>(7) == 0x80u);
static_assert(bit_right<int8_t>() == 1);
static_assert(bit_right<int8_t>(0) == 1);
static_assert(bit_right<int8_t>(1) == 2);
static_assert(bit_right<int8_t>(2) == 4);
static_assert(bit_right<int8_t>(3) == 8);
static_assert(bit_right<int8_t>(4) == 16);
static_assert(bit_right<int8_t>(5) == 32);
static_assert(bit_right<int8_t>(6) == 64);
static_assert(bit_right<int8_t>(7) == -128);
static_assert(bit_right<int16_t>() == 1);
static_assert(bit_right<int16_t>(0) == 1);
static_assert(bit_right<int16_t>(1) == 2);
static_assert(bit_right<int16_t>(2) == 4);
static_assert(bit_right<int16_t>(3) == 8);
static_assert(bit_right<int16_t>(4) == 16);
static_assert(bit_right<int16_t>(5) == 32);
static_assert(bit_right<int16_t>(6) == 64);
static_assert(bit_right<int16_t>(15) == -32768);
static_assert(bit_right<uint16_t>(15) == 0x8000u);
static_assert(bit_right<uint32_t>(31) == 0x80000000ul);
static_assert(bit_right<uint64_t>(63) == 0x8000000000000000ull);
static_assert(is_same_type<decltype(bit_right<int8_t>(0)), int8_t>);

// get_left
static_assert(!get_left(0xff)); // int
static_assert(get_left<uint8_t>(0xff));
static_assert(get_left<uint8_t>(0xff, 1));
static_assert(get_left<uint8_t>(0xff, 2));
static_assert(get_left<uint8_t>(0xff, 3));
static_assert(get_left<uint8_t>(0xff, 4));
static_assert(get_left<uint8_t>(0xff, 5));
static_assert(get_left<uint8_t>(0xff, 6));
static_assert(get_left<uint8_t>(0xff, 7));
static_assert(get_left<uint16_t>(0xffff, 8));
static_assert(get_left<uint16_t>(0xffff, 15));
static_assert(get_left<uint32_t>(0xffffffffl, 16));
static_assert(get_left<uint32_t>(0xffffffffl, 31));
static_assert(get_left<uint64_t>(0xffffffffffffffffll, 32));
static_assert(get_left<uint64_t>(0xffffffffffffffffll, 63));
static_assert(get_left<uint8_t>(0x01, 7));
static_assert(get_left<uint8_t>(0x02, 6));
static_assert(get_left<uint8_t>(0x04, 5));
static_assert(get_left<uint8_t>(0x08, 4));
static_assert(get_left<uint8_t>(0x10, 3));
static_assert(get_left<uint8_t>(0x20, 2));
static_assert(get_left<uint8_t>(0x40, 1));
static_assert(get_left<uint8_t>(0x80));
static_assert(!get_left(0x80)); // int
static_assert(get_left<uint8_t>(0x80));
static_assert(get_left<uint8_t>(0x80, 0));
static_assert(get_left<uint16_t>(0x0100, 7));
static_assert(get_left<uint16_t>(0x8000, 0));
static_assert(get_left<uint16_t>(0x0004, 13));
static_assert(get_left<uint32_t>(0x00010000l, 15));
static_assert(get_left<uint32_t>(0x80000000l));
static_assert(get_left<uint32_t>(0x80000000l, 0));
static_assert(get_left<uint64_t>(0x0000000100000000ll, 31));
static_assert(get_left<uint64_t>(0x8000000000000000ll));
static_assert(get_left<uint64_t>(0x8000000000000000ll, 0));
static_assert(get_left<int8_t>(0x01, 7));
static_assert(get_left<int8_t>(0x02, 6));
static_assert(get_left<int8_t>(0x04, 5));
static_assert(get_left<int8_t>(0x08, 4));
static_assert(get_left<int8_t>(0x10, 3));
static_assert(get_left<int8_t>(0x20, 2));
static_assert(get_left<int8_t>(0x40, 1));
static_assert(get_left<int8_t>(-128));
static_assert(get_left<int8_t>(-128, 0));
static_assert(get_left<int16_t>(0x0100, 7));
static_assert(get_left<int16_t>(-32768, 0));
static_assert(get_left<int16_t>(0x0004, 13));
static_assert(get_left<int32_t>(0x00010000l, 15));
static_assert(get_left<int32_t>(0x80000000l));
static_assert(get_left<int32_t>(0x80000000l, 0));
static_assert(get_left<int64_t>(0x0000000100000000ll, 31));
static_assert(get_left<int64_t>(0x8000000000000000ll));
static_assert(get_left<int64_t>(0x8000000000000000ll, 0));
static_assert(!get_left<uint16_t>(0x0001, 11));
static_assert(!get_left<uint16_t>(0x0002, 12));
static_assert(!get_left<uint16_t>(0x0008, 14));
static_assert(!get_left<uint16_t>(0x0010, 15));
static_assert(!get_left<uint32_t>(0x00000020l, 16));
static_assert(!get_left<uint32_t>(0x00000040l, 17));
static_assert(!get_left<uint32_t>(0x00000080l, 18));
static_assert(!get_left<uint32_t>(0x00000100l, 19));
static_assert(!get_left<uint32_t>(0x00008000l, 20));
static_assert(!get_left<uint32_t>(0x00010000l, 21));
static_assert(!get_left<uint32_t>(0x80000000l, 22));
static_assert(!get_left<uint64_t>(0x0000000100000000ll, 23));
static_assert(!get_left<uint64_t>(0x8000000000000000ll, 24));
static_assert(is_same_type<decltype(get_left<int8_t>(0, 0)), bool>);

// get_right
static_assert(get_right(0xff));
static_assert(get_right<uint8_t>(0xff, 1));
static_assert(get_right<uint8_t>(0xff, 2));
static_assert(get_right<uint8_t>(0xff, 3));
static_assert(get_right<uint8_t>(0xff, 4));
static_assert(get_right<uint8_t>(0xff, 5));
static_assert(get_right<uint8_t>(0xff, 6));
static_assert(get_right<uint8_t>(0xff, 7));
static_assert(get_right<uint16_t>(0xffff, 8));
static_assert(get_right<uint16_t>(0xffff, 15));
static_assert(get_right<uint32_t>(0xffffffffl, 16));
static_assert(get_right<uint32_t>(0xffffffffl, 31));
static_assert(get_right<uint64_t>(0xffffffffffffffffll, 32));
static_assert(get_right<uint64_t>(0xffffffffffffffffll, 63));
static_assert(get_right(0x01));
static_assert(get_right<uint8_t>(0x01, 0));
static_assert(get_right<uint8_t>(0x02, 1));
static_assert(get_right<uint8_t>(0x04, 2));
static_assert(get_right<uint8_t>(0x08, 3));
static_assert(get_right<uint8_t>(0x10, 4));
static_assert(get_right<uint8_t>(0x20, 5));
static_assert(get_right<uint8_t>(0x40, 6));
static_assert(get_right<uint8_t>(0x80, 7));
static_assert(get_right<uint16_t>(0x0100, 8));
static_assert(get_right<uint16_t>(0x8000, 15));
static_assert(get_right<uint32_t>(0x00010000l, 16));
static_assert(get_right<uint32_t>(0x80000000l, 31));
static_assert(get_right<uint64_t>(0x0000000100000000ll, 32));
static_assert(get_right<uint64_t>(0x8000000000000000ll, 63));
static_assert(get_right<int8_t>(0x01));
static_assert(get_right<int8_t>(0x01, 0));
static_assert(get_right<int8_t>(0x02, 1));
static_assert(get_right<int8_t>(0x04, 2));
static_assert(get_right<int8_t>(0x08, 3));
static_assert(get_right<int8_t>(0x10, 4));
static_assert(get_right<int8_t>(0x20, 5));
static_assert(get_right<int8_t>(0x40, 6));
static_assert(get_right<int8_t>(-128, 7));
static_assert(get_right<int16_t>(0x0100, 8));
static_assert(get_right<int16_t>(-32768, 15));
static_assert(get_right<int32_t>(0x00010000l, 16));
static_assert(get_right<int32_t>(0x80000000l, 31));
static_assert(get_right<int64_t>(0x0000000100000000ll, 32));
static_assert(get_right<int64_t>(0x8000000000000000ll, 63));
static_assert(!get_right<uint16_t>(0x0001, 11));
static_assert(!get_right<uint16_t>(0x0002, 12));
static_assert(!get_right<uint16_t>(0x0004, 13));
static_assert(!get_right<uint16_t>(0x0008, 14));
static_assert(!get_right<uint16_t>(0x0010, 15));
static_assert(!get_right<uint32_t>(0x00000020l, 16));
static_assert(!get_right<uint32_t>(0x00000040l, 17));
static_assert(!get_right<uint32_t>(0x00000080l, 18));
static_assert(!get_right<uint32_t>(0x00000100l, 19));
static_assert(!get_right<uint32_t>(0x00008000l, 20));
static_assert(!get_right<uint32_t>(0x00010000l, 21));
static_assert(!get_right<uint32_t>(0x80000000l, 22));
static_assert(!get_right<uint64_t>(0x0000000100000000ll, 23));
static_assert(!get_right<uint64_t>(0x8000000000000000ll, 24));
static_assert(is_same_type<decltype(get_right<int8_t>(0, 0)), bool>);

// set_left
static_assert(set_left<int8_t>(0x00, 0, false) == 0x00);
static_assert(set_left<int8_t>(0x00, 1, false) == 0x00);
static_assert(set_left<int8_t>(0x00, 2, false) == 0x00);
static_assert(set_left<int8_t>(0x00, 3, false) == 0x00);
static_assert(set_left<int8_t>(0x00, 4, false) == 0x00);
static_assert(set_left<int8_t>(0x00, 5, false) == 0x00);
static_assert(set_left<int8_t>(0x00, 6, false) == 0x00);
static_assert(set_left<int8_t>(0x00, 7, false) == 0x00);
static_assert(set_left<int8_t>(0x00) == -128);
static_assert(set_left<int8_t>(0x00, 0) == -128);
static_assert(set_left<int8_t>(0x00, 0, true) == -128);
static_assert(set_left<int8_t>(0x00, 1, true) == 0x40);
static_assert(set_left<int8_t>(0x00, 2, true) == 0x20);
static_assert(set_left<int8_t>(0x00, 3, true) == 0x10);
static_assert(set_left<int8_t>(0x00, 4, true) == 0x08);
static_assert(set_left<int8_t>(0x00, 5, true) == 0x04);
static_assert(set_left<int8_t>(0x00, 6, true) == 0x02);
static_assert(set_left<int8_t>(0x00, 7, true) == 0x01);
static_assert(set_left<uint8_t>(0x00, 0, false) == 0x00u);
static_assert(set_left<uint8_t>(0x00, 1, false) == 0x00u);
static_assert(set_left<uint8_t>(0x00, 2, false) == 0x00u);
static_assert(set_left<uint8_t>(0x00, 3, false) == 0x00u);
static_assert(set_left<uint8_t>(0x00, 4, false) == 0x00u);
static_assert(set_left<uint8_t>(0x00, 5, false) == 0x00u);
static_assert(set_left<uint8_t>(0x00, 6, false) == 0x00u);
static_assert(set_left<uint8_t>(0x00, 7, false) == 0x00u);
static_assert(set_left<uint8_t>(0x00) == 0x80u);
static_assert(set_left<uint8_t>(0x00, 0) == 0x80u);
static_assert(set_left<uint8_t>(0x00, 0, true) == 0x80u);
static_assert(set_left<uint8_t>(0x00, 1, true) == 0x40u);
static_assert(set_left<uint8_t>(0x00, 2, true) == 0x20u);
static_assert(set_left<uint8_t>(0x00, 3, true) == 0x10u);
static_assert(set_left<uint8_t>(0x00, 4, true) == 0x08u);
static_assert(set_left<uint8_t>(0x00, 5, true) == 0x04u);
static_assert(set_left<uint8_t>(0x00, 6, true) == 0x02u);
static_assert(set_left<uint8_t>(0x00, 7, true) == 0x01u);
static_assert(set_left<uint8_t>(0xff, 0, true) == 0xffu);
static_assert(set_left<uint8_t>(0xff, 1, true) == 0xffu);
static_assert(set_left<uint8_t>(0xff, 2, true) == 0xffu);
static_assert(set_left<uint8_t>(0xff, 3, true) == 0xffu);
static_assert(set_left<uint8_t>(0xff, 4, true) == 0xffu);
static_assert(set_left<uint8_t>(0xff, 5, true) == 0xffu);
static_assert(set_left<uint8_t>(0xff, 6, true) == 0xffu);
static_assert(set_left<uint8_t>(0xff, 7, true) == 0xffu);
static_assert(set_left<uint8_t>(0xff, 0, false) == 0x7fu);
static_assert(set_left<uint8_t>(0xff, 1, false) == 0xbfu);
static_assert(set_left<uint8_t>(0xff, 2, false) == 0xdfu);
static_assert(set_left<uint8_t>(0xff, 3, false) == 0xefu);
static_assert(set_left<uint8_t>(0xff, 4, false) == 0xf7u);
static_assert(set_left<uint8_t>(0xff, 5, false) == 0xfbu);
static_assert(set_left<uint8_t>(0xff, 6, false) == 0xfdu);
static_assert(set_left<uint8_t>(0xff, 7, false) == 0xfeu);
static_assert(is_same_type<decltype(set_left<uint8_t>(0, 0, false)), uint8_t>);

// set_right
static_assert(set_right<int8_t>(0x00, 0, false) == 0x00);
static_assert(set_right<int8_t>(0x00, 1, false) == 0x00);
static_assert(set_right<int8_t>(0x00, 2, false) == 0x00);
static_assert(set_right<int8_t>(0x00, 3, false) == 0x00);
static_assert(set_right<int8_t>(0x00, 4, false) == 0x00);
static_assert(set_right<int8_t>(0x00, 5, false) == 0x00);
static_assert(set_right<int8_t>(0x00, 6, false) == 0x00);
static_assert(set_right<int8_t>(0x00, 7, false) == 0x00);
static_assert(set_right<int8_t>(0x00) == 0x01);
static_assert(set_right<int8_t>(0x00, 0) == 0x01);
static_assert(set_right<int8_t>(0x00, 0, true) == 0x01);
static_assert(set_right<int8_t>(0x00, 1, true) == 0x02);
static_assert(set_right<int8_t>(0x00, 2, true) == 0x04);
static_assert(set_right<int8_t>(0x00, 3, true) == 0x08);
static_assert(set_right<int8_t>(0x00, 4, true) == 0x10);
static_assert(set_right<int8_t>(0x00, 5, true) == 0x20);
static_assert(set_right<int8_t>(0x00, 6, true) == 0x40);
static_assert(set_right<int8_t>(0x00, 7, true) == -128);
static_assert(set_right<uint8_t>(0x00, 0, false) == 0x00u);
static_assert(set_right<uint8_t>(0x00, 1, false) == 0x00u);
static_assert(set_right<uint8_t>(0x00, 2, false) == 0x00u);
static_assert(set_right<uint8_t>(0x00, 3, false) == 0x00u);
static_assert(set_right<uint8_t>(0x00, 4, false) == 0x00u);
static_assert(set_right<uint8_t>(0x00, 5, false) == 0x00u);
static_assert(set_right<uint8_t>(0x00, 6, false) == 0x00u);
static_assert(set_right<uint8_t>(0x00, 7, false) == 0x00u);
static_assert(set_right<uint8_t>(0x00) == 0x01u);
static_assert(set_right<uint8_t>(0x00, 0) == 0x01u);
static_assert(set_right<uint8_t>(0x00, 0, true) == 0x01u);
static_assert(set_right<uint8_t>(0x00, 1, true) == 0x02u);
static_assert(set_right<uint8_t>(0x00, 2, true) == 0x04u);
static_assert(set_right<uint8_t>(0x00, 3, true) == 0x08u);
static_assert(set_right<uint8_t>(0x00, 4, true) == 0x10u);
static_assert(set_right<uint8_t>(0x00, 5, true) == 0x20u);
static_assert(set_right<uint8_t>(0x00, 6, true) == 0x40u);
static_assert(set_right<uint8_t>(0x00, 7, true) == 0x80u);
static_assert(set_right<uint8_t>(0xff, 0, true) == 0xffu);
static_assert(set_right<uint8_t>(0xff, 1, true) == 0xffu);
static_assert(set_right<uint8_t>(0xff, 2, true) == 0xffu);
static_assert(set_right<uint8_t>(0xff, 3, true) == 0xffu);
static_assert(set_right<uint8_t>(0xff, 4, true) == 0xffu);
static_assert(set_right<uint8_t>(0xff, 5, true) == 0xffu);
static_assert(set_right<uint8_t>(0xff, 6, true) == 0xffu);
static_assert(set_right<uint8_t>(0xff, 7, true) == 0xffu);
static_assert(set_right<uint8_t>(0xff, 0, false) == 0xfeu);
static_assert(set_right<uint8_t>(0xff, 1, false) == 0xfdu);
static_assert(set_right<uint8_t>(0xff, 2, false) == 0xfbu);
static_assert(set_right<uint8_t>(0xff, 3, false) == 0xf7u);
static_assert(set_right<uint8_t>(0xff, 4, false) == 0xefu);
static_assert(set_right<uint8_t>(0xff, 5, false) == 0xdfu);
static_assert(set_right<uint8_t>(0xff, 6, false) == 0xbfu);
static_assert(set_right<uint8_t>(0xff, 7, false) == 0x7fu);
static_assert(is_same_type<decltype(set_right<uint8_t>(0, 0, false)), uint8_t>);

// mask_left
static_assert(mask_left<uint8_t>() == 0x7fu);
static_assert(mask_left<uint16_t>() == 0x7fffu);
static_assert(mask_left<uint32_t>() == 0x7fffffffull);
static_assert(mask_left<uint64_t>() == 0x7fffffffffffffffull);
static_assert(mask_left<uint8_t>(0) == 0xffu);
static_assert(mask_left<uint8_t>(1) == 0x7fu);
static_assert(mask_left<uint8_t>(2) == 0x3fu);
static_assert(mask_left<uint8_t>(3) == 0x1fu);
static_assert(mask_left<uint8_t>(4) == 0x0fu);
static_assert(mask_left<uint8_t>(5) == 0x07u);
static_assert(mask_left<uint8_t>(6) == 0x03u);
static_assert(mask_left<uint8_t>(7) == 0x01u);
static_assert(mask_left<uint8_t>(8) == 0x00u);
static_assert(mask_left<uint16_t>(8) == 0x00ffu);
static_assert(mask_left<uint16_t>(8) == 0x00ffu);
static_assert(mask_left<uint32_t>(8) == 0x00fffffful);
static_assert(mask_left<uint64_t>(8) == 0x00ffffffffffffffull);
static_assert(mask_left<uint8_t>(0xff, 8) == 0x00u);
static_assert(mask_left<uint16_t>(0xffff, 8) == 0x00ffu);
static_assert(mask_left<uint32_t>(0xffffffffl, 8) == 0x00fffffful);
static_assert(mask_left<uint64_t>(0xffffffffffffffffll, 8) == 0x00ffffffffffffffull);
static_assert(is_same_type<decltype(mask_left<uint8_t>(0)), uint8_t>);

// mask_right
static_assert(mask_right<uint8_t>() == 0xfeu);
static_assert(mask_right<uint16_t>() == 0xfffeu);
static_assert(mask_right<uint32_t>() == 0xfffffffeul);
static_assert(mask_right<uint64_t>() == 0xfffffffffffffffeull);
static_assert(mask_right<uint8_t>(0) == 0xffu);
static_assert(mask_right<uint8_t>(1) == 0xfeu);
static_assert(mask_right<uint8_t>(2) == 0xfcu);
static_assert(mask_right<uint8_t>(3) == 0xf8u);
static_assert(mask_right<uint8_t>(4) == 0xf0u);
static_assert(mask_right<uint8_t>(5) == 0xe0u);
static_assert(mask_right<uint8_t>(6) == 0xc0u);
static_assert(mask_right<uint8_t>(7) == 0x80u);
static_assert(mask_right<uint8_t>(8) == 0x00u);
static_assert(mask_right<uint16_t>(8) == 0xff00u);
static_assert(mask_right<uint32_t>(8) == 0xffffff00ul);
static_assert(mask_right<uint64_t>(8) == 0xffffffffffffff00ull);
static_assert(mask_right<uint8_t>(0xff, 8) == 0x00u);
static_assert(mask_right<uint16_t>(0xffff, 8) == 0xff00u);
static_assert(mask_right<uint32_t>(0xffffffff, 8) == 0xffffff00ul);
static_assert(mask_right<uint64_t>(0xffffffffffffffff, 8) == 0xffffffffffffff00ull);
static_assert(is_same_type<decltype(mask_right<uint8_t>(0)), uint8_t>);

// unmask_left
static_assert(unmask_left<uint8_t>() == 0x80u);
static_assert(unmask_left<uint16_t>() == 0x8000u);
static_assert(unmask_left<uint32_t>() == 0x80000000ul);
static_assert(unmask_left<uint64_t>() == 0x8000000000000000ull);
static_assert(unmask_left<uint8_t>(0) == 0x00u);
static_assert(unmask_left<uint8_t>(1) == 0x80u);
static_assert(unmask_left<uint8_t>(2) == 0xc0u);
static_assert(unmask_left<uint8_t>(3) == 0xe0u);
static_assert(unmask_left<uint8_t>(4) == 0xf0u);
static_assert(unmask_left<uint8_t>(5) == 0xf8u);
static_assert(unmask_left<uint8_t>(6) == 0xfcu);
static_assert(unmask_left<uint8_t>(7) == 0xfeu);
static_assert(unmask_left<uint8_t>(8) == 0xffu);
static_assert(unmask_left<uint16_t>(8) == 0xff00u);
static_assert(unmask_left<uint32_t>(8) == 0xff000000ul);
static_assert(unmask_left<uint64_t>(8) == 0xff00000000000000ull);
static_assert(unmask_left<uint8_t>(0x00, 8) == 0xffu);
static_assert(unmask_left<uint16_t>(0x0000, 8) == 0xff00u);
static_assert(unmask_left<uint32_t>(0x00000000l, 8) == 0xff000000ul);
static_assert(unmask_left<uint64_t>(0x0000000000000000ll, 8) == 0xff00000000000000ull);
static_assert(is_same_type<decltype(unmask_left<uint8_t>(0)), uint8_t>);

// unmask_right
static_assert(unmask_right<uint8_t>() == 0x01u);
static_assert(unmask_right<uint16_t>() == 0x0001u);
static_assert(unmask_right<uint32_t>() == 0x00000001ul);
static_assert(unmask_right<uint64_t>() == 0x0000000000000001ull);
static_assert(unmask_right<uint8_t>(0) == 0u);
static_assert(unmask_right<uint8_t>(1) == 0x01u);
static_assert(unmask_right<uint8_t>(2) == 0x03u);
static_assert(unmask_right<uint8_t>(3) == 0x07u);
static_assert(unmask_right<uint8_t>(4) == 0x0fu);
static_assert(unmask_right<uint8_t>(5) == 0x1fu);
static_assert(unmask_right<uint8_t>(6) == 0x3fu);
static_assert(unmask_right<uint8_t>(7) == 0x7fu);
static_assert(unmask_right<uint8_t>(8) == 0xffu);
static_assert(unmask_right<uint16_t>(8) == 0x00ffu);
static_assert(unmask_right<uint32_t>(8) == 0x000000fful);
static_assert(unmask_right<uint64_t>(8) == 0x00000000000000ffull);
static_assert(unmask_right<uint8_t>(0x00, 8) == 0xffu);
static_assert(unmask_right<uint16_t>(0x0000, 8) == 0x00ffu);
static_assert(unmask_right<uint32_t>(0x00000000l, 8) == 0x000000fful);
static_assert(unmask_right<uint64_t>(0x0000000000000000ll, 8) == 0x00000000000000ffull);
static_assert(is_same_type<decltype(unmask_right<uint8_t>(0)), uint8_t>);

// shift_left
static_assert(shift_left<uint8_t>(0, 0, true) == 0u);
static_assert(shift_left<uint8_t>(0) == 0u);
static_assert(shift_left<uint8_t>(0, 1) == 0u);
static_assert(shift_left<uint8_t>(0, 1, true) == 0u);
static_assert(shift_left<uint8_t>(0, 8, true) == 0u);
static_assert(shift_left<uint8_t>(1, 0, true) == 1u);
static_assert(shift_left<uint8_t>(1) == 2u);
static_assert(shift_left<uint8_t>(1, 1) == 2u);
static_assert(shift_left<uint8_t>(1, 1, true) == 2u);
static_assert(shift_left<uint8_t>(1, 2, true) == 4u);
static_assert(shift_left<uint8_t>(1, 3, true) == 8u);
static_assert(shift_left<uint8_t>(1, 4, true) == 16u);
static_assert(shift_left<uint8_t>(1, 5, true) == 32u);
static_assert(shift_left<uint8_t>(1, 6, true) == 64u);
static_assert(shift_left<uint8_t>(1, 7, true) == 128u);
static_assert(shift_left<uint8_t>(1, 8, true) == 0u);
static_assert(shift_left<uint8_t>(1, 9, true) == 0u);
static_assert(shift_left<uint8_t>(0, 0, false) == 0u);
static_assert(shift_left<uint8_t>(0, 1, false) == 0u);
static_assert(shift_left<uint8_t>(0, 8, false) == 0u);
static_assert(shift_left<uint8_t>(1, 0, false) == 1u);
static_assert(shift_left<uint8_t>(1, 1, false) == 2u);
static_assert(shift_left<uint8_t>(1, 2, false) == 4u);
static_assert(shift_left<uint8_t>(1, 3, false) == 8u);
static_assert(shift_left<uint8_t>(1, 4, false) == 16u);
static_assert(shift_left<uint8_t>(1, 5, false) == 32u);
static_assert(shift_left<uint8_t>(1, 6, false) == 64u);
static_assert(shift_left<uint8_t>(1, 7, false) == 128u);
static_assert(shift_left<uint8_t>(1, 8, false) == 1u);
static_assert(shift_left<uint8_t>(1, 9, false) == 2u);
static_assert(is_same_type<decltype(shift_left<uint8_t>(0, 0, true)), uint8_t>);

// shift_left
static_assert(shift_right<uint8_t>(0x00, 0, true) == 0u);
static_assert(shift_right<uint8_t>(0x00) == 0u);
static_assert(shift_right<uint8_t>(0x00, 1) == 0u);
static_assert(shift_right<uint8_t>(0x00, 1, true) == 0u);
static_assert(shift_right<uint8_t>(0x00, 8, true) == 0u);
static_assert(shift_right<uint8_t>(0x80, 0, true) == 0x80u);
static_assert(shift_right<uint8_t>(0x80) == 0x40u);
static_assert(shift_right<uint8_t>(0x80, 1) == 0x40u);
static_assert(shift_right<uint8_t>(0x80, 1, true) == 0x40u);
static_assert(shift_right<uint8_t>(0x80, 2, true) == 0x20u);
static_assert(shift_right<uint8_t>(0x80, 3, true) == 0x10u);
static_assert(shift_right<uint8_t>(0x80, 4, true) == 0x08u);
static_assert(shift_right<uint8_t>(0x80, 5, true) == 0x04u);
static_assert(shift_right<uint8_t>(0x80, 6, true) == 0x02u);
static_assert(shift_right<uint8_t>(0x80, 7, true) == 0x01u);
static_assert(shift_right<uint8_t>(0x80, 8, true) == 0u);
static_assert(shift_right<uint8_t>(0x80, 9, true) == 0u);
static_assert(shift_right<uint8_t>(0x00, 0, false) == 0u);
static_assert(shift_right<uint8_t>(0x00, 1, false) == 0u);
static_assert(shift_right<uint8_t>(0x00, 8, false) == 0u);
static_assert(shift_right<uint8_t>(0x80, 0, false) == 0x80u);
static_assert(shift_right<uint8_t>(0x80, 1, false) == 0x40u);
static_assert(shift_right<uint8_t>(0x80, 2, false) == 0x20u);
static_assert(shift_right<uint8_t>(0x80, 3, false) == 0x10u);
static_assert(shift_right<uint8_t>(0x80, 4, false) == 0x08u);
static_assert(shift_right<uint8_t>(0x80, 5, false) == 0x04u);
static_assert(shift_right<uint8_t>(0x80, 6, false) == 0x02u);
static_assert(shift_right<uint8_t>(0x80, 7, false) == 0x01u);
static_assert(shift_right<uint8_t>(0x80, 9, false) == 0x40u);
static_assert(is_same_type<decltype(shift_left<uint8_t>(0, 0, true)), uint8_t>);

// rotate_right
static_assert(rotate_right<uint8_t>(0x00, 0) == 0x00_u8);
static_assert(rotate_right<uint8_t>(0x00)    == 0x00_u8);
static_assert(rotate_right<uint8_t>(0x00, 1) == 0x00_u8);
static_assert(rotate_right<uint8_t>(0x00, 2) == 0x00_u8);
static_assert(rotate_right<uint8_t>(0x00, 3) == 0x00_u8);
static_assert(rotate_right<uint8_t>(0x00, 5) == 0x00_u8);
static_assert(rotate_right<uint8_t>(0x00, 6) == 0x00_u8);
static_assert(rotate_right<uint8_t>(0x00, 7) == 0x00_u8);
static_assert(rotate_right<uint8_t>(0x00, 8) == 0x00_u8);
static_assert(rotate_right<uint8_t>(0xff, 0) == 0xff_u8);
static_assert(rotate_right<uint8_t>(0xff)    == 0xff_u8);
static_assert(rotate_right<uint8_t>(0xff, 1) == 0xff_u8);
static_assert(rotate_right<uint8_t>(0xff, 2) == 0xff_u8);
static_assert(rotate_right<uint8_t>(0xff, 3) == 0xff_u8);
static_assert(rotate_right<uint8_t>(0xff, 5) == 0xff_u8);
static_assert(rotate_right<uint8_t>(0xff, 6) == 0xff_u8);
static_assert(rotate_right<uint8_t>(0xff, 7) == 0xff_u8);
static_assert(rotate_right<uint8_t>(0xff, 8) == 0xff_u8);
static_assert(rotate_right<uint8_t>(0x81, 0) == 0b10000001_u8);
static_assert(rotate_right<uint8_t>(0b10000001)    == 0b11000000_u8);
static_assert(rotate_right<uint8_t>(0b10000001, 1) == 0b11000000_u8);
static_assert(rotate_right<uint8_t>(0b10000001, 2) == 0b01100000_u8);
static_assert(rotate_right<uint8_t>(0b10000001, 3) == 0b00110000_u8);
static_assert(rotate_right<uint8_t>(0b10000001, 4) == 0b00011000_u8);
static_assert(rotate_right<uint8_t>(0b10000001, 5) == 0b00001100_u8);
static_assert(rotate_right<uint8_t>(0b10000001, 6) == 0b00000110_u8);
static_assert(rotate_right<uint8_t>(0b10000001, 7) == 0b00000011_u8);
static_assert(rotate_right<uint8_t>(0b10000001, 8) == 0b10000001_u8);
static_assert(is_same_type<decltype(rotate_right<uint8_t>(0, 0)), uint8_t>);

static_assert(rotate_right<int8_t>(0_i8, 0) == 0_i8);
static_assert(rotate_right<int8_t>(0_i8)    == 0_i8);
static_assert(rotate_right<int8_t>(0_i8, 1) == 0_i8);
static_assert(rotate_right<int8_t>(0_i8, 2) == 0_i8);
static_assert(rotate_right<int8_t>(0_i8, 3) == 0_i8);
static_assert(rotate_right<int8_t>(0_i8, 5) == 0_i8);
static_assert(rotate_right<int8_t>(0_i8, 6) == 0_i8);
static_assert(rotate_right<int8_t>(0_i8, 7) == 0_i8);
static_assert(rotate_right<int8_t>(0_i8, 8) == 0_i8);
static_assert(rotate_right<int8_t>(1_ni8, 0) == 1_ni8);
static_assert(rotate_right<int8_t>(1_ni8)    == 1_ni8);
static_assert(rotate_right<int8_t>(1_ni8, 1) == 1_ni8);
static_assert(rotate_right<int8_t>(1_ni8, 2) == 1_ni8);
static_assert(rotate_right<int8_t>(1_ni8, 3) == 1_ni8);
static_assert(rotate_right<int8_t>(1_ni8, 5) == 1_ni8);
static_assert(rotate_right<int8_t>(1_ni8, 6) == 1_ni8);
static_assert(rotate_right<int8_t>(1_ni8, 7) == 1_ni8);
static_assert(rotate_right<int8_t>(1_ni8, 8) == 1_ni8);
static_assert(rotate_right<int8_t>(0b10000001_i8, 0) == 0b10000001_i8);
static_assert(rotate_right<int8_t>(0b10000001_i8)    == 0b11000000_i8);
static_assert(rotate_right<int8_t>(0b10000001_i8, 1) == 0b11000000_i8);
static_assert(rotate_right<int8_t>(0b10000001_i8, 2) == 0b01100000_i8);
static_assert(rotate_right<int8_t>(0b10000001_i8, 3) == 0b00110000_i8);
static_assert(rotate_right<int8_t>(0b10000001_i8, 4) == 0b00011000_i8);
static_assert(rotate_right<int8_t>(0b10000001_i8, 5) == 0b00001100_i8);
static_assert(rotate_right<int8_t>(0b10000001_i8, 6) == 0b00000110_i8);
static_assert(rotate_right<int8_t>(0b10000001_i8, 7) == 0b00000011_i8);
static_assert(rotate_right<int8_t>(0b10000001_i8, 8) == 0b10000001_i8);
static_assert(is_same_type<decltype(rotate_right<int8_t>(0, 0)), int8_t>);

// en.cppreference.com/w/cpp/numeric/rotr
static_assert(rotate_right<uint8_t>(0x1d, 0) == 0x1d_u8);
static_assert(rotate_right<uint8_t>(0x1d, 1) == 0x8e_u8);
static_assert(rotate_right<uint8_t>(0x1d, 9) == 0x8e_u8);

// rotate_left
static_assert(rotate_left<uint8_t>(0x00, 0) == 0x00u);
static_assert(rotate_left<uint8_t>(0x00)    == 0x00u);
static_assert(rotate_left<uint8_t>(0x00, 1) == 0x00u);
static_assert(rotate_left<uint8_t>(0x00, 2) == 0x00u);
static_assert(rotate_left<uint8_t>(0x00, 3) == 0x00u);
static_assert(rotate_left<uint8_t>(0x00, 5) == 0x00u);
static_assert(rotate_left<uint8_t>(0x00, 6) == 0x00u);
static_assert(rotate_left<uint8_t>(0x00, 7) == 0x00u);
static_assert(rotate_left<uint8_t>(0x00, 8) == 0x00u);
static_assert(rotate_left<uint8_t>(0xff, 0) == 0xffu);
static_assert(rotate_left<uint8_t>(0xff)    == 0xffu);
static_assert(rotate_left<uint8_t>(0xff, 1) == 0xffu);
static_assert(rotate_left<uint8_t>(0xff, 2) == 0xffu);
static_assert(rotate_left<uint8_t>(0xff, 3) == 0xffu);
static_assert(rotate_left<uint8_t>(0xff, 5) == 0xffu);
static_assert(rotate_left<uint8_t>(0xff, 6) == 0xffu);
static_assert(rotate_left<uint8_t>(0xff, 7) == 0xffu);
static_assert(rotate_left<uint8_t>(0xff, 8) == 0xffu);
static_assert(rotate_left<uint8_t>(0x81, 0) == 0b10000001u);
static_assert(rotate_left<uint8_t>(0b10000001)    == 0b00000011u);
static_assert(rotate_left<uint8_t>(0b10000001, 1) == 0b00000011u);
static_assert(rotate_left<uint8_t>(0b10000001, 2) == 0b00000110u);
static_assert(rotate_left<uint8_t>(0b10000001, 3) == 0b00001100u);
static_assert(rotate_left<uint8_t>(0b10000001, 4) == 0b00011000u);
static_assert(rotate_left<uint8_t>(0b10000001, 5) == 0b00110000u);
static_assert(rotate_left<uint8_t>(0b10000001, 6) == 0b01100000u);
static_assert(rotate_left<uint8_t>(0b10000001, 7) == 0b11000000u);
static_assert(rotate_left<uint8_t>(0b10000001, 8) == 0b10000001u);
static_assert(is_same_type<decltype(rotate_left<uint8_t>(0, 0)), uint8_t>);

static_assert(rotate_left<int8_t>(0x00, 0) == 0_i8);
static_assert(rotate_left<int8_t>(0x00)    == 0_i8);
static_assert(rotate_left<int8_t>(0x00, 1) == 0_i8);
static_assert(rotate_left<int8_t>(0x00, 2) == 0_i8);
static_assert(rotate_left<int8_t>(0x00, 3) == 0_i8);
static_assert(rotate_left<int8_t>(0x00, 5) == 0_i8);
static_assert(rotate_left<int8_t>(0x00, 6) == 0_i8);
static_assert(rotate_left<int8_t>(0x00, 7) == 0_i8);
static_assert(rotate_left<int8_t>(0x00, 8) == 0_i8);
static_assert(rotate_left<int8_t>(1_ni8, 0) == 1_ni8);
static_assert(rotate_left<int8_t>(1_ni8)    == 1_ni8);
static_assert(rotate_left<int8_t>(1_ni8, 1) == 1_ni8);
static_assert(rotate_left<int8_t>(1_ni8, 2) == 1_ni8);
static_assert(rotate_left<int8_t>(1_ni8, 3) == 1_ni8);
static_assert(rotate_left<int8_t>(1_ni8, 5) == 1_ni8);
static_assert(rotate_left<int8_t>(1_ni8, 6) == 1_ni8);
static_assert(rotate_left<int8_t>(1_ni8, 7) == 1_ni8);
static_assert(rotate_left<int8_t>(1_ni8, 8) == 1_ni8);
static_assert(rotate_left<int8_t>(0b10000001_i8, 0) == 0b10000001_i8);
static_assert(rotate_left<int8_t>(0b10000001_i8)    == 0b00000011_i8);
static_assert(rotate_left<int8_t>(0b10000001_i8, 1) == 0b00000011_i8);
static_assert(rotate_left<int8_t>(0b10000001_i8, 2) == 0b00000110_i8);
static_assert(rotate_left<int8_t>(0b10000001_i8, 3) == 0b00001100_i8);
static_assert(rotate_left<int8_t>(0b10000001_i8, 4) == 0b00011000_i8);
static_assert(rotate_left<int8_t>(0b10000001_i8, 5) == 0b00110000_i8);
static_assert(rotate_left<int8_t>(0b10000001_i8, 6) == 0b01100000_i8);
static_assert(rotate_left<int8_t>(0b10000001_i8, 7) == 0b11000000_i8);
static_assert(rotate_left<int8_t>(0b10000001_i8, 8) == 0b10000001_i8);
static_assert(is_same_type<decltype(rotate_left<int8_t>(0, 0)), int8_t>);

// en.cppreference.com/w/cpp/numeric/rotl
static_assert(rotate_left<uint8_t>(0x1d, 0) == 0x1d_u8);
static_assert(rotate_left<uint8_t>(0x1d, 1) == 0x3a_u8);
static_assert(rotate_left<uint8_t>(0x1d, 9) == 0x3a_u8);

static_assert(hi_word<uint8_t>(0x0f02_u16) == 0x0f_u8);
static_assert(lo_word<uint8_t>(0x0f02_u16) == 0x02_u8);
static_assert(hi_word<uint16_t>(0x0f02a001_u32) == 0x0f02_u16);
static_assert(lo_word<uint16_t>(0x0f02a001_u32) == 0xa001_u16);
static_assert(hi_word<uint32_t>(0x0f000002a0000001_u64) == 0x0f000002_u32);
static_assert(lo_word<uint32_t>(0x0f000002a0000001_u64) == 0xa0000001_u32);

static_assert(hi_word<uint8_t>(uint128_t(0x0f02_u16)) == 0x0f_u8);
static_assert(lo_word<uint8_t>(uint128_t(0x0f02_u16)) == 0x02_u8);
static_assert(hi_word<uint16_t>(uint128_t(0x0f02a001_u32)) == 0x0f02_u16);
static_assert(lo_word<uint16_t>(uint128_t(0x0f02a001_u32)) == 0xa001_u16);
static_assert(hi_word<uint32_t>(uint128_t(0x0f000002a0000001_u64)) == 0x0f000002_u32);
static_assert(lo_word<uint32_t>(uint128_t(0x0f000002a0000001_u64)) == 0xa0000001_u32);
static_assert(hi_word<uint64_t>((uint128_t(0x0f000002a0000001_u64) << 64u) + 42u) == 0x0f000002a0000001_u64);
static_assert(lo_word<uint64_t>((uint128_t(0x0f000002a0000001_u64) << 64u) + 42u) == 42_u64);