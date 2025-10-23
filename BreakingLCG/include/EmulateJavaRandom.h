// SPDX-License-Identifier: LGPL-3.0-or-later
//
// File: EmulateJavaRandom.h
// Author: Edgard Lima (Zer0Leak)
// GitHub: https://github.com/Zer0Leak
//
// Copyright (c) 2025 Edgard Lima
//
// This software is licensed under the terms of the
// GNU Lesser General Public License version 3 or later.
// See: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <random>

// A Java-compatible RNG implemented on top of std::linear_congruential_engine
// It reproduces java.util.Random's LCG (48-bit state), seeding rule, and outputs.
class EmulateJavaRandom {
  public:
    // Java's Random LCG parameters
    static constexpr std::uint64_t MULTIPLIER = 0x5DEECE66DULL;
    static constexpr std::uint64_t ADDEND = 0xBULL;
    static constexpr std::uint64_t MODULUS = (1ULL << 48);
    static constexpr std::uint64_t MASK = MODULUS - 1ULL;

    using engine_type = std::linear_congruential_engine<std::uint64_t, MULTIPLIER, ADDEND, MODULUS>;

    explicit EmulateJavaRandom(std::int64_t seed = 0);

    // Sets the internal state directly (not a Java API)
    void setRawSeed(std::int64_t s);

    // Matches java.util.Random#setSeed(long)
    void setSeed(std::int64_t s);

    // Matches java.util.Random#next(int bits)
    std::int32_t next(int bits);

    // Matches java.util.Random#nextInt()
    std::int32_t nextInt();

    // Matches java.util.Random#nextLong()
    std::int64_t nextLong();

    // Java-compatible bounded nextInt(bound)
    std::int32_t nextInt(std::int32_t bound);

    // Convenience: return raw 48-bit state after one step (not a Java API)
    std::uint64_t next48();

  private:
    engine_type eng_{}; // holds the 48-bit LCG state
};
