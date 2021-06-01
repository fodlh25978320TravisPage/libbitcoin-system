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
#ifndef LIBBITCOIN_SYSTEM_MATH_MATH_HPP
#define LIBBITCOIN_SYSTEM_MATH_MATH_HPP

#include <cstddef>
#include <bitcoin/system/type_constraints.hpp>

namespace libbitcoin {
namespace system {

/// All operations below support signed and unsigned parameters.

/// Obtain the absolute value of the given integer value.
template <typename Integer, IS_SIGNED_INTEGER(Integer)=true>
inline Integer absolute(Integer value);

/// Obtain the ceilinged (rounded up) integer base 2 logarithm of given value.
template <typename Integer, IS_INTEGER(Integer)=true>
inline Integer ceilinged_log2(Integer value);

/// Obtain the floored (rounded down) integer base 2 logarithm of given value.
template <typename Integer, IS_INTEGER(Integer)=true>
inline Integer floored_log2(Integer value);

/// Obtain the integer base 2 power for given exponent (can overflow).
template <typename Integer, IS_INTEGER(Integer)=true>
inline Integer power2(Integer exponent);

/// Obtain the integer power of given base for given exponent (can overflow).
template <typename Integer, IS_INTEGER(Integer)=true>
inline Integer power(Integer base, Integer exponent);

/// Obtain the ceilinged (rounded up) integer modulo quotient.
/// This is equivalent to the c++ % operator for negative quotients.
/// Python integer division is floored and so can create ceilinged integer
/// division by -(numerator//-denominator).
template <typename Dividend, typename Divisor, typename Quotient=Dividend,
    IS_INTEGERS(Dividend, Divisor)=true>
inline Quotient ceilinged_divide(Dividend dividend, Divisor divisor);

/// Obtain the ceilinged (rounded up) integer modulo quotient.
/// This is equivalent to the c++ % operator for negative quotients.
template <typename Dividend, typename Divisor, typename Remainder=Dividend,
    IS_INTEGERS(Dividend, Divisor)=true>
inline Remainder ceilinged_modulo(Dividend dividend, Divisor divisor);

/// Obtain the floored (rounded down) integer modulo quotient.
/// This is equivalent to the c++ % operator for positive quotients.
/// Python integer division is floored.
template <typename Dividend, typename Divisor, typename Quotient=Dividend,
    IS_INTEGERS(Dividend, Divisor)=true>
inline Quotient floored_divide(Dividend dividend, Divisor divisor);

/// Obtain the floorded (rounded down) integer modulo quotient.
/// This is equivalent to the c++ % operator for positive quotients.
/// Python integer modulo (%) is floored.
template <typename Dividend, typename Divisor, typename Remainder=Dividend,
    IS_INTEGERS(Dividend, Divisor)=true>
inline Remainder floored_modulo(Dividend dividend, Divisor divisor);

/// Obtain the truncated (rounded toward zero) integer quotient.
/// This is the equivalent to the c++ / operation.
template <typename Dividend, typename Divisor, typename Quotient=Dividend,
    IS_INTEGERS(Dividend, Divisor)=true>
inline Quotient truncated_divide(Dividend dividend, Divisor divisor);

/// Obtain the truncated (rounded toward zero) integer divide remainder.
/// This is the equivalent to the c++ % operation.
template <typename Dividend, typename Divisor, typename Remainder=Dividend,
    IS_INTEGERS(Dividend, Divisor)=true>
inline Remainder truncated_modulo(Dividend dividend, Divisor divisor);

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/math/math.ipp>

#endif

