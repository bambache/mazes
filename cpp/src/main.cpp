#include "maze.h"
#include <vector>

using namespace std;

int main() {
  vector<vector<char>> maze = {{'#', '#', '#', '#', '#'},
                               {'#', '_', '_', '#', '#'},
                               {'#', '_', '#', '_', '#'},
                               {'#', '_', '_', '_', '#'},
                               {'#', '#', '#', '#', '#'}};

  printMaze(maze);

  AdjList graph = convertToAdjList(maze);
  printAdjList(graph);
  return 0;
}
