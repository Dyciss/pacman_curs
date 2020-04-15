#pragma once

#include "direction.h"
#include "game/gamemodel.h"
#include <GL/freeglut.h>

Direction direction_from_special_key(int key);
Direction direction_from_key(unsigned char key);
Direction direction_between_points(Game *game, int x1, int y1, int x2, int y2);
int direction_to_new_xy(Game *game, Direction d, int x, int y, int *new_x,
                        int *new_y);

int set_new_xy(Game *game, struct creature *c, int *new_x, int *new_y);