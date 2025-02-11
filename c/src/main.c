#include "maze.h"
#include <string.h>

int main(void) {
  const char *input[] = {
    "#S###",
    "#__##",
    "#_#_#",
    "#___#",
    "###E#"
  };

  int rows = sizeof(input) / sizeof(input[0]);
  int cols = strlen(input[0]);

  printInput(input, rows, cols);

  Maze *maze = initMaze(input, rows, cols);
  printMaze(maze);

  Path *path = BFS(maze);
  printPath(path);

  freePath(path);
  freeMaze(maze);

  return 0;
}
