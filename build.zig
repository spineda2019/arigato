const std = @import("std");
const builtin = @import("builtin");

const cppfiles = [_][]const u8{
    "src/main.cpp",
};

const cppflags = [_][]const u8{
    "-std=c++23",
    "-Wall",
    "-Wextra",
    "-Wpedantic",
    "-Wshadow",
    "-Wconversion",
    "-Werror",
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

pub fn build(b: *std.Build) std.mem.Allocator.Error!void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const create_compiledb = b.option(
        bool,
        "compiledb",
        "Generate the compilation database",
    ) orelse false;

    const mod = b.addModule("arigato", .{
        .target = target,
        .optimize = optimize,
        .link_libcpp = true,
    });
    for (cppfiles) |file| {
        mod.addCSourceFile(.{
            .file = b.path(file),
            .language = .cpp,
            .flags = try allFlags(.{
                .allocator = b.allocator,
                .for_file = file,
                .compiledb = create_compiledb,
            }),
        });
    }

    // Here we define an executable. An executable needs to have a root module
    // which needs to expose a `main` function. While we could add a main function
    // to the module defined above, it's sometimes preferable to split business
    // logic and the CLI into two separate modules.
    //
    // If your goal is to create a Zig library for others to use, consider if
    // it might benefit from also exposing a CLI tool. A parser library for a
    // data serialization format could also bundle a CLI syntax checker, for example.
    //
    // If instead your goal is to create an executable, consider if users might
    // be interested in also being able to embed the core functionality of your
    // program in their own executable in order to avoid the overhead involved in
    // subprocessing your CLI tool.
    //
    // If neither case applies to you, feel free to delete the declaration you
    // don't need and to put everything under a single module.
    const exe = b.addExecutable(.{
        .name = "arigato",
        .root_module = mod,
    });

    b.installArtifact(exe);

    const run_step = b.step("run", "Run the game");

    const run_cmd = b.addRunArtifact(exe);
    run_step.dependOn(&run_cmd.step);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    // Creates an executable that will run `test` blocks from the provided module.
    // Here `mod` needs to define a target, which is why earlier we made sure to
    // set the releative field.
    const mod_tests = b.addTest(.{
        .root_module = mod,
    });

    // A run step that will run the test executable.
    const run_mod_tests = b.addRunArtifact(mod_tests);

    // Creates an executable that will run `test` blocks from the executable's
    // root module. Note that test executables only test one module at a time,
    // hence why we have to create two separate ones.
    const exe_tests = b.addTest(.{
        .root_module = exe.root_module,
    });

    // A run step that will run the second test executable.
    const run_exe_tests = b.addRunArtifact(exe_tests);

    // A top level step for running all tests. dependOn can be called multiple
    // times and since the two run steps do not depend on one another, this will
    // make the two of them run in parallel.
    const test_step = b.step("test", "Run tests");
    test_step.dependOn(&run_mod_tests.step);
    test_step.dependOn(&run_exe_tests.step);

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
    compiledb_run.step.dependOn(&exe.step);
    compiledb_step.dependOn(&compiledb_run.step);
    if (create_compiledb) {
        b.getInstallStep().dependOn(compiledb_step);
    }
}
