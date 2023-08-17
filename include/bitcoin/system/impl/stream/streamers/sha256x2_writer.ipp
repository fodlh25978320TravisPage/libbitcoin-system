/**
 * Copyright (c) 2011-2023 libbitcoin developers (see AUTHORS)
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
#ifndef LIBBITCOIN_SYSTEM_STREAM_STREAMERS_SHA256X2_WRITER_IPP
#define LIBBITCOIN_SYSTEM_STREAM_STREAMERS_SHA256X2_WRITER_IPP

#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/hash/hash.hpp>
#include <bitcoin/system/stream/streamers/byte_reader.hpp>
#include <bitcoin/system/stream/streamers/byte_writer.hpp>

namespace libbitcoin {
namespace system {
    
// All public methods must rely on protected for stream state except validity.

// constructors
// ----------------------------------------------------------------------------

template <typename OStream>
sha256x2_writer<OStream>::sha256x2_writer(OStream& sink) NOEXCEPT
  : byte_writer<OStream>(sink)
{
}

template <typename OStream>
sha256x2_writer<OStream>::~sha256x2_writer() NOEXCEPT
{
    // Derived virtual destructor called before base destructor.
    flusher();
}

// protected
// ----------------------------------------------------------------------------

template <typename OStream>
void sha256x2_writer<OStream>::do_write_bytes(const uint8_t* data,
    size_t size) NOEXCEPT
{
    // Hash overflow produces update false, which requires (2^64-8)/8 bytes.
    // The stream could be invalidated, but writers shouldn't have to check it.
    context_.write(size, data);
}

template <typename OStream>
void sha256x2_writer<OStream>::do_flush() NOEXCEPT
{
    flusher();
    byte_writer<OStream>::do_flush();
}

// private
// ----------------------------------------------------------------------------

// Only hash overflow returns update false, which requires (2^64-8)/8 bytes.
// The stream could invalidate, but writers shouldn't have to check this.
template <typename OStream>
void sha256x2_writer<OStream>::flusher() NOEXCEPT
{
    BC_PUSH_WARNING(LOCAL_VARIABLE_NOT_INITIALIZED)
    hash_digest hash;
    BC_POP_WARNING()

    // Finalize streaming hash.
    context_.flush(hash.data());
    context_.reset();

    // Hash the result of the streaming hash (update cannot overflow).
    context_.write(hash_size, hash.data());
    context_.flush(hash.data());
    context_.reset();

    byte_writer<OStream>::do_write_bytes(hash.data(), hash_size);
}

} // namespace system
} // namespace libbitcoin

#endif
