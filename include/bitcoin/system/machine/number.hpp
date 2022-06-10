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
#ifndef LIBBITCOIN_SYSTEM_MACHINE_NUMBER_HPP
#define LIBBITCOIN_SYSTEM_MACHINE_NUMBER_HPP

#include <cstddef>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/math/math.hpp>

namespace libbitcoin {
namespace system {
namespace machine {

/**
 * Numeric opcodes (OP_1ADD, etc) are restricted to operating on
 * 4-byte integers. The semantics are subtle though: operands must be
 * in the range [-2^31 +1...2^31 -1], but results may overflow. This overflow
 * is valid as long as the result not used in a subsequent numeric operation in
 * a domain exceeded by the overflow.
 *
 * number enforces those semantics by storing results as an int64 and allowing
 * out-of-range values to be returned as a vector of bytes but throwing an
 * exception if arithmetic is done or the result is interpreted as an integer.
 */
class BC_API number
{
public:
    static const uint8_t positive_sign_byte = 0x00;
    static const uint8_t negative_sign_byte = bit_left<uint8_t>(0);

    /// Construct with zero value.
    number() noexcept;

    /// Construct with specified value.
    explicit number(int64_t value) noexcept;

    /// Replace the value derived from a little-endian byte vector.
    bool set_data(const data_chunk& data, size_t max_size) noexcept;

    // Properties
    // ------------------------------------------------------------------------

    /// Return the value as a byte vector with LSB first ordering.
    data_chunk data() const noexcept;

    /// Return the value bounded by the limits of int32.
    int32_t int32() const noexcept;

    /// Return the unbounded value.
    int64_t int64() const noexcept;

    // Stack Helpers
    // ------------------------------------------------------------------------

    /// Return value as stack boolean (nonzero is true).
    bool is_true() const noexcept;

    /// Return value as stack boolean (zero is false).
    bool is_false() const noexcept;

    /// Return true if the value is negative.
    bool is_negative() const noexcept;

    // Operators
    // ------------------------------------------------------------------------

    //*************************************************************************
    // CONSENSUS: number implements consensus critical overflow behavior for
    // all operators, specifically [-, +, +=, -=].
    //*************************************************************************

    bool operator>(int64_t value) const noexcept;
    bool operator<(int64_t value) const noexcept;
    bool operator>=(int64_t value) const noexcept;
    bool operator<=(int64_t value) const noexcept;
    bool operator==(int64_t value) const noexcept;
    bool operator!=(int64_t value) const noexcept;

    bool operator>(const number& other) const noexcept;
    bool operator<(const number& other) const noexcept;
    bool operator>=(const number& other) const noexcept;
    bool operator<=(const number& other) const noexcept;
    bool operator==(const number& other) const noexcept;
    bool operator!=(const number& other) const noexcept;

    number operator+() const noexcept;
    number operator-() const noexcept;
    number operator+(int64_t value) const noexcept;
    number operator-(int64_t value) const noexcept;
    number operator+(const number& other) const noexcept;
    number operator-(const number& other) const noexcept;

    number& operator+=(int64_t value) noexcept;
    number& operator-=(int64_t value) noexcept;
    number& operator+=(const number& other) noexcept;
    number& operator-=(const number& other) noexcept;

private:
    int64_t value_;
};

} // namespace machine
} // namespace system
} // namespace libbitcoin

#endif
