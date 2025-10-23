// SPDX-License-Identifier: LGPL-3.0-or-later
//
// File: brute_force.cpp
// Author: Edgard Lima (Zer0Leak)
// GitHub: https://github.com/Zer0Leak
//
// Copyright (c) 2025 Edgard Lima
//
// This software is licensed under the terms of the
// GNU Lesser General Public License version 3 or later.
// See: https://www.gnu.org/licenses/lgpl-3.0.html

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <sys/types.h>
#include <vector>

#include "brute_force.h"

// LCG(s) := ( floor(s/w), (a * s + b) mod q  )
// If w and q are power 2 ->  LCG(s) := ( s >> t, (a * s + b) & q_mask  )

static constexpr uint64_t a = 0x5DEECE66DULL;
static constexpr uint64_t b = 0xBULL;
static constexpr uint64_t q = 1ULL << 48;
static constexpr uint64_t q_mask = q - 1;
static constexpr uint32_t t = 16;
static constexpr uint32_t w = (1 << t); // 2^16 = 65536

uint64_t calculateLcgSeedByBruteForce(const std::vector<uint64_t> &values,
                                      std::function<uint64_t(uint64_t)> transform) {
    uint64_t seed = 0;

    if (values.size() < 2) {
        throw std::invalid_argument("Need at least one pair of values to brute force the seed.");
    }

    uint64_t s0 = values[0];
    for (uint32_t lower = 0; lower < w; ++lower) {
        uint64_t s0_full = s0 << t;
        s0_full |= lower;
        uint64_t s1_full = (a * s0_full + b) & q_mask;
        uint64_t s1 = transform(s1_full >> t);

        if (s1 == values[1]) {
            uint64_t si_full = s1_full;
            uint64_t sj_full = 0;
            size_t i = 1;
            for (; i < values.size() - 1; ++i) {
                sj_full = (a * si_full + b) & q_mask;
                uint64_t sj = transform(sj_full >> t);

                if (sj != values[i + 1]) {
                    break;
                } else {
                    si_full = sj_full;
                }
            }

            if (i == values.size() - 1) {
                seed = sj_full;
                goto Zer0Leak;
            }
        }
    }

    throw std::runtime_error("Seed not found!");

Zer0Leak:
    return seed;
}