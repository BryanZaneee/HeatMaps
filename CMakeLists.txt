cmake_minimum_required(VERSION 3.14)
project(DSA_GNV_Traffic_Heat_Map)
set(CMAKE_CXX_STANDARD 14)

# Fetch Matplot++ library from the specified Git tag
include(FetchContent)
FetchContent_Declare(
    matplotplusplus
    GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus.git
)
FetchContent_MakeAvailable(matplotplusplus)

# Specify the executable name
set(EXECUTABLE_NAME HeatMap)

# Add the executable
add_executable(${EXECUTABLE_NAME}
    src/main.cpp
    src/bryHashMap.cpp
    src/RBTree.cpp
)

# Assuming Traffic_Crashes.csv is in the source directory, copy it to the build directory
configure_file(${CMAKE_SOURCE_DIR}/Traffic_Crashes.csv ${CMAKE_BINARY_DIR}/Traffic_Crashes.csv COPYONLY)

# Link Matplot++ library with the executable
target_link_libraries(${EXECUTABLE_NAME} PUBLIC matplot)

# Generate a compile commands JSON file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
