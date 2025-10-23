
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

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
        std::string first, second;

        if (iss >> first >> second) {
            values.push_back(std::stoull(first, nullptr, 16));
            values.push_back(std::stoull(second, nullptr, 16));
        }
    }

    return values;
}

int main(int argc, char *argv[]) {
    std::string file_path = "/mnt/ram/BreakingLCG/lcg.txt";
    if (argc == 2) {
        file_path = argv[1];
    }

    auto values = readHexValues(file_path);

    // Print two values side-by-side (pair i from first half with i from second half)
    for (size_t i = 0; i < values.size(); i += 2) {
        std::cout << std::format("{:08X} {:08X}\n", static_cast<uint32_t>(values[i]),
                                 static_cast<uint32_t>(values[i + 1]));
    }
    
    uint64_t seed = bruteForceSeed(values);

    std::cout << std::format("Found seed: {:012X}\n", seed);

    return 0;
}
