cmake_minimum_required(VERSION 3.8)

project(battleship-game)

set(CMAKE_CXX_STANDARD 11)

include_directories(lib/ src/)

## OpendGL related libraries ##

link_directories(/usr/local/lib)

find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})

find_package(GLUT REQUIRED)

include_directories(${GLUT_INCLUDE_DIR})

########################################

add_executable(hello_world test/hello_world.cc)

add_executable(test_main test/test.cc test/test_timer.h)

add_executable(test_serialization test/test_serialization.cc)

add_executable(test_graphic test/test_graphic.cc)

add_executable(client src/main/client_main.cc src/client src/core src/utils src/graphic)


target_link_libraries(test_graphic ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
