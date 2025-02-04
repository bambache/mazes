#include "maze.h"

using namespace std;

// Function to check if a cell is within bounds and walkable
bool isValid(int row, int col, const vector<vector<char>> &maze) {
  return row >= 0 && row < maze.size() && col >= 0 && col < maze[0].size() &&
         maze[row][col] == '_';
}

AdjList convertToAdjList(const vector<vector<char>> &maze) {
  AdjList graph;
  vector<pair<int, int>> directions = {
      {-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Up, Down, Left, Right

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

void printMaze(const vector<vector<char>> &maze) {
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

void printAdjList(const AdjList &graph) {
  for (const auto &[node, neighbors] : graph) {
    cout << "(" << node.first << ", " << node.second << ") -> ";
    for (const auto &neighbor : neighbors) {
      cout << "(" << neighbor.first << ", " << neighbor.second << ") ";
    }
    cout << endl;
  }
}
