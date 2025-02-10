#include "maze.h"
#include <vector>

using namespace std;

int main() {
  vector<vector<char>> input = {{'#', 'S', '#', '#', '#'},
                                {'#', '_', '_', '#', '#'},
                                {'#', '_', '#', '_', '#'},
                                {'#', '_', '_', '_', '#'},
                                {'#', '#', '#', 'E', '#'}};

  Utils::printInput(input);

  auto maze = Maze{};

  maze.initialize(input);
  maze.print();

  auto path = maze.BFS();

  if (!path.empty()) {
    cout << "Shortest path: ";
    for (const auto &p : path) {
      p.print();
    }
    cout << endl;
  } else {
    cout << "No path found!" << endl;
  }

  cout << sizeof(Maze) << endl;

  return 0;
}
