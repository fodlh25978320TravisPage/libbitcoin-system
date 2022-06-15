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
#ifndef LIBBITCOIN_SYSTEM_DATA_COLLECTION_IPP
#define LIBBITCOIN_SYSTEM_DATA_COLLECTION_IPP

#include <algorithm>
#include <array>
#include <cstddef>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/math/math.hpp>

namespace libbitcoin {
namespace system {

// Assert fails arbitrarily if 'list' contains pointer elements (e.g. to
// const char*) because the comparison is between pointers, not values.
// However the binary_search call remains valid in the case where the
// 'value' parameter implements sufficient binary comparison operator
// overloads (e.g. operator<(const char* left, const std::string& right).
// Therefore we avoid this assertion and rely entirely on caller sorting.
////BC_ASSERT(is_sorted(list));
// TODO: provide optional comparison function, std::sort accepts:
// en.cppreference.com/w/cpp/utility/functional/less

// TODO: constrain to iterable Collection and constrain Element to
// comparable against Collection::value_type.
// Element is not required to be the Collection::value_type. 
// Collection requires size and empty methods.
template <typename Collection, typename Element>
typename Collection::difference_type
binary_search(const Collection& list, const Element& element) noexcept
{
    // C++17: std::size.
    const auto size = list.size();

    if (list.empty())
        return negative_one;

    typename Collection::difference_type left = zero;
    typename Collection::difference_type right = sub1(size);

    while (left <= right)
    {
        auto middle = to_half(left + right);
        const auto& value = list[middle];

        if (value < element)
            left = add1(middle);
        else if (value > element)
            right = sub1(middle);
        else
            return middle;
    }

    return negative_one;
}

// C++17: Parallel policy for std::transform.
template <typename To, typename From>
std::vector<To> cast(const std::vector<From>& source) noexcept
{
    // TODO: no_fill_allocator.
    std::vector<To> out(source.size());

    std::transform(std::begin(source), std::end(source), std::begin(out),
        [](const From& element)
        {
            return possible_sign_cast<To>(element);
        });

    return out;
}

// C++17: Parallel policy for std::transform.
template <typename To, typename From, size_t Size>
std::array<To, Size> cast(const std::array<From, Size>& source) noexcept
{
    // TODO: no_fill_allocator.
    std::array<To, Size> out;

    std::transform(std::begin(source), std::end(source), std::begin(out),
        [](const From& element) noexcept
        {
            return possible_sign_cast<To>(element);
        });

    return out;
}

// C++17: Parallel policy for std::transform.
template <typename To, typename From>
std::vector<To> pointer_cast(const std::vector<From>& source) noexcept
{
    // TODO: no_fill_allocator.
    std::vector<To> out(source.size());

    std::transform(std::begin(source), std::end(source), std::begin(out),
        [](const From& smart_pointer)
        {
            return smart_pointer.get();
        });

    return out;
}

// C++17: Parallel policy for std::any_of.
template <typename Collection, typename Element>
bool contains(const Collection& list, const Element& element) noexcept
{
    return std::any_of(std::begin(list), std::end(list),
        [&element](const Element& value) noexcept
        {
            return value == element;
        });
}

template <typename Element>
bool equal_points(const std::vector<std::shared_ptr<const Element>>& left,
    const std::vector<std::shared_ptr<const Element>>& right) noexcept
{
    // C++14: a new std::equal overload can replace this guard.
    if (left.size() != right.size())
        return false;

    return std::equal(left.begin(), left.end(), right.begin(),
        [](const std::shared_ptr<const Element>& first,
           const std::shared_ptr<const Element>& second) noexcept
        {
            return (first && second && (*first == *second)) ||
                (!first && !second);
        });
}

template <typename Iterator, typename Value>
void filler(Iterator begin, const Iterator& end, const Value& value) noexcept
{
    // C++17: Parallel policy for std::for_each.
    // std::fill warns on msvc and cannot be suppressed.
    std::for_each(begin, end, [&](Value& element)
    {
        element = value;
    });
}

// TODO: constrain to std::pair elements.
// C++17: Parallel policy for std::find_if.
// Collection requires std::pair elements.
template <typename Collection>
typename Collection::difference_type
find_pair_position(const Collection& list,
    const typename Collection::value_type::first_type& key) noexcept
{
    const auto position = std::find_if(std::begin(list), std::end(list),
        [&key](const typename Collection::value_type& pair) noexcept
        {
            return pair.first == key;
        });

    return position == std::end(list) ? negative_one :
        std::distance(std::begin(list), position);
}

// C++17: Parallel policy for std::find.
template <typename Collection>
typename Collection::difference_type
find_position(const Collection& list,
    const typename Collection::value_type& element) noexcept
{
    const auto position = std::find(std::begin(list), std::end(list), element);
    return position == std::end(list) ? negative_one :
        std::distance(std::begin(list), position);
}

// TODO: specialize vector and generalize on element type.
// Collection requires insert method (vector).
template <typename Collection, typename Predicate>
typename Collection::iterator
insert_sorted(Collection& list, typename Collection::value_type& element,
    Predicate predicate) noexcept
{
    return list.insert(std::upper_bound(std::begin(list),
        std::end(list), element, predicate), element);
}

// TODO: specialize vector and generalize on element type.
// Collection requires reserve and size methods (vector).
template <typename Collection>
void move_append(Collection& target, Collection& source) noexcept
{
    // C++17: std::size.
    target.reserve(target.size() + source.size());
    std::move(std::begin(source), std::end(source), std::back_inserter(target));
}

// TODO: specialize vector and generalize on element type.
// Collection requires empty, back and pop_back methods (vector).
template <typename Collection>
typename Collection::value_type
pop(Collection& stack) noexcept
{
    if (stack.empty())
        return {};

    auto element = stack.back();
    stack.pop_back();
    return element;
}

// C++17: Parallel policy for std::sort, std::unique.
template <typename Collection>
bool is_distinct(Collection&& list) noexcept
{
    std::sort(std::begin(list), std::end(list));
    return std::unique(std::begin(list), std::end(list)) == std::end(list);
}

template <typename Collection>
bool is_distinct(const Collection& list) noexcept
{
    // A single move function would require a move and a copy (least ideal).
    // This is necessary because the argument will be const if not moved.
    // A single pass-by-value function requires an external copy (not ideal).
    // A single pass-by-reference requires an internal copy (not ideal).
    // In theory the compiler would elide unnecessary copies (not guaranteed).
    // Two overloads ensure 1 copy here or 1 move in the move overload (ideal).
    // By providing both overloads this is not only ensured but the benefit of
    // move is exposed to callers, as both options are explicitly available.
    // This is the approach implemented in the C++ std library.
    // Mutable parameter      : ref.
    // Integral parameter     : copy.
    // Non-mutable parameter  : const ref.
    // Non-mutable param copy : move with const ref overload that calls into move.
    auto copy = list;
    return is_distinct(copy);
}

// TODO: provide optional comparison function
// C++17: Parallel policy for std::is_sorted.
template <typename Collection>
bool is_sorted(const Collection& list) noexcept
{
    return std::is_sorted(std::begin(list), std::end(list));
}

// TODO: specialize vector and generalize on element type.
// C++17: Parallel policy for std::sort, std::erase.
// Collection requires erase and shrink_to_fit methods (vector).

template <typename Collection>
void distinct(Collection& list) noexcept
{
    std::sort(std::begin(list), std::end(list));
    list.erase(std::unique(std::begin(list), std::end(list)), std::end(list));
    list.shrink_to_fit();
}

template <typename Collection>
Collection distinct(Collection&& list) noexcept
{
    distinct(list);
    return std::forward<Collection>(list);
}

template <typename Collection>
Collection distinct_copy(const Collection& list) noexcept
{
    auto copy = list;
    distinct(copy);
    return copy;
}

// Collection requires size, reserve and shrink_to_fit methods (vector).
template <typename Left, typename Right>
Left difference(const Left& left, const Right& right) noexcept
{
    return difference<Left>(left.begin(), left.end(), right);
}

// Collection requires size, reserve and shrink_to_fit methods (vector).
template <typename Left, typename Right>
Left difference(const typename Left::const_iterator& begin,
    const typename Left::const_iterator& end, const Right& right) noexcept
{
    Left copy{};
    copy.reserve(std::distance(begin, end));
    
    // Linear copy since creating a copy, more efficient than multiple erases.
    for (auto min = begin; min != end; ++min)
        if (!contains(right, *min))
            copy.push_back(*min);
    
    copy.shrink_to_fit();
    return copy;
    return {};
}

template <typename Left, typename Right>
bool intersecting(const Left& left, const Right& right) noexcept
{
    return intersecting<Left>(left.begin(), left.end(), right);
}

// C++17: Parallel policy for std::find_first_of.
template <typename Left, typename Right>
bool intersecting(const typename Left::const_iterator& begin,
    const typename Left::const_iterator& end, const Right& right) noexcept
{
    return std::find_first_of(begin, end, std::begin(right),
        std::end(right)) != end;
}

// C++17: Parallel policy for std::reverse.
template <typename Collection>
void reverse(Collection& list) noexcept
{
    std::reverse(std::begin(list), std::end(list));
}

template <typename Collection>
Collection reverse(Collection&& list) noexcept
{
    reverse(list);
    return std::forward<Collection>(list);
}

template <typename Collection>
Collection reverse_copy(const Collection& list) noexcept
{
    auto copy = list;
    reverse(copy);
    return copy;
}

// C++17: Parallel policy for std::sort.
template <typename Collection>
void sort(Collection& list) noexcept
{
    std::sort(std::begin(list), std::end(list));
}

template <typename Collection>
Collection sort(Collection&& list) noexcept
{
    sort(list);
    return std::forward<Collection>(list);
}

template <typename Collection>
Collection sort_copy(const Collection& list) noexcept
{
    auto copy = list;
    sort(copy);
    return copy;
}

template <typename Collection>
bool starts_with(
    const typename Collection::const_iterator& begin,
    const typename Collection::const_iterator& end,
    const Collection& value) noexcept
{
    // C++17: std::size.
    return !is_lesser(std::distance(begin, end), value.size()) &&
        std::equal(value.begin(), value.end(), begin);
}

} // namespace system
} // namespace libbitcoin

#endif
