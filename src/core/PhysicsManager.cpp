// \file
// \brief Physics handling
//
// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "include/PhysicsManager.hpp"

#include <algorithm>
#include <cstring>
#include <iterator>
#include <new>
#include <span>

namespace arigato::core {
PhysicsManager::PhysicsManager()
    : ptrs_(std::hardware_destructive_interference_size) {}

void PhysicsManager::Collide(std::span<Rectangle*> dynamic_bodies,
                             std::span<Rectangle*> static_bodies) noexcept {
    // resize default constructs elements in-place when grown. Since Rectangle
    // should be trivially constructable, this should be super cheap
    ptrs_.resize(dynamic_bodies.size() + static_bodies.size());
    ptrs_.append_range(static_bodies);
    auto it{ptrs_.begin() + std::ssize(static_bodies)};

    for (Rectangle* to_clamp : dynamic_bodies) {
        std::for_each(
            ptrs_.begin(), it, [to_clamp](Rectangle* committed_body) noexcept {
                if (to_clamp->Overlaps(*committed_body)) [[unlikely]] {
                    const int pen_left{to_clamp->RightEdge() -
                                       committed_body->pos.x};
                    const int pen_right{committed_body->RightEdge() -
                                        to_clamp->pos.x};
                    const int pen_top{
                        (to_clamp->pos.y + to_clamp->bounds.height) -
                        committed_body->pos.y};
                    const int pen_bottom{(committed_body->pos.y +
                                          committed_body->bounds.height) -
                                         to_clamp->pos.y};
                    const int pen_x = std::min(pen_left, pen_right);
                    const int pen_y = std::min(pen_top, pen_bottom);
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
}  // namespace arigato::core
