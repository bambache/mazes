#include "../maze.h"
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <cmocka.h>
#include <string.h>

static void test_maze_conversion(void **state) {
  (void)state; // Unused

  const char *input[] = {"###", "#S#", "#E#"};

  int rows = sizeof(input) / sizeof(input[0]);
  int cols = strlen(input[0]);

  Maze *maze = initMaze(input, rows, cols);
  assert_int_equal(maze->graph->node_count, 2);

  // // Verify node at (1,1)
  // AdjacencyListNode *node_1_1 = find_node_in_graph(&graph, 1, 1);
  // assert_non_null(node_1_1);
  // assert_int_equal(node_1_1->neighbor_count, 1);
  // assert_int_equal(node_1_1->neighbors[0].row, 2);
  // assert_int_equal(node_1_1->neighbors[0].col, 1);
  //
  // // Verify node at (2,1)
  // AdjacencyListNode *node_2_1 = find_node_in_graph(&graph, 2, 1);
  // assert_non_null(node_2_1);
  // assert_int_equal(node_2_1->neighbor_count, 1);
  // assert_int_equal(node_2_1->neighbors[0].row, 1);
  // assert_int_equal(node_2_1->neighbors[0].col, 1);

  // Clean up allocated memory.
  freeMaze(maze);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_maze_conversion),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
