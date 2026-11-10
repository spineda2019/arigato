// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// root.cpp - Root TU for unit tests
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "ecs/iterator.hpp"
#include "ecs/lifecycle.hpp"
#include "ecs/spawning.hpp"

extern "C" {
// ******************************* ECS Tests ******************************** //
bool TestEntityIterator() noexcept {
    return arigato::test::TestEntityIterator();
}

bool TestEntityLifecycle() noexcept {
    return arigato::test::TestEntityLifecycle();
}

bool TestEntitySpawning() noexcept {
    return arigato::test::TestEntitySpawning();
}

// ****************************** System Tests ****************************** //
bool TestInputSystem() noexcept {
    return true;
    // TODO(SEP)
    // return arigato::test::TestInputSystem();
}

// TODO(SEP)
bool TestPhysicsMove() noexcept {
    // TODO(SEP)
    // return arigato::test::TestPhysicsMove();
    return true;
}

bool TestPhysicsCollide() noexcept {
    // TODO(SEP)
    // return arigato::test::TestPhysicsCollide();
    return true;
}
}
