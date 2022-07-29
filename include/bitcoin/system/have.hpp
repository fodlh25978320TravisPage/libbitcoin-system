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
#ifndef LIBBITCOIN_SYSTEM_HAVE_HPP
#define LIBBITCOIN_SYSTEM_HAVE_HPP

#include <bitcoin/system/version.hpp>

///////////////////////////////////////////////////////////////////////////////
// Maintainers: update corresponding diagnostic HAVE emissions in define.cpp.
///////////////////////////////////////////////////////////////////////////////

/// Plaform: architecture, compiler, and standard libraries.
/// ---------------------------------------------------------------------------

#if defined(__linux__)
    #define HAVE_LINUX
#elif defined(__APPLE__)
    #define HAVE_APPLE
#elif defined(__FreeBSD__)
    #define HAVE_FREEBSD
#elif defined(__OpenBSD__)
    #define HAVE_OPENBSD
#elif defined(__NetBSD__)
    #define HAVE_NETBSD
#elif defined(__CYGWIN__)
    #define HAVE_CYGWIN
#endif

#if defined(__GNUC__)
    #define HAVE_GNUC
#endif
#if defined(_MSC_VER)
    #define HAVE_MSC
#endif

/// GNU/MSC defines for targeted CPU architecture.
/// HAVE_ITANIUM is defined but not used (no optimizations).
/// sourceforge.net/p/predef/wiki/Architectures
/// docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros
#if defined(__i386__) || defined(_M_IX86)
    #define HAVE_X32
#elif defined(__amd64__) || defined(_M_AMD64)
    #define HAVE_X64
#elif defined(__ia64__) || defined(_M_IA64)
    #define HAVE_ITANIUM
#elif defined(__arm__) || defined(_M_ARM)
    #define HAVE_ARM32
#elif defined(__aarch64__) || defined(_M_ARM64)
    #define HAVE_ARM64
#endif

/// Common CPU architecture (XCPU).
#if defined(HAVE_X32) || defined(HAVE_X64)
    #define HAVE_XCPU
#endif

/// XCPU architecture inline assembly.
#if defined(HAVE_XCPU) && !defined(HAVE_MSC)
    #define HAVE_XASSEMBLY
#endif

/// MSC, GCC11, Clang15(?)
/// XCPU architecture intrinsic _xgetbv and __cpuidex.
#if defined(HAVE_XCPU) && (defined(HAVE_MSC) || defined(HAVE_GNUC))
    #define HAVE_XCPUID
#endif

/// ARM Neon intrinsics.
#if defined(HAVE_ARM)
    #if defined(HAVE_GNUC) || defined(__ARM_NEON) || defined(HAVE_MSC)
        #define HAVE_NEON
    #endif
#endif

/// MSC predefined constant for Visual Studio version (exclusive).
#if defined(HAVE_MSC)
    #if   _MSC_VER >= 1930
        #define HAVE_VS2022
    #elif _MSC_VER >= 1920
        #define HAVE_VS2019
    #elif _MSC_VER >= 1910
        #define HAVE_VS2017
    #elif _MSC_VER >= 1900
        #define HAVE_VS2015
    #elif _MSC_VER >= 1800
        #define HAVE_VS2013
    #endif
#endif

/// C/C++ language and support by platform.
/// ---------------------------------------------------------------------------

/// ISO predefined constant for C++ version (inclusive).
#if __cplusplus >= 199711L
    #define HAVE_CPP03
#endif
#if __cplusplus >= 201103L
    #define HAVE_CPP11
#endif
#if __cplusplus >= 201402L
    #define HAVE_CPP14
#endif
#if __cplusplus >= 201703L
    #define HAVE_CPP17
#endif
#if __cplusplus >= 202002L
    #define HAVE_CPP20
#endif

/// Other platforms not as far along (C++20).
#if defined(HAVE_CPP20) && defined(HAVE_MSC)
    #define HAVE_RANGES
    #define HAVE_CONSTEVAL
    #define HAVE_STRING_CONSTEXPR
    #define HAVE_VECTOR_CONSTEXPR
#endif

/// No std::execution on macos clang (C++17).
#if defined(HAVE_CPP17) && !defined(HAVE_APPLE)
    #define HAVE_EXECUTION
#endif

/// TODO: define warning suppressions for other platforms.
#if defined(HAVE_MSC)
    #define HAVE_PRAGMA_WARNING
#endif

/// WITH_ indicates build symbol.
/// ---------------------------------------------------------------------------

/// Build configured (always available on msvc).
#if defined(HAVE_MSC) || defined(WITH_ICU)
    #define HAVE_ICU
#endif

/// These are manually configured here.
/// ---------------------------------------------------------------------------

/// Disable noexcept to capture stack trace.
#define HAVE_NOEXCEPT

// Deprecated is noisy, turn on to find dependencies.
////#define HAVE_DEPRECATED

/// Have slow test execution (scrypt is slow by design).
////#define HAVE_SLOW_TESTS

#endif
