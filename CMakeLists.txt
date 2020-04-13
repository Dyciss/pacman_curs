cmake_minimum_required(VERSION 2.8)

project(pacman)

include_directories(src)
add_executable(pacman ./src/main.c 
    ./src/pages/menu.c ./src/pages/about.c ./src/pages/game.c ./src/pages/settings.c
    ./src/ui/render.c  ./src/ui/windowsize.c ./src/ui/gameui.c ./src/ui/gamesizing.c
    ./src/settings/settings.c
    ./src/engine/engine.c
    ./src/directions.c ./src/gamemodel.c)


set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_C_FLAGS " -Werror -Wall -Wextra -Wpedantic -Wno-unused-parameter")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})

target_link_libraries(pacman ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})