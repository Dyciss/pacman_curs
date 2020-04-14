#include "game/gamemodel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void set_Ghost_direction(Game *game, int ghost_id) {
    struct creature *ghost = game->ghosts[ghost_id];
    ghost->direction = (rand()) % 4 + 1;
}
