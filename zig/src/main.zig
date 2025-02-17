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
    const input: []const []const u8 = &[_][]const u8{
        "#S###",
        "#__##",
        "#_#_#",
        "#___#",
        "###E#",
    };
    try printInput(input);

    const m = try maze.Maze.init(input);
    _ = m;
}

test "simple test" {
    var list = std.ArrayList(i32).init(std.testing.allocator);
    defer list.deinit(); // try commenting this out and see if zig detects the memory leak!
    try list.append(42);
    try std.testing.expectEqual(@as(i32, 42), list.pop());
}
