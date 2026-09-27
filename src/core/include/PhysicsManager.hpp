// \file
// \brief Physics handling
//
// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_PHYSICSMANAGER_HPP_
#define SRC_CORE_INCLUDE_PHYSICSMANAGER_HPP_

#include <algorithm>
#include <span>
#include <vector>
//
#include <arigato/physics.hpp>

namespace arigato::core {
class PhysicsManager final {
 public:
    using Rectangle = types::Rectangle<int>;

 public:
    explicit PhysicsManager();

 public:
    template <class D, class S>
    inline void Collide(
        std::span<types::Rectangle<D>*> dynamic_bodies,
        std::span<const types::Rectangle<S>> static_bodies) noexcept {
        auto it{dynamic_bodies.begin()};

        for (types::Rectangle<D>* to_clamp : dynamic_bodies) {
            std::ranges::for_each(
                static_bodies,
                [to_clamp](types::Rectangle<S> committed_body) noexcept {
                    if (to_clamp->Overlaps(committed_body)) [[unlikely]] {
                        const auto pen_left{to_clamp->RightEdge() -
                                            committed_body.pos.x};
                        const auto pen_right{committed_body.RightEdge() -
                                             to_clamp->pos.x};
                        const auto pen_top{
                            (to_clamp->pos.y + to_clamp->bounds.height) -
                            committed_body.pos.y};
                        const auto pen_bottom{(committed_body.pos.y +
                                               committed_body.bounds.height) -
                                              to_clamp->pos.y};
                        const auto pen_x = std::min(pen_left, pen_right);
                        const auto pen_y = std::min(pen_top, pen_bottom);
                        if (pen_x < pen_y) {
                            to_clamp->pos.x +=
                                pen_left < pen_right ? -pen_left : pen_right;
                        } else {
                            to_clamp->pos.y +=
                                pen_top < pen_bottom ? -pen_top : pen_bottom;
                        }
                    }
                });
            std::for_each(
                dynamic_bodies.begin(), it,
                [to_clamp](types::Rectangle<D>* committed_body) noexcept {
                    if (to_clamp->Overlaps(*committed_body)) [[unlikely]] {
                        const D pen_left{to_clamp->RightEdge() -
                                         committed_body->pos.x};
                        const D pen_right{committed_body->RightEdge() -
                                          to_clamp->pos.x};
                        const D pen_top{
                            (to_clamp->pos.y + to_clamp->bounds.height) -
                            committed_body->pos.y};
                        const D pen_bottom{(committed_body->pos.y +
                                            committed_body->bounds.height) -
                                           to_clamp->pos.y};
                        const D pen_x = std::min(pen_left, pen_right);
                        const D pen_y = std::min(pen_top, pen_bottom);
                        if (pen_x < pen_y) {
                            to_clamp->pos.x +=
                                pen_left < pen_right ? -pen_left : pen_right;
                        } else {
                            to_clamp->pos.y +=
                                pen_top < pen_bottom ? -pen_top : pen_bottom;
                        }
                    }
                });

            *it = to_clamp;
            ++it;
        }
    }

    void Collide(std::span<Rectangle*> dynamic_bodies,
                 std::span<Rectangle*> static_bodies) noexcept;

 private:
    std::vector<Rectangle*> ptrs_{};
};
}  // namespace arigato::core

#endif  // SRC_CORE_INCLUDE_PHYSICSMANAGER_HPP_
