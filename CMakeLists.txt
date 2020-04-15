cmake_minimum_required(VERSION 3.12)

# Project
project(Sudoku VERSION 1.0 LANGUAGES CXX)

# Base directory relative includes
include_directories(.)

# Flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto -march=native -mpopcnt")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Add the executable
add_executable(
    main
    src/main.cpp
    src/board.cpp
    src/boards/standard.cpp
)

# Add the test executable
add_executable(
    tests
    src/board.cpp
    src/boards/standard.cpp
    tests/main.cpp
    tests/test_checker.cpp
    tests/test_solver.cpp
)

set_property(TARGET tests PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE FALSE)
