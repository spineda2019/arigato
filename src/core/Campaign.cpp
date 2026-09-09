// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Campaign.hpp - Representation of a long-running game-state
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "include/Campaign.hpp"

#include <cstddef>

namespace arigato::core {
std::size_t Campaign::GetDay() const noexcept { return day_; }
void Campaign::NextDay() noexcept { ++day_; }
}  // namespace arigato::core
