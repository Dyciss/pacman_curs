cmake_minimum_required(VERSION 2.8)

project(pacman)

include_directories(.)
add_executable(pacman main.c 
    ./pages/menu.c ./pages/about.c ./pages/game.c 
    ./ui/render.c  ./ui/windowsize.c ./ui/gameui.c ./ui/gamesizing.c
    ./engine/engine.c
    directions.c gamemodel.c)


set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_C_FLAGS " -Werror -Wall -Wextra -Wpedantic -Wno-unused-parameter")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})

target_link_libraries(pacman ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})