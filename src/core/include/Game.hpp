// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Game.hpp - Representation of the game, its owned entities, and rules
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_GAME_HPP_
#define SRC_CORE_INCLUDE_GAME_HPP_

#include <cstddef>
#include <cstdint>
#include <vector>
//
#include <optional>
#include <span>
#include <type_traits>
//
#include <arigato/input.hpp>
#include <arigato/physics.hpp>
//
#include "Campaign.hpp"
#include "ECS.hpp"
#include "Level.hpp"

namespace arigato::core {
class Game final {
 public:  // types
    enum class State : std::uint8_t {
        Title,
        Playing,
        BetweenLevels,
    };

    struct Entities final {
        /// Represents all objects that can't move
        struct Static final {
            std::span<Level::PlacedCat> cats;
            std::span<Level::PlacedDecorum> decor;
        };

        static_assert(std::is_trivially_destructible_v<Static>);
        static_assert(std::is_trivially_constructible_v<
                      Static, std::span<Level::PlacedCat>,
                      std::span<Level::PlacedDecorum>>);
        static_assert(std::is_trivially_copy_constructible_v<Static>);
        static_assert(std::is_trivially_copy_assignable_v<Static>);

        /// Represents all objects that can move (whether by the player or game
        /// AI)
        struct Dynamic final {
            types::Rectangle<float> character;
            // TODO(SEP): hold span of customers
        };

        static_assert(std::is_trivially_destructible_v<Dynamic>);
        static_assert(std::is_trivially_constructible_v<Dynamic>);
        static_assert(std::is_trivially_copy_constructible_v<Dynamic>);
        static_assert(std::is_trivially_copy_assignable_v<Dynamic>);

        Static statics;
        Dynamic dynamics;
    };

    static_assert(std::is_trivially_destructible_v<Entities>);
    static_assert(std::is_trivially_constructible_v<Entities, Entities::Static,
                                                    Entities::Dynamic>);
    static_assert(std::is_trivially_copy_constructible_v<Entities>);
    static_assert(std::is_trivially_copy_assignable_v<Entities>);

 public:
    using Bounds = types::Bounds<int>;
    explicit Game(Bounds) noexcept;

    /// Returns the player's position in the world in _game units_, not pixels.
    Entities GetPositions() noexcept;
    Entities::Static GetStatics() noexcept;
    Entities::Dynamic GetDynamics() noexcept;
    std::size_t GetCurrentDay() const noexcept;
    std::uint8_t GetCustomersLeft() const noexcept;

    /// Here, the `Input` represents the _intent_. It is up to the game
    /// internally to validate the intent and update world state as appropriate
    /// (e.g. collision)
    void Update(input::Input, float dt) noexcept;

    void NextLevel() noexcept;

    [[nodiscard(
        "The return value should be the only observable effect of this "
        "function")]]
    State GetState() const noexcept;

    void StartNewCampaign() noexcept;

    void LoadExistingCampaign() noexcept;

    void FinishLevel() noexcept;

    [[nodiscard("Save operations may fail and must be reported")]]
    bool Save() const noexcept;

    float GetCharacterWidth() const noexcept;

 private:
    /// \brief (Re)create the level and respawn all ECS entities for it.
    ///
    /// The player's current body is preserved across rebuilds; on the very
    /// first build the player starts at `initial_player_body`.
    void RebuildLevel() noexcept;

 private:
    Campaign campaign_{};
    Level level_;
    ECS ecs_{};
    Bounds level_bounds_{};
    State state_{Game::State::Title};

 private:  // statics
    static inline constexpr float dyn_body_speed{7.0f};
    static inline constexpr types::Rectangle<float> initial_player_body{
        .pos{.x = 0.0f, .y = 0.0f},
        .bounds{.width = 2.0f, .height = 2.0f},
    };
};
}  // namespace arigato::core

#endif  // SRC_CORE_INCLUDE_GAME_HPP_
