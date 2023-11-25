cmake_minimum_required(VERSION 3.26)
project(HeatMaps)

# Pull in desired version of catch through cmake automatically, make it available
Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.1 # or a later release
)
FetchContent_MakeAvailable(Catch2)

set(CMAKE_CXX_STANDARD 17)

# Create a library target for the Heatmaps code to be used by both the main application and tests
add_library(heatmaps_lib
        src/heatmap.h
        src/heatmap.cpp)

# Executable for the main code
add_executable(Main src/main.cpp
        src/heatmap.h
        src/heatmap.cpp)

# Executable for the Catch2 testing code
add_executable(tests
        tests/tests.cpp)

# Link the test executable to the Catch2 testing library and the PageRank library
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain pagerank_lib)