// SPDX-License-Identifier: LGPL-3.0-or-later
//
// File: main.cpp
// Author: Edgard Lima (Zer0Leak)
// GitHub: https://github.com/Zer0Leak
//
// Copyright (c) 2025 Edgard Lima
//
// This software is licensed under the terms of the
// GNU Lesser General Public License version 3 or later.
// See: https://www.gnu.org/licenses/lgpl-3.0.html

#include <cstdint>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "EmulateJavaRandom.h"
#include "brute_force.h"

std::vector<uint64_t> readHexValues(const std::string &filename) {
    std::ifstream file(filename);
    std::vector<uint64_t> values;
    std::string line;

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    // First pass: collect all pairs
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string first, second, third;

        if (iss >> first >> second >> third) {
            values.push_back(std::stoull(first, nullptr, 16));
            values.push_back(std::stoull(second, nullptr, 16));
            values.push_back(std::stoull(third, nullptr, 16));
        }
    }

    return values;
}

void testOurLcg() {
    const std::string delimiter = "################################";
    const int N = 6;
    const bool OutputOnly32bits = true;
    const std::uint64_t shift = OutputOnly32bits ? 16ULL : 0ULL;

    EmulateJavaRandom rng(1);

    std::cout << "Generating " << N << " 48-bit values from EmulateJavaRandom:" << '\n';
    std::cout << delimiter << '\n';

    for (int i = 0; i < N; ++i) {
        std::uint32_t first = static_cast<std::uint32_t>(rng.next48() >> shift);
        std::uint32_t second = static_cast<std::uint32_t>(rng.next48() >> shift);
        std::uint32_t third = static_cast<std::uint32_t>(rng.next48() >> shift);

        if (i == N / 2) {
            std::cout << delimiter << '\n';
        }

        std::cout << std::format("{:08X} {:08X} {:08X}\n", first, second, third);
    }
}

// Given a recovered internal 48-bit LCG state (as returned by bruteForceSeed),
// seed EmulateJavaRandom equivalently and print 6 values (2 per line) like Lcg.java
void testWithRecoveredSeed(std::uint64_t internal48Seed) {
    const std::string delimiter = "################################";
    const int N = 6;
    const std::uint64_t shift = 16ULL; // top 32 bits from 48-bit state

    // Convert internal state S_internal to a public seed s such that
    // setSeed(s) produces S_internal:  S_internal = (s ^ MULTIPLIER) & MASK
    EmulateJavaRandom rng;
    rng.setRawSeed(internal48Seed);

    std::cout << "Pairs from recovered seed (EmulateJavaRandom):" << '\n';
    std::cout << delimiter << '\n';
    for (int i = 0; i < N / 2; ++i) {
        std::uint32_t first = static_cast<std::uint32_t>(rng.next48() >> shift);
        std::uint32_t second = static_cast<std::uint32_t>(rng.next48() >> shift);
        std::uint32_t third = static_cast<std::uint32_t>(rng.next48() >> shift);
        std::cout << std::format("{:08X} {:08X} {:08X}\n", first, second, third);
    }
}

void printInputValues(std::vector<uint64_t> &values) {
    const std::string delimiter = "################################";

    std::cout << "Input values:" << '\n';
    std::cout << delimiter << '\n';
    for (size_t i = 0; i < values.size(); i += 3) {
        std::cout << std::format("{:08X} {:08X} {:08X}\n", static_cast<uint32_t>(values[i]),
                                 static_cast<uint32_t>(values[i + 1]), static_cast<uint32_t>(values[i + 2]));
    }
}
int main(int argc, char *argv[]) {
    // Quick self-check of our Java-compatible LCG
    // testOurLcg();

    std::string file_path = "/mnt/ram/BreakingLCG/gen.txt";
    if (argc == 2) {
        file_path = argv[1];
    }

    auto values = readHexValues(file_path);

    // Print two values side-by-side (pair i from first half with i from second half)
    printInputValues(values);

    uint64_t seed = calculateLcgSeedByBruteForce(values);
    std::cout << std::format("Found internal 48-bit seed: {:012X}\n", seed);
    testWithRecoveredSeed(seed);

    return 0;
}
