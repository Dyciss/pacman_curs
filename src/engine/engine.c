#include "game/gamemodel.h"
#include "game/gamegraph.h"
#include "directions/directions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int taxicab_metric(int x1, int y1, int x2, int y2) {
    int delta_x = x1 - x2 > 0 ? x1 - x2 : x2 - x1;
    int delta_y = y1 - y2 > 0 ? y1 - y2 : y2 - y1;
    return delta_x + delta_y;
}

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
    } else if (game->difficalty == 2) {
        int len = 0;
        struct vertex* v;
        Direction *d;
        possible_moves(game, ghost->x, ghost->y, &v, &d, &len);

        if (len == 1) {
            ghost->direction = d[0];
            free(d);
            free(v);
            return;
        }

        int result_i = 0;
        int result_r = game->height + game->width + 1;

        for (int i = 0; i < len; i++) {
            if (d[i] != opposite_direciton(ghost->direction)) {
                int r = taxicab_metric(v[i].x, v[i].y, game->pacman->x, game->pacman->y);
                if (r < result_r) {
                    result_r = r;
                    result_i = i;
                }
            }
        }

        free(v);
        ghost->direction = d[result_i];
        free(d);
        return;
    }
}
