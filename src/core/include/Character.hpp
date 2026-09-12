// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Character.hpp - Independent representation of the game character
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_CHARACTER_HPP_
#define SRC_CORE_INCLUDE_CHARACTER_HPP_

#include <cstdint>
//
#include <arigato/physics.hpp>

namespace arigato::core {
class Character final {
 public:  // Types
    using Vec2D = types::Vec2D<float>;

    struct Action final {
        enum class Direction : std::int8_t {
            Positive = 1,
            Zero = 0,
            Negative = -1,
        };
        Direction move_x;
        Direction move_y;
        float dt;
    };
    static_assert(std::is_trivially_destructible_v<Action>);
    static_assert(std::is_nothrow_destructible_v<Action>);
    static_assert(std::is_trivially_constructible_v<Action, Action::Direction,
                                                    Action::Direction, float>);
    static_assert(std::is_nothrow_constructible_v<Action, Action::Direction,
                                                  Action::Direction, float>);
    static_assert(std::is_trivially_copy_constructible_v<Action>);
    static_assert(std::is_trivially_move_constructible_v<Action>);

 public:
    void Apply(Action) noexcept;
    /// Returns the characters positions w.r.t game _logical units_, not pixels.
    Vec2D GetPosition() const noexcept;

 private:
    /// While The character uses same the type of `types::Vec2D` as the
    /// display module, internally this is understood by _logical_ units, not
    /// necessarily _pixels_.
    Vec2D pos_;
};
}  // namespace arigato::core

#endif  // SRC_CORE_INCLUDE_CHARACTER_HPP_
