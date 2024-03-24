cmake_minimum_required(VERSION 3.8)
project(Tetris_Game)

set(CMAKE_CXX_STANDARD 23)
add_executable(Tetris_Game main.cpp)

set(CMAKE_BUILD_TYPE Debug)
file(GLOB LIBRARIES /Users/mohit.battu/SFML/lib/*.dylib)
target_link_libraries(Tetris_Game ${LIBRARIES})
include_directories(/Users/mohit.battu/SFML/include)


