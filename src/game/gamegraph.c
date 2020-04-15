#include "directions/directions.h"
#include "game/gamemodel.h"
#include <stdlib.h>
#include "game/gamegraph.h"

static void push_if_possible(Game *game, struct vertex *v, Direction d, int x,
                             int y, int *i) {
    // used in possible_moves
    int new_x = 0;
    int new_y = 0;
    if (direction_to_new_xy(game, d, x, y, &new_x, &new_y) && game->field[new_x-1][new_y-1].object != Wall) {
        v[*i].x = new_x;
        v[*i].y = new_y;
        (*i)++;
    }
}

struct vertex *possible_moves(Game *game, int x, int y, int *len) {
    //
    // returns array of vetex with (0, 0) in the end and sets len
    // Ex. : [(1, 2), (2, 1), (1, 3), (3, 1), (0,0)]
    //

    // 4 is maximum
    struct vertex *v = (struct vertex *)calloc(5, sizeof(struct vertex));
    int i = 0;

    push_if_possible(game, v, RIGHT, x, y, &i);
    push_if_possible(game, v, LEFT, x, y, &i);
    push_if_possible(game, v, TOP, x, y, &i);
    push_if_possible(game, v, BOTTOM, x, y, &i);

    *len = i;
    return v;
}