#pragma once

#include <GL/freeglut.h>
#include "direction.h"
#include "gamemodel.h"

Direction direction_from_special_key(int key);

int set_new_xy(Game * game, struct creature* c, int* new_x, int* new_y);