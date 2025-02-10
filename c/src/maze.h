#ifndef MAZE_H
#define MAZE_H

typedef struct {
  int row;
  int col;
} Position;

typedef struct {
  Position pos;
  Position *neighbors;
  int neighbor_count;
} AdjListNode;

typedef struct {
  AdjListNode *nodes;
  int node_count;
} AdjList;

typedef struct {
  AdjList *graph;
  Position *start;
  Position *end;
} Maze;

typedef struct {
  Position *positions;
  int position_count;
} Path;

void freeMaze(Maze *maze);
Maze *initMaze(const char **input, int rows, int cols);

void printInput(const char **input, int rows, int cols);
void printMaze(const Maze *maze);

Path *BFS(const Maze *maze);

#endif // MAZE_H
