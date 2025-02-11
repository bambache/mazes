#include "maze.h"
#include <malloc.h>
#include <memory.h>
#include <stdio.h>

static const int directions[4][2] = {
    {-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Up, Down, Left, Right

Position *createPosition(int row, int col);
AdjList *createGraph(void);
Maze *createMaze(void);
Path *createPath(void);

void freeGraph(AdjList *list);

void printPosition(const Position *position);
void printAdjList(const AdjList *graph);
void printMaze(const Maze *maze);

int addNodeToGraph(AdjList *list, Position node_pos);
int addNeighborToNode(AdjList *list, Position node_pos, Position neighbor);
int isValid(const char **maze, int rows, int cols, Position pos);
void handleCurrent(AdjList *graph, const char **maze, int rows, int cols,
                   int row, int col);

void printPosition(const Position *position) {
  if (position != NULL) {
    printf("(%d, %d)", position->row, position->col);
  }
}

int eqPosition(Position a, Position b) {
  return (a.row == b.row) && (a.col == b.col);
}

Path *createPath(void) {
  Path *p = malloc(sizeof(Path));
  if (!p)
    return NULL;
  p->size = 0;
  p->capacity = 10;
  p->positions = malloc(p->capacity * sizeof(Position));
  if (!p->positions) {
    free(p);
    return NULL;
  }
  return p;
}

void freePath(Path *p) {
  if (p) {
    free(p->positions);
    free(p);
  }
}

int pathAdd(Path *p, Position pos) {
  if (p->size >= p->capacity) {
    p->capacity *= 2;
    Position *new_positions =
        realloc(p->positions, p->capacity * sizeof(Position));
    if (!new_positions)
      return -1;
    p->positions = new_positions;
  }
  p->positions[p->size++] = pos;
  return 0;
}

int pathContains(const Path *path, Position pos) {
  for (int i = 0; i < path->size; i++) {
    if (eqPosition(path->positions[i], pos))
      return 1;
  }
  return 0;
}

Position pathLast(const Path *p) { return p->positions[p->size - 1]; }

Path *copyPath(const Path *p) {
  // Create a deep copy of a path.
  Path *new_path = malloc(sizeof(Path));
  if (!new_path)
    return NULL;
  new_path->size = p->size;
  new_path->capacity = p->capacity;
  new_path->positions = malloc(new_path->capacity * sizeof(Position));
  if (!new_path->positions) {
    free(new_path);
    return NULL;
  }
  memcpy(new_path->positions, p->positions, p->size * sizeof(Position));
  return new_path;
}
PathQueue *createQueue(void) {
  PathQueue *q = malloc(sizeof(PathQueue));
  if (!q)
    return NULL;
  q->capacity = 4;
  q->size = 0;
  q->front = 0;
  q->rear = 0;
  q->items = malloc(q->capacity * sizeof(Path *));
  if (!q->items) {
    free(q);
    return NULL;
  }
  return q;
}

void freeQueue(PathQueue *q) {
  // it assumes contained paths were freed before call
  if (q) {
    free(q->items);
    free(q);
  }
}

int emptyQueue(const PathQueue *q) { return q->size == 0; }

int enqueue(PathQueue *q, Path *p) {
  if (q->size >= q->capacity) {
    int new_capacity = q->capacity * 2;
    Path **new_items = malloc(new_capacity * sizeof(Path *));
    if (!new_items)
      return -1;
    for (int i = 0; i < q->size; i++) {
      new_items[i] = q->items[(q->front + i) % q->capacity];
    }
    free(q->items);
    q->items = new_items;
    q->capacity = new_capacity;
    q->front = 0;
    q->rear = q->size;
  }
  q->items[q->rear] = p;
  q->rear = (q->rear + 1) % q->capacity;
  q->size++;
  return 0;
}

Path *dequeue(PathQueue *q) {
  if (emptyQueue(q))
    return NULL;
  Path *p = q->items[q->front];
  q->front = (q->front + 1) % q->capacity;
  q->size--;
  return p;
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

AdjList *createGraph(void) {
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

AdjListNode *findNodeInGraph(AdjList *graph, Position node_pos) {
  for (int i = 0; i < graph->node_count; i++) {
    if (eqPosition(graph->nodes[i].pos, node_pos)) {
      return &graph->nodes[i];
    }
  }

  return NULL;
}

int addNodeToGraph(AdjList *graph, Position node_pos) {
  if (graph == NULL) {
    fprintf(stderr, "AdjList pointer is NULL in addNodeToAdjList.\n");
    return -1;
  }

  if (NULL != findNodeInGraph(graph, node_pos))
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

  AdjListNode *node = findNodeInGraph(graph, node_pos);
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

void freeGraph(AdjList *list) {
  if (list == NULL) {
    return;
  }
  for (int i = 0; i < list->node_count; i++) {
    free(list->nodes[i].neighbors);
  }
  free(list->nodes);
  free(list);
}

void freePosition(Position *p)
{
  if(p)
    free(p);
}

void freeMaze(Maze *maze) {
  if (maze == NULL) {
    return;
  }
  freeGraph(maze->graph);
  freePosition(maze->start);
  freePosition(maze->end);
  free(maze);
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
      addNodeToGraph(graph, node);
      addNeighborToNode(graph, node, neighbor);
    }
  }
}

Maze *initMaze(const char **input, int rows, int cols) {
  Maze *maze = createMaze();
  maze->graph = createGraph();
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

void printPath(const Path *p) {
  if (p == NULL)
    return;

  printf("Path:");
  for (int i = 0; i < p->size; i++)
    printPosition(&p->positions[i]);
  printf("\n");
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

Path *BFS(const Maze *maze) {
  if (maze == NULL)
    return NULL;

  PathQueue *queue = createQueue();
  if (!queue)
    return NULL;

  Path *visited = createPath();
  if (!visited) {
    freeQueue(queue);
    return NULL;
  }

  Path *init_path = createPath();
  if (!init_path) {
    freeQueue(queue);
    freePath(visited);
    return NULL;
  }

  pathAdd(init_path, *maze->start);
  enqueue(queue, init_path);
  pathAdd(visited, *maze->start);

  Path *result = NULL;

  while (!emptyQueue(queue)) {
    Path *cur_path = dequeue(queue);
    Position current = pathLast(cur_path);

    if (eqPosition(current, *maze->end)) {
      result = cur_path;
      break;
    }

    AdjListNode *node = findNodeInGraph(maze->graph, current);
    if (node != NULL) {
      for (int i = 0; i < node->neighbor_count; i++) {
        Position neighbor = node->neighbors[i];
        if (!pathContains(visited, neighbor)) {
          pathAdd(visited, neighbor);
          Path *new_path = copyPath(cur_path);
          if (!new_path)
            continue;
          pathAdd(new_path, neighbor);
          enqueue(queue, new_path);
        }
      }
    }
    freePath(cur_path);
  }

  // Free any remaining paths in the queue.
  while (!emptyQueue(queue)) {
    Path *p = dequeue(queue);
    freePath(p);
  }
  freeQueue(queue);
  freePath(visited);

  return result;
}
