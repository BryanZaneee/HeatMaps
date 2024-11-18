cmake_minimum_required(VERSION 3.14)
project(DSA_GNV_Traffic_Heat_Map)
set(CMAKE_CXX_STANDARD 14)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(CURL REQUIRED)

set(EXECUTABLE_NAME HeatMap)

add_executable(${EXECUTABLE_NAME}
    src/main.cpp
    src/bryHashMap.cpp
    src/RBTree.cpp
)

target_link_libraries(${EXECUTABLE_NAME} 
    sfml-graphics 
    sfml-window 
    sfml-system
    ${CURL_LIBRARIES}
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CURL_INCLUDE_DIRS})
configure_file(${CMAKE_SOURCE_DIR}/Traffic_Crashes.csv ${CMAKE_BINARY_DIR}/Traffic_Crashes.csv COPYONLY)