cmake_minimum_required(VERSION 3.26)
project(HeatMaps)

set(CMAKE_CXX_STANDARD 17)

add_executable(HeatMaps main.cpp
        heatmap.h
        heatmap.cpp)
