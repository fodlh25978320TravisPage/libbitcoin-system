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
#ifndef LIBBITCOIN_SYSTEM_HASH_RMD_ALGORITHM_HPP
#define LIBBITCOIN_SYSTEM_HASH_RMD_ALGORITHM_HPP

#include <bitcoin/system/define.hpp>
#include <bitcoin/system/hash/algorithm.hpp>
#include <bitcoin/system/math/math.hpp>

 // This file is a common include for rmd.
#include <bitcoin/system/hash/rmd/rmd.hpp>
#include <bitcoin/system/hash/rmd/rmd128.hpp>
#include <bitcoin/system/hash/rmd/rmd160.hpp>

namespace libbitcoin {
namespace system {
namespace rmd {

/// RMD hashing algorithm.
template <typename RMD, bool Vectorized = true, bool Concurrent = false,
    if_same<typename RMD::T, rmdh_t> = true>
class algorithm : algorithm_t
{
public:
    /// Types.
    /// -----------------------------------------------------------------------

    /// Aliases.
    using H         = RMD;
    using K         = typename RMD::K;
    using word_t    = typename RMD::word_t;
    using state_t   = typename RMD::state_t;

    /// Word-based types.
    using chunk_t   = std_array<word_t, RMD::chunk_words>;
    using words_t   = std_array<word_t, RMD::block_words>;
    using buffer_t  = std_array<word_t, K::rounds>;

    /// Byte-based types.
    using byte_t    = uint8_t;
    using half_t    = std_array<byte_t, RMD::chunk_words * RMD::word_bytes>;
    using block_t   = std_array<byte_t, RMD::block_words * RMD::word_bytes>;
    using digest_t  = std_array<byte_t, bytes<RMD::digest>>;
    using blocks_t  = std_vector<cref<block_t>>;

    /// Constants (and count_t).
    /// -----------------------------------------------------------------------
    /// count_t is always uint64_t for rmd.

    static constexpr auto count_bits    = RMD::block_words * RMD::word_bytes;
    static constexpr auto count_bytes   = bytes<count_bits>;
    using count_t = unsigned_exact_type<bytes<count_bits>>;

    static constexpr auto limit_bits    = maximum<count_t> - count_bits;
    static constexpr auto limit_bytes   = to_floored_bytes(limit_bits);
    static constexpr auto vectorized    = Vectorized;
    static constexpr auto concurrent    = Concurrent;
    static constexpr auto big_end_count = false;

    /// Hashing (finalized).
    /// -----------------------------------------------------------------------

    static constexpr digest_t hash(const block_t& block) NOEXCEPT;
    static constexpr digest_t hash(const half_t& half) NOEXCEPT;

    /// Streamed hashing (unfinalized).
    /// -----------------------------------------------------------------------

    static VCONSTEXPR void accumulate(state_t& state, const blocks_t& blocks) NOEXCEPT;
    static constexpr void accumulate(state_t& state, const block_t& block) NOEXCEPT;
    
    /// Finalize streaming state (converts state to little-endian bytes).
    static constexpr digest_t finalize(const state_t& state) NOEXCEPT;
    static constexpr void finalize(digest_t& digest,
        const state_t& state) NOEXCEPT;

protected:
    /// Functions
    /// -----------------------------------------------------------------------
    
    INLINE static constexpr auto f0(auto x, auto y, auto z) NOEXCEPT;
    INLINE static constexpr auto f1(auto x, auto y, auto z) NOEXCEPT;
    INLINE static constexpr auto f2(auto x, auto y, auto z) NOEXCEPT;
    INLINE static constexpr auto f3(auto x, auto y, auto z) NOEXCEPT;
    INLINE static constexpr auto f4(auto x, auto y, auto z) NOEXCEPT;

    /// Rounds
    /// -----------------------------------------------------------------------

    template<size_t Round, typename Auto>
    static CONSTEVAL auto functor() NOEXCEPT;

    template<size_t Round>
    INLINE static constexpr auto round(auto& a, auto b, auto c, auto d,
        auto x) NOEXCEPT;

    template<size_t Round>
    INLINE static constexpr auto round(auto& a, auto b, auto& c, auto d,
        auto e, auto x) NOEXCEPT;

    template<size_t Round>
    INLINE static constexpr void round(auto& state,
        const auto& words) NOEXCEPT;

    INLINE static constexpr void compress(state_t& state, const words_t& words) NOEXCEPT;
    INLINE static constexpr void summarize(state_t& out, const state_t& batch1,
        const state_t& batch2) NOEXCEPT;

    /// Padding
    /// -----------------------------------------------------------------------
    INLINE static constexpr void pad_one(words_t& words) NOEXCEPT;
    INLINE static constexpr void pad_half(words_t& words) NOEXCEPT;
    INLINE static constexpr void pad_n(words_t& words, count_t blocks) NOEXCEPT;
    
    /// Parsing
    /// -----------------------------------------------------------------------
    INLINE static constexpr void input(words_t& words, const block_t& block) NOEXCEPT;
    INLINE static constexpr void input(words_t& words, const half_t& half) NOEXCEPT;
    INLINE static constexpr digest_t output(const state_t& state) NOEXCEPT;

private:
    // Specialized padding type.
    using blocks_pad_t = std_array<word_t, subtract(RMD::block_words,
        count_bytes / RMD::word_bytes)>;

    static CONSTEVAL auto& concurrency() NOEXCEPT;
    static CONSTEVAL chunk_t chunk_pad() NOEXCEPT;
    static CONSTEVAL words_t block_pad() NOEXCEPT;
    static CONSTEVAL blocks_pad_t blocks_pad() NOEXCEPT;
};

} // namespace rmd
} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/hash/rmd/algorithm.ipp>

#endif
