#include "directions/directions.h"
#include "engine/queue.h"
#include "game/gamegraph.h"
#include "game/gamemodel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct {
    enum { Random, Metric } mode;
    int mode_moves_count;
    int **table;
} data;

typedef int (*Metric_func)(Game *game, int x1, int y1, int x2, int y2);

static int taxicab_metric(Game *game, int x1, int y1, int x2, int y2) {
    int delta_x = x1 - x2 > 0 ? x1 - x2 : x2 - x1;
    int delta_y = y1 - y2 > 0 ? y1 - y2 : y2 - y1;
    return delta_x + delta_y;
}

static int table_metric(Game *game, int x1, int y1, int x2, int y2) {
    int vertex1_i = vertex2int(game, (struct vertex){x1, y1});
    int vertex2_i = vertex2int(game, (struct vertex){x2, y2});
    return data.table[vertex1_i][vertex2_i];
}

#define CAN_BE_OPPOSITE (1)
#define CAN_BE_GHOST (1<<1)
#define CLEVER_FEAR (1<<2)
static void set_direction_by_metric(Game *game, int ghost_id, Metric_func rho,
                                    int target_x, int target_y, int options) {
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

    int fear = game->ghost_fear[ghost_id];
    if (options & CLEVER_FEAR &&
        game->fear_moves_now <
            rho(game, ghost->x, ghost->y, game->pacman->x, game->pacman->y)) {
        fear = 0;
    }
    int result_i = 0;
    int result_r = fear ? -1 : game->height + game->width + 1;
    for (int i = 0; i < len; i++) {
        int filter_opposite = options & CAN_BE_OPPOSITE ||
                              d[i] != opposite_direciton(ghost->direction);
        int filter_ghost = options & CAN_BE_GHOST ||
                           game->field[v[i].x][v[i].y].object != Ghost;
        if (filter_opposite && filter_ghost) {
            int r = rho(game, v[i].x, v[i].y, game->pacman->x, game->pacman->y);
            if ((!fear && r < result_r) || (fear && r > result_r)) {
                result_r = r;
                result_i = i;
            }
        }
    }

    ghost->direction = d[result_i];
    free(v);
    free(d);
}

static void l0(Game *game, int ghost_id) {
    struct creature *ghost = game->ghosts[ghost_id];
    int len = 0;
    Direction *d;
    possible_moves(game, ghost->x, ghost->y, NULL, &d, &len);
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

static void l1(Game *game, int ghost_id) {
    data.mode_moves_count++;

    if (data.mode == Random) {
        l0(game, ghost_id);
        if (data.mode_moves_count > (game->height + game->width) * 2) {
            data.mode = Metric;
            data.mode_moves_count = 0;
        }
    } else if (data.mode == Metric) {
        set_direction_by_metric(game, ghost_id, taxicab_metric, game->pacman->x,
                                game->pacman->y, CAN_BE_GHOST);
        if (data.mode_moves_count > (game->height + game->width) * 2) {
            data.mode = Random;
            data.mode_moves_count = 0;
        }
    }
}

static void l2(Game *game, int ghost_id) {
    data.mode_moves_count++;

    if (data.mode == Random) {
        l0(game, ghost_id);
        if (data.mode_moves_count > (game->height + game->width) * 2) {
            data.mode = Metric;
            data.mode_moves_count = 0;
        }
    } else if (data.mode == Metric) {
        set_direction_by_metric(game, ghost_id, table_metric, game->pacman->x,
                                game->pacman->y, CAN_BE_GHOST);
        if (data.mode_moves_count > (game->height + game->width) * 2) {
            data.mode = Random;
            data.mode_moves_count = 0;
        }
    }
}

static void l3(Game *game, int ghost_id) {
    data.mode_moves_count++;

    if (data.mode == Random) {
        l0(game, ghost_id);
        if (data.mode_moves_count > (game->height + game->width) * 2) {
            data.mode = Metric;
            data.mode_moves_count = 0;
        }
    } else if (data.mode == Metric) {
        set_direction_by_metric(game, ghost_id, table_metric, game->pacman->x,
                                game->pacman->y,
                                CAN_BE_OPPOSITE | CLEVER_FEAR);
        if (data.mode_moves_count > (game->height + game->width) * 4) {
            data.mode = Random;
            data.mode_moves_count = 0;
        }
    }
}

void set_Ghost_direction(Game *game, int ghost_id) {
    switch (game->difficalty) {
    case 0:
        l0(game, ghost_id);
        break;
    case 1:
        l1(game, ghost_id);
        break;
    case 2:
        l2(game, ghost_id);
        break;
    case 3:
        l3(game, ghost_id);
        break;

    default:
        break;
    }
}

static void init_table(Game *game) {
    data.table = (int **)malloc(sizeof(int *) * game->width * game->height);
    for (int i = 0; i < game->width * game->height; i++) {
        data.table[i] = (int *)malloc(game->width * game->height * sizeof(int));
        memset(data.table[i], 0xff,
               game->width * game->height * sizeof(int)); // -1
    }

    for (int pacman = 0; pacman < game->height * game->width; pacman++) {
        struct vertex pacman_v = int2vertex(game, pacman);

        if (game->field[pacman_v.x][pacman_v.y].object == Wall) {
            continue;
        }

        queue_int_t *qi_v = new_qi();
        push_qi(qi_v, pacman);

        // data.l3.table[pacman][pacman] = pacman;
        data.table[pacman][pacman] = 0;
        while (qi_v->first->prev) {
            int current_vertex_i = pop_qi(&qi_v);
            struct vertex current_vertex = int2vertex(game, current_vertex_i);

            int len = 0;
            struct vertex *vs;
            possible_moves(game, current_vertex.x, current_vertex.y, &vs, NULL,
                           &len);

            for (int i = 0; i < len; i++) {
                int vs_i = vertex2int(game, vs[i]);
                // if visited
                if (data.table[pacman][vs_i] >= 0)
                    continue;

                // data.l3.table[pacman][vs_i] = current_vertex_i;
                data.table[pacman][vs_i] =
                    data.table[pacman][current_vertex_i] + 1;
                push_qi(qi_v, vs_i);
            }

            free(vs);
        }

        free_qi(qi_v);
    }
}

void init_engine(Game *game) {
    data.mode_moves_count = 0;
    data.mode = Random;
    if (game->difficalty >= 2) {
        init_table(game);
    }
}

static void free_table(Game *game) {
    if (!data.table)
        return;
    for (int i = 0; i < game->width * game->height; i++) {
        if (!data.table[i])
            continue;
        free(data.table[i]);
    }
    free(data.table);
    data.table = NULL;
}

void free_engine(Game *game) {
    if (game->difficalty >= 2) {
        free_table(game);
    }
}