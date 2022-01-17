/**
 * Copyright (c) 2011-2021 libbitcoin developers (see AUTHORS)
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
#ifndef LIBBITCOIN_SYSTEM_STREAM_STREAMERS_SHA256_WRITER_HPP
#define LIBBITCOIN_SYSTEM_STREAM_STREAMERS_SHA256_WRITER_HPP

#include <cstdint>
#include <cstddef>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/stream/streamers/byte_reader.hpp>
#include <bitcoin/system/stream/streamers/byte_writer.hpp>

namespace libbitcoin {
namespace system {
    
/// A hash writer that accepts an ostream.
template <typename OStream = std::ostream>
class sha256_writer
  : public byte_writer<OStream>
{
public:
    /// Constructors.
    sha256_writer(OStream& sink) noexcept;
    ~sha256_writer() noexcept override;

protected:
    /// The maximum addressable stream position.
    static constexpr size_t maximum = hash_size;

    void do_write_bytes(const uint8_t* data, size_t size) noexcept override;
    void do_flush() noexcept override;

private:
    void flusher() noexcept;

    intrinsics::sha256_context context_;
};

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/stream/streamers/sha256_writer.ipp>

#endif
