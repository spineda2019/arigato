const std = @import("std");
const mem_options = @import("mem_options");
const allocation_strategy = switch (mem_options.allocation_strategy) {
    .smp => @import("alloc_strategy/smp.zig"),
    .arena => @import("alloc_strategy/buffer_first.zig"),
};
const arena = @import("alloc_strategy/arena.zig");

var ptr_map: std.HashMap(
    [*]u8,
    usize,
    struct {
        pub fn hash(_: @This(), key: [*]u8) u64 {
            return @intFromPtr(key);
        }
        pub fn eql(_: @This(), key: [*]u8, other: [*]u8) bool {
            return key == other;
        }
    },
    80,
) = .init(std.heap.smp_allocator);

fn new(allocator: *std.mem.Allocator, amount: usize) ?[*]u8 {
    const mem: []u8 = allocator.alignedAlloc(
        u8,
        .@"16",
        amount,
    ) catch @panic("OOM");
    std.debug.assert(ptr_map.get(mem.ptr) == null);
    ptr_map.put(mem.ptr, mem.len) catch @panic("OOM");
    return mem.ptr;
}

/// unsigned char* zig_new(std::size_t) noexcept;
export fn zig_new(amount: usize) ?[*]u8 {
    const alloc: *std.mem.Allocator = allocation_strategy.getAllocator();
    return new(alloc, amount);
}
/// unsigned char* zig_new_aligned(std::size_t, Align) noexcept;
export fn zig_new_aligned(amount: usize, alignment: usize) ?*u8 {
    _ = amount;
    _ = alignment;
    return null;
}

export fn zig_arena_new(amount: usize) ?[*]u8 {
    const alloc: *std.mem.Allocator = arena.getArena();
    return new(alloc, amount);
}

/// void zig_delete(void* ptr) noexcept;
export fn zig_delete(ptr: [*]u8) void {
    if (ptr_map.fetchRemove(ptr)) |slice| {
        const alloc: *std.mem.Allocator = allocation_strategy.getAllocator();
        alloc.free(slice.key[0..slice.value]);
    } else {
        // only reachable in double-deletes from C++, which is UB anyway
        unreachable;
    }
}
/// void zig_delete_aligned(void*, Align) noexcept;
export fn zig_delete_aligned(ptr: *u8, alignment: usize) void {
    _ = ptr;
    _ = alignment;
}
