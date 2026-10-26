const std = @import("std");

var allocator = std.heap.smp_allocator;

pub fn getAllocator() *std.mem.Allocator {
    return &allocator;
}
