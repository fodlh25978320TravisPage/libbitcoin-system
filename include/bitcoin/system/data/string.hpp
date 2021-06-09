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
#ifndef LIBBITCOIN_SYSTEM_DATA_STRING_HPP
#define LIBBITCOIN_SYSTEM_DATA_STRING_HPP

#include <string>
#include <bitcoin/system/data/data_slice.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/type_constraints.hpp>
#include <bitcoin/system/unicode/code_points.hpp>

namespace libbitcoin {
namespace system {
    
/// std::to_string is a formatter (and not thread safe).
/// Use system::serialize for string formatting.
/// Cast integer bytes directly into the string member.
template <typename Integer, if_integer<Integer> = true>
std::string to_string(Integer value, bool big_endian=true);

/// Use system::serialize for string formatting.
/// Casts bytes directly into the string member.
/// to_string(to_chunk(string)) == string.
/// to_string(to_array(string)) == string.
BC_API std::string to_string(const data_slice& bytes);

/// Join tokens by the specified delimiter.
BC_API std::string join(const string_list& tokens,
    const std::string& delimiter=ascii_space);

/// Split text into tokens by the specified delimiter(s).
/// Compression removes all but a last empty token (after trimming).
/// Compression removes all but a last empty token (after trimming).
BC_API string_list split(const std::string& text, bool compress=true);
BC_API string_list split(const std::string& text,
    const std::string& delimiter, bool trim=true, bool compress=true);
BC_API string_list split(const std::string& text,
    const string_list& delimiters, const string_list& trim_tokens,
    bool compress=true);

BC_API bool trim_left(std::string& text,
    const string_list& trim_tokens=ascii_whitespace);
BC_API bool trim_right(std::string& text,
    const string_list& trim_tokens=ascii_whitespace);
BC_API void trim(std::string& text,
    const string_list& trim_tokens=ascii_whitespace);

BC_API std::string trim_left_copy(const std::string& text,
    const string_list& trim_tokens=ascii_whitespace);
BC_API std::string trim_right_copy(const std::string& text,
    const string_list& trim_tokens=ascii_whitespace);
BC_API std::string trim_copy(const std::string& text,
    const string_list& trim_tokens=ascii_whitespace);

BC_API void reduce(string_list& tokens,
    const string_list& trim_tokens=ascii_whitespace, bool compress=true);
BC_API string_list reduce_copy(const string_list& tokens,
    const string_list& trim_tokens=ascii_whitespace, bool compress=true);

BC_API size_t replace(std::string& text,
    const std::string& from, const std::string& to);
BC_API std::string replace_copy(const std::string& text,
    const std::string& from, const std::string& to);

BC_API bool ends_with(const std::string& text, const std::string& suffix);
BC_API bool starts_with(const std::string& text, const std::string& prefix);

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/data/string.ipp>

#endif
