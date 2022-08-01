/**
 * Copyright (c) 2011-2022 libbitcoin developers (see AUTHORS)
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
#include "functions.hpp"

BOOST_AUTO_TEST_SUITE(functions_tests)

// TODO: move to tests for accumulator/hmac/pbkd.
// TODO: add singular tests for each wrapper function here.
// merkle_root [see: /test/sha/sha256.cpp]

// rmd128: (no vectors)
// ----------------------------------------------------------------------------

////BOOST_AUTO_TEST_CASE(functions__rmd128_hash__test_vectors__expected)
////{
////    for (const auto& test: rmd128_tests)
////    {
////        const auto hash = accumulator<rmd128>::hash(test.data);
////        BOOST_REQUIRE_EQUAL(hash, test.expected);
////    }
////}

// rmd160: accumulator<rmd160> (no hmac/pbkd vectors)
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(functions__rmd160_hash__test_vectors__expected)
{
    for (const auto& test: ripemd_tests)
    {
        const auto hash = ripemd160_hash(test.data);
        ////const auto hash = accumulator<rmd160>::hash(test.data);
        BOOST_REQUIRE_EQUAL(hash, test.expected);
    }
}

// sha1: accumulator<sha160> (no hmac/pbkd vectors)
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(functions__sha160_hash__test_vectors__expected)
{
    for (const auto& test: sha1_tests)
    {
        const auto hash = accumulator<sha160>::hash(test.data);
        BOOST_REQUIRE_EQUAL(hash, test.expected);
    }
}

// sha256: accumulator<sha256>/hmac<sha256>/pbkd<sha256>
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(functions__sha256_hash__test_vectors__expected)
{
    for (const auto& test: sha256_tests)
    {
        const auto hash = accumulator<sha256>::hash(test.data);
        BOOST_REQUIRE_EQUAL(hash, test.expected);
    }
}

BOOST_AUTO_TEST_CASE(functions__hmac_sha256__test_vectors__expected)
{
    for (const auto& test: hmac_sha256_tests)
    {
        const auto hash = hmac<sha256>::code(test.text, test.key);
        BOOST_REQUIRE_EQUAL(hash, test.expected);
    }
}

BOOST_AUTO_TEST_CASE(functions__pbkd_sha256__test_vectors__expected)
{
    for (const auto& test: pbkd_sha256_tests)
    {
        const auto hash = pbkd<sha256>::key<long_hash_size>(test.passphrase, test.salt, test.count);
        BOOST_REQUIRE_EQUAL(hash, test.expected);
    }
}

// sha512: accumulator<sha512>/hmac<sha512>/pbkd<sha512>
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(functions__sha512_hash__test_vectors__expected)
{
    for (const auto& test: sha512_tests)
    {
        const auto hash = accumulator<sha512>::hash_slice(test.data);
        BOOST_REQUIRE_EQUAL(hash, test.expected);
    }
}

BOOST_AUTO_TEST_CASE(functions__hmac_sha512__test_vectors__expected)
{
    for (const auto& test: hmac_sha512_tests)
    {
        const auto hash = hmac<sha512>::code(test.text, test.key);
        BOOST_REQUIRE_EQUAL(hash, test.expected);
    }
}

BOOST_AUTO_TEST_CASE(functions__pbkd_sha512__test_vectors__expected)
{

    for (const auto& test: pbkd_sha512_tests)
    {
        const auto hash = pbkd<sha512>::key<long_hash_size>(test.passphrase, test.salt, test.count);
        BOOST_REQUIRE_EQUAL(hash, test.expected);
    }
}

// scrypt_hash (litecoin)
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(functions__scrypt_hash__test_vectors__expected)
{
    for (const auto& test: scrypt_tests)
    {
        const auto hash = scrypt_hash(test.data);
        BOOST_REQUIRE_EQUAL(hash, test.expected);
    }
}

// scrypt (rfc7914 test vectors)
// ----------------------------------------------------------------------------

template<size_t W, size_t R, size_t P, bool C>
class scrypt_accessor
  : public scrypt<W, R, P, C>
{
public:
    using base = scrypt<W, R, P, C>;
    using block_t = typename base::block_t;
    using rblock_t = typename base::rblock_t;

    static void salsa_8(block_t& block) NOEXCEPT
    {
        base::salsa_8(block);
    }

    static bool block_mix(rblock_t& rblock) NOEXCEPT
    {
        return base::block_mix(rblock);
    }

    static bool romix(rblock_t& rblock) NOEXCEPT
    {
        return base::romix(rblock);
    }
};

BOOST_AUTO_TEST_CASE(functions__scrypt__rfc7914_salsa_8__expected)
{
    // W/R/P unused.
    using test = scrypt_accessor<16, 1, 1, true>;
    constexpr auto expected = base16_array("a41f859c6608cc993b81cacb020cef05044b2181a2fd337dfd7b1c6396682f29b4393168e3c9e6bcfe6bc5b7a06d96bae424cc102c91745c24ad673dc7618f81");
    auto data = base16_array("7e879a214f3ec9867ca940e641718f26baee555b8c61c1b50df846116dcd3b1dee24f319df9b3d8514121e4b5ac5aa3276021d2909c74829edebc68db8b8c25e");
    test::salsa_8(data);
    BOOST_REQUIRE_EQUAL(data, expected);
}

BOOST_AUTO_TEST_CASE(functions__scrypt__rfc7914_block_mix__expected)
{
    // W/P unused.
    using test = scrypt_accessor<16, 1, 1, true>;
    constexpr auto expected = base16_array("a41f859c6608cc993b81cacb020cef05044b2181a2fd337dfd7b1c6396682f29b4393168e3c9e6bcfe6bc5b7a06d96bae424cc102c91745c24ad673dc7618f8120edc975323881a80540f64c162dcd3c21077cfe5f8d5fe2b1a4168f953678b77d3b3d803b60e4ab920996e59b4d53b65d2a225877d5edf5842cb9f14eefe425");
    auto data = base16_array("f7ce0b653d2d72a4108cf5abe912ffdd777616dbbb27a70e8204f3ae2d0f6fad89f68f4811d1e87bcc3bd7400a9ffd29094f0184639574f39ae5a1315217bcd7894991447213bb226c25b54da86370fbcd984380374666bb8ffcb5bf40c254b067d27c51ce4ad5fed829c90b505a571b7f4d1cad6a523cda770e67bceaaf7e89");
    BOOST_REQUIRE(test::block_mix(array_cast<test::block_t>(data)));
    BOOST_REQUIRE_EQUAL(data, expected);
}

BOOST_AUTO_TEST_CASE(functions__scrypt__rfc7914_romix__expected)
{
    // P unused.
    using test = scrypt_accessor<16, 1, 1, true>;
    constexpr auto expected = base16_array("79ccc193629debca047f0b70604bf6b62ce3dd4a9626e355fafc6198e6ea2b46d58413673b99b029d665c357601fb426a0b2f4bba200ee9f0a43d19b571a9c71ef1142e65d5a266fddca832ce59faa7cac0b9cf1be2bffca300d01ee387619c4ae12fd4438f203a0e4e1c47ec314861f4e9087cb33396a6873e8f9d2539a4b8e");
    auto data = base16_array("f7ce0b653d2d72a4108cf5abe912ffdd777616dbbb27a70e8204f3ae2d0f6fad89f68f4811d1e87bcc3bd7400a9ffd29094f0184639574f39ae5a1315217bcd7894991447213bb226c25b54da86370fbcd984380374666bb8ffcb5bf40c254b067d27c51ce4ad5fed829c90b505a571b7f4d1cad6a523cda770e67bceaaf7e89");
    BOOST_REQUIRE(test::romix(array_cast<test::block_t>(data)));
    BOOST_REQUIRE_EQUAL(data, expected);
}

BOOST_AUTO_TEST_CASE(functions__scrypt__rfc7914_hash_1__expected)
{
    using test = scrypt<16, 1, 1, true>;
    constexpr auto expected = base16_array("77d6576238657b203b19ca42c18a0497f16b4844e3074ae8dfdffa3fede21442fcd0069ded0948f8326a753a0fc81f17e8d3e0fb2e0d3628cf35e20c38d18906");
    constexpr auto size = size_of<decltype(expected)>();
    const auto hash = test::hash<size>("", "");
    BOOST_REQUIRE_EQUAL(hash, expected);
}

#if defined(HAVE_SLOW_TESTS)
BOOST_AUTO_TEST_CASE(functions__scrypt__rfc7914_hash_2__expected)
{
    using test = scrypt<1024, 8, 16, true>;
    constexpr auto expected = base16_array("fdbabe1c9d3472007856e7190d01e9fe7c6ad7cbc8237830e77376634b3731622eaf30d92e22a3886ff109279d9830dac727afb94a83ee6d8360cbdfa2cc0640");
    constexpr auto size = size_of<decltype(expected)>();
    const auto hash = test::hash<size>("password", "NaCl");
    BOOST_REQUIRE_EQUAL(hash, expected);
}

BOOST_AUTO_TEST_CASE(functions__scrypt__rfc7914_hash_3__expected)
{
    using test = scrypt<16384, 8, 1, true>;
    constexpr auto expected = base16_array("7023bdcb3afd7348461c06cd81fd38ebfda8fbba904f8e3ea9b543f6545da1f2d5432955613f0fcf62d49705242a9af9e61e85dc0d651e40dfcf017b45575887");
    constexpr auto size = size_of<decltype(expected)>();
    const auto hash = test::hash<size>("pleaseletmein", "SodiumChloride");
    BOOST_REQUIRE_EQUAL(hash, expected);
}

BOOST_AUTO_TEST_CASE(functions__scrypt__rfc7914_hash_4__expected)
{
    using test = scrypt<1048576, 8, 1, true>;
    constexpr auto expected = base16_array("2101cb9b6a511aaeaddbbe09cf70f881ec568d574a2ffd4dabe5ee9820adaa478e56fd8f4ba5d09ffa1c6d927c40f4c337304049e8a952fbcbf45c6fa77a41a4");
    constexpr auto size = size_of<decltype(expected)>();
    const auto hash = test::hash<size>("pleaseletmein", "SodiumChloride");
    BOOST_REQUIRE_EQUAL(hash, expected);
}
#endif

BOOST_AUTO_TEST_CASE(functions__scrypt__rfc7914_pbkdf2_hmac_sha256_1__expected)
{
    // c=1, dkLen=64
    constexpr auto expected = base16_array("55ac046e56e3089fec1691c22544b605f94185216dde0465e68b9d57c20dacbc49ca9cccf179b645991664b39d77ef317c71b845b1e30bd509112041d3a19783");
    constexpr auto size = size_of<decltype(expected)>(); 
    const auto hash = pbkd<sha256>::key<size>("passwd", "salt", 1);
    BOOST_REQUIRE_EQUAL(hash, expected);
}

BOOST_AUTO_TEST_CASE(functions__scrypt__rfc7914_pbkdf2_hmac_sha256_2__expected)
{
    // c=80000, dkLen=64
    constexpr auto expected = base16_array("4ddcd8f60b98be21830cee5ef22701f9641a4418d04c0414aeff08876b34ab56a1d425a1225833549adb841b51c9b3176a272bdebba1d078478f62b397f33c8d");
    constexpr auto size = size_of<decltype(expected)>();
    const auto hash = pbkd<sha256>::key<size>("Password", "NaCl", 80000);
    BOOST_REQUIRE_EQUAL(hash, expected);
}

// djb2
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(functions__djb2_hash__ad_hoc__0xe1669c01)
{
    const auto hash = djb2_hash("01234567890abcdefghijklmnopqrstuvwxyz");

    if constexpr (sizeof(size_t) == sizeof(uint32_t))
    {
        BOOST_REQUIRE_EQUAL(hash, 3781598209_u32);
    }
    else
    {
        BOOST_REQUIRE_EQUAL(hash, 9646636626660989953_u64);
    }
}

BOOST_AUTO_TEST_SUITE_END()
