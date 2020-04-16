#include "game/gamegraph.h"
#include "directions/directions.h"
#include "game/gamemodel.h"
#include <stdlib.h>

static void push_if_possible(Game *game, struct vertex *v, Direction *d,
                             Direction current_direction, int x, int y,
                             int *i) {
    // used in possible_moves
    int new_x = 0;
    int new_y = 0;
    if (direction_to_new_xy(game, current_direction, x, y, &new_x, &new_y) &&
        game->field[new_x - 1][new_y - 1].object != Wall) {
        v[*i].x = new_x;
        v[*i].y = new_y;
        d[*i] = current_direction;
        (*i)++;
    }
}

void possible_moves(Game *game, int x, int y, struct vertex **v, Direction **d,
                    int *len) {
    //
    // returns array of vetex, len
    // Ex. : (x,y) == (2,2)
    // Ex. : [(1, 2), (2, 1), (2, 3), (3, 2)] - v
    // Ex. : [LEFT, TOP, BOTTOM, RIGHT] - d
    // Ex. : 4 - len
    //

    // 4 is maximum
    *v = (struct vertex *)calloc(4, sizeof(struct vertex));
    *d = (Direction *)calloc(4, sizeof(Direction));
    int i = 0;

    push_if_possible(game, *v, *d, RIGHT, x, y, &i);
    push_if_possible(game, *v, *d, LEFT, x, y, &i);
    push_if_possible(game, *v, *d, TOP, x, y, &i);
    push_if_possible(game, *v, *d, BOTTOM, x, y, &i);

    *len = i;
}