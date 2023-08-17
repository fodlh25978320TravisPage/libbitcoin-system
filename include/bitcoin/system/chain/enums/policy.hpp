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
#ifndef LIBBITCOIN_SYSTEM_CHAIN_ENUMS_POLICIES_HPP
#define LIBBITCOIN_SYSTEM_CHAIN_ENUMS_POLICIES_HPP

#include <bitcoin/system/define.hpp>
#include <bitcoin/system/math/math.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

/// Analog to chain::forks but without impacting validation. These provide
/// additional validation context, giving a node the option to reject an
/// unconfirmed transaction.
enum policy : uint32_t
{
    no_policy = 0,

    /// Mask to set all rule bits.
    all_policy = bit_all<uint32_t>
};

} // namespace chain
} // namespace system
} // namespace libbitcoin

#endif
