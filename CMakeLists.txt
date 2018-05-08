cmake_minimum_required(VERSION 3.8)

project(battleship-game)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib/ src/)

add_executable(hello_world test/hello_world.cc)

add_executable(test_main test/test.cc test/test_timer.h)

add_executable(test_serialization test/test_serialization.cc)
