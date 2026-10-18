//! Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//!
//! root.zig - root module for the game unit tests
//!
//! This Source Code Form is subject to the terms of the Mozilla Public
//! License, v. 2.0. If a copy of the MPL was not distributed with this
//! file, You can obtain one at https://mozilla.org/MPL/2.0/.

const std = @import("std");

const TestFn = *const fn () callconv(.c) bool;

test "ECS.Iterator" {
    const testEntityIterator = @extern(TestFn, .{ .name = "TestEntityIterator" });
    try std.testing.expect(testEntityIterator());
}
