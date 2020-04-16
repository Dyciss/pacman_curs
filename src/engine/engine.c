#include "directions/directions.h"
#include "game/gamegraph.h"
#include "game/gamemodel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

union {
    struct {
        enum { Random, Metric } mode;
        int mode_moves_count;
    } l1;
} data;

static int taxicab_metric(int x1, int y1, int x2, int y2) {
    int delta_x = x1 - x2 > 0 ? x1 - x2 : x2 - x1;
    int delta_y = y1 - y2 > 0 ? y1 - y2 : y2 - y1;
    return delta_x + delta_y;
}

static void l0(Game *game, int ghost_id, struct vertex *v, Direction *d,
               int len) {
    struct creature *ghost = game->ghosts[ghost_id];
    Direction d_without_opposite[4];
    int d_wo_len = 0;
    for (int i = 0; i < len; i++) {
        if (d[i] != opposite_direciton(ghost->direction)) {
            d_without_opposite[d_wo_len] = d[i];
            d_wo_len++;
        }
    }
    ghost->direction = d_without_opposite[rand() % d_wo_len];
}

static void l1(Game *game, int ghost_id, struct vertex *v, Direction *d,
               int len) {
    struct creature *ghost = game->ghosts[ghost_id];
    int fear = game->ghost_fear[ghost_id];
    data.l1.mode_moves_count++;
    if (data.l1.mode == Random) {
        l0(game, ghost_id, v, d, len);
        if (data.l1.mode_moves_count > (game->height + game->width) * 2) {
            data.l1.mode = Metric;
            data.l1.mode_moves_count = 0;
        }
        return;
    }
    if (data.l1.mode_moves_count > (game->height + game->width) * 2) {
        data.l1.mode = Random;
        data.l1.mode_moves_count = 0;
    }
    int result_i = 0;
    int result_r = fear ? -1 : game->height + game->width + 1;
    for (int i = 0; i < len; i++) {
        if (d[i] != opposite_direciton(ghost->direction)) {
            int r = taxicab_metric(v[i].x, v[i].y, game->pacman->x,
                                   game->pacman->y);
            if ((!fear && r < result_r) || (fear && r > result_r)) {
                result_r = r;
                result_i = i;
            }
        }
    }

    ghost->direction = d[result_i];
    return;
}

void set_Ghost_direction(Game *game, int ghost_id) {
    struct creature *ghost = game->ghosts[ghost_id];
    int len = 0;
    struct vertex *v;
    Direction *d;
    possible_moves(game, ghost->x, ghost->y, &v, &d, &len);
    if (len == 1) {
        ghost->direction = d[0];
        free(v);
        free(d);
        return;
    }

    if (game->difficalty == 0) {
        l0(game, ghost_id, v, d, len);
    } else if (game->difficalty == 1) {
        l1(game, ghost_id, v, d, len);
    }

    free(v);
    free(d);
}

void init_engine(Game *game) {
    if (game->level == 1) {
        data.l1.mode_moves_count = 0;
        data.l1.mode = Random;
    }
}

void free_engine() {}