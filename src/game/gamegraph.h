#include "game/gamemodel.h"

struct vertex {
    //
    // (0, 0) is like \0
    // x in range [1, width]
    // y in range [1, height]
    //
    int x;
    int y;
};

void possible_moves(Game *game, int x, int y, struct vertex **v, Direction **d,
                    int *len);