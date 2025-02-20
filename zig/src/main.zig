const std = @import("std");
const maze = @import("./maze.zig");

fn printInput(input: []const []const u8) !void {
    var bw = maze.getBufferedWriter();
    const stdout = bw.writer();

    const cols = input[0].len;
    try stdout.print("-", .{});
    for (0..cols) |i| {
        try stdout.print("{d}", .{i});
    }
    var r: i32 = 0;
    try stdout.print("\n", .{});
    for (input) |row| {
        try stdout.print("{d}{s}\n", .{ r, row });
        r += 1;
    }
    try bw.flush();
}

pub fn main() !void {
    var general_purpose_allocator = std.heap.GeneralPurposeAllocator(.{}){};
    defer std.debug.assert(general_purpose_allocator.deinit() == .ok);
    const gpa = general_purpose_allocator.allocator();

    const input: []const []const u8 = &[_][]const u8{
        "#S###",
        "#__##",
        "#_#_#",
        "#___#",
        "###E#",
    };
    try printInput(input);

    var m = try maze.Maze.init(gpa, input);
    defer m.deinit();

    try m.print();
}

test "simple test" {
    var list = std.ArrayList(i32).init(std.testing.allocator);
    defer list.deinit(); // try commenting this out and see if zig detects the memory leak!
    try list.append(42);
    try std.testing.expectEqual(@as(i32, 42), list.pop());
}
