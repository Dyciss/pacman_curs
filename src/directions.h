#pragma once

#include <GL/freeglut.h>
#include "gamemodel.h"


typedef enum Direction {
    NONE_DIRECTION = 0,
    TOP,
    BOTTOM,
    RIGHT,
    LEFT
} Direction;

Direction direction_from_special_key(int key);

int set_new_xy(Game * game, struct creature* c, int* new_x, int* new_y);