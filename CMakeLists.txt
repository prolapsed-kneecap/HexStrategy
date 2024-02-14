cmake_minimum_required(VERSION 3.23)
project(HexStrategy)

set(CMAKE_CXX_STANDARD 14)

add_executable(HexStrategy main.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/SFML/include)
target_link_libraries(HexStrategy sfml-system sfml-window sfml-graphics sfml-audio)