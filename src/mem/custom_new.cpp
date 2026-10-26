// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// custom_new.cpp - Custom global allocation functions
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "custom_new.hpp"

#include <cstddef>
#include <new>
#include <utility>

// TAIL CALL OPTIMIZATION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void* operator new(std::size_t amnt, std::nothrow_t const&) noexcept {
    return zig_new(amnt);
}
void* operator new[](std::size_t amnt, std::nothrow_t const&) noexcept {
    return zig_new_array(amnt);
}
void* operator new(std::size_t amnt, std::align_val_t al,
                   std::nothrow_t const&) noexcept {
    return zig_new_aligned(amnt, std::to_underlying(al));
}
void* operator new[](std::size_t amnt, std::align_val_t al,
                     std::nothrow_t const&) noexcept {
    return zig_new_array_aligned(amnt, std::to_underlying(al));
}
void* operator new(std::size_t amnt) {
    if (void* const ptr{::operator new(amnt, std::nothrow_t{})}; ptr)
        [[likely]] {
        return ptr;
    } else {
        throw std::bad_alloc{};
    }
}
void* operator new[](std::size_t amnt) {
    if (void* const ptr{::operator new[](amnt, std::nothrow_t{})}; ptr)
        [[likely]] {
        return ptr;
    } else {
        throw std::bad_alloc{};
    }
}
void* operator new(std::size_t amnt, std::align_val_t al) {
    if (void* const ptr{::operator new(amnt, al, std::nothrow_t{})}; ptr)
        [[likely]] {
        return ptr;
    } else {
        throw std::bad_alloc{};
    }
}
void* operator new[](std::size_t amnt, std::align_val_t al) {
    if (void* const ptr{::operator new[](amnt, al, std::nothrow_t{})}; ptr)
        [[likely]] {
        return ptr;
    } else {
        throw std::bad_alloc{};
    }
}

void operator delete(void* ptr) noexcept {
    if (ptr) {
        zig_delete(ptr);
    }
}
void operator delete[](void* ptr) noexcept {
    if (ptr) {
        zig_delete_array(ptr);
    }
}
void operator delete(void* ptr, std::align_val_t al) noexcept {
    if (ptr) {
        zig_delete_aligned(ptr, std::to_underlying(al));
    }
}
void operator delete[](void* ptr, std::align_val_t al) noexcept {
    if (ptr) {
        zig_delete_array_aligned(ptr, std::to_underlying(al));
    }
}
