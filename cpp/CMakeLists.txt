cmake_minimum_required(VERSION 3.10)
project(MazeSolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Create executable
add_executable(maze_solver src/main.cpp)

