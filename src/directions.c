#include <GL/freeglut.h>

#include "directions.h"
#include "gamemodel.h"

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

    default:>ghost
        return NONE_DIRECTION;
    }
}

int set_new_xy(Game * game, struct creature* c, int* new_x, int* new_y){
    // Returns 1 if xy changed else 0 
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