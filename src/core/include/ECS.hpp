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
#include <type_traits>
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
#include "zig.hpp"

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

 public:
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

    /// \brief exposes iterable capabilities (e.g. range-for loops) for entities
    /// in the ECS
    template <BitSetIndex... Ts>
    class EntityIterable final {
        static_assert((... &&
                       (Ts != BitSetIndex::Alive && Ts != BitSetIndex::Count)),
                      "EntityIterable must not include Alive or Count, which "
                      "are meta-data");
        static_assert(
            sizeof...(Ts) > 0,
            "Entities in the iterable must have at least 1 component");

        template <BitSetIndex... BitSet>
        struct IsEntityAt final {
            static_assert(false, "Unrecognized set of components (entity)");
            static inline constexpr bool Call(std::size_t,
                                              ECS const*) noexcept {
                return false;
            }
        };

        template <>
        struct IsEntityAt<BitSetIndex::SpriteComponent,
                          BitSetIndex::PhysicsBodyComponent>
            final {
            static inline constexpr bool Call(std::size_t entity_id,
                                              ECS* ecs) noexcept {
                return ecs->IsDrawableAt(entity_id);
            }
        };

        /// \brief Helper concrete base for specializations of the Iterator type
        ///
        /// In this custom iterator of the encapsulating `EntityIterable`
        /// container, `operator==` and `operator++` are always the same. It is
        /// just `operator*` that differs (and that differentiation will be
        /// decided at compile time with template partialization on the types
        /// passed in to the encapsulating `EntityIterable`)
        struct IteratorBase {
            /// \brief the index into the ECS component collections that refers
            /// to the current Entity in the iterable
            std::size_t index;
            /// \brief the ecs who owns the component collections
            ECS* ecs;

            constexpr bool operator==(IteratorBase const& rhs) const noexcept {
                return (this->index == rhs.index) && (this->ecs == rhs.ecs);
            }

            /// \brief increment the iterator to refer to the next valid
            /// component, or refer to the end of the container (whichever comes
            /// first)
            ///
            /// This uses deducing-this to prevent weird reference-slicing since
            /// this is a simple helper base class. When called on a derived
            /// real Iterator (see below for some impls), `self` will resolve
            /// to the correct derived type at compile time
            constexpr auto& operator++(this auto& self) noexcept {
                do {
                    self.index += 1;
                } while (self.index < self.ecs->component_masks_.size() &&
                         !IsEntityAt<Ts...>::Call(self.index, self.ecs));
                return self;
            }
        };
        static_assert(std::is_trivially_constructible_v<IteratorBase>);
        static_assert(std::is_trivially_destructible_v<IteratorBase>);

        /// \brief the final iterator. Usable Iterators will be specialized on
        /// `BitSet`
        ///
        /// The specialization on `BitSet` represents that actual entity that is
        /// being requested to iterate through. For example,
        /// `Iterator<BitSetIndex::SpriteComponent,
        /// BitSetIndex::PhysicsBodyComponent>` represents an Iterator for
        /// "Drawable" entites in the ECS
        template <BitSetIndex... BitSet>
        struct Iterator final {
            static_assert(false, "Unrecognized set of components (entity)");
        };

     public:
        template <>
        struct Iterator<BitSetIndex::SpriteComponent,
                        BitSetIndex::PhysicsBodyComponent>
            final : public IteratorBase {
            constexpr Iterator(std::size_t idx, ECS* ptr)
                : IteratorBase{idx, ptr} {}

            /// \brief return the drawable refered to by the current iterator
            ///
            /// NOTE: We return by value here, mostly because its cheap to do
            /// so and simpler. If a caller ever needs references (which they
            /// shouldn't, internal mutation should be handled by the ECS),
            /// you'll need to do two things:
            /// * Think really _really_ hard if a caller really should be
            /// getting a reference, which may be opening the door to semantic
            /// ownership issues
            /// * If you answer 'yes' to number one, somehow store/cache the
            /// current
            ///   drawable as a member, compute it if needed, then return it by
            ///   reference
            Drawable operator*() noexcept {
                return this->ecs->GetDrawableAt(this->index);
            }
        };

        using Iter = Iterator<Ts...>;

     public:
        constexpr explicit EntityIterable(ECS* owner) noexcept : ecs_{owner} {}

        constexpr Iter begin() const {
            for (std::size_t entity_id{0};
                 entity_id < ecs_->component_masks_.size();  //
                 ++entity_id) {
                // find the first valid entity comprised of the requested
                // components
                if (IsEntityAt<Ts...>::Call(entity_id, ecs_)) {
                    return {entity_id, ecs_};
                }
            }

            // If no entities of the requested type was found, container is
            // empty
            return this->end();
        }
        constexpr Iter end() const {
            return {ecs_->component_masks_.size(), ecs_};
        }

     private:
        ECS* ecs_;
    };
    using DrawableIterable = EntityIterable<BitSetIndex::SpriteComponent,
                                            BitSetIndex::PhysicsBodyComponent>;
    static_assert(std::is_trivially_destructible_v<DrawableIterable>);
    static_assert(std::is_trivially_destructible_v<DrawableIterable::Iter>);

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

    bool IsDrawableAt(std::size_t) noexcept;
    Drawable GetDrawableAt(std::size_t) noexcept;
    DrawableIterable GetDrawables() noexcept(
        std::is_nothrow_constructible_v<DrawableIterable, ECS*>);

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
