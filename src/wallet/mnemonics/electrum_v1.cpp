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
#include <bitcoin/system/wallet/mnemonics/electrum_v1.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/data/string.hpp>
#include <bitcoin/system/iostream/iostream.hpp>
#include <bitcoin/system/math/hash.hpp>
#include <bitcoin/system/math/math.hpp>
#include <bitcoin/system/wallet/context.hpp>
#include <bitcoin/system/wallet/keys/ec_private.hpp>
#include <bitcoin/system/wallet/keys/ec_public.hpp>
#include <bitcoin/system/wallet/mnemonics/language.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

// local constants
// ----------------------------------------------------------------------------

constexpr size_t stretch_iterations = 100000;

static const auto size = static_cast<int>(electrum_v1::dictionary::size());
static const auto size0 = power(size, 0);
static const auto size1 = power(size, 1);
static const auto size2 = power(size, 2);

// private static
// ----------------------------------------------------------------------------

const electrum_v1::dictionaries electrum_v1::dictionaries_
{
    {
        electrum_v1::dictionary{ language::en, electrum_v1::en },
        electrum_v1::dictionary{ language::pt, electrum_v1::pt }
    }
};

// protected static (coders)
// ----------------------------------------------------------------------------

// electrum/old_mnemonic.py#L1669
string_list electrum_v1::encoder(const data_chunk& entropy, language identifier)
{
    string_list words;
    words.reserve(word_count(entropy));
    data_source source(entropy);
    istream_reader reader(source);

    // Entropy size and dictionary existence must have been validated.
    while (!reader.is_exhausted())
    {
        // Guard: below cannot overflow even if this reads max_uint32.
        const auto value = reader.read_4_bytes_big_endian();

        // [(value/size0 + 0)%size1] is a nop, shown for consistency.
        // Pos quotient integer div/mod is floor in c++ and python[2/3].
        const auto one = (value / size0 + 0x0) % size1;
        const auto two = (value / size1 + one) % size1;
        const auto tri = (value / size2 + two) % size1;

        // Guard: any positive number modulo 1626 is always [0-1625].
        words.push_back(dictionaries_.at(one, identifier));
        words.push_back(dictionaries_.at(two, identifier));
        words.push_back(dictionaries_.at(tri, identifier));
    }

    return words;
}

// Decoder Overflow Bug
// ============================================================================
// github.com/spesmilo/electrum/issues/3149
// "While (32-long) hex seeds necessarily map to (12-long) seed
// words/"mnemonics", the inverse is not always true. For example, 'hurry idiot
// prefer sunset mention mist jaw inhale impossible kingdom rare squeeze'
// maps to 025d2f2d005036911003ca78900ca155c (33 chars)."
// By word triplet this is: [025d2f2d][00503691][1003ca789][00ca155c], where
// 'jaw inhale impossible' maps to 1003ca789 (33 bits, overflowing uint32).
// It is the distance between words that creates the overflow.
// Electrum v1 failed to catch this overflow, so "old seed" checks presently
// allow it, despite the invalidity. In other words, any list of 12/24 electrum
// v1 (en) words is considered valid v1 by later versions of electrum. It
// passes the decoded entropy to the strecher, creating the master private key
// despite the error. These seed words cannot be recovered from entropy as the
// encoding algorithm only parses 32 bits of entropy for each word triplet.
// But electrum itself cannot round trip these seeds, so we do not support it.
// Electrum concatenates hex characters for each triplet.
// The result can be an odd number of characters (9 total max).
// print('%08x' % x) emits >= 8 hex characters, and can be odd length.
// ============================================================================

// Guard: below 'value' cannot exceed 0x1003ca7a7, safe in int64_t but can
// overflow int32_t/uint32_t. Only one bit (0x[1][003ca7a7]) can overflow.
// So we upcast indexes to int64 and later check the result for overflow.
static_assert((1625 * 1) + (1625 * 1626) + (1625 * 1626 * 1626ll) ==
    0x1003ca7a7, "upper bound on electrum_v1 decoded triplet");

// electrum/old_mnemonic.py#L1682
electrum_v1::result electrum_v1::decoder(const string_list& words,
    language identifier)
{
    data_chunk entropy;
    entropy.reserve(entropy_size(words));
    data_sink sink(entropy);
    ostream_writer writer(sink);
    bit_vector overflows(words.size() / word_multiple);
    auto overflow = overflows.begin();

    // Word count and dictionary membership must have been validated.
    for (auto word = words.begin(); word != words.end();)
    {
        // Electrum modulos these indexes by size1, which are nops.
        const int64_t one = dictionaries_.index(*word++, identifier);
        const int64_t two = dictionaries_.index(*word++, identifier);
        const int64_t tri = dictionaries_.index(*word++, identifier);

        // [floored_modulo(one-0, size1)*size0] is a nop, shown for consistency.
        // Neg quotient integer div/mod is ceil in c++ and floor in python[2/3].
        const auto value =
            floored_modulo(one - 0x0, size1) * size0 +
            floored_modulo(two - one, size1) * size1 +
            floored_modulo(tri - two, size1) * size2;

        // Overflow: if true then this mnemonic was not encoded from entropy.
        *overflow++ = value > static_cast<int64_t>(max_uint32);

        // Guard: floored modulo with positive divisor is always positive.
        // Casting away a non-zero (overflow bit) will prevent round trip.
        writer.write_4_bytes_big_endian(static_cast<uint32_t>(value));
    }

    sink.flush();
    return { entropy, overflows };
}

data_chunk electrum_v1::result::overflowed_entropy() const
{
    // TODO: determine if return type should be string (base16 encoded).
    // TODO: modify return value for overflow bug.
    return entropy;
}

// electrum/keystore.py#L692
hash_digest electrum_v1::strecher(const result& result)
{
    // Denormalize entropy for overflow bug.
    const auto entropy = result.overflowed_entropy();

    auto streched = entropy;
    for (size_t i = 0; i < stretch_iterations; ++i)
        streched = sha256_hash_chunk(splice(streched, entropy));

    return to_array<hash_size>(streched);
}

// protected static (sizers)
// ----------------------------------------------------------------------------

size_t electrum_v1::entropy_bits(const data_slice& entropy)
{
    return entropy.size() * byte_bits;
}

size_t electrum_v1::entropy_bits(const string_list& words)
{
    return (words.size() * sizeof(uint32_t)) / word_multiple;
}

size_t electrum_v1::entropy_size(const string_list& words)
{
    return entropy_bits(words) / byte_bits;
}

size_t electrum_v1::word_count(const data_slice& entropy)
{
    return entropy_bits(entropy) / sizeof(uint32_t);
}

// public static
// ----------------------------------------------------------------------------

language electrum_v1::contained_by(const string_list& words,
    language identifier)
{
    return dictionaries_.contains(words, identifier);
}

bool electrum_v1::is_valid_dictionary(language identifier)
{
    return dictionaries_.exists(identifier);
}

bool electrum_v1::is_valid_entropy_size(size_t size)
{
    return size == entropy_minimum || size == entropy_maximum;
}

bool electrum_v1::is_valid_word_count(size_t count)
{
    return count == word_minimum || count == word_maximum;
}

// construction
// ----------------------------------------------------------------------------

electrum_v1::electrum_v1()
  : languages()
{
}

electrum_v1::electrum_v1(const electrum_v1& other)
  : languages(other)
{
}

electrum_v1::electrum_v1(const std::string& sentence, language identifier)
  : electrum_v1(split(sentence, identifier), identifier)
{
}

electrum_v1::electrum_v1(const string_list& words, language identifier)
  : electrum_v1(from_words(words, identifier))
{
}

electrum_v1::electrum_v1(const data_chunk& entropy, language identifier)
  : electrum_v1(from_entropy(entropy, identifier))
{
}

electrum_v1::electrum_v1(const minimum_entropy& entropy, language identifier)
  : electrum_v1(from_entropy(to_chunk(entropy), identifier))
{
}

electrum_v1::electrum_v1(const maximum_entropy& entropy, language identifier)
  : electrum_v1(from_entropy(to_chunk(entropy), identifier))
{
}

// protected
electrum_v1::electrum_v1(const data_chunk& entropy, const string_list& words,
    language identifier)
  : electrum_v1(result{ entropy, {} }, words, identifier)
{
}

// protected
electrum_v1::electrum_v1(const result& result, const string_list& words,
    language identifier)
  : languages(result.entropy, words, identifier),
    overflows_(result.overflows)
{
}

// protected static (factories)
// ----------------------------------------------------------------------------

electrum_v1 electrum_v1::from_entropy(const data_chunk& entropy,
    language identifier) const
{
    if (!is_valid_entropy_size(entropy.size()))
        return {};

    if (!dictionaries_.exists(identifier))
        return {};

    // Save entropy and derived words.
    return { entropy, encoder(entropy, identifier), identifier };
}

electrum_v1 electrum_v1::from_words(const string_list& words,
    language identifier) const
{
    if (!is_valid_word_count(words.size()))
        return {};

    // Normalize to improve chance of dictionary matching.
    const auto tokens = try_normalize(words);
    const auto lexicon = contained_by(tokens, identifier);

    if (lexicon == language::none)
        return {};

    if (identifier != language::none && lexicon != identifier)
        return {};

    // Save derived entropy and dictionary words, originals are discarded.
    return { decoder(tokens, lexicon), tokens, lexicon };
}

// public methods
// ----------------------------------------------------------------------------

ec_private electrum_v1::to_seed(const context& context) const
{
    if (!(*this))
        return {};

    // Combine base class entropy with derived class overflows.
    const result combined{ entropy_, overflows_ };

    // Sets compression to false as this is the electrum convention.
    // Causes derived payment addresses to use the uncompressed public key.
    // Context sets the version byte for derived payment addresses.
    // The private key will be invalid if the value does not ec verify.
    return { ec_scalar{ strecher(combined) }, context.address_version, false };
}

ec_public electrum_v1::to_public_key(const context& context) const
{
    if (!(*this))
        return {};

    // The public key will be invalid if the private key is invalid.
    return to_seed(context).to_public();
}

bool electrum_v1::is_overflow() const
{
    return std::any_of(overflows_.begin(), overflows_.end(), [](bool value)
    {
        return value;
    });
}

const electrum_v1::bit_vector& electrum_v1::overflows() const
{
    return overflows_;
}

} // namespace wallet
} // namespace system
} // namespace libbitcoin
