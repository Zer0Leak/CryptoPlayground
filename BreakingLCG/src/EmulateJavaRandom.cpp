// SPDX-License-Identifier: LGPL-3.0-or-later
//
// File: EmulateJavaRandom.cpp
// Author: Edgard Lima (Zer0Leak)
// GitHub: https://github.com/Zer0Leak
//
// Copyright (c) 2025 Edgard Lima
//
// This software is licensed under the terms of the
// GNU Lesser General Public License version 3 or later.
// See: https://www.gnu.org/licenses/lgpl-3.0.html

#include "EmulateJavaRandom.h"
#include <cstdint>
#include <stdexcept>

EmulateJavaRandom::EmulateJavaRandom(std::int64_t seed) { setSeed(seed); }

void EmulateJavaRandom::setRawSeed(std::int64_t s) { eng_.seed(s); }

void EmulateJavaRandom::setSeed(std::int64_t s) {
    // Internal 48-bit state is (seed ^ MULTIPLIER) & MASK
    eng_.seed((static_cast<std::uint64_t>(s) ^ MULTIPLIER) & MASK);
}

std::int32_t EmulateJavaRandom::next(int bits) {
    if (bits <= 0 || bits > 32) {
        throw std::invalid_argument("bits must be in 1..32");
    }
    const std::uint64_t state = eng_(); // advance once
    return static_cast<std::int32_t>(state >> (48 - bits));
}

std::int32_t EmulateJavaRandom::nextInt() { return next(32); }

std::int64_t EmulateJavaRandom::nextLong() {
    const std::uint64_t hi = static_cast<std::uint32_t>(next(32));
    const std::uint64_t lo = static_cast<std::uint32_t>(next(32));
    return static_cast<std::int64_t>((hi << 32) | lo);
}

std::int32_t EmulateJavaRandom::nextInt(std::int32_t bound) {
    if (bound <= 0) {
        throw std::invalid_argument("bound must be positive");
    }
    // If bound is power of two, fast path
    if ((bound & -bound) == bound) {
        return static_cast<std::int32_t>((static_cast<std::int64_t>(bound) * next(31)) >> 31);
    }
    std::int32_t bits;
    std::int32_t val;
    do {
        bits = next(31);
        val = bits % bound;
    } while (bits - val + (bound - 1) < 0);
    return val;
}

std::uint64_t EmulateJavaRandom::next48() { return eng_(); }
