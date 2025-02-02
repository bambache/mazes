#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

// Custom specialization of std::hash can be injected in namespace std.
template<>
struct std::hash<pair<int,int>>
{
    size_t operator()(const pair<int,int>& s) const noexcept
    {
        size_t h1 = hash<int>{}(s.first);
        size_t h2 = hash<int>{}(s.second);
        return h1 ^ (h2 << 1);
    }
};

// Type alias for adjacency list
typedef unordered_map<pair<int, int>, vector<pair<int, int>>, hash<pair<int,int>>> AdjList;

// Function to check if a cell is within bounds and walkable
bool isValid(int row, int col, const vector<vector<char>>& maze) {
    return row >= 0 && row < maze.size() && col >= 0 && col < maze[0].size() && maze[row][col] == '_';
}

// Function to convert maze to adjacency list
AdjList convertToAdjList(const vector<vector<char>>& maze) {
    AdjList graph;
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Up, Down, Left, Right

    for (int row = 0; row < maze.size(); row++) {
        for (int col = 0; col < maze[0].size(); col++) {
            if (maze[row][col] == '_') {
                pair<int, int> node = {row, col};
                for (auto [dr, dc] : directions) {
                    int newRow = row + dr, newCol = col + dc;
                    if (isValid(newRow, newCol, maze)) {
                        graph[node].push_back({newRow, newCol});
                    }
                }
            }
        }
    }
    return graph;
}

// Helper function to print adjacency list
void printMaze(const vector<vector<char>>& maze) {
    cout << '-';
    for (int col = 0; col < maze[0].size(); col++) {
        cout << col;
    }
    cout << endl;
    for (int row = 0; row < maze.size(); row++) {
        cout << row;
        for (int col = 0; col < maze[0].size(); col++) {
            cout << maze[row][col];
        }
        cout << endl;
    }
}

// Helper function to print adjacency list
void printAdjList(const AdjList& graph) {
    for (const auto& [node, neighbors] : graph) {
        cout << "(" << node.first << ", " << node.second << ") -> ";
        for (const auto& neighbor : neighbors) {
            cout << "(" << neighbor.first << ", " << neighbor.second << ") ";
        }
        cout << endl;
    }
}

int main() {
    vector<vector<char>> maze = {
        {'#', '#', '#', '#', '#'},
        {'#', '_', '_', '#', '#'},
        {'#', '_', '#', '_', '#'},
        {'#', '_', '_', '_', '#'},
        {'#', '#', '#', '#', '#'}
    };

    printMaze(maze);

    AdjList graph = convertToAdjList(maze);
    printAdjList(graph);
    return 0;
}
