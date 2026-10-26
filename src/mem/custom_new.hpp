// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// custom_new.hpp - Custom global allocation functions
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_MEM_CUSTOM_NEW_HPP_
#define SRC_MEM_CUSTOM_NEW_HPP_

#include <concepts>
#include <cstddef>
#include <new>
#include <type_traits>

using Align = std::underlying_type_t<std::align_val_t>;
static_assert(std::same_as<std::size_t, Align>);
extern "C" {
unsigned char* zig_new(std::size_t) noexcept;
unsigned char* zig_new_aligned(std::size_t, Align) noexcept;

void zig_delete(void* ptr) noexcept;
void zig_delete_aligned(void*, Align) noexcept;

unsigned char* zig_arena_new(std::size_t) noexcept;
unsigned char* zig_arena_new_aligned(std::size_t, Align) noexcept;
}
#endif  // SRC_MEM_CUSTOM_NEW_HPP_
