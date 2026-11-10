const std = @import("std");

var buf: [4096]u8 = undefined;
var fallback_allocator = std.heap.smp_allocator;

var buffer_first: std.heap.BufferFirstAllocator = undefined;
var backing_allocator: std.mem.Allocator = undefined;
var arena: std.heap.ArenaAllocator = undefined;
var arena_allocator: ?std.mem.Allocator = null;

pub fn getArena() *std.mem.Allocator {
    if (arena_allocator) |*a| {
        @branchHint(.likely);
        return a;
    } else {
        buffer_first = .init(&buf, fallback_allocator);
        backing_allocator = buffer_first.allocator();
        arena = .init(backing_allocator);

        arena_allocator = arena.allocator();
        return &(arena_allocator.?);
    }
}
