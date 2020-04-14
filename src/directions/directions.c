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

Direction direction_from_key(unsigned char key){
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
    // Set new xy to next creature position (after move along creature direction)
    // Returns 1 if xy changed else 0
    //
    *new_x = c->x;
    *new_y = c->y;

    switch (c->direction) {
    case TOP:
        *new_y = (game->height + c->y - 1 - 1) % game->height + 1;
        return 1;
    case BOTTOM:
        *new_y = (game->height + c->y - 1 + 1) % game->height + 1;
        return 1;
    case LEFT:
        *new_x = (game->width + c->x - 1 - 1) % game->width + 1;
        return 1;
    case RIGHT:
        *new_x = (game->width + c->x - 1 + 1) % game->width + 1;
        return 1;
    default:
        return 0;
    }
}