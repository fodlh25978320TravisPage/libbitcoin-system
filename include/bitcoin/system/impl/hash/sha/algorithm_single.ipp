/**
 * Copyright (c) 2011-2024 libbitcoin developers (see AUTHORS)
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
#ifndef LIBBITCOIN_SYSTEM_HASH_SHA_ALGORITHM_SINGLE_IPP
#define LIBBITCOIN_SYSTEM_HASH_SHA_ALGORITHM_SINGLE_IPP

// Single hashing.
// ============================================================================

namespace libbitcoin {
namespace system {
namespace sha {

// public
// ----------------------------------------------------------------------------

TEMPLATE
template <size_t Size>
constexpr typename CLASS::digest_t CLASS::
hash(const ablocks_t<Size>& blocks) NOEXCEPT
{
    buffer_t buffer{};
    auto state = H::get;
    iterate(state, blocks);
    schedule_n<Size>(buffer);
    compress(state, buffer);
    return output(state);
}

TEMPLATE
typename CLASS::digest_t CLASS::
hash(iblocks_t&& blocks) NOEXCEPT
{
    // Save block count, as iterable decrements.
    const auto count = blocks.size();

    buffer_t buffer{};
    auto state = H::get;
    iterate(state, blocks);
    schedule_n(buffer, count);
    compress(state, buffer);
    return output(state);
}

TEMPLATE
constexpr typename CLASS::digest_t CLASS::
hash(const block_t& block) NOEXCEPT
{
    buffer_t buffer{};
    auto state = H::get;
    input(buffer, block);
    schedule(buffer);
    compress(state, buffer);
    schedule_1(buffer);
    compress(state, buffer);
    return output(state);
}

TEMPLATE
constexpr typename CLASS::digest_t CLASS::
hash(const half_t& half) NOEXCEPT
{
    buffer_t buffer{};
    auto state = H::get;
    input_left(buffer, half);
    pad_half(buffer);
    schedule(buffer);
    compress(state, buffer);
    return output(state);
}

TEMPLATE
constexpr typename CLASS::digest_t CLASS::
hash(const half_t& left, const half_t& right) NOEXCEPT
{
    buffer_t buffer{};
    auto state = H::get;
    input_left(buffer, left);
    input_right(buffer, right);
    schedule(buffer);
    compress(state, buffer);
    schedule_1(buffer);
    compress(state, buffer);
    return output(state);
}

} // namespace sha
} // namespace system
} // namespace libbitcoin

#endif
