#include "game/gamemodel.h"
#include "game/gamegraph.h"
#include "directions/directions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void set_Ghost_direction(Game *game, int ghost_id) {
    struct creature *ghost = game->ghosts[ghost_id];
    int len = 0;
    struct vertex* v = possible_moves(game, ghost->x, ghost->y, &len);

    struct vertex target = v[rand() % len];
    free(v);
    ghost->direction = direction_between_points(game, ghost->x, ghost->y, target.x, target.y);
}
