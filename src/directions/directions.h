#pragma once

#include <GL/freeglut.h>
#include "direction.h"
#include "game/gamemodel.h"

Direction direction_from_special_key(int key);
Direction direction_from_key(unsigned char key);

int set_new_xy(Game * game, struct creature* c, int* new_x, int* new_y);