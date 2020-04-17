#include "game/gamemodel.h"

struct vertex {
    //
    // x in range [0, width)
    // y in range [0, height)
    //
    int x;
    int y;
};

void possible_moves(Game *game, int x, int y, struct vertex **v, Direction **d,
                    int *len);