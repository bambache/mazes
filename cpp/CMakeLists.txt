cmake_minimum_required(VERSION 3.10)
project(MazeSolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add debug symbols in Debug mode
set(CMAKE_CXX_FLAGS_DEBUG "-g")

# Add optimizations in Release mode
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s")

# Enable testing
enable_testing()

# Add the main executable
add_executable(maze_solver src/main.cpp src/maze.cpp)

# GoogleTest setup
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# Add test executable
add_executable(maze_tests src/tests/test_maze.cpp src/maze.cpp)
target_link_libraries(maze_tests GTest::gtest_main)

# Register the tests
add_test(NAME MazeTests COMMAND maze_tests)
