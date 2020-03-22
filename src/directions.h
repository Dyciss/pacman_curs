#pragma once

#include <GL/freeglut.h>

typedef enum Direction {
    NONE_DIRECTION = 0,
    TOP,
    BOTTOM,
    RIGHT,
    LEFT
} Direction;

Direction direction_from_special_key(int key);