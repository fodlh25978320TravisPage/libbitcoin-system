/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_THREAD_HPP
#define LIBBITCOIN_THREAD_HPP

#include <memory>
#include <boost/thread.hpp>
#include <bitcoin/bitcoin/define.hpp>

namespace libbitcoin {

// Adapted from: stackoverflow.com/a/18298965/1172329
#ifndef thread_local
    #if (__STDC_VERSION__ >= 201112) && (!defined __STDC_NO_THREADS__)
        #define thread_local _Thread_local
    #elif (defined _MSC_VER)
        #define thread_local __declspec(thread) 
    #elif (defined __GNUC__)
        #define thread_local __thread
    #else
        #error "Cannot define thread_local"
    #endif
#endif

enum class thread_priority
{
    high,
    normal,
    low,
    lowest
};

typedef boost::shared_mutex shared_mutex;
typedef boost::upgrade_mutex upgrade_mutex;

typedef boost::unique_lock<shared_mutex> unique_lock;
typedef boost::shared_lock<shared_mutex> shared_lock;

BC_API void set_thread_priority(thread_priority priority);

} // namespace libbitcoin

#endif
