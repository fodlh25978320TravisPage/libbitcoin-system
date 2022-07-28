// Based on:
/* libsodium: hash_sha256_.c, v0.4.5 2014/04/16 */
/**
 * Copyright 2005,2007,2009 Colin Percival. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include <bitcoin/system/crypto/sha256.hpp>

#include <array>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/endian/endian.hpp>
#include <bitcoin/system/math/math.hpp>

namespace libbitcoin {
namespace system {
namespace sha256 {

#ifndef VISUAL
// movable-type.co.uk/scripts/sha256.html

// TODO: use std::rotr (intrinsic/inline).
#define rotr(x, n)        ((x >> n) | (x << (32 - n)))

// TODO: eliminate macros and (constexpr).
#define shr(x, n)         (x >> n)
#define choice(x, y, z)   ((x & (y ^ z)) ^ z)
#define majority(x, y, z) ((x & (y | z)) | (y & z))

// TODO: eliminate macros (inline).
#define SIGMA0(x)    (rotr(x,  2) ^ rotr(x, 13) ^ rotr(x, 22))
#define SIGMA1(x)    (rotr(x,  6) ^ rotr(x, 11) ^ rotr(x, 25))
#define sigma0(x)    (rotr(x,  7) ^ rotr(x, 18) ^  shr(x,  3))
#define sigma1(x)    (rotr(x, 17) ^ rotr(x, 19) ^  shr(x, 10))

// TODO: eliminate macros (inline). d&, h&
#define round(a, b, c, d, e, f, g, h, k) \
    t0 = h + SIGMA1(e) + choice(e, f, g) + k;  \
    t1 =     SIGMA0(a) + majority(a, b, c); \
    d += t0; \
    h = t0 + t1;
    
// TODO: eliminate macros and normalize case (inline). S&, <i>
#define RNDr(S, W, i, k) \
    round( \
        S[(64 - i) % 8], \
        S[(65 - i) % 8], \
        S[(66 - i) % 8], \
        S[(67 - i) % 8], \
        S[(68 - i) % 8], \
        S[(69 - i) % 8], \
        S[(70 - i) % 8], \
        S[(71 - i) % 8], \
        W[i] + k)

// TODO: eliminate macros and normalize case (inline). W&, <i>
#define Wi(W, i) \
    W[i] = sigma1(W[i - 2]) + W[i - 7] + sigma0(W[i - 15]) + W[i - 16]
#endif
    
////void single_hash(state& state, const blocks& blocks) NOEXCEPT;
void single_hash(state& state, const block& block) NOEXCEPT
{
    BC_PUSH_WARNING(NO_ARRAY_INDEXING)
    BC_PUSH_WARNING(NO_UNGUARDED_POINTERS)
    BC_PUSH_WARNING(NO_POINTER_ARITHMETIC)
    BC_PUSH_WARNING(NO_DYNAMIC_ARRAY_INDEXING)

    constexpr auto integers = block_size / sizeof(uint32_t);

    uint32_t t0, t1;
    sha256::state S{ state };
    std::array<uint32_t, block_size> W;

    // Populates W[0..15] with block integers (16 X 4 = 64).
    to_big_endian_set(
        narrowing_array_cast<uint32_t, integers>(W),
        array_cast<uint32_t>(block));

#ifndef VISUAL
    // Computes W[16..63].
    Wi(W, 16);
    Wi(W, 17);
    Wi(W, 18);
    Wi(W, 19);
    Wi(W, 20);
    Wi(W, 21);
    Wi(W, 22);
    Wi(W, 23);
    Wi(W, 24);
    Wi(W, 25);
    Wi(W, 26);
    Wi(W, 27);
    Wi(W, 28);
    Wi(W, 29);
    Wi(W, 30);
    Wi(W, 31);
    Wi(W, 32);
    Wi(W, 33);
    Wi(W, 34);
    Wi(W, 35);
    Wi(W, 36);
    Wi(W, 37);
    Wi(W, 38);
    Wi(W, 39);
    Wi(W, 40);
    Wi(W, 41);
    Wi(W, 42);
    Wi(W, 43);
    Wi(W, 44);
    Wi(W, 45);
    Wi(W, 46);
    Wi(W, 47);
    Wi(W, 48);
    Wi(W, 49);
    Wi(W, 50);
    Wi(W, 51);
    Wi(W, 52);
    Wi(W, 53);
    Wi(W, 54);
    Wi(W, 55);
    Wi(W, 56);
    Wi(W, 57);
    Wi(W, 58);
    Wi(W, 59);
    Wi(W, 60);
    Wi(W, 61);
    Wi(W, 62);
    Wi(W, 63);

    // 64 rounds.
    RNDr(S, W, 0, 0x428a2f98);
    RNDr(S, W, 1, 0x71374491);
    RNDr(S, W, 2, 0xb5c0fbcf);
    RNDr(S, W, 3, 0xe9b5dba5);
    RNDr(S, W, 4, 0x3956c25b);
    RNDr(S, W, 5, 0x59f111f1);
    RNDr(S, W, 6, 0x923f82a4);
    RNDr(S, W, 7, 0xab1c5ed5);
    RNDr(S, W, 8, 0xd807aa98);
    RNDr(S, W, 9, 0x12835b01);
    RNDr(S, W, 10, 0x243185be);
    RNDr(S, W, 11, 0x550c7dc3);
    RNDr(S, W, 12, 0x72be5d74);
    RNDr(S, W, 13, 0x80deb1fe);
    RNDr(S, W, 14, 0x9bdc06a7);
    RNDr(S, W, 15, 0xc19bf174);
    RNDr(S, W, 16, 0xe49b69c1);
    RNDr(S, W, 17, 0xefbe4786);
    RNDr(S, W, 18, 0x0fc19dc6);
    RNDr(S, W, 19, 0x240ca1cc);
    RNDr(S, W, 20, 0x2de92c6f);
    RNDr(S, W, 21, 0x4a7484aa);
    RNDr(S, W, 22, 0x5cb0a9dc);
    RNDr(S, W, 23, 0x76f988da);
    RNDr(S, W, 24, 0x983e5152);
    RNDr(S, W, 25, 0xa831c66d);
    RNDr(S, W, 26, 0xb00327c8);
    RNDr(S, W, 27, 0xbf597fc7);
    RNDr(S, W, 28, 0xc6e00bf3);
    RNDr(S, W, 29, 0xd5a79147);
    RNDr(S, W, 30, 0x06ca6351);
    RNDr(S, W, 31, 0x14292967);
    RNDr(S, W, 32, 0x27b70a85);
    RNDr(S, W, 33, 0x2e1b2138);
    RNDr(S, W, 34, 0x4d2c6dfc);
    RNDr(S, W, 35, 0x53380d13);
    RNDr(S, W, 36, 0x650a7354);
    RNDr(S, W, 37, 0x766a0abb);
    RNDr(S, W, 38, 0x81c2c92e);
    RNDr(S, W, 39, 0x92722c85);
    RNDr(S, W, 40, 0xa2bfe8a1);
    RNDr(S, W, 41, 0xa81a664b);
    RNDr(S, W, 42, 0xc24b8b70);
    RNDr(S, W, 43, 0xc76c51a3);
    RNDr(S, W, 44, 0xd192e819);
    RNDr(S, W, 45, 0xd6990624);
    RNDr(S, W, 46, 0xf40e3585);
    RNDr(S, W, 47, 0x106aa070);
    RNDr(S, W, 48, 0x19a4c116);
    RNDr(S, W, 49, 0x1e376c08);
    RNDr(S, W, 50, 0x2748774c);
    RNDr(S, W, 51, 0x34b0bcb5);
    RNDr(S, W, 52, 0x391c0cb3);
    RNDr(S, W, 53, 0x4ed8aa4a);
    RNDr(S, W, 54, 0x5b9cca4f);
    RNDr(S, W, 55, 0x682e6ff3);
    RNDr(S, W, 56, 0x748f82ee);
    RNDr(S, W, 57, 0x78a5636f);
    RNDr(S, W, 58, 0x84c87814);
    RNDr(S, W, 59, 0x8cc70208);
    RNDr(S, W, 60, 0x90befffa);
    RNDr(S, W, 61, 0xa4506ceb);
    RNDr(S, W, 62, 0xbef9a3f7);
    RNDr(S, W, 63, 0xc67178f2);
#endif

    state[0] += S[0];
    state[1] += S[1];
    state[2] += S[2];
    state[3] += S[3];
    state[4] += S[4];
    state[5] += S[5];
    state[6] += S[6];
    state[7] += S[7];

    BC_POP_WARNING()
    BC_POP_WARNING()
    BC_POP_WARNING()
    BC_POP_WARNING()
}

// ----------------------------------------------------------------------------

void single_hash(state& state, const block1& blocks) NOEXCEPT
{
    single_hash(state, blocks.front());
}

void double_hash(hash1& out, const block1& blocks) NOEXCEPT
{
    auto state = sha256::initial;
    single_hash(state, blocks);
    single_hash(state, sha256::pad_64);
    auto buffer = sha256::padded_32;
    to_big_endian_set(narrowing_array_cast<uint32_t, state_size>(buffer), state);
    state = sha256::initial;
    single_hash(state, buffer);
    to_big_endian_set(array_cast<uint32_t>(out.front()), state);
}

} // namespace sha256
} // namespace system
} // namespace libbitcoin
