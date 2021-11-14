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
#include <bitcoin/system/error/error_condition.hpp>

#include <system_error>
#include <bitcoin/system/error/error.hpp>
#include <bitcoin/system/error/error_category.hpp>

namespace std {

std::error_condition make_error_condition(
    bc::system::error::error_t value) noexcept
{
    return std::error_condition(value,
        bc::system::error::error_category::singleton);
}

} // namespace std
