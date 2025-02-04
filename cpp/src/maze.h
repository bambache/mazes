#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

// Custom specialization of std::hash can be injected in namespace std.
template <> struct std::hash<pair<int, int>> {
  size_t operator()(const pair<int, int> &s) const noexcept {
    size_t h1 = hash<int>{}(s.first);
    size_t h2 = hash<int>{}(s.second);
    return h1 ^ (h2 << 1);
  }
};

// Type alias for adjacency list
typedef unordered_map<pair<int, int>, vector<pair<int, int>>,
                      hash<pair<int, int>>>
    AdjList;

// Function to convert maze to adjacency list
AdjList convertToAdjList(const vector<vector<char>> &maze);

void printMaze(const vector<vector<char>> &maze);
void printAdjList(const AdjList &graph);

#endif // MAZE_H
