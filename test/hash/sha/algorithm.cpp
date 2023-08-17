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
    
BOOST_AUTO_TEST_SUITE(sha_algorithm_tests)

// Other test vectors are dependent upon the correctness of these.
static_assert(sha160::hash(sha160::half_t{})  == sha_half160);
static_assert(sha160::hash(sha160::block_t{}) == sha_full160);
static_assert(sha256::hash(sha256::half_t{})  == sha_half256);
static_assert(sha256::hash(sha256::block_t{}) == sha_full256);
static_assert(sha512::hash(sha512::half_t{})  == sha_half512);
static_assert(sha512::hash(sha512::block_t{}) == sha_full512);

// Currently sha::algorithm schedule pads are cached for 1,2,3,4 blocks.
// This verifies that sha::algorithm::schedule_n() is properly configured.
// Testing any sha algorithm is sufficient to verify the configuration for all.
BOOST_AUTO_TEST_CASE(verify_array_cache)
{
    // Empty block scheduled pad is not currently cached, but included here for completeness.
    constexpr auto expected0 = base16_array("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    constexpr auto expected1 = base16_array("f5a5fd42d16a20302798ef6ed309979b43003d2320d9f0e8ea9831a92759fb4b");
    constexpr auto expected2 = base16_array("38723a2e5e8a17aa7950dc008209944e898f69a7bd10a23c839d341e935fd5ca");
    constexpr auto expected3 = base16_array("5d89f056865052bcb89c910d2d62872e029fb273c3db03f8968a52a41593c1b5");
    constexpr auto expected4 = base16_array("5341e6b2646979a70e57653007a1f310169421ec9bdd9f1a5648f75ade005af1");

    constexpr auto blocks0 = std_array<sha256::block_t, 0>{};
    constexpr auto blocks1 = std_array<sha256::block_t, 1>{};
    constexpr auto blocks2 = std_array<sha256::block_t, 2>{};
    constexpr auto blocks3 = std_array<sha256::block_t, 3>{};
    constexpr auto blocks4 = std_array<sha256::block_t, 4>{};

    static_assert(sha256::hash(blocks0) == expected0);
    static_assert(sha256::hash(blocks1) == expected1);
    static_assert(sha256::hash(blocks2) == expected2);
    static_assert(sha256::hash(blocks3) == expected3);
    static_assert(sha256::hash(blocks4) == expected4);

    // Array caching is performed independently from chunk caching, though if
    // configured correctly the cache for each is the same values/locations.
    BOOST_CHECK_EQUAL(sha256::hash(blocks0), expected0);
    BOOST_CHECK_EQUAL(sha256::hash(blocks1), expected1);
    BOOST_CHECK_EQUAL(sha256::hash(blocks2), expected2);
    BOOST_CHECK_EQUAL(sha256::hash(blocks3), expected3);
    BOOST_CHECK_EQUAL(sha256::hash(blocks4), expected4);

    const data_chunk chunk0(0 * array_count<sha256::block_t>, 0x00_u8);
    const data_chunk chunk1(1 * array_count<sha256::block_t>, 0x00_u8);
    const data_chunk chunk2(2 * array_count<sha256::block_t>, 0x00_u8);
    const data_chunk chunk3(3 * array_count<sha256::block_t>, 0x00_u8);
    const data_chunk chunk4(4 * array_count<sha256::block_t>, 0x00_u8);

    BOOST_CHECK_EQUAL(sha256_hash(chunk0), expected0);
    BOOST_CHECK_EQUAL(sha256_hash(chunk1), expected1);
    BOOST_CHECK_EQUAL(sha256_hash(chunk2), expected2);
    BOOST_CHECK_EQUAL(sha256_hash(chunk3), expected3);
    BOOST_CHECK_EQUAL(sha256_hash(chunk4), expected4);
}

using namespace sha;

// k<,>
static_assert(k<160, 80>::strength == 160);
static_assert(k<160, 80>::rounds == 80);
static_assert(k<160, 80>::columns == 20);
static_assert(k<160, 80>::rows == 8);
static_assert(k<160, 80>::size == 256);
static_assert(is_same_type<k<160, 80>::constants_t, std_array<uint32_t, 80>>);
static_assert(k<256, 64>::strength == 256);
static_assert(k<256, 64>::rounds == 64);
static_assert(k<256, 80>::columns == 20);
static_assert(k<256, 80>::rows == 12);
static_assert(k<256, 64>::size == 256);
static_assert(is_same_type<k<256, 64>::constants_t, std_array<uint32_t, 64>>);
static_assert(k<512, 80>::strength == 512);
static_assert(k<512, 80>::rounds == 80);
static_assert(k<512, 80>::columns == 20);
static_assert(k<512, 80>::rows == 25);
static_assert(k<512, 80>::size == 512);
static_assert(is_same_type<k<512, 80>::constants_t, std_array<uint64_t, 80>>);

// k160
static_assert(k160::strength == 160);
static_assert(k160::rounds == 80);
static_assert(k160::columns == 20);
static_assert(k160::rows == 8);
static_assert(k160::size == 256);
static_assert(k160::get[0] == 0x5a827999);
static_assert(k160::get[19] == 0x5a827999);
static_assert(k160::get[20] == 0x6ed9eba1);
static_assert(k160::get[39] == 0x6ed9eba1);
static_assert(k160::get[40] == 0x8f1bbcdc);
static_assert(k160::get[59] == 0x8f1bbcdc);
static_assert(k160::get[60] == 0xca62c1d6);
static_assert(k160::get[79] == 0xca62c1d6);
static_assert(is_same_type<k160::constants_t, std_array<uint32_t, 80>>);

// k256
static_assert(k256::strength == 256);
static_assert(k256::rounds == 64);
static_assert(k256::columns == 20);
static_assert(k256::rows == 12);
static_assert(k256::size == 256);
static_assert(k256::get[0] == 0x428a2f98);
static_assert(k256::get[63] == 0xc67178f2);
static_assert(is_same_type<k256::constants_t, std_array<uint32_t, 64>>);

// k512
static_assert(k512::strength == 512);
static_assert(k512::rounds == 80);
static_assert(k512::columns == 20);
static_assert(k512::rows == 25);
static_assert(k512::size == 512);
static_assert(k512::get[0] == 0x428a2f98d728ae22);
static_assert(k512::get[79] == 0x6c44198c4a475817);
static_assert(is_same_type<k512::constants_t, std_array<uint64_t, 80>>);

// h<k160,...>
////static_assert(h<rmd::k160>::size == 256);
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

// h<k256,...>
static_assert(h<k256, 256>::size == 256);
static_assert(h<k256, 224>::size == 256);
static_assert(h<k256, 256>::digest == 256);
static_assert(h<k256, 224>::digest == 224);
static_assert(h<k256, 256>::strength == 256);
static_assert(h<k256, 224>::strength == 256);
static_assert(h<k256, 256>::word_bits == 32);
static_assert(h<k256, 224>::word_bits == 32);
static_assert(h<k256, 256>::word_bytes == 4);
static_assert(h<k256, 224>::word_bytes == 4);
static_assert(h<k256, 256>::chunk_words == 8);
static_assert(h<k256, 224>::chunk_words == 8);
static_assert(h<k256, 256>::block_words == 16);
static_assert(h<k256, 224>::block_words == 16);
static_assert(h<k256, 256>::state_words == 8);
static_assert(h<k256, 224>::state_words == 8);
static_assert(is_same_type<h<k256, 256>::K, k256>);
static_assert(is_same_type<h<k256, 224>::K, k256>);
static_assert(is_same_type<h<k256, 256>::word_t, uint32_t>);
static_assert(is_same_type<h<k256, 224>::word_t, uint32_t>);
static_assert(is_same_type<h<k256, 256>::state_t, std_array<uint32_t, 8>>);
static_assert(is_same_type<h<k256, 224>::state_t, std_array<uint32_t, 8>>);

// h<k512,...>
static_assert(h<k512, 512>::size == 512);
static_assert(h<k512, 384>::size == 512);
static_assert(h<k512, 224>::size == 512);
static_assert(h<k512, 256>::size == 512);
static_assert(h<k512, 512>::digest == 512);
static_assert(h<k512, 384>::digest == 384);
static_assert(h<k512, 224>::digest == 224);
static_assert(h<k512, 256>::digest == 256);
static_assert(h<k512, 512>::strength == 512);
static_assert(h<k512, 384>::strength == 512);
static_assert(h<k512, 224>::strength == 512);
static_assert(h<k512, 256>::strength == 512);
static_assert(h<k512, 512>::word_bits == 64);
static_assert(h<k512, 384>::word_bits == 64);
static_assert(h<k512, 224>::word_bits == 64);
static_assert(h<k512, 256>::word_bits == 64);
static_assert(h<k512, 512>::word_bytes == 8);
static_assert(h<k512, 384>::word_bytes == 8);
static_assert(h<k512, 224>::word_bytes == 8);
static_assert(h<k512, 256>::word_bytes == 8);
static_assert(h<k512, 512>::chunk_words == 8);
static_assert(h<k512, 384>::chunk_words == 8);
static_assert(h<k512, 224>::chunk_words == 8);
static_assert(h<k512, 256>::chunk_words == 8);
static_assert(h<k512, 512>::block_words == 16);
static_assert(h<k512, 384>::block_words == 16);
static_assert(h<k512, 224>::block_words == 16);
static_assert(h<k512, 256>::block_words == 16);
static_assert(h<k512, 512>::state_words == 8);
static_assert(h<k512, 384>::state_words == 8);
static_assert(h<k512, 224>::state_words == 8);
static_assert(h<k512, 256>::state_words == 8);
static_assert(is_same_type<h<k512, 512>::K, k512>);
static_assert(is_same_type<h<k512, 384>::K, k512>);
static_assert(is_same_type<h<k512, 224>::K, k512>);
static_assert(is_same_type<h<k512, 256>::K, k512>);
static_assert(is_same_type<h<k512, 512>::word_t, uint64_t>);
static_assert(is_same_type<h<k512, 384>::word_t, uint64_t>);
static_assert(is_same_type<h<k512, 224>::word_t, uint64_t>);
static_assert(is_same_type<h<k512, 256>::word_t, uint64_t>);
static_assert(is_same_type<h<k512, 512>::state_t, std_array<uint64_t, 8>>);
static_assert(is_same_type<h<k512, 384>::state_t, std_array<uint64_t, 8>>);
static_assert(is_same_type<h<k512, 224>::state_t, std_array<uint64_t, 8>>);
static_assert(is_same_type<h<k512, 256>::state_t, std_array<uint64_t, 8>>);

// h160
static_assert(h160::size == 256);
static_assert(h160::rounds == 80);
static_assert(h160::digest == 160);
static_assert(h160::strength == 160);
static_assert(h160::get[0] == 0x67452301);
static_assert(h160::get[4] == 0xc3d2e1f0);
static_assert(h160::get.size() == 5);
static_assert(h160::K::rounds == 80);
static_assert(h160::K::get[0] == 0x5a827999);
static_assert(h160::K::get[79] == 0xca62c1d6);
static_assert(h160::K::get.size() == 80);

// h256<>
static_assert(h256<>::size == 256);
static_assert(h256<>::rounds == 64);
static_assert(h256<>::digest == 256);
static_assert(h256<>::strength == 256);
static_assert(h256<>::get[0] == 0x6a09e667);
static_assert(h256<>::get[7] == 0x5be0cd19);
static_assert(h256<>::get.size() == 8);
static_assert(h256<>::K::rounds == 64);
static_assert(h256<>::K::get[0] == 0x428a2f98);
static_assert(h256<>::K::get[63] == 0xc67178f2);
static_assert(h256<>::K::get.size() == 64);

// h512<>
static_assert(h512<>::size == 512);
static_assert(h512<>::rounds == 80);
static_assert(h512<>::digest == 512);
static_assert(h512<>::strength == 512);
static_assert(h512<>::get[0] == 0x6a09e667f3bcc908);
static_assert(h512<>::get[7] == 0x5be0cd19137e2179);
static_assert(h512<>::get.size() == 8);
static_assert(h512<>::K::rounds == 80);
static_assert(h512<>::K::get[0] == 0x428a2f98d728ae22);
static_assert(h512<>::K::get[79] == 0x6c44198c4a475817);
static_assert(h512<>::K::get.size() == 80);

// sha160
////static_assert(rmd160::big_end_count);
static_assert(sha160::big_end_count);
static_assert(sha160::count_bits == 64u);
static_assert(sha160::count_bytes == 8u);
static_assert(sha160::H::get.size() == 5u);
static_assert(sha160::K::get.size() == 80u);
static_assert(sha160::limit_bits == std::numeric_limits<uint64_t>::max() - 64u);
static_assert(sha160::limit_bytes == sha160::limit_bits / byte_bits);
static_assert(is_same_type<sha160::byte_t, uint8_t>);
static_assert(is_same_type<sha160::word_t, uint32_t>);
static_assert(is_same_type<sha160::state_t, std_array<uint32_t, 5>>);
static_assert(is_same_type<sha160::chunk_t, std_array<uint32_t, 8>>);
static_assert(is_same_type<sha160::words_t, std_array<uint32_t, 16>>);
static_assert(is_same_type<sha160::buffer_t, std_array<uint32_t, 80>>);
static_assert(is_same_type<sha160::block_t, std_array<uint8_t, 64>>);
static_assert(is_same_type<sha160::half_t, std_array<uint8_t, 32>>);
static_assert(is_same_type<sha160::digest_t, std_array<uint8_t, 20>>);
static_assert(is_same_type<sha160::count_t, uint64_t>);
static_assert(is_same_type<sha160::digests_t, std_vector<std_array<uint8_t, 20>>>);
static_assert(is_same_type<sha160::ablocks_t<42>, std_array<std_array<uint8_t, 64>, 42>>);
static_assert(is_same_type<sha160::iblocks_t, iterable<std_array<uint8_t, 64>>>);
static_assert(is_same_type<decltype(sha160::limit_bits), const uint64_t>);
static_assert(is_same_type<decltype(sha160::limit_bytes), const uint64_t>);

// sha256
static_assert(sha256::big_end_count);
static_assert(sha256::count_bits == 64u);
static_assert(sha256::count_bytes == 8u);
static_assert(sha256::H::get.size() == 8u);
static_assert(sha256::K::get.size() == 64u);
static_assert(sha256::limit_bits == std::numeric_limits<uint64_t>::max() - 64u);
static_assert(sha256::limit_bytes == sha256::limit_bits / byte_bits);
static_assert(is_same_type<sha256::byte_t, uint8_t>);
static_assert(is_same_type<sha256::word_t, uint32_t>);
static_assert(is_same_type<sha256::state_t, std_array<uint32_t, 8>>);
static_assert(is_same_type<sha256::chunk_t, std_array<uint32_t, 8>>);
static_assert(is_same_type<sha256::words_t, std_array<uint32_t, 16>>);
static_assert(is_same_type<sha256::buffer_t, std_array<uint32_t, 64>>);
static_assert(is_same_type<sha256::block_t, std_array<uint8_t, 64>>);
static_assert(is_same_type<sha256::half_t, std_array<uint8_t, 32>>);
static_assert(is_same_type<sha256::digest_t, std_array<uint8_t, 32>>);
static_assert(is_same_type<sha256::count_t, uint64_t>);
static_assert(is_same_type<sha256::digests_t, std_vector<std_array<uint8_t, 32>>>);
static_assert(is_same_type<sha256::ablocks_t<42>, std_array<std_array<uint8_t, 64>, 42>>);
static_assert(is_same_type<sha256::iblocks_t, iterable<std_array<uint8_t, 64>>>);
static_assert(is_same_type<decltype(sha256::limit_bits), const uint64_t>);
static_assert(is_same_type<decltype(sha256::limit_bytes), const uint64_t>);

// sha512
static_assert(sha512::big_end_count);
static_assert(sha512::count_bits == 128u);
static_assert(sha512::count_bytes == 16u);
static_assert(sha512::H::get.size() == 8u);
static_assert(sha512::K::get.size() == 80u);
static_assert(sha512::limit_bits == std::numeric_limits<uint128_t>::max() - 128u);
static_assert(sha512::limit_bytes == sha512::limit_bits / byte_bits);
static_assert(is_same_type<sha512::byte_t, uint8_t>);
static_assert(is_same_type<sha512::word_t, uint64_t>);
static_assert(is_same_type<sha512::state_t, std_array<uint64_t, 8>>);
static_assert(is_same_type<sha512::chunk_t, std_array<uint64_t, 8>>);
static_assert(is_same_type<sha512::words_t, std_array<uint64_t, 16>>);
static_assert(is_same_type<sha512::buffer_t, std_array<uint64_t, 80>>);
static_assert(is_same_type<sha512::block_t, std_array<uint8_t, 128>>);
static_assert(is_same_type<sha512::half_t, std_array<uint8_t, 64>>);
static_assert(is_same_type<sha512::digest_t, std_array<uint8_t, 64>>);
static_assert(is_same_type<sha512::count_t, uint128_t>);
static_assert(is_same_type<sha512::digests_t, std_vector<std_array<uint8_t, 64>>>);
static_assert(is_same_type<sha512::ablocks_t<42>, std_array<std_array<uint8_t, 128>, 42>>);
static_assert(is_same_type<sha512::iblocks_t, iterable<std_array<uint8_t, 128>>>);
static_assert(is_same_type<decltype(sha512::limit_bits), const uint128_t>);
static_assert(is_same_type<decltype(sha512::limit_bytes), const uint128_t>);

// Truncations.
static_assert(sha256_224::big_end_count);
static_assert(sha256_224::big_end_count);
static_assert(sha512_256::big_end_count);
static_assert(sha512_224::big_end_count);
static_assert(sha512_384::big_end_count);

// Sigma optimizations from Intel.
// ----------------------------------------------------------------------------

// Normal form.
template <unsigned int A, unsigned int B, unsigned int C>
constexpr auto sigma(auto x) noexcept
{
    using namespace sha;
    constexpr auto s = bits<decltype(x)>;
    return f::xor_
    (
        f::xor_
        (
            f::ror<A, s>(x),
            f::ror<B, s>(x)
        ),
        f::shr<C>(x)
    );
}

// This is intended for vectorization, otherwise it's a negative.
template <unsigned int A, unsigned int B, unsigned int C, bool s0512 = false>
constexpr auto sigma_ex(auto x) noexcept
{
    using namespace sha;
    constexpr auto s = bits<decltype(x)>;
    constexpr auto c = s0512 ? A : C;
    constexpr auto ac = s0512 ? C - A : A - C;
    constexpr auto bac = s0512 ? B - C : B - A;

    return f::xor_
    (
        f::shr<c> // (A) for s0_512
        (
            f::xor_
            (
                f::shr<ac> // (C-A) for s0_512
                (
                    f::xor_(f::shr<bac>(x), x) // (B-C) for s0_512
                ), x
            )
        ),
        f::shl<s-B>
        (
            f::xor_
            (
                f::shl<B-A>(x), x
            )
        )
    );
}

// sigma is symmtrical in AB, but not ABC. While the reversed order produces the same result,
// it is an incorrect result in 64 bits. C must remain in place in the optimization.
////static_assert(sigma< 7,  8, 1>(563567345_u64) == sigma_ex< 7,  8,  1, false>(563567345_u64));  // s0_512
////static_assert(sigma< 7,  8, 1>(563567345_u64) == sigma_ex< 7,  8,  1, false>(563567345_u64));  // s0_512
static_assert(sigma< 1,  8,  7>(563567345_u64) == sigma_ex< 1,  8,  7, true>(563567345_u64));      // s0_512
static_assert(sigma< 8,  1,  7>(563567345_u64) == sigma_ex< 1,  8,  7, true>(563567345_u64));      // s0_512

// In 32 bits the optimization works in s1 form but not in the modified s0 form.
static_assert(sigma< 7, 18,  3>(563567345_u32) == sigma_ex< 7, 18,  3, false>(563567345_u32));      // s0_256
static_assert(sigma<18,  7,  3>(563567345_u32) == sigma_ex< 7, 18,  3, false>(563567345_u32));      // s0_256
////static_assert(sigma< 3, 18,  7>(563567345_u32) == sigma_ex< 3, 18,  7, true>(563567345_u32));  // s0_256
////static_assert(sigma<18,  3,  7>(563567345_u32) == sigma_ex< 3, 18,  7, true>(563567345_u32));  // s0_256

static_assert(sigma<17, 19, 10>(563567345_u32) == sigma_ex<17, 19, 10, false>(563567345_u32));     // s1_256
static_assert(sigma<19, 61,  6>(563567345_u64) == sigma_ex<19, 61,  6, false>(563567345_u64));     // s1_512

static_assert(sigma<18,  3,  7>(563567345_u32) == sigma_ex< 3, 18,  7, true>(563567345_u32));
static_assert(sigma< 3, 18,  7>(563567345_u32) == sigma_ex< 3, 18,  7, true>(563567345_u32));

// Little sigma is not symmetrical.
static_assert(sigma< 7, 18,  3>(563567345_u32) == sigma_ex< 7, 18,  3>(563567345_u32));
static_assert(sigma<18,  7,  3>(563567345_u32) == sigma_ex< 7, 18,  3>(563567345_u32));
static_assert(sigma<17, 19, 10>(563567345_u32) == sigma_ex<17, 19, 10>(563567345_u32));
static_assert(sigma<19, 17, 10>(563567345_u32) == sigma_ex<17, 19, 10>(563567345_u32));

// sigma0_512 matches default rewrite with constants reversed, but is incorrect.
static_assert(sigma< 7,  8,  1>(563567345_u64) == sigma_ex< 7,  8,  1, false>(563567345_u64));
static_assert(sigma< 8,  7,  1>(563567345_u64) == sigma_ex< 7,  8,  1, false>(563567345_u64));

// sigma0_512 constants remain ordered and the rewrite is adjusted.
static_assert(sigma< 1,  8,  7>(563567345_u64) == sigma_ex< 1,  8,  7, true>(563567345_u64));
static_assert(sigma< 8,  1,  7>(563567345_u64) == sigma_ex< 1,  8,  7, true>(563567345_u64));

// also not symmetrical.
static_assert(sigma< 1,  8,  7>(563567345_u64) == sigma_ex< 1,  8,  7, true>(563567345_u64));
static_assert(sigma< 8,  1,  7>(563567345_u64) == sigma_ex< 1,  8,  7, true>(563567345_u64));
static_assert(sigma<19, 61,  6>(563567345_u64) == sigma_ex<19, 61,  6>(563567345_u64));
static_assert(sigma<61, 19,  6>(563567345_u64) == sigma_ex<19, 61,  6>(563567345_u64));

// Normal form.
template <unsigned int A, unsigned int B, unsigned int C>
constexpr auto Sigma(auto x) noexcept
{
    using namespace sha;
    constexpr auto s = bits<decltype(x)>;
    return f::xor_
    (
        f::xor_
        (
            f::ror<A, s>(x),
            f::ror<B, s>(x)
        ),
        f::ror<C, s>(x)
    );
}

// This is designed for non-vectorized code.
template <unsigned int A, unsigned int B, unsigned int C>
constexpr auto Sigma_ex(auto x) noexcept
{
    using namespace sha;
    constexpr auto s = bits<decltype(x)>;
    return f::ror<A, s>
    (
        f::xor_
        (
            f::ror<B-A, s>
            (
                f::xor_
                (
                    f::ror<C-B, s>(x), x
                )
            ), x
        )
    );
}

// Big sigma is not symmetrical.
static_assert(Sigma< 2, 13, 22>(66666_u32) == Sigma_ex< 2, 13, 22>(66666_u32));
static_assert(Sigma< 6, 11, 25>(66666_u32) == Sigma_ex< 6, 11, 25>(66666_u32));
static_assert(Sigma<28, 34, 39>(66666_u64) == Sigma_ex<28, 34, 39>(66666_u64));
static_assert(Sigma<14, 18, 41>(66666_u64) == Sigma_ex<14, 18, 41>(66666_u64));

static_assert(Sigma<22, 13, 2>(66666_u32) == Sigma_ex<22, 13, 2>(66666_u32));
static_assert(Sigma<22, 2, 13>(66666_u32) == Sigma_ex<22, 2, 13>(66666_u32));
static_assert(Sigma<13, 22, 2>(66666_u32) == Sigma_ex<13, 22, 2>(66666_u32));
static_assert(Sigma<13, 13, 2>(66666_u32) == Sigma_ex<13, 13, 2>(66666_u32));
static_assert(Sigma<2, 22, 13>(66666_u32) == Sigma_ex<2, 22, 13>(66666_u32));
static_assert(Sigma<2, 13, 22>(66666_u32) == Sigma_ex<2, 13, 22>(66666_u32));

static_assert(Sigma<25, 11, 6>(66666_u32) == Sigma_ex<25, 11, 6>(66666_u32));
static_assert(Sigma<25, 6, 11>(66666_u32) == Sigma_ex<25, 6, 11>(66666_u32));
static_assert(Sigma<11, 25, 6>(66666_u32) == Sigma_ex<11, 25, 6>(66666_u32));
static_assert(Sigma<11, 6, 25>(66666_u32) == Sigma_ex<11, 6, 25>(66666_u32));
static_assert(Sigma<6, 25, 11>(66666_u32) == Sigma_ex<6, 25, 11>(66666_u32));
static_assert(Sigma<6, 11, 25>(66666_u32) == Sigma_ex<6, 11, 25>(66666_u32));

static_assert(Sigma<39, 34, 28>(66666_u64) == Sigma_ex<39, 34, 28>(66666_u64));
static_assert(Sigma<39, 28, 34>(66666_u64) == Sigma_ex<39, 28, 34>(66666_u64));
static_assert(Sigma<34, 39, 28>(66666_u64) == Sigma_ex<34, 39, 28>(66666_u64));
static_assert(Sigma<34, 28, 39>(66666_u64) == Sigma_ex<34, 28, 39>(66666_u64));
static_assert(Sigma<28, 39, 34>(66666_u64) == Sigma_ex<28, 39, 34>(66666_u64));
static_assert(Sigma<28, 34, 39>(66666_u64) == Sigma_ex<28, 34, 39>(66666_u64));

static_assert(Sigma<41, 18, 14>(66666_u64) == Sigma_ex<41, 18, 14>(66666_u64));
static_assert(Sigma<41, 14, 18>(66666_u64) == Sigma_ex<41, 14, 18>(66666_u64));
static_assert(Sigma<18, 41, 14>(66666_u64) == Sigma_ex<18, 41, 14>(66666_u64));
static_assert(Sigma<18, 14, 41>(66666_u64) == Sigma_ex<18, 14, 41>(66666_u64));
static_assert(Sigma<14, 41, 18>(66666_u64) == Sigma_ex<14, 41, 18>(66666_u64));
static_assert(Sigma<14, 18, 41>(66666_u64) == Sigma_ex<14, 18, 41>(66666_u64));

BOOST_AUTO_TEST_SUITE_END()
