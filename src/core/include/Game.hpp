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
//
#include <optional>
#include <span>
#include <type_traits>
#include <utility>
//
#include <arigato/input.hpp>
#include <arigato/physics.hpp>
//
#include "Campaign.hpp"
#include "Character.hpp"
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
            std::span<const Level::PlacedCat> cats;
            std::span<const Level::PlacedDecorum> decor;
        };

        static_assert(std::is_trivially_destructible_v<Static>);
        static_assert(std::is_trivially_constructible_v<
                      Static, std::span<const Level::PlacedCat>,
                      std::span<const Level::PlacedDecorum>>);
        static_assert(std::is_trivially_copy_constructible_v<Static>);
        static_assert(std::is_trivially_copy_assignable_v<Static>);

        /// Represents all objects that can move (whether by the player or game
        /// AI)
        struct Dynamic final {
            struct Movement final {
                using Delta = types::Vec2D<float>;

                Delta delta;
            };

            static_assert(std::is_trivially_destructible_v<Movement>);
            static_assert(std::is_trivially_constructible_v<Movement>);
            static_assert(
                std::is_trivially_constructible_v<Movement, Movement::Delta>);
            static_assert(std::is_trivially_copy_constructible_v<Movement>);
            static_assert(std::is_trivially_copy_assignable_v<Movement>);

            struct Action final {
                enum class Direction : std::int8_t {
                    Positive = 1,
                    Zero = 0,
                    Negative = -1,
                };
                Direction move_x;
                Direction move_y;

                inline constexpr Movement Translate(std::uint8_t speed,
                                                    float dt) const noexcept {
                    constexpr auto translate =
                        [](Direction move, std::uint8_t speed_arg,
                           float dt_arg) noexcept -> float {
                        return static_cast<float>(std::to_underlying(move) *
                                                  speed_arg) *
                               dt_arg;
                    };

                    return {
                        .delta{
                            .x = translate(move_x, speed, dt),
                            .y = translate(move_y, speed, dt),
                        },
                    };
                }
            };
            static_assert(std::is_trivially_destructible_v<Action>);
            static_assert(std::is_trivially_constructible_v<Action>);
            static_assert(std::is_trivially_constructible_v<
                          Action, Action::Direction, Action::Direction>);
            static_assert(std::is_trivially_copy_constructible_v<Action>);
            static_assert(std::is_trivially_copy_assignable_v<Action>);

            Character::Rectangle character;
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

    struct Action final {
        Entities::Dynamic::Action character_action;
        Level::Action level_action;
    };
    static_assert(std::is_trivially_destructible_v<Action>);
    static_assert(std::is_trivially_constructible_v<Action>);
    static_assert(std::is_trivially_constructible_v<
                  Action, Entities::Dynamic::Action, Level::Action>);
    static_assert(std::is_trivially_copy_constructible_v<Action>);
    static_assert(std::is_trivially_copy_assignable_v<Action>);

 public:
    using Bounds = types::Bounds<int>;
    explicit Game(Bounds) noexcept;

    /// Returns the player's position in the world in _game units_, not pixels.
    Entities GetPositions() const noexcept;
    Entities::Static GetStatics() const noexcept;
    Entities::Dynamic GetDynamics() const noexcept;
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

 private:
    Campaign campaign_{};
    std::optional<Level> level_{};
    Character character_{{.pos{}, .bounds{.width = 1, .height = 1}}};
    Bounds level_bounds_{};
    State state_{Game::State::Title};
};
}  // namespace arigato::core

#endif  // SRC_CORE_INCLUDE_GAME_HPP_
