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
#include <span>
#include <type_traits>
#include <vector>
//
#include <arigato/id.hpp>
#include <arigato/physics.hpp>

namespace arigato::core {
/// Representation of the currently running game-state. Theoretically should be
/// easily (or at least tractably) serializable for saving
class Campaign final {
 public:  // Types
          /// \brief Represents a cat currently owned by the player
          ///
          /// Can be randomly placed within a level
    struct Cat final {
        id::CatId id;
    };
    static_assert(std::is_trivially_destructible_v<Cat>);
    static_assert(std::is_trivially_constructible_v<Cat>);
    static_assert(std::is_trivially_constructible_v<Cat, id::CatId>);

    /// \brief Represents a decorum currently owned by the player
    ///
    /// Can NOT be randomly placed within a level. The position of a decorum
    /// is customizable by the player, and is thus part of this type for
    /// persistence.
    struct Decorum final {
        using Position = types::Rectangle<int>;

        Position pos;
        id::DecorId id;
    };

    static_assert(std::is_trivially_destructible_v<Decorum>);
    static_assert(std::is_trivially_constructible_v<Decorum>);
    static_assert(std::is_trivially_copy_constructible_v<Decorum>);
    static_assert(std::is_trivially_constructible_v<Decorum, Decorum::Position,
                                                    id::DecorId>);

 public:  // APIs
    std::size_t GetDay() const noexcept;
    std::span<const Decorum> GetDecor() const noexcept;
    std::span<const Cat> GetCats() const noexcept;
    void NextDay() noexcept;

    void AddCats(std::span<id::CatId>);
    void AddDecor(std::span<id::DecorId>);

    void AddCat(id::CatId);
    void AddDecorum(id::DecorId);

 private:
    inline static constexpr Cat kitters{.id = id::CatId::Kitters};
    inline static constexpr Decorum cafe_bar{
        .pos{
            .pos{
                .x = 13,
                .y = 7,
            },
            .bounds{
                .width = 8,
                .height = 4,
            },
        },
        .id = id::DecorId::CafeBar,
    };

 private:
    std::size_t day_{1};
    /// Always initialized with at least kitters
    std::vector<Cat> cats_{kitters};
    std::vector<Decorum> decor_{cafe_bar};
};
}  // namespace arigato::core
#endif  // SRC_CORE_INCLUDE_CAMPAIGN_HPP_
