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
#include "../test.hpp"

BOOST_AUTO_TEST_SUITE(chain_state_tests)

struct test_chain_state
  : chain::chain_state
{
    using chain::chain_state::work_required;
};

chain::chain_state::data get_values(size_t retargeting_interval)
{
    chain::chain_state::data values;
    values.height = retargeting_interval;
    values.bits.ordered.push_back(0x1e0ffff0u);
    values.timestamp.ordered.push_back(1692625u);
    values.timestamp.retarget = 0;
    return values;
}

BOOST_AUTO_TEST_CASE(chain_state__work_required_retarget__overflow_patch_disabled__lower_value)
{
    settings settings(chain::selection::mainnet);
    settings.proof_of_work_limit = 0x1e0fffff;
    const auto values = get_values(settings.retargeting_interval());
    const auto forks = chain::forks::retarget;
    const auto work = test_chain_state::work_required(values, forks, settings);
    BOOST_REQUIRE_EQUAL(work, 0x1e0884d1u);
}

BOOST_AUTO_TEST_CASE(chain_state__work_required_retarget__overflow_patch_enabled__correct_value)
{
    settings settings(chain::selection::mainnet);
    settings.proof_of_work_limit = 0x1e0fffff;
    const auto values = get_values(settings.retargeting_interval());
    const auto forks = chain::forks::retarget | chain::forks::retarget_overflow_patch;
    const auto work = test_chain_state::work_required(values, forks, settings);
    BOOST_REQUIRE_EQUAL(work, settings.proof_of_work_limit);
}

BOOST_AUTO_TEST_SUITE_END()
