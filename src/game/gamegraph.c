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
        game->field[new_x][new_y].object != Wall) {
        if (v) {
            v[*i].x = new_x;
            v[*i].y = new_y;
        }
        if (d) {
            d[*i] = current_direction;
        }
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
    // if **v == NULL it wont be set
    // if **d == NULL it wont be set
    //

    // 4 is maximum
    if (v)
        *v = (struct vertex *)calloc(4, sizeof(struct vertex));
    if (d)
        *d = (Direction *)calloc(4, sizeof(Direction));
    int i = 0;

    push_if_possible(game, v ? *v : NULL, d ? *d: NULL, RIGHT, x, y, &i);
    push_if_possible(game, v ? *v : NULL, d ? *d: NULL, LEFT, x, y, &i);
    push_if_possible(game, v ? *v : NULL, d ? *d: NULL, TOP, x, y, &i);
    push_if_possible(game, v ? *v : NULL, d ? *d: NULL, BOTTOM, x, y, &i);

    *len = i;
}

int vertex2int(Game *game, struct vertex v) { return v.x * game->height + v.y; }

struct vertex int2vertex(Game *game, int i) {
    return (struct vertex){i / game->height, i % game->height};
}