const std = @import("std");
const mem_options = @import("mem_options");

var buf: [mem_options.arena_size * 1024]u8 = undefined;
var fallback_allocator: std.mem.Allocator = std.heap.smp_allocator;
var backing_allocator: std.heap.BufferFirstAllocator = undefined;

var allocator: ?std.mem.Allocator = null;

pub fn getAllocator() *std.mem.Allocator {
    if (allocator) |*a| {
        @branchHint(.likely);
        return a;
    } else {
        backing_allocator = .init(&buf, fallback_allocator);
        allocator = backing_allocator.allocator();
        return &(allocator.?);
    }
}
