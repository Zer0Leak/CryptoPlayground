// SPDX-License-Identifier: LGPL-3.0-or-later
//
// File: brute_force.h
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
#include <functional>
#include <vector>

extern uint64_t calculateLcgSeedByBruteForce(const std::vector<uint64_t> &values,
                                             std::function<uint64_t(uint64_t)> transform);
