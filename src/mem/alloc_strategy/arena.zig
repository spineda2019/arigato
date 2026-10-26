const std = @import("std");
var backing_allocator = std.heap.smp_allocator;
var arena_allocator: ?std.mem.Allocator = null;
var arena: std.heap.ArenaAllocator = undefined;

pub fn getArena() *std.mem.Allocator {
    if (arena_allocator) |*a| {
        @branchHint(.likely);
        return a;
    } else {
        arena = .init(backing_allocator);
        arena_allocator = arena.allocator();
        return &(arena_allocator.?);
    }
}
