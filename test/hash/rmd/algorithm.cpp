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
#include "../../test.hpp"
#include "../hash.hpp"
    
BOOST_AUTO_TEST_SUITE(rmd_algorithm_tests)

// Other test vectors are dependent upon the correctness of these.
static_assert(rmd128::hash(rmd128::half_t{})  == rmd_half128);
static_assert(rmd128::hash(rmd128::block_t{}) == rmd_full128);
static_assert(rmd160::hash(rmd160::half_t{})  == rmd_half160);
static_assert(rmd160::hash(rmd160::block_t{}) == rmd_full160);

// rmd128
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(sha__rmd128_hash__null_hash__expected)
{
    // Correlate non-const-evaluated to const-evaluated.
    BOOST_REQUIRE_EQUAL(rmd128::hash(rmd128::half_t{}), rmd_half128);
    BOOST_REQUIRE_EQUAL(rmd128::hash(rmd128::block_t{}), rmd_full128);
}

BOOST_AUTO_TEST_CASE(sha__accumulator_rmd128_hash__test_vectors__expected)
{
    // Verify non-const-evaluated to against public vectors.
    for (const auto& test: rmd128_tests)
    {
        const auto hash = accumulator<rmd128>::hash(test.data);
        BOOST_REQUIRE_EQUAL(hash, test.expected);
    }
}

// rmd160
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(rmd__rmd160_hash__null_hash__expected)
{
    // Correlate non-const-evaluated to const-evaluated.
    BOOST_REQUIRE_EQUAL(rmd160::hash(rmd160::half_t{}), rmd_half160);
    BOOST_REQUIRE_EQUAL(rmd160::hash(rmd160::block_t{}), rmd_full160);
}

BOOST_AUTO_TEST_CASE(rmd__rmd160_hash__test_vectors__expected)
{
    // Verify non-const-evaluated to against public vectors
    for (const auto& test: rmd160_tests)
    {
        const auto hash = accumulator<rmd160>::hash(test.data);
        BOOST_REQUIRE_EQUAL(hash, test.expected);
    }
}

// Verify types.
// ----------------------------------------------------------------------------

using namespace rmd;

// k<>
static_assert(k<128>::strength == 128);
static_assert(k<128>::rounds == 128);
static_assert(k<128>::columns == 16);
static_assert(k<128>::rows == 8);
static_assert(k<128>::size == 256);
static_assert(is_same_type<k<128>::constants_t, std_array<uint32_t, 8>>);
static_assert(is_same_type<k<128>::schedule_t, std_array<uint32_t, 128>>);
static_assert(k<160>::strength == 160);
static_assert(k<160>::rounds == 160);
static_assert(k<160>::columns == 16);
static_assert(k<160>::rows == 10);
static_assert(k<160>::size == 256);
static_assert(is_same_type<k<160>::constants_t, std_array<uint32_t, 10>>);
static_assert(is_same_type<k<160>::schedule_t, std_array<uint32_t, 160>>);

// k128
static_assert(k128::strength == 128);
static_assert(k128::rounds == 128);
static_assert(k128::columns == 16);
static_assert(k128::rows == 8);
static_assert(k128::size == 256);
static_assert(k128::get[0] == 0x00000000);
static_assert(k128::get[3] == 0x8f1bbcdc);
static_assert(k128::get[4] == 0x50a28be6);
static_assert(k128::get[7] == 0x00000000);
static_assert(k128::word[0] == 0u);
static_assert(k128::word[127] == 14u);
static_assert(k128::rot[0] == 11u);
static_assert(k128::rot[127] == 8u);
static_assert(is_same_type<k128::constants_t, std_array<uint32_t, 8>>);
static_assert(is_same_type<k128::schedule_t, std_array<uint32_t, 128>>);

// k160
static_assert(k160::strength == 160);
static_assert(k160::rounds == 160);
static_assert(k160::columns == 16);
static_assert(k160::rows == 10);
static_assert(k160::size == 256);
static_assert(k160::get[0] == 0x00000000);
static_assert(k160::get[4] == 0xa953fd4e);
static_assert(k160::get[5] == 0x50a28be6);
static_assert(k160::get[9] == 0x00000000);
static_assert(k160::word[0] == 0u);
static_assert(k160::word[159] == 11u);
static_assert(k160::rot[0] == 11u);
static_assert(k160::rot[159] == 11u);
static_assert(is_same_type<k160::constants_t, std_array<uint32_t, 10>>);
static_assert(is_same_type<k160::schedule_t, std_array<uint32_t, 160>>);

// h<k128,...>
static_assert(h<k128>::size == 256);
static_assert(h<k128>::digest == 128);
static_assert(h<k128>::strength == 128);
static_assert(h<k128, 128>::word_bits == 32);
static_assert(h<k128, 128>::word_bytes == 4);
static_assert(h<k128, 128>::chunk_words == 8);
static_assert(h<k128, 128>::block_words == 16);
static_assert(h<k128, 128>::state_words == 4);
static_assert(is_same_type<h<k128, 128>::K, k128>);
static_assert(is_same_type<h<k128, 128>::word_t, uint32_t>);
static_assert(is_same_type<h<k128, 128>::state_t, std_array<uint32_t, 4>>);

// h<k160,...>
////static_assert(h<sha::k160>::size == 256);
static_assert(h<k160>::size == 256);
static_assert(h<k160>::digest == 160);
static_assert(h<k160>::strength == 160);
static_assert(h<k160, 160>::word_bits == 32);
static_assert(h<k160, 160>::word_bytes == 4);
static_assert(h<k160, 160>::chunk_words == 8);
static_assert(h<k160, 160>::block_words == 16);
static_assert(h<k160, 160>::state_words == 5);
static_assert(is_same_type<h<k160, 160>::K, k160>);
static_assert(is_same_type<h<k160, 160>::word_t, uint32_t>);
static_assert(is_same_type<h<k160, 160>::state_t, std_array<uint32_t, 5>>);

// h128<>
static_assert(h128<>::size == 256);
static_assert(h128<>::rounds == 128);
static_assert(h128<>::digest == 128);
static_assert(h128<>::strength == 128);
static_assert(h128<>::get[0] == 0x67452301);
static_assert(h128<>::get[3] == 0x10325476);
static_assert(h128<>::get.size() == 4);
static_assert(h128<>::K::rounds == 128);
static_assert(h128<>::K::get[0] == 0x00000000);
static_assert(h128<>::K::get[7] == 0x00000000);
static_assert(h128<>::K::get.size() == 8);

// h160<>
static_assert(h160<>::size == 256);
static_assert(h160<>::rounds == 160);
static_assert(h160<>::digest == 160);
static_assert(h160<>::strength == 160);
static_assert(h160<>::get[0] == 0x67452301);
static_assert(h160<>::get[4] == 0xc3d2e1f0);
static_assert(h160<>::get.size() == 5);
static_assert(h160<>::K::rounds == 160);
static_assert(h160<>::K::get[0] == 0x00000000);
static_assert(h160<>::K::get[4] == 0xa953fd4e);
static_assert(h160<>::K::get[5] == 0x50a28be6);
static_assert(h160<>::K::get[9] == 0x00000000);
static_assert(h160<>::K::get.size() == 10);

// rmd128
static_assert(!rmd128::big_end_count);
static_assert(rmd128::count_bits == 64u);
static_assert(rmd128::count_bytes == 8u);
static_assert(rmd128::H::get.size() == 4u);
static_assert(rmd128::K::get.size() == 8u);
static_assert(rmd128::limit_bits == std::numeric_limits<uint64_t>::max() - 64u);
static_assert(rmd128::limit_bytes == rmd128::limit_bits / byte_bits);
static_assert(is_same_type<rmd128::byte_t, uint8_t>);
static_assert(is_same_type<rmd128::word_t, uint32_t>);
static_assert(is_same_type<rmd128::state_t, std_array<uint32_t, 4>>);
static_assert(is_same_type<rmd128::chunk_t, std_array<uint32_t, 8>>);
static_assert(is_same_type<rmd128::words_t, std_array<uint32_t, 16>>);
static_assert(is_same_type<rmd128::block_t, std_array<uint8_t, 64>>);
static_assert(is_same_type<rmd128::half_t, std_array<uint8_t, 32>>);
static_assert(is_same_type<rmd128::digest_t, std_array<uint8_t, 16>>);
static_assert(is_same_type<rmd128::count_t, uint64_t>);
static_assert(is_same_type<decltype(rmd128::limit_bits), const uint64_t>);
static_assert(is_same_type<decltype(rmd128::limit_bytes), const uint64_t>);

// rmd160
static_assert(!rmd160::big_end_count);
static_assert(rmd160::count_bits == 64u);
static_assert(rmd160::count_bytes == 8u);
static_assert(rmd160::H::get.size() == 5u);
static_assert(rmd160::K::get.size() == 10u);
static_assert(rmd160::limit_bits == std::numeric_limits<uint64_t>::max() - 64u);
static_assert(rmd160::limit_bytes == rmd160::limit_bits / byte_bits);
static_assert(is_same_type<rmd160::byte_t, uint8_t>);
static_assert(is_same_type<rmd160::word_t, uint32_t>);
static_assert(is_same_type<rmd160::state_t, std_array<uint32_t, 5>>);
static_assert(is_same_type<rmd160::chunk_t, std_array<uint32_t, 8>>);
static_assert(is_same_type<rmd160::words_t, std_array<uint32_t, 16>>);
static_assert(is_same_type<rmd160::block_t, std_array<uint8_t, 64>>);
static_assert(is_same_type<rmd160::half_t, std_array<uint8_t, 32>>);
static_assert(is_same_type<rmd160::digest_t, std_array<uint8_t, 20>>);
static_assert(is_same_type<rmd160::count_t, uint64_t>);
static_assert(is_same_type<decltype(rmd160::limit_bits), const uint64_t>);
static_assert(is_same_type<decltype(rmd160::limit_bytes), const uint64_t>);

// Expansions.
static_assert(!rmd128_256::big_end_count);
static_assert(!rmd160_320::big_end_count);

// Verify indirection (functor and offset selection).
// ----------------------------------------------------------------------------

using k_128 = typename rmd128::K;
using rmd_128 = rmd::algorithm<rmd::h128<>, true>;

struct accessor128
  : public rmd_128
{
    template<size_t Round>
    static constexpr auto functor()
    {
        return rmd_128::functor<Round, uint32_t>();
    }

    static constexpr auto get_f0()
    {
        return &rmd_128::f0<uint32_t, uint32_t, uint32_t>;
    }

    static constexpr auto get_f1()
    {
        return &rmd_128::f1<uint32_t, uint32_t, uint32_t>;
    }

    static constexpr auto get_f2()
    {
        return &rmd_128::f2<uint32_t, uint32_t, uint32_t>;
    }

    static constexpr auto get_f3()
    {
        return &rmd_128::f3<uint32_t, uint32_t, uint32_t>;
    }
};

// Selection on round/columns (16) boundary yields the following indexes.
// batch1
static_assert( 0 / k_128::columns == 0);
static_assert(15 / k_128::columns == 0);
static_assert(16 / k_128::columns == 1);
static_assert(31 / k_128::columns == 1);
static_assert(32 / k_128::columns == 2);
static_assert(47 / k_128::columns == 2);
static_assert(48 / k_128::columns == 3);
static_assert(63 / k_128::columns == 3);
// batch2
static_assert( 64 / k_128::columns == 4);
static_assert( 79 / k_128::columns == 4);
static_assert( 80 / k_128::columns == 5);
static_assert( 95 / k_128::columns == 5);
static_assert( 96 / k_128::columns == 6);
static_assert(111 / k_128::columns == 6);
static_assert(112 / k_128::columns == 7);
static_assert(127 / k_128::columns == 7);

// Word index selection is on round boundary.
// batch1
static_assert(k_128::word[ 0] == 0);
static_assert(k_128::word[15] == 15);
static_assert(k_128::word[16] == 7);
static_assert(k_128::word[31] == 8);
static_assert(k_128::word[32] == 3);
static_assert(k_128::word[47] == 12);
static_assert(k_128::word[48] == 1);
static_assert(k_128::word[63] == 2);
// batch2
static_assert(k_128::word[ 64] == 5);
static_assert(k_128::word[ 79] == 12);
static_assert(k_128::word[ 80] == 6);
static_assert(k_128::word[ 95] == 2);
static_assert(k_128::word[ 96] == 15);
static_assert(k_128::word[111] == 13);
static_assert(k_128::word[112] == 8);
static_assert(k_128::word[127] == 14);

// Rotation selection is on round boundary.
// batch1
static_assert(k_128::rot[ 0] == 11);
static_assert(k_128::rot[15] == 8);
static_assert(k_128::rot[16] == 7);
static_assert(k_128::rot[31] == 12);
static_assert(k_128::rot[32] == 11);
static_assert(k_128::rot[47] == 5);
static_assert(k_128::rot[48] == 11);
static_assert(k_128::rot[63] == 12);
// batch2
static_assert(k_128::rot[ 64] == 8);
static_assert(k_128::rot[ 79] == 6);
static_assert(k_128::rot[ 80] == 9);
static_assert(k_128::rot[ 95] == 11);
static_assert(k_128::rot[ 96] == 9);
static_assert(k_128::rot[111] == 5);
static_assert(k_128::rot[112] == 15);
static_assert(k_128::rot[127] == 8);

// Constant selection is on round/columns boundary.
// batch1
static_assert(k_128::get[ 0 / k_128::columns] == 0x00000000);
static_assert(k_128::get[15 / k_128::columns] == 0x00000000);
static_assert(k_128::get[16 / k_128::columns] == 0x5a827999);
static_assert(k_128::get[31 / k_128::columns] == 0x5a827999);
static_assert(k_128::get[32 / k_128::columns] == 0x6ed9eba1);
static_assert(k_128::get[47 / k_128::columns] == 0x6ed9eba1);
static_assert(k_128::get[48 / k_128::columns] == 0x8f1bbcdc);
static_assert(k_128::get[63 / k_128::columns] == 0x8f1bbcdc);
// batch2
static_assert(k_128::get[ 64 / k_128::columns] == 0x50a28be6);
static_assert(k_128::get[ 79 / k_128::columns] == 0x50a28be6);
static_assert(k_128::get[ 80 / k_128::columns] == 0x5c4dd124);
static_assert(k_128::get[ 95 / k_128::columns] == 0x5c4dd124);
static_assert(k_128::get[ 96 / k_128::columns] == 0x6d703ef3);
static_assert(k_128::get[111 / k_128::columns] == 0x6d703ef3);
static_assert(k_128::get[112 / k_128::columns] == 0x00000000);
static_assert(k_128::get[127 / k_128::columns] == 0x00000000);

// Functor selection is on round/columns boundary.
// batch1
static_assert(accessor128::functor< 0>() == accessor128::get_f0());
static_assert(accessor128::functor<15>() == accessor128::get_f0());
static_assert(accessor128::functor<16>() == accessor128::get_f1());
static_assert(accessor128::functor<31>() == accessor128::get_f1());
static_assert(accessor128::functor<32>() == accessor128::get_f2());
static_assert(accessor128::functor<47>() == accessor128::get_f2());
static_assert(accessor128::functor<48>() == accessor128::get_f3());
static_assert(accessor128::functor<63>() == accessor128::get_f3());
// batch1
static_assert(accessor128::functor< 64>() == accessor128::get_f3());
static_assert(accessor128::functor< 79>() == accessor128::get_f3());
static_assert(accessor128::functor< 80>() == accessor128::get_f2());
static_assert(accessor128::functor< 95>() == accessor128::get_f2());
static_assert(accessor128::functor< 96>() == accessor128::get_f1());
static_assert(accessor128::functor<111>() == accessor128::get_f1());
static_assert(accessor128::functor<112>() == accessor128::get_f0());
static_assert(accessor128::functor<127>() == accessor128::get_f0());

using k_160 = typename rmd160::K;
using rmd_160 = rmd::algorithm<rmd::h160<>, true>;

struct accessor160
  : public rmd_160
{
    template<size_t Round>
    static constexpr auto functor()
    {
        return rmd_160::functor<Round, uint32_t>();
    }

    static constexpr auto get_f0()
    {
        return &rmd_160::f0<uint32_t, uint32_t, uint32_t>;
    }

    static constexpr auto get_f1()
    {
        return &rmd_160::f1<uint32_t, uint32_t, uint32_t>;
    }

    static constexpr auto get_f2()
    {
        return &rmd_160::f2<uint32_t, uint32_t, uint32_t>;
    }

    static constexpr auto get_f3()
    {
        return &rmd_160::f3<uint32_t, uint32_t, uint32_t>;
    }

    static constexpr auto get_f4()
    {
        return &rmd_160::f4<uint32_t, uint32_t, uint32_t>;
    }
};

// Selection on round/columns (16) boundary yields the following indexes.
// batch1
static_assert( 0 / k_160::columns == 0);
static_assert(15 / k_160::columns == 0);
static_assert(16 / k_160::columns == 1);
static_assert(31 / k_160::columns == 1);
static_assert(32 / k_160::columns == 2);
static_assert(47 / k_160::columns == 2);
static_assert(48 / k_160::columns == 3);
static_assert(63 / k_160::columns == 3);
static_assert(64 / k_160::columns == 4);
static_assert(79 / k_160::columns == 4);
// batch2
static_assert( 80 / k_160::columns == 5);
static_assert( 95 / k_160::columns == 5);
static_assert( 96 / k_160::columns == 6);
static_assert(111 / k_160::columns == 6);
static_assert(112 / k_160::columns == 7);
static_assert(127 / k_160::columns == 7);
static_assert(128 / k_160::columns == 8);
static_assert(143 / k_160::columns == 8);
static_assert(144 / k_160::columns == 9);
static_assert(159 / k_160::columns == 9);

// Word index selection is on round boundary.
// batch1
static_assert(k_160::word[ 0] == 0);
static_assert(k_160::word[15] == 15);
static_assert(k_160::word[16] == 7);
static_assert(k_160::word[31] == 8);
static_assert(k_160::word[32] == 3);
static_assert(k_160::word[47] == 12);
static_assert(k_160::word[48] == 1);
static_assert(k_160::word[63] == 2);
static_assert(k_160::word[64] == 4);
static_assert(k_160::word[79] == 13);
// batch2
static_assert(k_160::word[ 80] == 5);
static_assert(k_160::word[ 95] == 12);
static_assert(k_160::word[ 96] == 6);
static_assert(k_160::word[111] == 2);
static_assert(k_160::word[112] == 15);
static_assert(k_160::word[127] == 13);
static_assert(k_160::word[128] == 8);
static_assert(k_160::word[143] == 14);
static_assert(k_160::word[144] == 12);
static_assert(k_160::word[159] == 11);

// Rotation selection is on round boundary.
// batch1
static_assert(k_160::rot[ 0] == 11);
static_assert(k_160::rot[15] == 8);
static_assert(k_160::rot[16] == 7);
static_assert(k_160::rot[31] == 12);
static_assert(k_160::rot[32] == 11);
static_assert(k_160::rot[47] == 5);
static_assert(k_160::rot[48] == 11);
static_assert(k_160::rot[63] == 12);
static_assert(k_160::rot[64] == 9);
static_assert(k_160::rot[79] == 6);
// batch2
static_assert(k_160::rot[ 80] == 8);
static_assert(k_160::rot[ 95] == 6);
static_assert(k_160::rot[ 96] == 9);
static_assert(k_160::rot[111] == 11);
static_assert(k_160::rot[112] == 9);
static_assert(k_160::rot[127] == 5);
static_assert(k_160::rot[128] == 15);
static_assert(k_160::rot[143] == 8);
static_assert(k_160::rot[144] == 8);
static_assert(k_160::rot[159] == 11);

// Constant selection is on round/columns boundary.
// batch1
static_assert(k_160::get[ 0 / k_160::columns] == 0x00000000);
static_assert(k_160::get[15 / k_160::columns] == 0x00000000);
static_assert(k_160::get[16 / k_160::columns] == 0x5a827999);
static_assert(k_160::get[31 / k_160::columns] == 0x5a827999);
static_assert(k_160::get[32 / k_160::columns] == 0x6ed9eba1);
static_assert(k_160::get[47 / k_160::columns] == 0x6ed9eba1);
static_assert(k_160::get[48 / k_160::columns] == 0x8f1bbcdc);
static_assert(k_160::get[63 / k_160::columns] == 0x8f1bbcdc);
static_assert(k_160::get[64 / k_160::columns] == 0xa953fd4e);
static_assert(k_160::get[79 / k_160::columns] == 0xa953fd4e);
// batch2
static_assert(k_160::get[ 80 / k_160::columns] == 0x50a28be6);
static_assert(k_160::get[ 95 / k_160::columns] == 0x50a28be6);
static_assert(k_160::get[ 96 / k_160::columns] == 0x5c4dd124);
static_assert(k_160::get[111 / k_160::columns] == 0x5c4dd124);
static_assert(k_160::get[112 / k_160::columns] == 0x6d703ef3);
static_assert(k_160::get[127 / k_160::columns] == 0x6d703ef3);
static_assert(k_160::get[128 / k_160::columns] == 0x7a6d76e9);
static_assert(k_160::get[143 / k_160::columns] == 0x7a6d76e9);
static_assert(k_160::get[144 / k_160::columns] == 0x00000000);
static_assert(k_160::get[159 / k_160::columns] == 0x00000000);

// Functor selection is on round/columns boundary.
// batch1
static_assert(accessor160::functor< 0>() == accessor160::get_f0());
static_assert(accessor160::functor<15>() == accessor160::get_f0());
static_assert(accessor160::functor<16>() == accessor160::get_f1());
static_assert(accessor160::functor<31>() == accessor160::get_f1());
static_assert(accessor160::functor<32>() == accessor160::get_f2());
static_assert(accessor160::functor<47>() == accessor160::get_f2());
static_assert(accessor160::functor<48>() == accessor160::get_f3());
static_assert(accessor160::functor<63>() == accessor160::get_f3());
static_assert(accessor160::functor<64>() == accessor160::get_f4());
static_assert(accessor160::functor<79>() == accessor160::get_f4());
// batch2
static_assert(accessor160::functor< 80>() == accessor160::get_f4());
static_assert(accessor160::functor< 95>() == accessor160::get_f4());
static_assert(accessor160::functor< 96>() == accessor160::get_f3());
static_assert(accessor160::functor<111>() == accessor160::get_f3());
static_assert(accessor160::functor<112>() == accessor160::get_f2());
static_assert(accessor160::functor<127>() == accessor160::get_f2());
static_assert(accessor160::functor<128>() == accessor160::get_f1());
static_assert(accessor160::functor<143>() == accessor160::get_f1());
static_assert(accessor160::functor<144>() == accessor160::get_f0());
static_assert(accessor160::functor<159>() == accessor160::get_f0());

BOOST_AUTO_TEST_SUITE_END()
