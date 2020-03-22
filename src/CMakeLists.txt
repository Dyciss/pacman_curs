cmake_minimum_required(VERSION 2.8)

project(pacman)

add_executable(pacman main.c menu.c about.c render.c game.c windowsize.c directions.c gamemodel.c gameui.c engine.c)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_C_FLAGS "-pedantic -Werror")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})

target_link_libraries(pacman ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})