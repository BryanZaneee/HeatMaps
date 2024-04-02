cmake_minimum_required(VERSION 3.14)

project(DSA_GNV_Traffic_Heat_Map)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
        matplotplusplus
        GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus.git
        GIT_TAG origin/master # Specify the version you want
)
FetchContent_MakeAvailable(matplotplusplus)


# Add the executable
add_executable(DSA_GNV_Traffic_Heat_Map
        main.cpp
        README.md
        cmake-build-debug/Traffic_Crashes.csv
        bryHashMap.h
        bryHashMap.cpp
        RBTree.cpp)

target_link_libraries(DSA_GNV_Traffic_Heat_Map PUBLIC matplot)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)