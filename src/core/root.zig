//! Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//!
//! root.zig - Zig compilation unit for the core game library
//!
//! This Source Code Form is subject to the terms of the Mozilla Public
//! License, v. 2.0. If a copy of the MPL was not distributed with this
//! file, You can obtain one at https://mozilla.org/MPL/2.0/.

const builtin = @import("builtin");
const std = @import("std");

export fn zig_assert(cond: bool) void {
    if (builtin.mode == .Debug or builtin.mode == .ReleaseSafe) {
        std.debug.assert(cond);
    }
}
