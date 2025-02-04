#include "../maze.h"
#include <gtest/gtest.h>

using namespace std;

TEST(MazeTest, AdjacencyListConversion) {
  vector<vector<char>> maze = {
      {'#', '#', '#'}, {'#', '_', '#'}, {'#', '_', '#'}};

  auto adjList = convertToAdjList(maze);

  EXPECT_EQ(adjList.size(), 2);       // Should have two open nodes
  EXPECT_TRUE(adjList.count({1, 1})); // Node (1,1) exists
  EXPECT_TRUE(adjList.count({2, 1})); // Node (2,1) exists
  const auto value = adjList[{1, 1}];
  const auto expected = vector<pair<int, int>>{{2, 1}};
  EXPECT_EQ(value, expected);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
