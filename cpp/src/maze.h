#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>

using namespace std;

class Position {
public:
  Position() : row_(-1), col_(-1) {}
  Position(int row, int col) : row_(row), col_(col) {}

  constexpr int getRow() const { return row_; }
  constexpr int getCol() const { return col_; }
  constexpr bool isInvalid() const { return row_ == -1 || col_ == -1; }

  void print() const;

private:
  int row_;
  int col_;
};

bool operator==(const Position &lhs, const Position &rhs);

// Custom specialization of std::hash can be injected in namespace std.
template <> struct std::hash<Position> {
  size_t operator()(const Position &p) const noexcept {
    size_t h1 = hash<int>{}(p.getRow());
    size_t h2 = hash<int>{}(p.getCol());
    return h1 ^ (h2 << 1);
  }
};

class Utils {
public:
  static void printInput(const vector<vector<char>> &maze);
};

class Maze {
public:
  typedef std::unordered_map<Position, std::vector<Position>> AdjList;

public:
  Maze() : valid_(false) {}

  void initialize(const vector<vector<char>> &maze);
  AdjList getGraph() { return valid_ ? graph_ : AdjList{}; };
  void print();

  vector<Position> BFS();

private:
  AdjList graph_;
  Position start_;
  Position end_;
  bool valid_;
};

#endif // MAZE_H
