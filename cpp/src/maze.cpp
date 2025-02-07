#include "maze.h"
#include <queue>
#include <unordered_set>
#include <vector>

using namespace std;

void Position::print() const {
  cout << "(" << getRow() << ", " << getCol() << ")";
}

bool operator==(const Position &lhs, const Position &rhs) {
  return lhs.getRow() == rhs.getRow() && lhs.getCol() == rhs.getCol();
}

// Function to check if a cell is within bounds and walkable
bool isValid(int row, int col, const vector<vector<char>> &maze) {
  return row >= 0 && row < maze.size() && col >= 0 && col < maze[0].size() &&
         (maze[row][col] == '_' || maze[row][col] == 'S' ||
          maze[row][col] == 'E');
}

void Maze::initialize(const vector<vector<char>> &maze) {
  graph_.clear();
  valid_ = false;

  static vector<pair<int, int>> directions = {
      {-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Up, Down, Left, Right

  auto handleCurrent = [this, &maze](int row, int col) {
    const auto node = Position{row, col};
    for (auto [dr, dc] : directions) {
      int newRow = row + dr, newCol = col + dc;
      if (isValid(newRow, newCol, maze)) {
        graph_[node].push_back({newRow, newCol});
      }
    }
  };

  for (int row = 0; row < maze.size(); row++) {
    for (int col = 0; col < maze[0].size(); col++) {
      switch (maze[row][col]) {
      case 'S': {
        start_ = Position{row, col};
        handleCurrent(row, col);
        break;
      }
      case 'E': {
        end_ = Position{row, col};
        handleCurrent(row, col);
        break;
      }
      case '_':
        handleCurrent(row, col);
        break;
      default:
        break;
      }
    }
  }

  if (start_.isInvalid() || end_.isInvalid())
    // Invalid
    return;

  valid_ = true;
}

void Utils::printInput(const vector<vector<char>> &maze) {
  cout << '-';
  for (int col = 0; col < maze[0].size(); col++) {
    cout << col;
  }
  cout << endl;
  for (int row = 0; row < maze.size(); row++) {
    cout << row;
    for (int col = 0; col < maze[0].size(); col++) {
      cout << maze[row][col];
    }
    cout << endl;
  }
}

void Maze::print() {
  cout << (valid_ ? "Valid" : "Invalid") << endl;
  cout << "Start:";
  start_.print();
  cout << ", End:";
  end_.print();
  cout << endl;
  for (const auto &[node, neighbors] : graph_) {
    node.print();
    cout << " -> ";
    for (const auto &neighbor : neighbors) {
      neighbor.print();
    }

    cout << endl;
  }
}

vector<Position> Maze::BFS() {
  if (!valid_)
    return {};

  queue<vector<Position>> queue;
  unordered_set<Position> visited;

  queue.push({start_});
  visited.insert(start_);

  while (!queue.empty()) {
    auto path = queue.front();
    queue.pop();

    auto current = path.back();

    if (current == end_) {
      return path;
    }

    for (const auto &neighbor : graph_.at(current)) {
      if (visited.find(neighbor) == visited.end()) {
        visited.insert(neighbor);
        auto newPath = path;
        newPath.push_back(neighbor);
        queue.push(newPath);
      }
    }
  }
  return {};
}
