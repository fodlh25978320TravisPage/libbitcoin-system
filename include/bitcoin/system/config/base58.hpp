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
#ifndef LIBBITCOIN_SYSTEM_CONFIG_BASE58_HPP
#define LIBBITCOIN_SYSTEM_CONFIG_BASE58_HPP

#include <iostream>
#include <string>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {
namespace config {

/// Serialization helper for base58 encoded text.
class BC_API base58
{
public:
    /// Defaults.
    base58(base58&&) = default;
    base58(const base58&) = default;
    base58& operator=(base58&&) = default;
    base58& operator=(const base58&) = default;
    ~base58() = default;

    /// Constructors.
    base58() noexcept;
    base58(data_chunk&& value) noexcept;
    base58(const data_chunk& value) noexcept;
    base58(const std::string& base58) noexcept(false);

    /// Operators.

    operator const data_chunk&() const noexcept;

    friend std::istream& operator>>(std::istream& stream,
        base58& argument) noexcept(false);
    friend std::ostream& operator<<(std::ostream& stream,
        const base58& argument) noexcept;

private:
    data_chunk value_;
};

} // namespace config
} // namespace system
} // namespace libbitcoin

#endif
