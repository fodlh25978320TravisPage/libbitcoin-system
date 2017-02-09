/**
 * Copyright (c) 2011-2017 libbitcoin developers (see AUTHORS)
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
#include <bitcoin/bitcoin/wallet/select_outputs.hpp>

#include <algorithm>
#include <cstdint>
#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/bitcoin/utility/assert.hpp>

namespace libbitcoin {
namespace wallet {

using namespace bc::chain;

// unspent list is sorted in this method, so a copied parameter is used
static void greedy_select(points_info& out, output_info::list unspent,
    uint64_t minimum_value)
{
    const auto below_minimum = [minimum_value](const output_info& out_info)
    {
        return out_info.value < minimum_value;
    };

    const auto lesser = [](const output_info& left, const output_info& right)
    {
        return left.value < right.value;
    };

    const auto greater = [](const output_info& left, const output_info& right)
    {
        return left.value > right.value;
    };

    const auto lesser_begin = unspent.begin();
    const auto lesser_end = std::partition(unspent.begin(), unspent.end(),
        below_minimum);

    const auto greater_begin = lesser_end;
    const auto greater_end = unspent.end();
    const auto minimum_greater = std::min_element(greater_begin, greater_end,
        lesser);

    if (minimum_greater != greater_end)
    {
        BITCOIN_ASSERT(minimum_greater->value >= minimum_value);
        out.change = minimum_greater->value - minimum_value;
        out.points.push_back(minimum_greater->point);
        return;
    }

    // Not found in greaters so try several lessers instead.
    // Sort descending, to use the fewest inputs possible.
    std::sort(lesser_begin, lesser_end, greater);

    for (auto it = lesser_begin; it != lesser_end; ++it)
    {
        out.change = safe_add(out.change, it->value);
        out.points.push_back(it->point);

        if (out.change >= minimum_value)
        {
            out.change -= minimum_value;
            return;
        }
    }

    out.change = 0;
    out.points.clear();
}

static void individual_select(points_info& out,
    const output_info::list& unspent_list, uint64_t minimum_value)
{
    for (const auto& unspent: unspent_list)
        if (unspent.value >= minimum_value)
            out.points.push_back(unspent.point);
}

void select_outputs::select(points_info& out, const output_info::list& unspent,
    uint64_t minimum_value, algorithm option)
{
    out.change = 0;
    out.points.clear();

    if (unspent.empty())
        return;

    switch(option)
    {
        case algorithm::individual:
            individual_select(out, unspent, minimum_value);
            break;

        case algorithm::greedy:
        default:
            greedy_select(out, unspent, minimum_value);
            break;
    }
}

} // namspace wallet
} // namspace libbitcoin
