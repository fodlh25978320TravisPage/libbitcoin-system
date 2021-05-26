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
#include "../../../test.hpp"
#include "electrum_v1.hpp"

BOOST_AUTO_TEST_SUITE(dictionaries_electrum_v1_tests)

using namespace test::dictionaries_electrum_v1;
using namespace bc::system::wallet;

const auto dictionary_count = 2u;
const auto dictionary_size = 1626;
const auto intersection_en_pt = 5;
const auto intersection_en_electrum_en = 861;
const auto intersection_en_electrum_es = 7;
const auto intersection_en_electrum_fr = 50;
const auto intersection_en_electrum_cs = 2;
const auto intersection_en_electrum_pt = 1;
const auto intersection_pt_electrum_en = 14;
const auto intersection_pt_electrum_es = 69;
const auto intersection_pt_electrum_it = 42;
const auto intersection_pt_electrum_fr = 7;
const auto intersection_pt_electrum_cs = 2;
const auto intersection_pt_electrum_pt = 185;

BOOST_AUTO_TEST_CASE(dictionaries_electrum_v1__count__all__expected)
{
    // Any new dictionary must be added below to guarantee lack of normalization.
    // Failure to do so may lead to invalid seed generation, which is very bad.
    BOOST_REQUIRE_MESSAGE(electrum_v1::dictionaries::count() == dictionary_count, "new dictionary");
}

BOOST_AUTO_TEST_CASE(dictionaries_electrum_v1__sorted__sorted__false)
{
    BOOST_REQUIRE(!sorted(electrum_v1::pt) && !electrum_v1::pt.sorted);
    BOOST_REQUIRE(!sorted(electrum_v1::en) && !electrum_v1::en.sorted);
}

// abnormal (requires WITH_ICU)

// These dictionaries are in normal form.
// So there is no need to nfkd normalize these for wordlist-based seedings.
// This also removes the ICU dependency for these language.
BOOST_AUTO_TEST_CASE(dictionaries_electrum_v1__normal__normal_words__true)
{
    // The result is definitive only when WITH_ICU is defined.
    BOOST_REQUIRE(!abnormal(electrum_v1::en));
    BOOST_REQUIRE(!abnormal(electrum_v1::pt));
}


BOOST_AUTO_TEST_CASE(dictionaries_electrum_v1__intersections__en__expected)
{
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::en, electrum_v1::en), dictionary_size);
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::en, electrum_v1::pt), intersection_en_pt);
}

BOOST_AUTO_TEST_CASE(dictionaries_electrum_v1__intersections__pt__expected)
{
    ////BOOST_REQUIRE_EQUAL(intersection(electrum_v1::pt, electrum_v1::en), intersection_en_pt);
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::pt, electrum_v1::pt), dictionary_size);
}

BOOST_AUTO_TEST_CASE(dictionaries_electrum_v1__intersections__en_electrum__expected)
{
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::en, electrum::en), intersection_en_electrum_en);
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::en, electrum::es), intersection_en_electrum_es);
    BOOST_REQUIRE(!intersects(electrum_v1::en, electrum::it));
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::en, electrum::fr), intersection_en_electrum_fr);
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::en, electrum::cs), intersection_en_electrum_cs);
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::en, electrum::pt), intersection_en_electrum_pt);
    BOOST_REQUIRE(!intersects(electrum_v1::en, electrum::ja));
    BOOST_REQUIRE(!intersects(electrum_v1::en, electrum::ko));
    BOOST_REQUIRE(!intersects(electrum_v1::en, electrum::zh_Hans));
    BOOST_REQUIRE(!intersects(electrum_v1::en, electrum::zh_Hant));
}

BOOST_AUTO_TEST_CASE(dictionaries_electrum_v1__intersections__pt_electrum__expected)
{
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::pt, electrum::en), intersection_pt_electrum_en);
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::pt, electrum::es), intersection_pt_electrum_es);
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::pt, electrum::it), intersection_pt_electrum_it);
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::pt, electrum::fr), intersection_pt_electrum_fr);
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::pt, electrum::cs), intersection_pt_electrum_cs);
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::pt, electrum::pt), intersection_pt_electrum_pt);
    BOOST_REQUIRE(!intersects(electrum_v1::pt, electrum::ja));
    BOOST_REQUIRE(!intersects(electrum_v1::pt, electrum::ko));
    BOOST_REQUIRE(!intersects(electrum_v1::pt, electrum::zh_Hans));
    BOOST_REQUIRE(!intersects(electrum_v1::pt, electrum::zh_Hant));
}

BOOST_AUTO_TEST_SUITE_END()
