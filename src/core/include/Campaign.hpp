// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Campaign.hpp - Representation of a long-running game-state
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_CAMPAIGN_HPP_
#define SRC_CORE_INCLUDE_CAMPAIGN_HPP_

#include <cstddef>
#include <cstdint>
#include <vector>

namespace arigato::core {
/// Representation of the currently running game-state. Theoretically should be
/// easily (or at least tractably) serializable for saving
class Campaign final {
 public:  // APIs
    std::size_t GetDay() const noexcept;
    void NextDay() noexcept;

 private:  // Types
    struct Cat final {
        using Id = std::uint8_t;

        Id id{};
    };
    struct Decorum final {
        using Id = std::uint8_t;

        Id id{};
    };

 private:
    std::size_t day_{1};
    std::vector<Cat> cats_{};
    std::vector<Decorum> decor_;
};
}  // namespace arigato::core
#endif  // SRC_CORE_INCLUDE_CAMPAIGN_HPP_
