// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Character.hpp - Independent representation of the game character
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_CHARACTER_HPP_
#define SRC_CORE_INCLUDE_CHARACTER_HPP_

#include <type_traits>
//
#include <arigato/physics.hpp>

namespace arigato::core {
class Character final {
 public:  // Types
    using WorldUnit = float;
    using Rectangle = types::Rectangle<WorldUnit>;
    using Vec = types::Vec2D<WorldUnit>;

 public:
    explicit Character(Rectangle) noexcept;

 private:  // passkeys
    class Game;
    struct Passkey final {
        friend Game;
    };

 public:
    void Apply(Vec, Passkey) noexcept;
    /// Returns the characters positions w.r.t game _logical units_, not pixels.
    Rectangle GetPosition() const noexcept;

 private:
    /// While The character uses same the type of `types::Vec2D` as the
    /// display module, internally this is understood by _logical_ units, not
    /// necessarily _pixels_.
    Rectangle bounds_;
};

static_assert(std::is_trivially_destructible_v<Character>);
}  // namespace arigato::core

#endif  // SRC_CORE_INCLUDE_CHARACTER_HPP_
