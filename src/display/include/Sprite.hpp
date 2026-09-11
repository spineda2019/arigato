/// \file
/// \brief Independent representation of a game sprite/spritesheet
///
/// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
///
/// This Source Code Form is subject to the terms of the Mozilla Public
/// License, v. 2.0. If a copy of the MPL was not distributed with this
/// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_DISPLAY_INCLUDE_SPRITE_HPP_
#define SRC_DISPLAY_INCLUDE_SPRITE_HPP_

#include <memory>
#include <type_traits>
//
#include <arigato/physics.hpp>

namespace arigato::display {

/// A 2D Texture. May represent a single sprite or an entire spritesheet
class Sprite final {
 public:  // types
    using Area = types::Rectangle<float>;
    using IntegralArea = types::Rectangle<int>;

 private:  // types
    struct Impl;

 public:
    explicit Sprite(const char* path) noexcept;
    explicit Sprite(const char* path, Area sprite_area) noexcept;
    using ReadonlyImplRef_t = std::unique_ptr<Impl> const&;
    ReadonlyImplRef_t ReadonlyImplRef() const noexcept;
    float GetWidth() const noexcept;
    float GetHeight() const noexcept;

 public:                 // rule of 5
    ~Sprite() noexcept;  // in cpp file for pimpl reasons so we can use the
                         // unique_ptr
    Sprite(Sprite const&) = delete;
    Sprite& operator=(Sprite const&) = delete;
    Sprite(Sprite&&) = default;
    Sprite& operator=(Sprite&&) = default;

 private:
    std::unique_ptr<Impl> impl_{};
    Area area_{};
};

static_assert(!std::is_copy_assignable_v<Sprite>,
              "Sprites require unique ownership");
static_assert(!std::is_copy_constructible_v<Sprite>,
              "Sprites require unique ownership");

static_assert(std::is_move_constructible_v<Sprite>);
static_assert(std::is_move_assignable_v<Sprite>);
}  // namespace arigato::display

#endif  // SRC_DISPLAY_INCLUDE_SPRITE_HPP_
