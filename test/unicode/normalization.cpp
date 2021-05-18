/**
 * Copyright (c) 2011-2019 libbitcoin developers (see AUTHORS)
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
#include "normalization.hpp"
#include <vector>

BOOST_AUTO_TEST_SUITE(unicode_normalization_tests)

#ifdef WITH_ICU

// to_lower

BOOST_AUTO_TEST_CASE(normalization__to_lower__empty__empty)
{
    BOOST_REQUIRE(to_lower("").empty());
}

BOOST_AUTO_TEST_CASE(normalization__to_lower__lower_ascii__unchanged)
{
    const std::string lower = "abcdefghijklmnopqrstuvwxyz";
    BOOST_REQUIRE_EQUAL(to_lower(lower), lower);
}

BOOST_AUTO_TEST_CASE(normalization__to_lower__upper_ascii__lowered)
{
    const auto upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const auto expected = "abcdefghijklmnopqrstuvwxyz";
    BOOST_REQUIRE_EQUAL(to_lower(upper), expected);
}

BOOST_AUTO_TEST_CASE(normalization__to_lower__mixed_ascii__lowered)
{
    const auto mixed = "AbCdEfGhIjKlMnOpQrStUvWxYz";
    const auto expected = "abcdefghijklmnopqrstuvwxyz";
    BOOST_REQUIRE_EQUAL(to_lower(mixed), expected);
}

BOOST_AUTO_TEST_CASE(normalization__to_lower__ideographic_space__unchanged)
{
    BOOST_REQUIRE_EQUAL(to_lower(ideographic_space), ideographic_space);
}

// to_upper

BOOST_AUTO_TEST_CASE(normalization__to_upper__empty__empty)
{
    BOOST_REQUIRE(to_upper("").empty());
}

BOOST_AUTO_TEST_CASE(normalization__to_upper__upper_ascii__unchanged)
{
    const auto upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    BOOST_REQUIRE_EQUAL(to_upper(upper), upper);
}

BOOST_AUTO_TEST_CASE(normalization__to_upper__lower_ascii__uppered)
{
    const auto lower = "abcdefghijklmnopqrstuvwxyz";
    const auto expected = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    BOOST_REQUIRE_EQUAL(to_upper(lower), expected);
}

BOOST_AUTO_TEST_CASE(normalization__to_upper__mixed_ascii__uppered)
{
    const auto mixed = "AbCdEfGhIjKlMnOpQrStUvWxYz";
    const auto expected = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    BOOST_REQUIRE_EQUAL(to_upper(mixed), expected);
}

BOOST_AUTO_TEST_CASE(normalization__to_upper__ideographic_space__unchanged)
{
    BOOST_REQUIRE_EQUAL(to_upper(ideographic_space), ideographic_space);
}

// to_canonical_composition (nfc)

BOOST_AUTO_TEST_CASE(normalization__to_canonical_composition__empty__empty)
{
    BOOST_REQUIRE(to_canonical_composition("").empty());
}

// github.com/bitcoin/bips/blob/master/bip-0038.mediawiki
BOOST_AUTO_TEST_CASE(normalization__to_canonical_composition__bip38__expected)
{
    const auto original = to_string(base16_literal("cf92cc8100f0909080f09f92a9"));
    const auto expected = to_string(base16_literal("cf9300f0909080f09f92a9"));
    BOOST_REQUIRE_EQUAL(to_canonical_composition(original), expected);
}

// to_canonical_decomposition (nfkc)

BOOST_AUTO_TEST_CASE(normalization__to_canonical_decomposition__empty__empty)
{
    BOOST_REQUIRE(to_canonical_decomposition("").empty());
}

BOOST_AUTO_TEST_CASE(normalization__to_canonical_decomposition__todo__expected)
{
    ////// TODO: create test vector (this is copied from nfc).
    ////const auto original = to_string(base16_literal("cf92cc8100f0909080f09f92a9"));
    ////const auto expected = to_string(base16_literal("cf9300f0909080f09f92a9"));
    ////BOOST_REQUIRE_EQUAL(to_canonical_decomposition(original), expected);
}

// to_compatibility_composition (nfd)

BOOST_AUTO_TEST_CASE(normalization__to_compatibility_composition__empty__empty)
{
    BOOST_REQUIRE(to_compatibility_composition("").empty());
}

BOOST_AUTO_TEST_CASE(normalization__to_compatibility_composition__todo__expected)
{
    ////// TODO: create test vector (this is copied from nfc).
    ////const auto original = to_string(base16_literal("cf92cc8100f0909080f09f92a9"));
    ////const auto expected = to_string(base16_literal("cf9300f0909080f09f92a9"));
    ////BOOST_REQUIRE_EQUAL(to_compatibility_composition(original), expected);
}

// to_compatibility_demposition (nfkd)

BOOST_AUTO_TEST_CASE(normalization__to_compatibility_demposition__ideographic_space_sandwich__expected)
{
    const auto ascii_space_sandwich = "space-> <-space";
    const auto ideographic_space_sandwich = "space->　<-space";
    const auto normalized = to_compatibility_demposition(ideographic_space_sandwich);
    BOOST_REQUIRE_EQUAL(normalized.c_str(), ascii_space_sandwich);
}

BOOST_AUTO_TEST_CASE(normalization__to_compatibility_demposition__61cc81__c3a1)
{
    const std::string literal_c3a1 = "á";

    // This is copied from the the BIP39 encoding:
    // github.com/bitcoin/bips/blob/master/bip-0039/spanish.txt#L93
    const std::string literal_61cc81 = "á";

    const std::string string_c3a1 = "\xc3\xa1";
    const std::string string_61cc81 = "\x61\xcc\x81";
    BOOST_REQUIRE_EQUAL(string_c3a1, literal_c3a1);
    BOOST_REQUIRE_EQUAL(string_61cc81, literal_61cc81);

    const auto normalized_c3a1 = to_compatibility_demposition(string_c3a1);
    const auto normalized_61cc81 = to_compatibility_demposition(string_61cc81);
    BOOST_REQUIRE_EQUAL(normalized_61cc81.c_str(), string_61cc81);
    BOOST_REQUIRE_EQUAL(normalized_c3a1.c_str(), string_61cc81);
}

#endif // WITH_ICU


// is_separator

BOOST_AUTO_TEST_CASE(ascii__is_separator__all_separators__true)
{
    // Non-ASCII Unicode separator-space characters.
    BOOST_REQUIRE(is_separator(0x000000a0));
    BOOST_REQUIRE(is_separator(0x000000a0));
    BOOST_REQUIRE(is_separator(0x00001680));
    BOOST_REQUIRE(is_separator(0x00002000));
    BOOST_REQUIRE(is_separator(0x00002001));
    BOOST_REQUIRE(is_separator(0x00002002));
    BOOST_REQUIRE(is_separator(0x00002003));
    BOOST_REQUIRE(is_separator(0x00002004));
    BOOST_REQUIRE(is_separator(0x00002005));
    BOOST_REQUIRE(is_separator(0x00002006));
    BOOST_REQUIRE(is_separator(0x00002007));
    BOOST_REQUIRE(is_separator(0x00002008));
    BOOST_REQUIRE(is_separator(0x00002009));
    BOOST_REQUIRE(is_separator(0x0000200a));
    BOOST_REQUIRE(is_separator(0x0000202f));
    BOOST_REQUIRE(is_separator(0x0000205f));
    BOOST_REQUIRE(is_separator(0x00003000));
    BOOST_REQUIRE(is_separator(0x00003000));
}

BOOST_AUTO_TEST_CASE(ascii__is_separator__dash__false)
{
    BOOST_REQUIRE(!is_separator('-'));
}

// is_whitespace

BOOST_AUTO_TEST_CASE(ascii__is_whitespace__all_whitespace__true)
{
    // ASCII/Unicode whitespace characters (C whitespace).
    BOOST_REQUIRE(is_whitespace(0x00000009));
    BOOST_REQUIRE(is_whitespace(0x0000000a));
    BOOST_REQUIRE(is_whitespace(0x0000000b));
    BOOST_REQUIRE(is_whitespace(0x0000000c));
    BOOST_REQUIRE(is_whitespace(0x0000000d));
    BOOST_REQUIRE(is_whitespace(0x00000020));

    // ASCII/Unicode whitespace separator-space characters.
    BOOST_REQUIRE(is_whitespace(0x000000a0));

    // Unicode separator-space characters.
    BOOST_REQUIRE(is_whitespace(0x000000a0));
    BOOST_REQUIRE(is_whitespace(0x00001680));
    BOOST_REQUIRE(is_whitespace(0x00002000));
    BOOST_REQUIRE(is_whitespace(0x00002001));
    BOOST_REQUIRE(is_whitespace(0x00002002));
    BOOST_REQUIRE(is_whitespace(0x00002003));
    BOOST_REQUIRE(is_whitespace(0x00002004));
    BOOST_REQUIRE(is_whitespace(0x00002005));
    BOOST_REQUIRE(is_whitespace(0x00002006));
    BOOST_REQUIRE(is_whitespace(0x00002007));
    BOOST_REQUIRE(is_whitespace(0x00002008));
    BOOST_REQUIRE(is_whitespace(0x00002009));
    BOOST_REQUIRE(is_whitespace(0x0000200a));

    // Unicode other whitespace characters.
    BOOST_REQUIRE(is_whitespace(0x00000085));
    BOOST_REQUIRE(is_whitespace(0x00002028));
    BOOST_REQUIRE(is_whitespace(0x00002029));
}

BOOST_AUTO_TEST_CASE(ascii__is_whitespace__dash__false)
{
    BOOST_REQUIRE(!is_whitespace('-'));
}

// to_non_diacritic_form (Electrum)

BOOST_AUTO_TEST_CASE(normalization__to_non_diacritic_form__empty__empty)
{
    BOOST_REQUIRE(to_non_diacritic_form("").empty());
}

BOOST_AUTO_TEST_CASE(normalization__to_non_diacritic_form__ascii_only__unchanged)
{
    const std::u32string ascii{ 0x42, 0x43 };
    const std::string expected{ 0x42, 0x43 };
    BOOST_REQUIRE_EQUAL(to_non_diacritic_form(to_utf8(ascii)), expected);
}

BOOST_AUTO_TEST_CASE(normalization__to_non_diacritic_form__circumflex_accent_only__empty)
{
    const std::u32string circumflex_accent{ 0x0000005e };
    BOOST_REQUIRE(to_non_diacritic_form(to_utf8(circumflex_accent)).empty());
}

BOOST_AUTO_TEST_CASE(normalization__to_non_diacritic_form__bassa_vah_tones_only__empty)
{
    const std::u32string bassa_vah{ 0x00016af1, 0x00016af2, 0x00016af3 };
    BOOST_REQUIRE(to_non_diacritic_form(to_utf8(bassa_vah)).empty());
}

BOOST_AUTO_TEST_CASE(normalization__to_non_diacritic_form__bassa_vah_tones_ascii_sandwich__stripped)
{
    const std::string expected{ 0x42, 0x43 };
    const std::u32string bassa_vah{ 0x42, 0x00016af1, 0x00016af2, 0x00016af3, 0x43 };
    BOOST_REQUIRE_EQUAL(to_non_diacritic_form(to_utf8(bassa_vah)), expected);
}

BOOST_AUTO_TEST_CASE(normalization__to_non_diacritic_form__circumflex_accent_and_bassa_vah_tones_and_ascii_mix__stripped)
{
    const std::string expected{ 0x42, 0x43, 0x44 };
    const std::u32string bassa_vah{ 0x00016af1, 0x42, 0x43, 0x0000005e, 0x00016af2, 0x44, 0x00016af3 };
    BOOST_REQUIRE_EQUAL(to_non_diacritic_form(to_utf8(bassa_vah)), expected);
}

// to_compressed_cjk_form (Electrum)

BOOST_AUTO_TEST_CASE(normalization__to_compressed_cjk_form__empty__empty)
{
    BOOST_REQUIRE(to_compressed_cjk_form("").empty());
}

BOOST_AUTO_TEST_CASE(normalization__to_compressed_cjk_form__ascii_with_spaces__unchanged)
{
    const auto value = " foo  bar  baz ";
    const auto result = to_compressed_cjk_form(value);
    BOOST_REQUIRE_EQUAL(result.c_str(), value);
}

BOOST_AUTO_TEST_CASE(normalization__to_compressed_cjk__cjk_unified_ideograph_only__unchanged)
{
    const std::u32string cjk_unified_ideograph{ 0x00004e00 };
    const auto utf8 = to_utf8(cjk_unified_ideograph);
    BOOST_REQUIRE_EQUAL(to_compressed_cjk_form(utf8), utf8);
}

BOOST_AUTO_TEST_CASE(normalization__to_compressed_cjk__cjk_unified_ideograph_sandwich__stripped)
{
    const std::u32string expected{ 0x00004e00, 0x00004e00 };
    const std::u32string cjk_unified_ideograph_sandwich{ 0x00004e00, 0x20, 0x00004e00 };
    const auto utf8 = to_utf8(cjk_unified_ideograph_sandwich);
    BOOST_REQUIRE_EQUAL(to_compressed_cjk_form(utf8), to_utf8(expected));
}

BOOST_AUTO_TEST_CASE(normalization__to_compressed_cjk__cjk_unified_ideograph_right__unchanged)
{
    const std::u32string expected{ 0x20, 0x00004e00, 0x00004e00 };
    const std::u32string cjk_unified_ideograph_right{ 0x20, 0x00004e00, 0x00004e00 };
    const auto utf8 = to_utf8(cjk_unified_ideograph_right);
    BOOST_REQUIRE_EQUAL(to_compressed_cjk_form(utf8), to_utf8(expected));
}

BOOST_AUTO_TEST_CASE(normalization__to_compressed_cjk__cjk_unified_ideograph_left__unchanged)
{
    const std::u32string expected{ 0x00004e00, 0x00004e00, 0x20 };
    const std::u32string cjk_unified_ideograph_left{ 0x00004e00, 0x00004e00, 0x20 };
    const auto utf8 = to_utf8(cjk_unified_ideograph_left);
    BOOST_REQUIRE_EQUAL(to_compressed_cjk_form(utf8), to_utf8(expected));
}

BOOST_AUTO_TEST_CASE(normalization__to_compressed_cjk__cjk_unified_ideograph_both__unchanged)
{
    const std::u32string expected{ 0x20, 0x00004e00, 0x00004e00, 0x20 };
    const std::u32string cjk_unified_ideograph_both{ 0x20, 0x00004e00, 0x00004e00, 0x20 };
    const auto utf8 = to_utf8(cjk_unified_ideograph_both);
    BOOST_REQUIRE_EQUAL(to_compressed_cjk_form(utf8), to_utf8(expected));
}

BOOST_AUTO_TEST_CASE(normalization__to_compressed_cjk__cjk_unified_ideograph_all__middle_stripped)
{
    const std::u32string expected{ 0x20, 0x00004e00, 0x00004e00, 0x20 };
    const std::u32string cjk_unified_ideograph_all{ 0x20, 0x00004e00, 0x0c, 0x00004e00, 0x20 };
    const auto utf8 = to_utf8(cjk_unified_ideograph_all);
    BOOST_REQUIRE_EQUAL(to_compressed_cjk_form(utf8), to_utf8(expected));
}

BOOST_AUTO_TEST_CASE(normalization__to_compressed_cjk__single_and_double_contained_whitespace__single_stripped)
{
    const std::u32string expected
    {
        0x20, 0x00004e00, 0x00004e00, 0x20,
        0x20, 0x00004e00, 0x00004e00, 0x20,
        0x20, 0x00004e00, 0x00004e00, 0x20,
        0x20, 0x00004e00, 0x00004e00, 0x20,
        0x20, 0x00004e00, 0x00004e00, 0x20,
        0x20, 0x00004e00, 0x00004e00, 0x20
    };
    const std::u32string single_and_double_contained
    {
        0x20, 0x00004e00, 0x20, 0x00004e00, 0x20,
        0x20, 0x00004e00, 0x0c, 0x00004e00, 0x20,
        0x20, 0x00004e00, 0x0a, 0x00004e00, 0x20,
        0x20, 0x00004e00, 0x0d, 0x00004e00, 0x20,
        0x20, 0x00004e00, 0x09, 0x00004e00, 0x20,
        0x20, 0x00004e00, 0x0b, 0x00004e00, 0x20
    };
    const auto utf8 = to_utf8(single_and_double_contained);
    BOOST_REQUIRE_EQUAL(to_compressed_cjk_form(utf8), to_utf8(expected));
}

BOOST_AUTO_TEST_CASE(normalization__to_compressed_cjk__alternating_single_contained_whitespace__contained_stripped)
{
    const std::u32string expected
    {
        0x20, 
        0x00004e00, 0x00004e00,
        0x00004e00, 0x00004e00,
        0x00004e00, 0x00004e00,
        0x00004e00, 0x00004e00,
        0x00004e00, 0x00004e00,
        0x00004e00, 0x00004e00,
        0x20
    };
    const std::u32string alternating_single_contained
    {
        0x20, 0x00004e00, 0x20, 0x00004e00,
        0x20, 0x00004e00, 0x20, 0x00004e00,
        0x20, 0x00004e00, 0x20, 0x00004e00,
        0x20, 0x00004e00, 0x20, 0x00004e00,
        0x20, 0x00004e00, 0x20, 0x00004e00,
        0x20, 0x00004e00, 0x20, 0x00004e00,
        0x20
    };
    const auto utf8 = to_utf8(alternating_single_contained);
    BOOST_REQUIRE_EQUAL(to_compressed_cjk_form(utf8), to_utf8(expected));
}

// is_separator

BOOST_AUTO_TEST_CASE(normalization__is_separator__points__17)
{
    size_t points = 0;
    for (uint32_t character = 0; character < maximum_code_point; ++character)
        if (is_separator(character))
            ++points;

    BOOST_REQUIRE_EQUAL(points, char32_separators_count);
}

// is_whitespace

BOOST_AUTO_TEST_CASE(normalization__is_whitespace__points__25)
{
    size_t points = 0;
    for (uint32_t character = 0; character < maximum_code_point; ++character)
        if (is_whitespace(character))
            ++points;

    BOOST_REQUIRE_EQUAL(points, char32_whitespace_count);
}

// is_chinese_japanese_or_korean

BOOST_AUTO_TEST_CASE(normalization__chinese_japanese_or_korean__points__90286)
{
    size_t points = 0;
    for (size_t index = 0; index < char32_chinese_japanese_korean_count; ++index)
    {
        const auto pair = char32_chinese_japanese_korean[index];
        points += ((pair.second - pair.first) + 1u);
    }

    BOOST_REQUIRE_EQUAL(points, 90286u);
}

BOOST_AUTO_TEST_CASE(normalization__is_chinese_japanese_or_korean__points__90286)
{
    size_t points = 0;
    for (uint32_t character = 0; character < maximum_code_point; ++character)
        if (is_chinese_japanese_or_korean(character))
            ++points;

    BOOST_REQUIRE_EQUAL(points, 90286u);
}

// is_diacritic

BOOST_AUTO_TEST_CASE(normalization__diacritics__points__882)
{
    size_t points = 0;
    for (size_t index = 0; index < char32_diacritics_count; ++index)
    {
        const auto pair = char32_diacritics[index];
        points += ((pair.second - pair.first) + 1u);
    }

    BOOST_REQUIRE_EQUAL(points, 836u);
}

BOOST_AUTO_TEST_CASE(normalization__is_diacritic__points__882)
{
    size_t points = 0;
    for (uint32_t character = 0; character < maximum_code_point; ++character)
        if (is_diacritic(character))
            ++points;

    BOOST_REQUIRE_EQUAL(points, 836u);
}

// is_combining

// TODO: determine if we have transcribed the table incorrectly (reimport using our split()).
BOOST_AUTO_TEST_CASE(normalization__is_combining__points__882)
{
    size_t points = 0;
    for (uint32_t character = 0; character < maximum_code_point; ++character)
        if (is_combining(character))
            ++points;

    BOOST_REQUIRE_EQUAL(points, 872u);
}

// TODO: determine if the python algorithm is working as expected (use python compiler).
BOOST_AUTO_TEST_CASE(normalization__is_combining__is_diacritic__differences)
{
    std::vector<uint32_t> exceptions;
    for (uint32_t character = 0; character < maximum_code_point; ++character)
        if (is_diacritic(character) != is_combining(character))
            exceptions.push_back(character);

    const std::vector<uint32_t> expected;
    BOOST_REQUIRE_EQUAL(exceptions.size(), 608u);
    BOOST_REQUIRE_EQUAL(serialize(exceptions), "94 96 168 175 180 183 184 688 689 690 691 692 693 694 695 696 697 698 699 700 701 702 703 704 705 706 707 708 709 710 711 712 713 714 715 716 717 718 719 720 721 722 723 724 725 726 727 728 729 730 731 732 733 734 735 736 737 738 739 740 741 742 743 744 745 746 747 748 749 750 751 752 753 754 755 756 757 758 759 760 761 762 763 764 765 766 767 884 885 890 900 901 1369 1442 1477 1479 1552 1553 1554 1555 1556 1557 1558 1559 1560 1561 1562 1619 1620 1621 1622 1625 1626 1627 1628 1629 1630 1631 1648 1750 1751 1752 1753 1754 1755 1756 1761 1762 1763 1764 1765 1766 1767 1768 1773 1809 1958 1959 1960 1961 1962 1963 1964 1965 1966 1967 1968 2036 2037 2045 2070 2071 2075 2076 2077 2078 2079 2080 2081 2082 2083 2085 2086 2087 2089 2090 2091 2092 2093 2137 2138 2139 2259 2260 2261 2262 2263 2264 2265 2266 2267 2268 2269 2270 2271 2272 2273 2303 2417 2558 2813 2814 2815 3157 3158 3640 3641 3642 3655 3660 3662 3768 3769 3770 3788 3902 3903 3953 3954 3956 3962 3963 3964 3965 3968 4231 4232 4233 4234 4235 4236 4239 4250 4251 4957 4958 4959 5908 5940 6089 6090 6091 6092 6093 6094 6095 6096 6097 6099 6313 6679 6680 6752 6847 6848 7142 7154 7155 7222 7288 7289 7290 7291 7292 7293 7379 7393 7415 7468 7469 7470 7471 7472 7473 7474 7475 7476 7477 7478 7479 7480 7481 7482 7483 7484 7485 7486 7487 7488 7489 7490 7491 7492 7493 7494 7495 7496 7497 7498 7499 7500 7501 7502 7503 7504 7505 7506 7507 7508 7509 7510 7511 7512 7513 7514 7515 7516 7517 7518 7519 7520 7521 7522 7523 7524 7525 7526 7527 7528 7529 7530 7616 7617 7618 7619 7632 7633 7634 7635 7636 7637 7638 7639 7640 7641 7642 7643 7644 7645 7646 7647 7648 7649 7650 7651 7652 7653 7654 7655 7656 7657 7658 7659 7660 7661 7662 7663 7664 7665 7666 7667 7668 7675 7676 8125 8127 8128 8129 8141 8142 8143 8157 8158 8159 8173 8174 8175 8189 8190 8400 8401 8402 8403 8404 8405 8406 8407 8408 8409 8410 8411 8412 8417 8421 8422 8423 8424 8425 8426 8427 8428 8429 8430 8431 8432 11647 11744 11745 11746 11747 11748 11749 11750 11751 11752 11753 11754 11755 11756 11757 11758 11759 11760 11761 11762 11763 11764 11765 11766 11767 11768 11769 11770 11771 11772 11773 11774 11775 11823 12443 12444 12540 42612 42613 42614 42615 42616 42617 42618 42619 42623 42652 42653 42654 42655 42775 42776 42777 42778 42779 42780 42781 42782 42783 42784 42785 42888 43000 43001 43014 43052 43310 43493 43643 43644 43645 43696 43698 43699 43700 43703 43704 43710 43712 43714 43867 43868 43869 43870 43871 44012 65342 65344 65392 65438 65439 65507 66045 66422 66423 66424 66425 66426 68109 68111 68152 68153 68154 68159 68898 68899 69291 69292 69702 69759 69888 69889 69890 70091 70092 70459 70750 71997 71998 72003 72160 92976 92977 92978 92979 92980 92981 92982 94095 94096 94097 94098 94099 94100 94101 94102 94103 94104 94105 94106 94107 94108 94109 94110 94111 94192 94193 113822 119141 119142 119362 119363 119364 122880 122881 122882 122883 122884 122885 122886 122888 122889 122890 122891 122892 122893 122894 122895 122896 122897 122898 122899 122900 122901 122902 122903 122904 122907 122908 122909 122910 122911 122912 122913 122915 122916 122918 122919 122920 122921 122922 123184 123185 123186 123187 123188 123189 123190 123628 123629 123630 123631 125255");
}

BOOST_AUTO_TEST_SUITE_END()
