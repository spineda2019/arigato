// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Campaign.hpp - Representation of a long-running game-state
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "include/Campaign.hpp"

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <span>

namespace arigato::core {
std::size_t Campaign::GetDay() const noexcept { return day_; }
void Campaign::NextDay() noexcept { ++day_; }

void Campaign::AddCats(std::span<Cat::Id> cats_to_add) {
    auto deduped_cats_to_add =
        cats_to_add | std::views::filter([this](Cat::Id id) noexcept -> bool {
            // The 3rd argument is a projection.
            // It tells find() to compare 'id' against 'cat.id'
            return std::ranges::find(cats_, id, &Campaign::Cat::id) ==
                   cats_.end();
        });

    for (Cat::Id id : deduped_cats_to_add) {
        this->AddCat(id);
    }
}
void Campaign::AddDecor(std::span<Decorum::Id> decorum_to_add) {
    auto deduped_cats_to_add =
        decorum_to_add |
        std::views::filter([this](Decorum::Id id) noexcept -> bool {
            // The 3rd argument is a projection.
            // It tells find() to compare 'id' against 'cat.id'
            return std::ranges::find(decor_, id, &Campaign::Decorum::id) ==
                   decor_.end();
        });

    for (Decorum::Id id : deduped_cats_to_add) {
        this->AddDecorum(id);
    }
}

void Campaign::AddCat(Cat::Id id) { cats_.emplace_back(id); }
void Campaign::AddDecorum(Decorum::Id id) {
    decor_.emplace_back(Campaign::Decorum::Position{}, id);
}

std::span<const Campaign::Decorum> Campaign::GetDecor() const noexcept {
    return {decor_.cbegin(), decor_.size()};
}
std::span<const Campaign::Cat> Campaign::GetCats() const noexcept {
    return {cats_.cbegin(), cats_.size()};
}

}  // namespace arigato::core
