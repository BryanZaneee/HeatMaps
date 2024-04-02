cmake_minimum_required(VERSION 3.14)
project(DSA_GNV_Traffic_Heat_Map)
set(CMAKE_CXX_STANDARD 14)

# Fetch Matplot++ library from the specified Git tag
include(FetchContent)
FetchContent_Declare(
    matplotplusplus
    GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus.git
    # Replace with a specific tag or commit hash
)
FetchContent_MakeAvailable(matplotplusplus)

# Add the executable
add_executable(DSA_GNV_Traffic_Heat_Map
    main.cpp
    bryHashMap.cpp
    RBTree.cpp
)

# Copy non-source files to the build directory
configure_file(build/Traffic_Crashes.csv Traffic_Crashes.csv COPYONLY)

# Link Matplot++ library with the executable
target_link_libraries(DSA_GNV_Traffic_Heat_Map PUBLIC matplot)

# Generate a compile commands JSON file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
