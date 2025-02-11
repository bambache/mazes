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
  int size;
  int capacity;
} Path;

typedef struct {
  Path **items;
  int front;
  int rear;
  int size;
  int capacity;
} PathQueue;

void freePath(Path *p);
void freeMaze(Maze *maze);
Maze *initMaze(const char **input, int rows, int cols);

AdjListNode *findNodeInGraph(AdjList *graph, Position node_pos);

void printInput(const char **input, int rows, int cols);
void printMaze(const Maze *maze);
void printPath(const Path *p);

Path *BFS(const Maze *maze);

#endif // MAZE_H
