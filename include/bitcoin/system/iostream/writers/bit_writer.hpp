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

// Sponsored in part by Digital Contract Design, LLC

#ifndef LIBBITCOIN_SYSTEM_IOSTREAM_WRITERS_BIT_WRITER_HPP
#define LIBBITCOIN_SYSTEM_IOSTREAM_WRITERS_BIT_WRITER_HPP

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <ostream>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/iostream/writers/byte_writer.hpp>

namespace libbitcoin {
namespace system {
    
/// A bit reader that accepts an ostream.
class BC_API bit_writer
  : public byte_writer
{
public:
    /// Constructors.
    bit_writer(std::ostream& sink) noexcept;
    ~bit_writer() override;

    /// Write one bit (high to low).
    virtual void write_bit(bool value) noexcept;

    /// Write size bits from an integer (high to low).
    virtual void write_bits(uint64_t value, uint8_t bits) noexcept;

    /// Advance iterator by writing false.
    virtual void skip_bit(size_t bits=one) noexcept;

protected:
    void do_write(uint8_t byte) noexcept override;
    void do_write(const uint8_t* data, size_t size) noexcept override;
    void do_skip(size_t size) noexcept override;
    void do_flush() noexcept override;
    bool get_valid() const noexcept override;

private:
    bool is_aligned() const noexcept;
    uint8_t shift() const noexcept;
    void dump() noexcept;

    uint8_t buffer_;
    uint8_t offset_;
};

} // namespace system
} // namespace libbitcoin

#endif
