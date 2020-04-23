cmake_minimum_required(VERSION 2.8)

project(pacman)

add_executable(pacman ./src/main.c 
    ./src/pages/menu.c ./src/pages/about.c ./src/pages/game.c ./src/pages/settings.c ./src/pages/result.c ./src/pages/score.c
    ./src/ui/render.c  ./src/ui/windowsize.c ./src/ui/gameui.c ./src/ui/gamesizing.c
    ./src/settings/settings.c
    ./src/engine/engine.c ./src/engine/queue.c
    ./src/directions/directions.c 
    ./src/game/gamemodel.c ./src/game/gamegraph.c
    ./src/score/score.c)

set(FREEGLUT_BUILD_SHARED_LIBS OFF CACHE BOOL "freeglut shared")
set(FREEGLUT_BUILD_DEMOS OFF CACHE BOOL "freeglut demos")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/FreeGLUT/freeglut/freeglut EXCLUDE_FROM_ALL)

target_include_directories(pacman PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if(NOT MSVC)
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -Wpedantic -Wno-unused-parameter -Og -ggdb -g3")
endif()

find_package(OpenGL REQUIRED)
target_include_directories(pacman PRIVATE ${OPENGL_INCLUDE_DIRS})

target_link_libraries(pacman ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
target_link_libraries(pacman freeglut_static)

install(TARGETS pacman DESTINATION bin)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/canonical DESTINATION bin/data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/.pacman-settings DESTINATION bin)

set(CPACK_PACKAGE_NAME "Pacman")
set(CPACK_PACKAGE_DESCRIPTION "Coursework. Pacman.")

set(CPACK_NSIS_MUI_ICON ${CMAKE_CURRENT_SOURCE_DIR}/data/icon.ico)

include(CPack)