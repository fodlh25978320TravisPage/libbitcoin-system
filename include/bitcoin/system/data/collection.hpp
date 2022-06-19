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
#ifndef LIBBITCOIN_SYSTEM_DATA_COLLECTION_HPP
#define LIBBITCOIN_SYSTEM_DATA_COLLECTION_HPP

#include <array>
#include <cstddef>
#include <iterator>
#include <memory>
#include <ranges>
#include <utility>
#include <vector>

namespace libbitcoin {
namespace system {

/// Find the position of an element in a *lexically sorted* collection.
/// Returns the position or negative if not found or list size > max_int32.
template <typename Collection, typename Element>
typename Collection::difference_type
constexpr binary_search(const Collection& list,
    const Element& element) noexcept;

/// Copy a collection of From members to a new collection of To members.
template <typename To, typename From>
inline To projection(const From& source) noexcept;

/// Determine if collection of pointers to elements have equal elements.
template <typename Left, typename Right>
constexpr bool deep_equal(const Left& left, const Right& right) noexcept;

/// Determine if a collection contains the specified element.
template <typename Collection, typename Element>
constexpr bool contains(const Collection& list,
    const Element& element) noexcept;

/// Find the position of a std::pair in an ordered list.
template <typename Collection>
typename Collection::difference_type
constexpr find_pair_position(const Collection& list,
    const typename Collection::value_type::first_type& key) noexcept;

/// Find the position of an element in an ordered collection.
template <typename Collection>
typename Collection::difference_type
constexpr find_position(const Collection& list,
    const typename Collection::value_type& value) noexcept;

/// Facilitate a list insertion sort by inserting into a sorted position.
template <typename Collection, typename Predicate>
typename Collection::iterator
constexpr insert_sorted(Collection& list,
    const typename Collection::value_type& element,
    Predicate predicate) noexcept;

/// Pop an element from the stack and return its value.
template <typename Collection>
typename Collection::value_type
inline pop(Collection& stack) noexcept;

/// Determine if a collection contains only distinct members.
template <typename Collection>
constexpr bool is_distinct(Collection&& list) noexcept;
template <typename Collection>
constexpr bool is_distinct(const Collection& list) noexcept;

/// Determine if a collection is lexically sorted.
template <typename Collection>
constexpr bool is_sorted(const Collection& list) noexcept;

/// Obtain the (sorted) distinct elements of a collection.
template <typename Collection>
constexpr void distinct(Collection& list) noexcept;
template <typename Collection>
inline Collection distinct(Collection&& list) noexcept;
template <typename Collection>
inline Collection distinct_copy(const Collection& list) noexcept;

/// Obtain the set difference of left less right.
template <typename Left, typename Right>
inline Left difference(const Left& left, const Right& right) noexcept;
template <typename Left, typename Right>
inline Left difference(const typename Left::const_iterator& begin,
    const typename Left::const_iterator& end, const Right& right) noexcept;

/// Determing if the sets have an intersection.
template <typename Left, typename Right>
constexpr bool is_intersecting(const Left& left, const Right& right) noexcept;
template <typename Left, typename Right>
constexpr bool is_intersecting(const typename Left::const_iterator& begin,
    const typename Left::const_iterator& end, const Right& right) noexcept;

/// Reverse the order of collection elements.
/// Use std::views::reverse for reverse iteration.
template <typename Collection>
inline Collection reverse(Collection&& list) noexcept;
template <typename Collection>
inline Collection reverse_copy(const Collection& list) noexcept;

/// Sort collection elements.
template <typename Collection>
constexpr void sort(Collection& list) noexcept;
template <typename Collection>
inline Collection sort(Collection&& list) noexcept;
template <typename Collection>
inline Collection sort_copy(const Collection& list) noexcept;

/// Determine if a collection range starts with another collection.
template <typename Collection>
constexpr bool starts_with(const typename Collection::const_iterator& begin,
    const typename Collection::const_iterator& end,
    const Collection& value) noexcept;

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/data/collection.ipp>

#endif
