#include "game/gamemodel.h"
#include "game/gamegraph.h"
#include "directions/directions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void set_Ghost_direction(Game *game, int ghost_id) {
    struct creature *ghost = game->ghosts[ghost_id];


    if (game->difficalty == 0) {
        int len = 0;
        struct vertex* v;
        Direction *d;
        possible_moves(game, ghost->x, ghost->y, &v, &d, &len);

        Direction dir = d[rand() % len];
        free(v);
        free(d);

        ghost->direction = dir;
    } else if (game->difficalty == 1) {
        int len = 0;
        struct vertex* v;
        Direction *d;
        possible_moves(game, ghost->x, ghost->y, &v, &d, &len);

        free(v);
        if (len == 1) {
            ghost->direction = d[0];
            free(d);
            return;
        }

        Direction d_without_opposite[4];
        int d_wo_len = 0;
        for (int i = 0; i < len; i++) {
            if (d[i] != opposite_direciton(ghost->direction)) {
                d_without_opposite[d_wo_len] = d[i];
                d_wo_len++;
            }
        }

        ghost->direction = d_without_opposite[rand() % d_wo_len];
        free(d);
    }
}
