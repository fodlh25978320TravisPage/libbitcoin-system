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

BOOST_AUTO_TEST_SUITE(dictionaries_tests)

using namespace bc::system::wallet;

const size_t test_dictionary_size = 10;
typedef std::array<const char*, test_dictionary_size> test_words;
typedef wallet::dictionary<test_dictionary_size> test_dictionary;
typedef wallet::dictionaries<5, test_dictionary::size()> test_dictionaries;

const test_words test_words_en
{
    {
        "abandon",
        "ability",
        "able",
        "about",
        "above",
        "absent",
        "absorb",
        "abstract",
        "absurd",
        "abuse"
    }
};

const test_words test_words_es
{
    {
        "ábaco",
        "abdomen",
        "abeja",
        "abierto",
        "abogado",
        "abono",
        "aborto",
        "abrazo",
        "abrir",
        "abuelo"
    }
};

const test_words test_words_ja
{
    {
        "あいこくしん",
        "あいさつ",
        "あいだ",
        "あおぞら",
        "あかちゃん",
        "あきる",
        "あけがた",
        "あける",
        "あこがれる",
        "あさい"
    }
};

const test_words test_words_zh_Hans
{
    {
        "的",
        "一",
        "是",
        "在",
        "不",
        "了",
        "有",
        "和",
        "人",
        "这"
    }
};

const test_words test_words_zh_Hant
{
    {
        "的",
        "一",
        "是",
        "在",
        "不",
        "了",
        "有",
        "和",
        "人",
        "這"
    }
};

// This is the instance under test.
const test_dictionaries instance
{
    {
        test_dictionary{ language::en, test_words_en },
        test_dictionary{ language::es, test_words_es },
        test_dictionary{ language::ja, test_words_ja },
        test_dictionary{ language::zh_Hans, test_words_zh_Hans },
        test_dictionary{ language::zh_Hant, test_words_zh_Hant }
    }
};

BOOST_AUTO_TEST_CASE(dictionaries__count__42__42)
{
    const auto count = dictionaries<42, 24>::count();
    BOOST_REQUIRE_EQUAL(count, 42u);
}

BOOST_AUTO_TEST_CASE(dictionaries__count__instance__expected)
{
    BOOST_REQUIRE_EQUAL(instance.count(), 5u);
}

BOOST_AUTO_TEST_CASE(dictionaries__size__42__42)
{
    const auto size = dictionaries<24, 42>::size();
    BOOST_REQUIRE_EQUAL(size, 42u);
}

BOOST_AUTO_TEST_CASE(dictionaries__size__instance__expected)
{
    BOOST_REQUIRE_EQUAL(instance.size(), 10u);
}

BOOST_AUTO_TEST_CASE(dictionaries__exists__valid_identifiers__true)
{
    BOOST_REQUIRE(instance.exists(language::en));
    BOOST_REQUIRE(instance.exists(language::es));
    BOOST_REQUIRE(instance.exists(language::ja));
    BOOST_REQUIRE(instance.exists(language::zh_Hans));
    BOOST_REQUIRE(instance.exists(language::zh_Hant));
}

BOOST_AUTO_TEST_CASE(dictionaries__exists__invalid_identifier__false)
{
    BOOST_REQUIRE(!instance.exists(language::ko));
}

BOOST_AUTO_TEST_CASE(dictionaries__to_identifier__valid_names__expected)
{
    BOOST_REQUIRE(instance.to_identifier("en") == language::en);
    BOOST_REQUIRE(instance.to_identifier("es") == language::es);
    BOOST_REQUIRE(instance.to_identifier("ja") == language::ja);
    BOOST_REQUIRE(instance.to_identifier("zh_Hans") == language::zh_Hans);
    BOOST_REQUIRE(instance.to_identifier("zh_Hant") == language::zh_Hant);
}

BOOST_AUTO_TEST_CASE(dictionaries__to_identifier__invalid_name__none)
{
    BOOST_REQUIRE(instance.to_identifier("ko") == language::none);
}

BOOST_AUTO_TEST_CASE(dictionaries__to_name__valid_identifiers__expected)
{
    BOOST_REQUIRE_EQUAL(instance.to_name(language::en), "en");
    BOOST_REQUIRE_EQUAL(instance.to_name(language::es), "es");
    BOOST_REQUIRE_EQUAL(instance.to_name(language::ja), "ja");
    BOOST_REQUIRE_EQUAL(instance.to_name(language::zh_Hans), "zh_Hans");
    BOOST_REQUIRE_EQUAL(instance.to_name(language::zh_Hant), "zh_Hant");
}

BOOST_AUTO_TEST_CASE(dictionaries__to_name__invalid_identifier__empty)
{
    BOOST_REQUIRE_EQUAL(instance.to_name(language::ko), "");
}

BOOST_AUTO_TEST_CASE(dictionaries__at1__valid_langauges__expected)
{
    BOOST_REQUIRE_EQUAL(instance.at(0, language::en), test_words_en[0]);
    BOOST_REQUIRE_EQUAL(instance.at(1, language::es), test_words_es[1]);
    BOOST_REQUIRE_EQUAL(instance.at(2, language::ja), test_words_ja[2]);
    BOOST_REQUIRE_EQUAL(instance.at(3, language::zh_Hans), test_words_zh_Hans[3]);
    BOOST_REQUIRE_EQUAL(instance.at(4, language::zh_Hant), test_words_zh_Hant[4]);
}

BOOST_AUTO_TEST_CASE(dictionaries__at1__invalid_langauge__empty)
{
    BOOST_REQUIRE_EQUAL(instance.at(5, language::ko), "");
}

BOOST_AUTO_TEST_CASE(dictionaries__at1__invalid_index__empty)
{
    BOOST_REQUIRE_EQUAL(instance.at(10, language::ja), "");
}

BOOST_AUTO_TEST_CASE(dictionaries__at2__zh_Hans__expected)
{
    const auto words = instance.at({ 0, 2, 4, 6, 8, 10 }, language::zh_Hans);
    BOOST_REQUIRE_EQUAL(words[0], test_words_zh_Hans[0]);
    BOOST_REQUIRE_EQUAL(words[1], test_words_zh_Hans[2]);
    BOOST_REQUIRE_EQUAL(words[2], test_words_zh_Hans[4]);
    BOOST_REQUIRE_EQUAL(words[3], test_words_zh_Hans[6]);
    BOOST_REQUIRE_EQUAL(words[4], test_words_zh_Hans[8]);
    BOOST_REQUIRE_EQUAL(words[5], "");
}

BOOST_AUTO_TEST_CASE(dictionaries__at2__none__empty)
{
    const auto words = instance.at({ 0, 2, 4 }, language::none);
    BOOST_REQUIRE(words.empty());
}

BOOST_AUTO_TEST_CASE(dictionaries__index1__valid_words__expected)
{
    BOOST_REQUIRE_EQUAL(instance.index(test_words_en[0], language::en), 0);
    BOOST_REQUIRE_EQUAL(instance.index(test_words_es[1], language::es), 1);
    BOOST_REQUIRE_EQUAL(instance.index(test_words_ja[2], language::ja), 2);
    BOOST_REQUIRE_EQUAL(instance.index(test_words_zh_Hans[3], language::zh_Hans), 3);
    BOOST_REQUIRE_EQUAL(instance.index(test_words_zh_Hant[4], language::zh_Hant), 4);
}

BOOST_AUTO_TEST_CASE(dictionaries__index1__incorrect_identifiers___not_found)
{
    BOOST_REQUIRE_EQUAL(instance.index(test_words_en[0], language::zh_Hant), -1);
    BOOST_REQUIRE_EQUAL(instance.index(test_words_es[1], language::ja), -1);
    BOOST_REQUIRE_EQUAL(instance.index(test_words_ja[2], language::zh_Hans), -1);
    BOOST_REQUIRE_EQUAL(instance.index(test_words_zh_Hans[3], language::es), -1);
    BOOST_REQUIRE_EQUAL(instance.index(test_words_zh_Hant[4], language::en), -1);
}

BOOST_AUTO_TEST_CASE(dictionaries__index1__overlapping_wordlists___expected)
{
    // Order does not matter since these words overlap in the same position.
    BOOST_REQUIRE_EQUAL(instance.index(test_words_zh_Hans[3], language::zh_Hant), 3);
    BOOST_REQUIRE_EQUAL(instance.index(test_words_zh_Hant[3], language::zh_Hans), 3);
}

BOOST_AUTO_TEST_CASE(dictionaries__index1__invalid_identifier__not_found)
{
    BOOST_REQUIRE_EQUAL(instance.index(test_words_en[0], language::ko), -1);
}

BOOST_AUTO_TEST_CASE(dictionaries__index1__invalid_word__not_found)
{
    BOOST_REQUIRE_EQUAL(instance.index("", language::ja), -1);
}

BOOST_AUTO_TEST_CASE(dictionaries__index1__none_with_valid_word__not_found)
{
    BOOST_REQUIRE_EQUAL(instance.index(test_words_ja[0], language::none), -1);
}

BOOST_AUTO_TEST_CASE(dictionaries__index1__none_with_invalid_word__not_found)
{
    BOOST_REQUIRE_EQUAL(instance.index("foo", language::none), -1);
}

BOOST_AUTO_TEST_CASE(dictionaries__index2__words__expected)
{
    const auto indexes = instance.index(
    {
        test_words_es[0],
        test_words_es[2],
        test_words_es[4],
        test_words_es[6],
        test_words_es[8],
        test_words_zh_Hant[3]
    }, language::es);
    BOOST_REQUIRE_EQUAL(indexes[0], 0);
    BOOST_REQUIRE_EQUAL(indexes[1], 2);
    BOOST_REQUIRE_EQUAL(indexes[2], 4);
    BOOST_REQUIRE_EQUAL(indexes[3], 6);
    BOOST_REQUIRE_EQUAL(indexes[4], 8);
    BOOST_REQUIRE_EQUAL(indexes[5], -1);
}

BOOST_AUTO_TEST_CASE(dictionaries__index2__none__empty)
{
    const auto indexes = instance.index(
    {
        test_words_es[0],
        test_words_es[1],
        test_words_es[2]
    }, language::none);
    BOOST_REQUIRE(indexes.empty());
}

BOOST_AUTO_TEST_CASE(dictionaries__contains1__words__expected)
{
    BOOST_REQUIRE(instance.contains(test_words_en[0]) == language::en);
    BOOST_REQUIRE(instance.contains(test_words_es[1]) == language::es);
    BOOST_REQUIRE(instance.contains(test_words_ja[2]) == language::ja);
    BOOST_REQUIRE(instance.contains("foo") == language::none);
    BOOST_REQUIRE(instance.contains("bar") == language::none);
    BOOST_REQUIRE(instance.contains("") == language::none);
}

BOOST_AUTO_TEST_CASE(dictionaries__contains1__overlapping_wordlists__expected)
{
    // Order is not guaranteed, search can be parallelized.
    const auto lingo = instance.contains(test_words_zh_Hans[3]);
    BOOST_REQUIRE(lingo == language::zh_Hans || lingo == language::zh_Hant);
}

BOOST_AUTO_TEST_CASE(dictionaries__contains1__empty__none)
{
    BOOST_REQUIRE(instance.contains("") == language::none);
}

BOOST_AUTO_TEST_CASE(dictionaries__contains2__valid_words__expected)
{
    BOOST_REQUIRE(instance.contains(
    {
        test_words_es[0],
        test_words_es[2],
        test_words_es[4],
        test_words_es[6],
        test_words_es[8]
    }, language::es) == language::es);
}

BOOST_AUTO_TEST_CASE(dictionaries__contains2__valid_words_none__expected)
{
    BOOST_REQUIRE(instance.contains(
    {
        test_words_es[1],
        test_words_es[3],
        test_words_es[5],
        test_words_es[7],
        test_words_es[9]
    }, language::none) == language::es);
}

BOOST_AUTO_TEST_CASE(dictionaries__contains2__inconsistent_words_none__none)
{
    BOOST_REQUIRE(instance.contains(
    {
        test_words_es[1],
        test_words_es[3],
        test_words_zh_Hant[5],
        test_words_es[7],
        test_words_es[9]
    }, language::none) == language::none);
}

BOOST_AUTO_TEST_CASE(dictionaries__contains2__mismatched_identifier__none)
{
    BOOST_REQUIRE(instance.contains(
    {
        test_words_es[1],
        test_words_es[3],
        test_words_es[5],
        test_words_es[7],
        test_words_es[9]
    }, language::ja) == language::none);
}

BOOST_AUTO_TEST_CASE(dictionaries__contains2__empty__en)
{
    // This is based on the order of dictionary insertion (first).
    BOOST_REQUIRE(instance.contains(string_list{}) == language::en);
}

BOOST_AUTO_TEST_SUITE_END()
