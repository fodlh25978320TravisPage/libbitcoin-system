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
#ifndef LIBBITCOIN_SYSTEM_ENDIAN_STREAM_IPP
#define LIBBITCOIN_SYSTEM_ENDIAN_STREAM_IPP

#include <iostream>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/math/math.hpp>

namespace libbitcoin {
namespace system {

// iostreams are not costexpr.
// if stream.gcount() != size there should be a stream error.

template <typename Integral, typename IStream,
    if_integral_integer<Integral>,
    if_base_of<std::istream, IStream>,
    if_one_byte<typename IStream::char_type>>
inline Integral from_big_endian(IStream& stream) NOEXCEPT
{
    Integral value{};
    using character = typename IStream::char_type;
    auto& cast = byte_cast<character>(value);
    stream.read(cast.data(), cast.size());
    return native_from_big_end(value);
}

template <typename Integral, typename IStream,
    if_integral_integer<Integral>,
    if_base_of<std::istream, IStream>,
    if_one_byte<typename IStream::char_type>>
inline Integral from_little_endian(IStream& stream) NOEXCEPT
{
    Integral value{};
    using character = typename IStream::char_type;
    auto& cast = byte_cast<character>(value);
    stream.read(cast.data(), cast.size());
    return native_from_little_end(value);
}

template <typename Integral, typename OStream,
    if_integral_integer<Integral>,
    if_base_of<std::ostream, OStream>,
    if_one_byte<typename OStream::char_type>>
inline void to_big_endian(OStream& stream, Integral value) NOEXCEPT
{
    using character = typename OStream::char_type;
    const auto& cast = byte_cast<character>(native_to_big_end(value));
    stream.write(cast.data(), cast.size());
}

template <typename Integral, typename OStream,
    if_integral_integer<Integral>,
    if_base_of<std::ostream, OStream>,
    if_one_byte<typename OStream::char_type>>
inline void to_little_endian(OStream& stream, Integral value) NOEXCEPT
{
    using character = typename OStream::char_type;
    const auto& cast = byte_cast<character>(native_to_little_end(value));
    stream.write(cast.data(), cast.size());
}

} // namespace system
} // namespace libbitcoin

#endif
