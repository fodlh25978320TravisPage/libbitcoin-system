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
#ifndef LIBBITCOIN_SYSTEM_UNICODE_NORMALIZATION_HPP
#define LIBBITCOIN_SYSTEM_UNICODE_NORMALIZATION_HPP

#include <string>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

#ifdef WITH_ICU

/// Change case of text, complies with section 3.13 of Unicode Standard.
BC_API std::string to_lower(const std::string& value);
BC_API std::string to_upper(const std::string& value);

/// Convert text to Unicode normal form (Unicode Standard).
/// Failure indicated by empty string return for non-empty value.
BC_API std::string to_canonical_composition(const std::string& value);
BC_API std::string to_canonical_decomposition(const std::string& value);
BC_API std::string to_compatibility_composition(const std::string& value);
BC_API std::string to_compatibility_demposition(const std::string& value);

#endif // WITH_ICU

/// Remove accent (diacritic) characters (Electrum).
BC_API std::string to_unaccented_form(const std::string& value);

/// Remove spaces between chinese, japanese, and korean characters (Electrum).
BC_API std::string to_compressed_cjk_form(const std::string& value);

} // namespace system
} // namespace libbitcoin

#endif
