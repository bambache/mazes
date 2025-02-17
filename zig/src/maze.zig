const std = @import("std");
const testing = std.testing;
const Path = std.ArrayList(Position);
const Graph = std.AutoHashMap(Position, Path);

pub const Position = struct {
    row: i32 = -1,
    col: i32 = -1,

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
    start: Position = undefined,
    end: Position = undefined,
    graph: Graph = undefined,

    pub fn init(input: []const []const u8) !Maze {
        var maze = Maze{};
        maze.graph = Graph.init(std.heap.page_allocator);
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
        return maze;
    }
};

pub fn getBufferedWriter() std.io.BufferedWriter(4096, std.fs.File.Writer) {
    return std.io.bufferedWriter(std.io.getStdOut().writer());
}

export fn add(a: i32, b: i32) i32 {
    return a + b;
}

test "basic add functionality" {
    try testing.expect(add(3, 7) == 10);
}
