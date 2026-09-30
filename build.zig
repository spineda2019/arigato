/// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
///
/// build.zig - The build script for the arigato! game
///
/// This Source Code Form is subject to the terms of the Mozilla Public
/// License, v. 2.0. If a copy of the MPL was not distributed with this
/// file, You can obtain one at https://mozilla.org/MPL/2.0/.
const std = @import("std");
const builtin = @import("builtin");

const CppFiles = struct {
    const core = [_][]const u8{
        "src/core/Game.cpp",
        "src/core/Character.cpp",
        "src/core/Level.cpp",
        "src/core/Campaign.cpp",
        "src/core/PhysicsManager.cpp",
    };
    const display = [_][]const u8{
        "src/display/Window.cpp",
        "src/display/SpriteManager.cpp",
    };
    const game = [_][]const u8{
        "src/game/main.cpp",
    };
};

const CppIncludeDirs = struct {
    const core = [_][]const u8{
        "src/core/include/",
    };
    const display = [_][]const u8{
        "src/display/include/",
    };
    const common = [_][]const u8{
        "src/common_types/",
    };
};

const cppflags = [_][]const u8{
    "-std=c++23",
    "-Wall",
    "-Wextra",
    "-Wpedantic",
    "-Wshadow",
    "-Wconversion",
    "-Werror",
    "-fexperimental-library",
};

fn allFlags(config: struct {
    allocator: std.mem.Allocator,
    for_file: []const u8,
    compiledb: bool,
}) std.mem.Allocator.Error![]const []const u8 {
    if (!config.compiledb) {
        return &cppflags;
    } else {
        var flag_buf: std.ArrayList([]const u8) = .empty;
        errdefer flag_buf.deinit(config.allocator);

        try flag_buf.appendSlice(config.allocator, &cppflags);
        try flag_buf.append(config.allocator, "-MJ");
        try flag_buf.append(config.allocator, name: {
            var name_buf: std.ArrayList(u8) = .empty;
            for (config.for_file) |letter| {
                try name_buf.append(config.allocator, switch (letter) {
                    '/', '\\' => '.',
                    else => |other| other,
                });
            }
            try name_buf.appendSlice(config.allocator, ".json.tmp");
            break :name name_buf.items;
        });

        return flag_buf.items;
    }
}

const Modules = struct {
    core: *std.Build.Module,
    display: *std.Build.Module,
    game: *std.Build.Module,

    fn init(config: struct {
        b: *std.Build,
        target: std.Build.ResolvedTarget,
        optimize: std.builtin.OptimizeMode,
        create_compiledb: bool,
    }) std.mem.Allocator.Error!Modules {
        const mod_core = config.b.createModule(.{
            .root_source_file = config.b.path("src/core/root.zig"),
            .target = config.target,
            .optimize = config.optimize,
            .link_libc = true,
            .link_libcpp = true,
        });
        for (CppFiles.core) |file| {
            mod_core.addCSourceFile(.{
                .file = config.b.path(file),
                .language = .cpp,
                .flags = try allFlags(.{
                    .allocator = config.b.allocator,
                    .for_file = file,
                    .compiledb = config.create_compiledb,
                }),
            });
        }

        const mod_display = config.b.createModule(.{
            .target = config.target,
            .optimize = config.optimize,
            .link_libc = true,
            .link_libcpp = true,
        });
        for (CppFiles.display) |file| {
            mod_display.addCSourceFile(.{
                .file = config.b.path(file),
                .language = .cpp,
                .flags = try allFlags(.{
                    .allocator = config.b.allocator,
                    .for_file = file,
                    .compiledb = config.create_compiledb,
                }),
            });
        }
        const raylib_dep = blk: {
            if (!config.target.query.isNative()) {
                std.debug.print(
                    "INFO: Cross-compiling uses raylib's memory platform\n",
                    .{},
                );
                break :blk switch (config.target.result.os.tag) {
                    .windows => config.b.dependency("raylib", .{
                        .target = config.target,
                        .optimize = config.optimize,
                        .platform = .win32,
                    }),
                    else => config.b.dependency("raylib", .{
                        .target = config.target,
                        .optimize = config.optimize,
                        .platform = .memory,
                    }),
                };
            } else {
                break :blk config.b.dependency("raylib", .{
                    .target = config.target,
                    .optimize = config.optimize,
                });
            }
        };
        const raylib_artifact = raylib_dep.artifact("raylib");
        mod_display.linkLibrary(raylib_artifact);

        const mod_game = config.b.createModule(.{
            .target = config.target,
            .optimize = config.optimize,
            .link_libc = true,
            .link_libcpp = true,
        });
        for (CppFiles.game) |file| {
            mod_game.addCSourceFile(.{
                .file = config.b.path(file),
                .language = .cpp,
                .flags = try allFlags(.{
                    .allocator = config.b.allocator,
                    .for_file = file,
                    .compiledb = config.create_compiledb,
                }),
            });
        }

        for (CppIncludeDirs.core) |inc| {
            mod_game.addIncludePath(config.b.path(inc));
        }
        for (CppIncludeDirs.display) |inc| {
            mod_game.addIncludePath(config.b.path(inc));
        }
        for (CppIncludeDirs.common) |inc| {
            mod_game.addIncludePath(config.b.path(inc));
            mod_core.addIncludePath(config.b.path(inc));
            mod_display.addIncludePath(config.b.path(inc));
        }

        return .{ .core = mod_core, .display = mod_display, .game = mod_game };
    }

    const Compilations = struct {
        core: *std.Build.Step.Compile,
        display: *std.Build.Step.Compile,
        game: *std.Build.Step.Compile,

        pub fn init(config: struct {
            b: *std.Build,
            modules: *const Modules,
        }) Compilations {
            const lib_core = config.b.addLibrary(.{
                .linkage = .static,
                .name = "core",
                .root_module = config.modules.core,
            });

            const lib_display = config.b.addLibrary(.{
                .linkage = .static,
                .name = "display",
                .root_module = config.modules.display,
            });

            config.modules.game.linkLibrary(lib_core);
            config.modules.game.linkLibrary(lib_display);

            const game = config.b.addExecutable(.{
                .name = "arigato",
                .root_module = config.modules.game,
            });

            return .{ .core = lib_core, .display = lib_display, .game = game };
        }
    };
};

const BuildOptions = struct {
    create_compiledb: bool,
    draw_hitboxes: bool,

    fn init(b: *std.Build) BuildOptions {
        return .{
            .create_compiledb = b.option(
                bool,
                "compiledb",
                "Generate the compilation database",
            ) orelse false,
            .draw_hitboxes = b.option(
                bool,
                "drawhit",
                "Draw hitboxes around physics entities",
            ) orelse false,
        };
    }
};

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const options: BuildOptions = .init(b);

    const mods: Modules = try .init(.{
        .b = b,
        .target = target,
        .optimize = optimize,
        .create_compiledb = options.create_compiledb,
    });

    inline for (comptime std.meta.fieldNames(Modules)) |field_name| {
        if (optimize == .Debug) {
            const mod: *std.Build.Module = @field(mods, field_name);
            mod.addCMacro("ARIGATO_DEBUG", "");
        }
        if (optimize == .Debug or optimize == .ReleaseSafe) {
            const mod: *std.Build.Module = @field(mods, field_name);
            mod.addCMacro("ARIGATO_ASSERT", "");
        }
    }
    if (options.draw_hitboxes) {
        mods.game.addCMacro("ARIGATO_DRAW_HITBOXES", "");
    }

    const compilations: Modules.Compilations = .init(.{
        .b = b,
        .modules = &mods,
    });
    compilations.game.lto = switch (optimize) {
        .Debug => .none,
        else => switch (target.result.os.tag) {
            .windows, .macos => .none,
            else => .full,
        },
    };

    b.installArtifact(compilations.game);
    b.installDirectory(.{
        .source_dir = b.path("assets/"),
        .install_dir = .bin,
        .install_subdir = "assets",
    });

    const run_step = b.step("run", "Run the game");

    const run_cmd = b.addRunArtifact(compilations.game);
    run_step.dependOn(&run_cmd.step);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    // Creates an executable that will run `test` blocks from the executable's
    // root module. Note that test executables only test one module at a time,
    // hence why we have to create two separate ones.
    const mod_tests = b.addTest(.{
        .name = "test",
        .root_module = b.createModule(.{
            .optimize = optimize,
            .target = target,
            .link_libcpp = true,
            .link_libc = true,
            .root_source_file = b.path("test/root.zig"),
            .sanitize_c = .full,
        }),
    });
    mod_tests.root_module.linkLibrary(compilations.core);
    // A run step that will run the test executable.
    const run_mod_tests = b.addRunArtifact(mod_tests);

    // A top level step for running all tests. dependOn can be called multiple
    // times and since the two run steps do not depend on one another, this will
    // make the two of them run in parallel.
    const test_step = b.step("test", "Run tests");
    test_step.dependOn(&run_mod_tests.step);

    const compiledb_step = b.step("compiledb", "Combine compiledb fragments");
    const compiledb_exe = b.addExecutable(.{
        .name = "compiledb",
        .root_module = b.createModule(.{
            .target = .{
                .query = .fromTarget(&builtin.target),
                .result = builtin.target,
            },
            .optimize = .Debug,
            .root_source_file = b.path("cleandb/main.zig"),
        }),
    });
    const compiledb_run = b.addRunArtifact(compiledb_exe);
    compiledb_run.addFileArg(b.path(""));
    compiledb_run.step.dependOn(&compilations.game.step);
    compiledb_step.dependOn(&compiledb_run.step);
    if (options.create_compiledb) {
        b.getInstallStep().dependOn(compiledb_step);
    }
}
