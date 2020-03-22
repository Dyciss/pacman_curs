#pragma once
#include "gamemodel.h"


void map_xy_to_window_xy(int alpha, int x, int y, float *xw, float *yw);
void sync_sizing_props(Game *game);
