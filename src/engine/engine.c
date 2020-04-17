#include "directions/directions.h"
#include "engine/queue.h"
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
    struct {
        Direction **table;
    } l2;
    struct {
        int **table;
    } l3;
} data;

static int taxicab_metric(int x1, int y1, int x2, int y2) {
    int delta_x = x1 - x2 > 0 ? x1 - x2 : x2 - x1;
    int delta_y = y1 - y2 > 0 ? y1 - y2 : y2 - y1;
    return delta_x + delta_y;
}

// static int euclid_metric (int x1, int y1, int x2, int y2) {
//     int delta_x = x1 - x2 > 0 ? x1 - x2 : x2 - x1;
//     int delta_y = y1 - y2 > 0 ? y1 - y2 : y2 - y1;
//     return delta_x*delta_x + delta_y*delta_y;
// }

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

    if (game->difficalty == 2) {
        int ghost_i = vertex2int(game, (struct vertex){ghost->x, ghost->y});
        int pacman_i =
            vertex2int(game, (struct vertex){game->pacman->x, game->pacman->y});
        ghost->direction = data.l2.table[ghost_i][pacman_i];
        return;
    }

    if (game->difficalty == 3) {

        Direction one_move = direction_between_points(game, ghost->x, ghost->y, game->pacman->x, game->pacman->y);
        if (one_move) {
            ghost->direction = one_move;
            return;
        }

        int ghost_type = ghost_id % 4;
        struct vertex target;
        if (ghost_type == 0) {
            target = (struct vertex) {(game->pacman->x + 1) % game->width, game->pacman->y};
        } else if (ghost_type == 1) {
            target = (struct vertex) {(game->pacman->x - 1 + game->width) % game->width,game->pacman->y};
        } else if (ghost_type == 2) {
            target = (struct vertex) {(game->pacman->x), (game->pacman->y + 1) % game->height};
        } else if (ghost_type == 3) {
            target = (struct vertex) {(game->pacman->x), (game->pacman->y - 1 + game->height) % game->height};
        }

        if (game->field[target.x][target.y].object == Wall){
            target = (struct vertex) {game->pacman->x, game->pacman->y};
        };
        
        int target_i =
            vertex2int(game, target);
        int ghost_i = vertex2int(game, (struct vertex){ghost->x, ghost->y});

        int next_vertex_i = data.l3.table[target_i][ghost_i];
        struct vertex next_vertex = int2vertex(game, next_vertex_i);
        ghost->direction = direction_between_points(game, ghost->x, ghost->y, next_vertex.x, next_vertex.y);
        return;
    }

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

#define FORALL_G_AND_P(game)                                                   \
    for (int ghost = 0; ghost < game->width * game->height; ghost++) {         \
        for (int pacman = 0; pacman < game->width * game->height; pacman++) {

#define END_FORALL                                                             \
    }                                                                          \
    }

static void l2_init(Game *game) {
    data.l2.table =
        (Direction **)malloc(sizeof(Direction *) * game->width * game->height);
    for (int i = 0; i < game->width * game->height; i++) {
        data.l2.table[i] =
            (Direction *)calloc(game->width * game->height, sizeof(Direction));
    }

    // radius here is optional
    int radius = 8;

    // parent index
    int *visited = (int *)malloc(game->width * game->height * sizeof(int));
    // set nothing was visited
    memset(visited, 0xff,
           game->width * game->height * sizeof(int)); // 0xfffff... is -1

    FORALL_G_AND_P(game)

    queue_int_t *qi_v = new_qi();
    queue_int_t *qi_r = new_qi();

    struct vertex ghost_v = int2vertex(game, ghost);
    struct vertex pacman_v = int2vertex(game, pacman);

    if (game->field[ghost_v.x][ghost_v.y].object == Wall ||
        game->field[pacman_v.x][pacman_v.y].object == Wall) {
        continue;
    }

    // the vertex that we want to be
    int min_distance = game->width + game->height + 1;
    struct vertex min_vertex = ghost_v;

    push_qi(qi_v, ghost);
    push_qi(qi_r, 0);

    visited[ghost] = ghost;
    while (qi_v->first->prev) {
        int current_vertex_i = pop_qi(&qi_v);
        struct vertex current_v = int2vertex(game, current_vertex_i);
        int current_r = pop_qi(&qi_r);
        int current_distance =
            taxicab_metric(current_v.x, current_v.y, pacman_v.x, pacman_v.y);

        // if distance in children vertexes can never be less then minimal
        // distance now
        if (current_distance - (radius - current_r) >= min_distance)
            break;

        // pacman is inside circle
        if (current_distance <= 1) {
            min_distance = 0;
            min_vertex.x = current_v.x;
            min_vertex.y = current_v.y;
            break;
        }

        // border
        if (current_r >= radius) {
            // here curren_distance < min_distance -- we checked it above:
            // current distance - (r - r) >= min_distance -- is false
            min_distance = current_distance;
            min_vertex.x = current_v.x;
            min_vertex.y = current_v.y;
        } else {
            int len = 0;
            struct vertex *vs;
            Direction *ds;
            possible_moves(game, current_v.x, current_v.y, &vs, &ds, &len);
            for (int i = 0; i < len; i++) {
                int v_int = vertex2int(game, vs[i]);
                if (visited[v_int] < 0) {
                    push_qi(qi_v, v_int);
                    push_qi(qi_r, current_r + 1);
                    visited[v_int] = current_vertex_i;
                }
            }
            free(vs);
            free(ds);
        }
    }
    // here we found min_vertex

    // backtracking
    int min_vertex_i = vertex2int(game, min_vertex);
    while (visited[min_vertex_i] != ghost) {
        min_vertex_i = visited[min_vertex_i];
    }

    min_vertex = int2vertex(game, min_vertex_i);
    data.l2.table[ghost][pacman] = direction_between_points(
        game, ghost_v.x, ghost_v.y, min_vertex.x, min_vertex.y);

    // remove visited, free queues
    memset(visited, 0xff, game->width * game->height * sizeof(int));
    free_qi(qi_r);
    free_qi(qi_v);
    END_FORALL
    free(visited);
}

static void l3_init(Game *game) {
    data.l3.table = (int **)malloc(sizeof(int *) * game->width * game->height);
    for (int i = 0; i < game->width * game->height; i++) {
        data.l3.table[i] =
            (int *)malloc(game->width * game->height * sizeof(int));
        memset(data.l3.table[i], 0xff,
               game->width * game->height * sizeof(int));
    }

    for (int pacman = 0; pacman < game->height * game->width; pacman++) {
        struct vertex pacman_v = int2vertex(game, pacman);

        if (game->field[pacman_v.x][pacman_v.y].object == Wall) {
            continue;
        }

        queue_int_t *qi_v = new_qi();
        push_qi(qi_v, pacman);

        data.l3.table[pacman][pacman] = pacman;
        while (qi_v->first->prev) {
            int current_vertex_i = pop_qi(&qi_v);
            struct vertex current_vertex = int2vertex(game, current_vertex_i);

            int len = 0;
            struct vertex *vs;
            Direction *ds;
            possible_moves(game, current_vertex.x, current_vertex.y, &vs, &ds,
                           &len);

            free(ds);
            for (int i = 0; i < len; i++) {
                int vs_i = vertex2int(game, vs[i]);
                // if visited
                if (data.l3.table[pacman][vs_i] >= 0)
                    continue;

                data.l3.table[pacman][vs_i] = current_vertex_i;
                push_qi(qi_v, vs_i);
            }

            free(vs);
        }

        free_qi(qi_v);
    }
}

void init_engine(Game *game) {
    if (game->difficalty == 1) {
        data.l1.mode_moves_count = 0;
        data.l1.mode = Random;
    } else if (game->difficalty == 2) {
        l2_init(game);
    } else if (game->difficalty == 3) {
        l3_init(game);
    }
}

void free_engine(Game *game) {
    if (game->difficalty == 2) {
        for (int i = 0; i < game->width * game->height; i++) {
            free(data.l2.table[i]);
        }
        free(data.l2.table);
    } else if (game->difficalty == 3) {
        for (int i = 0; i < game->width * game->height; i++) {
            free(data.l3.table[i]);
        }
        free(data.l3.table);
    }
}