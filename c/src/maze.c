#include "maze.h"
#include <malloc.h>
#include <stdio.h>

static const int directions[4][2] = {
    {-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Up, Down, Left, Right

Position *createPosition(int row, int col);
AdjList *createAdjList(void);
Maze *createMaze(void);

void freeAdjList(AdjList *list);

void printPosition(const Position *position);
void printAdjList(const AdjList *graph);
void printMaze(const Maze *maze);

AdjListNode *findNodeInAdjList(AdjList *graph, Position node_pos);
int addNodeToAdjList(AdjList *list, Position node_pos);
int addNeighborToNode(AdjList *list, Position node_pos, Position neighbor);
int isValid(const char **maze, int rows, int cols, Position pos);
void handleCurrent(AdjList *graph, const char **maze, int rows, int cols,
                   int row, int col);

void printPosition(const Position *position) {
  if (position != NULL) {
    printf("(%d, %d)", position->row, position->col);
  }
}

Position *createPosition(int row, int col) {
  Position *p = malloc(sizeof(Position *));
  if (p == NULL) {
    fprintf(stderr, "Failed to allocate Point.\n");
  }
  p->row = row;
  p->col = col;
  return p;
}

AdjList *createAdjList(void) {
  AdjList *list = malloc(sizeof(AdjList));
  if (list == NULL) {
    fprintf(stderr, "Failed to allocate AdjList.\n");
  }
  list->node_count = 0;
  list->nodes = NULL;
  return list;
}

Maze *createMaze(void) {
  Maze *maze = malloc(sizeof(Maze));
  if (maze == NULL) {
    fprintf(stderr, "Failed to allocate Maze.\n");
  }
  maze->graph = NULL;
  maze->start = NULL;
  maze->end = NULL;
  return maze;
}

AdjListNode *findNodeInAdjList(AdjList *graph, Position node_pos) {
  for (int i = 0; i < graph->node_count; i++) {
    if (graph->nodes[i].pos.row == node_pos.row &&
        graph->nodes[i].pos.col == node_pos.col) {
      return &graph->nodes[i];
    }
  }

  return NULL;
}

int addNodeToAdjList(AdjList *graph, Position node_pos) {
  if (graph == NULL) {
    fprintf(stderr, "AdjList pointer is NULL in addNodeToAdjList.\n");
    return -1;
  }

  if (NULL != findNodeInAdjList(graph, node_pos))
    return -1;

  AdjListNode *temp =
      realloc(graph->nodes, (graph->node_count + 1) * sizeof(AdjListNode));
  if (temp == NULL) {
    fprintf(stderr, "Memory reallocation failed in addNodeToAdjList.\n");
    return -1;
  }
  graph->nodes = temp;

  graph->nodes[graph->node_count].pos = node_pos;
  graph->nodes[graph->node_count].neighbors = NULL;
  graph->nodes[graph->node_count].neighbor_count = 0;

  graph->node_count++;
  return 0;
}

int addNeighborToNode(AdjList *graph, Position node_pos, Position neighbor) {
  if (graph == NULL) {
    fprintf(stderr, "AdjList pointer is NULL in addNeighborToNode.\n");
    return -1;
  }

  AdjListNode *node = findNodeInAdjList(graph, node_pos);
  Position *temp =
      realloc(node->neighbors, (node->neighbor_count + 1) * sizeof(Position));
  if (temp == NULL) {
    fprintf(stderr, "Memory reallocation failed in addNeighborToNode.\n");
    return -1;
  }
  node->neighbors = temp;
  node->neighbors[node->neighbor_count] = neighbor;
  node->neighbor_count++;
  return 0;

  fprintf(stderr,
          "Node with position (%d, %d) not found in addNeighborToNode.\n",
          node_pos.row, node_pos.col);
  return -1;
}

void freeAdjList(AdjList *list) {
  if (list == NULL) {
    return;
  }
  for (int i = 0; i < list->node_count; i++) {
    free(list->nodes[i].neighbors);
  }
  free(list->nodes);
  free(list);
}

void freeMaze(Maze *maze) {
  if (maze == NULL) {
    return;
  }
  freeAdjList(maze->graph);
  free(maze->start);
  free(maze->end);
}

int isValid(const char **input, int rows, int cols, Position pos) {
  return pos.row >= 0 && pos.row < rows && pos.col >= 0 && pos.col < cols &&
         (input[pos.row][pos.col] == '_' || input[pos.row][pos.col] == 'S' ||
          input[pos.row][pos.col] == 'E');
}

void handleCurrent(AdjList *graph, const char **input, int rows, int cols,
                   int row, int col) {
  const Position node = {row, col};
  for (int i = 0; i < 4; ++i) {
    int newRow = row + directions[i][0], newCol = col + directions[i][1];
    Position neighbor = {newRow, newCol};
    if (isValid(input, rows, cols, neighbor)) {
      addNodeToAdjList(graph, node);
      addNeighborToNode(graph, node, neighbor);
    }
  }
}

Maze *initMaze(const char **input, int rows, int cols) {
  Maze *maze = createMaze();
  maze->graph = createAdjList();
  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
      switch (input[row][col]) {
      case 'S': {
        maze->start = createPosition(row, col);
        handleCurrent(maze->graph, input, rows, cols, row, col);
        break;
      }
      case 'E': {
        maze->end = createPosition(row, col);
        handleCurrent(maze->graph, input, rows, cols, row, col);
        break;
      }
      case '_':
        handleCurrent(maze->graph, input, rows, cols, row, col);
        break;
      default:
        break;
      }
    }
  }

  if (maze->start == NULL || maze->end == NULL) {
    // Invalid
    freeMaze(maze);
    return NULL;
  }

  return maze;
}

void printInput(const char **maze, int rows, int cols) {
  printf("-");
  for (int col = 0; col < cols; col++) {
    printf("%d", col);
  }
  printf("\n");
  for (int row = 0; row < rows; row++) {
    printf("%d", row);
    for (int col = 0; col < cols; col++) {
      printf("%c", maze[row][col]);
    }
    printf("\n");
  }
}

void printMaze(const Maze *maze) {
  printf("Start: ");
  printPosition(maze->start);
  printf("\nEnd: ");
  printPosition(maze->end);
  printf("\nGraph:\n");
  printAdjList(maze->graph);
}
void printAdjList(const AdjList *list) {
  if (list == NULL) {
    return;
  }
  for (int i = 0; i < list->node_count; i++) {
    printPosition(&list->nodes[i].pos);
    printf(" -> ");
    for (int j = 0; j < list->nodes[i].neighbor_count; j++)
      printPosition(&list->nodes[i].neighbors[j]);
    printf("\n");
  }
  printf("\n");
}
// Path *BFS(const AdjList *graph, const Position *start, const Position
// *end);
