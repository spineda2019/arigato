// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Campaign.hpp - Representation of a long-running game-state
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <cstddef>
#include <span>
//
#include <arigato/id.hpp>
//
#include "include/Campaign.hpp"
namespace arigato::core {
std::size_t Campaign::GetDay() const noexcept { return day_; }
void Campaign::NextDay() noexcept { ++day_; }

std::span<const Campaign::Decorum> Campaign::GetDecor() const noexcept {
    return {decor_.cbegin(), decor_.size()};
}
std::span<const Campaign::Cat> Campaign::GetCats() const noexcept {
    return {cats_.cbegin(), cats_.size()};
}

}  // namespace arigato::core
