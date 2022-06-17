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
/// It is the responsibility of the caller to ensure that parameters implement
/// sufficient comparison operator overloads (LT and GT). Either the 'list' 
/// elements must implement (at least) member comparison operator overloads or
/// the 'value' parameter must implement binary comparison operator overloads.
/// Be aware that C-style arrays/strings are compared by pointers, not values.
/// A 'list' of C-style arrays of char may be seached with std::string 'value'.
/// std::string comparisons are not locale aware.
/// Returns the position or negative if not found or list size > max_int32.
template <typename Collection, typename Element>
typename Collection::difference_type
constexpr binary_search(const Collection& list,
    const Element& element) noexcept;

/// Copy/move collection of From members to a new collection of To members.
template <typename To, typename From>
constexpr std::vector<To> cast(const std::vector<From>& source) noexcept;
template <typename To, typename From, size_t Size>
constexpr std::array<To, Size> cast(
    const std::array<From, Size>& source) noexcept;

/// Copy collection of smart pointer values to collection of pointers. 
template <typename To, typename From>
constexpr std::vector<To> pointer_cast(
    const std::vector<From>& source) noexcept;

/// Determine if a collection contains the specified element.
template <typename Collection, typename Element>
constexpr bool contains(const Collection& list,
    const Element& element) noexcept;

/// Determine if a vector of shared pointers to elements have equal elements.
template <typename Element>
constexpr bool equal_points(
    const std::vector<std::shared_ptr<const Element>>& left,
    const std::vector<std::shared_ptr<const Element>>& right) noexcept;

/// Fill a buffer with values.
template <typename Iterator, typename Value>
constexpr void filler(Iterator begin, const Iterator& end,
    const Value& value) noexcept;

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

/// Move members of a source list to end of a target list. Source members are 
/// undefined upon return.
template <typename Collection>
constexpr void move_append(Collection& target, Collection& source) noexcept;

/// Pop an element from the stack and return its value.
template <typename Collection>
typename Collection::value_type
constexpr pop(Collection& stack) noexcept;

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
constexpr Collection distinct(Collection&& list) noexcept;
template <typename Collection>
constexpr Collection distinct_copy(const Collection& list) noexcept;

/// Obtain the set difference of left less right.
template <typename Left, typename Right>
constexpr Left difference(const Left& left, const Right& right) noexcept;

template <typename Left, typename Right>
constexpr Left difference(const typename Left::const_iterator& begin,
    const typename Left::const_iterator& end, const Right& right) noexcept;

/// Determing if the sets have an intersection.
template <typename Left, typename Right>
constexpr bool intersecting(const Left& left, const Right& right) noexcept;

template <typename Left, typename Right>
constexpr bool intersecting(const typename Left::const_iterator& begin,
    const typename Left::const_iterator& end, const Right& right) noexcept;

/// Reverse the order of collection elements.
/// Use boost::adaptors::reverse for reverse iteration.
template <typename Collection>
constexpr void reverse(Collection& list) noexcept;
template <typename Collection>
constexpr Collection reverse(Collection&& list) noexcept;
template <typename Collection>
constexpr Collection reverse_copy(const Collection& list) noexcept;

/// Sort collection elements.
template <typename Collection>
constexpr void sort(Collection& list) noexcept;
template <typename Collection>
constexpr Collection sort(Collection&& list) noexcept;
template <typename Collection>
constexpr Collection sort_copy(const Collection& list) noexcept;

/// Determine if a collection range starts with another collection.
template <typename Collection>
constexpr bool starts_with(const typename Collection::const_iterator& begin,
    const typename Collection::const_iterator& end,
    const Collection& value) noexcept;

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/data/collection.ipp>

#endif
