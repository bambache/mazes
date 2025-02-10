cmake_minimum_required(VERSION 3.10)
project(MazeSolver)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add debug symbols in Debug mode
set(CMAKE_C_FLAGS_DEBUG "-g")

# Add optimizations in Release mode
set(CMAKE_C_FLAGS_RELEASE "-O3 -s")

# Enable testing
enable_testing()

# Add the main executable
add_executable(maze_solver src/main.c src/maze.c)

include(FetchContent)
FetchContent_Declare(
    cmocka
    GIT_REPOSITORY https://git.cryptomilk.org/projects/cmocka.git
    GIT_TAG cmocka-1.1.7
)
FetchContent_MakeAvailable(cmocka)

# Add test executable
add_executable(maze_tests src/tests/test_maze.c src/maze.c)
target_link_libraries(maze_tests cmocka)

# Register the tests
add_test(NAME MazeTests COMMAND maze_tests)
