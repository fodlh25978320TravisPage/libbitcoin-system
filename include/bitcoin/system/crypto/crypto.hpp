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
#ifndef LIBBITCOIN_SYSTEM_CRYPTO_CRYPTO_HPP
#define LIBBITCOIN_SYSTEM_CRYPTO_CRYPTO_HPP

#include <bitcoin/system/crypto/accumulator.hpp>
#include <bitcoin/system/crypto/aes256.hpp>
#include <bitcoin/system/crypto/checksum.hpp>
#include <bitcoin/system/crypto/der_parser.hpp>
#include <bitcoin/system/crypto/elliptic_curve.hpp>
#include <bitcoin/system/crypto/encryption.hpp>
#include <bitcoin/system/crypto/golomb_coding.hpp>
#include <bitcoin/system/crypto/hash.hpp>
#include <bitcoin/system/crypto/hmac_sha256.hpp>
#include <bitcoin/system/crypto/hmac_sha512.hpp>
#include <bitcoin/system/crypto/pbkd_sha256.hpp>
#include <bitcoin/system/crypto/pbkd_sha512.hpp>
#include <bitcoin/system/crypto/pseudo_random.hpp>
#include <bitcoin/system/crypto/ring_signature.hpp>
#include <bitcoin/system/crypto/ripemd160.hpp>
#include <bitcoin/system/crypto/rmd_algorithm.hpp>
#include <bitcoin/system/crypto/scrypt.hpp>
#include <bitcoin/system/crypto/sha_algorithm.hpp>
#include <bitcoin/system/crypto/siphash.hpp>

// vectorization intrinsics into algorithms using math templates.
// sha-ni intrinsics into sha algorithm using shani math template.

// sha    (algorithm)
// ripemd (algorithm)
// accumulator<algorithm>
// hmac<accumulator<algorithm>>
// pbkd<hmac<accumulator<algorithm>>>

// scrypt          -> hash
// checksum        -> hash
// elliptic_curve  -> hash
// siphash         -> hash
// golomb_coding   -> siphash
// ring_signature  -> elliptic_curve, hash

#endif
