const std = @import("std");
const testing = std.testing;
const Path = std.ArrayList(Position);
const Graph = std.AutoHashMap(Position, Path);

pub const Position = struct {
    row: i32,
    col: i32,

    const default: Position = .{
        .row = -1,
        .col = -1,
    };

    pub fn valid(self: Position) bool {
        return self.row != -1 and self.col != -1;
    }

    pub fn print(self: Position) !void {
        var bw = getBufferedWriter();
        const stdout = bw.writer();
        try stdout.print("({}, {})", .{ self.row, self.col });
        try bw.flush();
    }
};

pub fn isValid(row: i32, col: i32, input: []const []const u8) bool {
    return row >= 0 and row < input.len and col >= 0 and col < input[0].len and switch (input[@intCast(row)][@intCast(col)]) {
        '_', 'E', 'S' => return true,
        else => false,
    };
}

const directions = [4][2]i4{
    [_]i4{ -1, 0 }, //up
    [_]i4{ 1, 0 }, // down
    [_]i4{ 0, -1 }, // left
    [_]i4{ 0, 1 }, // right
};

pub fn addNodeToGraph(graph: *Graph, position: Position, input: []const []const u8) !void {
    for (directions) |dir| {
        const newRow = position.row + dir[0];
        const newCol = position.col + dir[1];
        if (isValid(newRow, newCol, input)) {
            const v = try graph.getOrPut(position);
            if (!v.found_existing) {
                v.value_ptr.* = Path.init(std.heap.page_allocator);
            }

            try v.value_ptr.*.append(Position{ .row = newRow, .col = newCol });
        }
    }
}

pub const Maze = struct {
    start: Position,
    end: Position,
    graph: Graph,

    pub fn init(ally: std.mem.Allocator, input: []const []const u8) !Maze {
        var maze = Maze{ .start = Position.default, .end = Position.default, .graph = Graph.init(ally) };
        for (0..input.len) |row| {
            for (0..input[0].len) |col| {
                // std.debug.print("{c}\n", .{input[row][col]});
                switch (input[row][col]) {
                    'S' => {
                        maze.start.row = @intCast(row);
                        maze.start.col = @intCast(col);
                        try addNodeToGraph(&maze.graph, maze.start, input);
                    },
                    'E' => {
                        maze.end.row = @intCast(row);
                        maze.end.col = @intCast(col);
                        try addNodeToGraph(&maze.graph, maze.end, input);
                    },
                    '_' => {
                        try addNodeToGraph(&maze.graph, Position{ .row = @intCast(row), .col = @intCast(col) }, input);
                    },
                    '#' => {},
                    else => return error.InvalidChar,
                }
            }
        }
        if (!maze.start.valid() or !maze.end.valid()) {
            maze.deinit();
            return error.InvalidMaze;
        }
        return maze;
    }

    pub fn deinit(self: *Maze) void {
        self.graph.deinit();
    }

    pub fn print(self: Maze) !void {
        var bw = getBufferedWriter();
        const stdout = bw.writer();

        try stdout.print("Start: ", .{});
        try bw.flush();
        try self.start.print();
        try stdout.print("\nEnd: ", .{});
        try bw.flush();
        try self.end.print();

        try stdout.print("\n", .{});
        try bw.flush();

        var iterator = self.graph.iterator();
        while (iterator.next()) |entry| {
            try entry.key_ptr.print();
            try stdout.print(" -> ", .{});
            try bw.flush();
            for (entry.value_ptr.*.items) |position| {
                try position.print();
            }
            try stdout.print("\n", .{});
            try bw.flush();
        }
        try bw.flush();
    }
};

pub fn getBufferedWriter() std.io.BufferedWriter(4096, std.fs.File.Writer) {
    return std.io.bufferedWriter(std.io.getStdOut().writer());
}

test "invalid maze" {
    const ally = std.testing.allocator;
    const input: []const []const u8 = &[_][]const u8{
        "###",
        "#_#",
        "#_#",
    };
    try testing.expectError(error.InvalidMaze, Maze.init(ally, input));
}

test "adjacency list conversion" {
    const ally = std.testing.allocator;
    const input: []const []const u8 = &[_][]const u8{
        "###",
        "#S#",
        "#E#",
    };
    var m = try Maze.init(ally, input);
    defer m.deinit();
    try testing.expectEqual(m.graph.count(), 2);
    try testing.expect(m.graph.contains(Position{ .row = 1, .col = 1 }));
    try testing.expect(m.graph.contains(Position{ .row = 2, .col = 1 }));
    const path = m.graph.get(Position{ .row = 1, .col = 1 }) orelse unreachable;
    try testing.expectEqual(path.items.len, 1);
    try testing.expectEqual(path.getLast(), Position{ .row = 2, .col = 1 });
}
