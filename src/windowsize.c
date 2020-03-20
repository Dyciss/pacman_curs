#include <GL/freeglut.h>

// We need it because glutGet isnt fast
int window_h = 0;
int window_w = 0;

int window_height() {
    return window_h;
}

int window_width() {
    return window_w;
}

void set_window_size() {
    window_w = glutGet(GLUT_WINDOW_WIDTH);
    window_h = glutGet(GLUT_WINDOW_HEIGHT);
}