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

BOOST_AUTO_TEST_SUITE(sha256_tests)

// sha256::hash

// Compares sha256::sha256_single to data < block expectation.
BOOST_AUTO_TEST_CASE(sha256__hash__less_than_one_block__expected)
{
    hash_digest hash{};
    const data_chunk chunk{ 'd', 'a', 't', 'a' };
    sha256::hash(hash.data(), chunk.size(), chunk.data());
    BOOST_REQUIRE_EQUAL(hash, base16_array("3a6eb0790f39ac87c94f3856b2dd2c5d110e6811602261a9a923d3bb23adc8b7"));
}

// Compares sha256::sha256_single data > block to expectation.
BOOST_AUTO_TEST_CASE(sha256__hash__more_than_one_block__expected)
{
    hash_digest hash{};
    const auto data = base16_array("6eb0790f39ac87c94f3856b2dd2c5d110e6811602261a9a923d3bb23adc8b73a6eb0790f39ac87c94f3856b2dd2c5d110e6811602261a9a923d3bb23adc8b73a6eb0790f39ac87c94f3856b2dd2c5d110e6811602261a9a923d3bb23adc8b7");
    sha256::hash(hash.data(), data.size(), data.data());
    BOOST_REQUIRE_EQUAL(hash, base16_array("cec2696f81dc7448039064e1d4668018853441169bd5bf7d9c5041d28369b63d"));
}

// sha256::transform[2]

// Compares sha256::transform to single via sha256_hash (one block).
BOOST_AUTO_TEST_CASE(sha256__transform2__vs_bitcoin_hash_one_block__same)
{
    const hash_list hashes
    {
        hash_digest{ 0 },
        hash_digest{ 1 }
    };

    hash_digest once{};
    hash_list doubled{ hashes };

    // Hashed single 2 x hash.
    sha256::context context{};
    sha256::update(context, hashes.size() * hash_size, hashes[0].data());
    sha256::finalize(context, once.data());
    const auto two = sha256_hash(once);

    // Hashed single hash x 2.
    context.reset();
    sha256::update(context, hash_size, hashes[0].data());
    sha256::update(context, hash_size, hashes[1].data());
    sha256::finalize(context, once.data());
    const auto twice = sha256_hash(once);
    BOOST_REQUIRE_EQUAL(twice, two);

    // Doubled hash.
    const auto blocks = to_half(hashes.size());
    auto buffer = doubled.front().data();
    sha256::transform(buffer, blocks, buffer);
    doubled.resize(one);
    BOOST_REQUIRE_EQUAL(doubled.front(), twice);
}

// Compares sha256::transform to single via sha256_hash (two blocks).
BOOST_AUTO_TEST_CASE(sha256__transform2__vs_bitcoin_hash_two_blocks__same)
{
    const hash_list hashes
    {
        hash_digest{ 0 },
        hash_digest{ 1 },
        hash_digest{ 2 },
        hash_digest{ 3 }
    };

    hash_digest once{};
    hash_list doubled{ hashes };

    // Hashed single 4 x hash.
    sha256::context context{};
    sha256::update(context, hash_size, hashes[0].data());
    sha256::update(context, hash_size, hashes[1].data());
    sha256::finalize(context, once.data());
    const auto first = sha256_hash(once);

    // Hashed single hash x 4.
    context.reset();
    sha256::update(context, hash_size, hashes[2].data());
    sha256::update(context, hash_size, hashes[3].data());
    sha256::finalize(context, once.data());
    const auto second = sha256_hash(once);

    // Doubled hash.
    const auto blocks = to_half(hashes.size());
    auto buffer = doubled.front().data();
    sha256::transform(buffer, blocks, buffer);
    doubled.resize(one);

    BOOST_REQUIRE_EQUAL(doubled[0], first);
    BOOST_REQUIRE_EQUAL(doubled[1], second);
}

// satoshi test case
// Compares sha256::transform to single via sha256_hash (32 blocks).
BOOST_AUTO_TEST_CASE(sha256__transform2__vs_sha256x2_writer__same)
{
    BC_PUSH_WARNING(NO_POINTER_ARITHMETIC)

    for (int i = 0; i <= 32; ++i)
    {
        uint8_t in[64 * 32]{};
        uint8_t out1[32 * 32]{};
        uint8_t out2[32 * 32]{};
        const auto to = [&](int j) { return out1 + 32 * j; };
        const auto from = [&](int j) { return in + 64 * j; };

        // arbitrary fill
        for (int j = 0; j < 64 * i; ++j)
            in[j] = 42;

        // streaming double hash
        for (int j = 0; j < i; ++j)
        {
            hash::sha256x2::copy sink({ to(j), to(j) + 32 });
            sink.write_bytes({ from(j), from(j) + 64 });
            sink.flush();
        }

        // in place double hash
        sha256::transform(&out2[0], i, &in[0]);
        BOOST_REQUIRE_EQUAL(memcmp(&out1[0], &out2[0], 32 * i), 0);
    }

    BC_POP_WARNING()
}

// merkle_root
// Compares sha256::merkle_root to single via sha256_hash (1/2/4/8 blocks).

// shani1, neon, sse4, portable  (1 block iteration)
hash_digest merkle_root_expectation(const hash_list& hashes)
{
    if (hashes.empty())
        return {};

    hash_list update;
    hash_list merkle{ hashes };

    while (merkle.size() > one)
    {
        if (is_odd(merkle.size()))
            merkle.push_back(merkle.back());

        for (auto it = merkle.begin(); it != merkle.end(); it += two)
            update.push_back(bitcoin_hash(splice(it[0], it[1])));

        std::swap(merkle, update);
        update.clear();
    }

    return merkle.front();
}

BOOST_AUTO_TEST_CASE(hash__to_merkle_root__one_block__expected)
{
    hash_list hashes
    {
        hash_digest{ 0 },
        hash_digest{ 1 }
    };

    const auto root = merkle_root_expectation(hashes);

    // shani1, neon, sse4, portable  (1 block)
    to_merkle_root(hashes);
    BOOST_REQUIRE_EQUAL(hashes.front(), root);
}

BOOST_AUTO_TEST_CASE(hash__to_merkle_root__two_blocks__expected)
{
    hash_list hashes
    {
        hash_digest{ 0 },
        hash_digest{ 1 },
        hash_digest{ 2 },
        hash_digest{ 3 }
    };

    const auto root = merkle_root_expectation(hashes);

    // shani2 (2 blocks)
    // shani1, neon, sse4, portable  (1 block)
    to_merkle_root(hashes);
    BOOST_REQUIRE_EQUAL(hashes.front(), root);
}

// BUGBUG: throws on 32 bit builds.
BOOST_AUTO_TEST_CASE(hash__hto_merkle_root__four_blocks__expected)
{
    hash_list hashes
    {
        hash_digest{ 0 },
        hash_digest{ 1 },
        hash_digest{ 2 },
        hash_digest{ 3 },
        hash_digest{ 4 },
        hash_digest{ 5 },
        hash_digest{ 6 },
        hash_digest{ 7 }
    };

    const auto root = merkle_root_expectation(hashes);

    // sse41 (4 blocks)
    // shani2 (2 blocks)
    // shani1, neon, sse4, portable  (1 block)
    to_merkle_root(hashes);
    BOOST_REQUIRE_EQUAL(hashes.front(), root);
}

// TODO: This should throw if above throws, unless bug is integer alignment.
BOOST_AUTO_TEST_CASE(hash__to_merkle_root__eight_blocks__expected)
{
    hash_list hashes
    {
        hash_digest{ 0 },
        hash_digest{ 1 },
        hash_digest{ 2 },
        hash_digest{ 3 },
        hash_digest{ 4 },
        hash_digest{ 5 },
        hash_digest{ 6 },
        hash_digest{ 7 },
        hash_digest{ 8 },
        hash_digest{ 9 },
        hash_digest{ 10 },
        hash_digest{ 11 },
        hash_digest{ 12 },
        hash_digest{ 13 },
        hash_digest{ 14 },
        hash_digest{ 15 }
    };

    const auto root = merkle_root_expectation(hashes);

    // avx2 (8 blocks)
    // sse41 (4 blocks)
    // shani2 (2 blocks)
    // shani1, neon, sse4, portable  (1 block)
    to_merkle_root(hashes);
    BOOST_REQUIRE_EQUAL(hashes.front(), root);
}

BOOST_AUTO_TEST_SUITE_END()
