#pragma once
#include "gamemodel.h"

Game *new_Game();
void rebirth(Game *game);
void set_Ghost_direction(Game *game, int ghost_id);