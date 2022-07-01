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
#include <bitcoin/system/data/data_slice.hpp>

#include <array>
#include <initializer_list>
#include <iterator>
#include <string>
#include <vector>
// DELETEMENOW
#include <bitcoin/system/radix/radix.hpp>

namespace libbitcoin {
namespace system {

// constructors
// ----------------------------------------------------------------------------

data_slice::data_slice() NOEXCEPT
  : data_slice(nullptr, nullptr, zero)
{
}

data_slice::data_slice(const std::string& text) NOEXCEPT
  : data_slice(from_size(text.begin(), text.size()))
{
}

data_slice::data_slice(std::initializer_list<value_type> bytes) NOEXCEPT
  : data_slice(from_size(bytes.begin(), bytes.size()))
{
}

// private
data_slice::data_slice(pointer begin, pointer end, size_type size) NOEXCEPT
  : begin_(begin), end_(end), size_(size)
{
}

// methods
// ----------------------------------------------------------------------------

std::vector<data_slice::value_type> data_slice::to_chunk() const NOEXCEPT
{
    return { begin_, end_ };
}

std::string data_slice::to_string() const NOEXCEPT
{
    return { begin_, end_ };
}

// Cannot provide a "decode" factory since the data is not owned.
std::string data_slice::encoded() const NOEXCEPT
{
    return encode_base16(to_chunk());
}

bool data_slice::resize(size_t size) NOEXCEPT
{
    if (size >= size_)
        return false;

    end_ = std::next(begin_, size);
    size_ = size;
    return true;
}

// properties
// ----------------------------------------------------------------------------

// Undefined to dereference >= end.
data_slice::pointer data_slice::data() const NOEXCEPT
{
    return begin_;
}

// Undefined to dereference >= end.
data_slice::pointer data_slice::begin() const NOEXCEPT
{
    return begin_;
}

// Undefined to dereference >= end.
data_slice::pointer data_slice::end() const NOEXCEPT
{
    return end_;
}

data_slice::value_type data_slice::front() const NOEXCEPT
{
    // Guard against end overrun (return zero).
    return empty() ? 0x00 : *begin_;
}

data_slice::value_type data_slice::back() const NOEXCEPT
{
    // Guard against begin underrun (return zero).
    return empty() ? 0x00 : *std::prev(end_);
}

data_slice::size_type data_slice::size() const NOEXCEPT
{
    return size_;
}

bool data_slice::empty() const NOEXCEPT
{
    return is_zero(size_);
}

// operators
// ----------------------------------------------------------------------------

data_slice::operator std::vector<data_slice::value_type>() const NOEXCEPT
{
    return data_slice::to_chunk();
}

data_slice::value_type data_slice::operator[](size_type index) const NOEXCEPT
{
    // Guard against end overrun (return zero).
    return index < size_ ? *std::next(begin_, index) : 0x00;
}

bool operator==(const data_slice& left, const data_slice& right) NOEXCEPT
{
    if (left.size() != right.size())
        return false;

    // std::vector performs value comparison, so this does too.
    for (data_slice::size_type index = 0; index < left.size(); ++index)
        if (left[index] != right[index])
            return false;

    return true;
}

bool operator!=(const data_slice& left, const data_slice& right) NOEXCEPT
{
    return !(left == right);
}

////data_slice& data_slice::operator=(const data_slice& other)
////{
////    begin_ = other.begin_;
////    end_ = other.end_;
////    size_ = other.size_;
////    return *this;
////}

} // namespace system
} // namespace libbitcoin
