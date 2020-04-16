#include <GL/freeglut.h>

#include "directions/directions.h"
#include "game/gamemodel.h"

Direction direction_from_special_key(int key) {
    //
    // returns 0 (NONE_DIRECTION) if no direction with such key
    //
    switch (key) {
    case GLUT_KEY_UP:
        return TOP;
    case GLUT_KEY_DOWN:
        return BOTTOM;
    case GLUT_KEY_RIGHT:
        return RIGHT;
    case GLUT_KEY_LEFT:
        return LEFT;

    default:
        return NONE_DIRECTION;
    }
}

Direction direction_from_key(unsigned char key) {
    switch (key) {
    case 'w':
        return TOP;
    case 'a':
        return LEFT;
    case 's':
        return BOTTOM;
    case 'd':
        return RIGHT;

    default:
        return NONE_DIRECTION;
    }
}

int set_new_xy(Game *game, struct creature *c, int *new_x, int *new_y) {
    //
    // Set new xy to next creature position (after move along creature
    // direction) Returns 1 if xy changed else 0
    //
    return direction_to_new_xy(game, c->direction, c->x, c->y, new_x, new_y);
}

Direction direction_between_points(Game *game, int x1, int y1, int x2, int y2) {
    //
    // x, y in range [1; width/height]
    // returns None direciton in case there is no single move: (x1, y1) -> (x2,
    // y2)
    //
    if (y1 == y2) {
        if ((game->width + x1 - 1 - 1) % game->width + 1 == x2)
            return LEFT;
        if ((game->width + x1 - 1 + 1) % game->width + 1 == x2)
            return RIGHT;
        return NONE_DIRECTION;
    } else if (x1 == x2) {
        if ((game->height + y1 - 1 + 1) % game->height + 1 == y2)
            return BOTTOM;
        if ((game->height + y1 - 1 - 1) % game->height + 1 == y2)
            return TOP;
        return NONE_DIRECTION;
    }

    return NONE_DIRECTION;
}

int direction_to_new_xy(Game *game, Direction d, int x, int y, int *new_x,
                        int *new_y) {
    // returns 1 if d != None direction else 0

    *new_x = x;
    *new_y = y;

    switch (d) {
    case TOP:
        *new_y = (game->height + y - 1 - 1) % game->height + 1;
        return 1;
    case BOTTOM:
        *new_y = (game->height + y - 1 + 1) % game->height + 1;
        return 1;
    case LEFT:
        *new_x = (game->width + x - 1 - 1) % game->width + 1;
        return 1;
    case RIGHT:
        *new_x = (game->width + x - 1 + 1) % game->width + 1;
        return 1;
    default:
        return 0;
    }
}

Direction opposite_direciton(Direction d) {
    switch (d)
    {
    case LEFT:
        return RIGHT;
    case RIGHT:
        return LEFT;
    case TOP:
        return BOTTOM;
    case BOTTOM:
        return TOP;
    default:
        return NONE_DIRECTION;
    }
}