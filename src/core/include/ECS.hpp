// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// ECS.hpp - Central ECS management
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_ECS_HPP_
#define SRC_CORE_INCLUDE_ECS_HPP_

#include <bitset>
#include <cstddef>
#include <cstdint>
#include <tuple>
#include <utility>
#include <vector>
//
#include <arigato/input.hpp>
#include <arigato/physics.hpp>
//
#include "arigato/id.hpp"
#include "components/Component.hpp"
#include "components/InteractableComponent.hpp"
#include "components/PhysicsComponent.hpp"
#include "components/SpriteComponent.hpp"
#include "systems/LevelSystem.hpp"
//
#include "entities/Id.hpp"

namespace arigato::core {
class ECS final {
 public:
    enum struct BitSetIndex : std::uint8_t {
        InteractableComponent,
        PhysicsBodyComponent,
        PhysicsSpeedComponent,
        PhysicsIntentComponent,
        SpriteComponent,
        /// \brief just used to check if an entity in our component arrays is in
        /// use
        Alive,
        /// \brief I dislike this. Used for `component_masks_` without C++26
        Count,
    };

    using BitMask = std::bitset<std::to_underlying(BitSetIndex::Count)>;

    struct PrivateAccesor;
    friend PrivateAccesor;

    // HACK(SEP) Retrofits into main. Explore passing some type of callabale for
    // rendering into the Game update API
    struct Drawable final {
        types::Rectangle<float> rect;
        arigato::id::SpriteId sprite_id;
    };
    static_assert(std::is_trivially_constructible_v<Drawable>);
    static_assert(std::is_trivially_destructible_v<Drawable>);

 public:
    explicit ECS() = default;

    /// \brief Advance the simulation by one frame
    void Update(input::Input, float dt) noexcept;

    /// \brief Forget every entity and recycle all storage. Ids handed out
    /// before this call are invalid afterwards.
    void Clear() noexcept;

    /// \brief Create a new entity with no components
    arigato::entities::Id CreateEmptyEntity();

    /// \brief Unregister the entity from the ECS and recycle the ID for future
    /// entities
    ///
    /// Double destructions should be memory-safe
    void DestroyEntity(arigato::entities::Id);

    /// \brief Small helper to Spawn the playable character
    entities::Id SpawnPlayer(types::Rectangle<float> body, float speed);
    /// \brief Small helper to spawn an immovable, collidable body
    entities::Id SpawnStatic(types::Rectangle<float> body,
                             arigato::id::SpriteId);
    /// \brief Randomly generates the target position
    entities::Id SpawnStatic(types::Bounds<float> bounds,
                             arigato::id::SpriteId);
    /// \brief The player's current body in game _logical units_
    types::Rectangle<float> GetPlayerBody() noexcept;

    std::vector<Drawable> GetBodies();

 private:
    /// \brief sugar-helper to enfore component concept on container elements
    template <component::Component C>
    using ComponentVec = std::vector<C>;

    using AllComponents =
        std::tuple<ComponentVec<component::InteractableComponent>,
                   ComponentVec<component::PhysicsBodyComponent>,
                   ComponentVec<component::PhysicsSpeedComponent>,
                   ComponentVec<component::PhysicsIntentComponent>,
                   ComponentVec<component::SpriteComponent>>;

 private:
    AllComponents components_;
    /// \brief the bitmask indicating if the component index by id is in use by
    /// the entity of valud id.
    ///
    /// Until C++26, this is maintained by the enum sentinel. I would ideally
    /// like the number of bits to be calculated with
    /// std::meta::enumerators_of(^^BitSetIndex).size()
    std::vector<BitMask> component_masks_;
    /// \brief candidate set for reusable ids that have been destroyed after
    /// newer IDs were created
    std::vector<arigato::entities::Id> free_ids_;
    LevelSystem level_system_{};
    arigato::entities::Id entity_count{};
    arigato::entities::Id player_id_{};

 private:
    /// \brief Create a new entity at the tail end of the arrays with the alive
    /// bit set
    void AppendEmptyEntity();
};
}  // namespace arigato::core

#endif  // SRC_CORE_INCLUDE_ECS_HPP_
