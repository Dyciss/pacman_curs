#include <GL/freeglut.h>

#include "directions.h"

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