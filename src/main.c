#include "main.h"

#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>

#include "./ui/windowsize.h"
#include "pages/about.h"
#include "pages/game.h"
#include "pages/menu.h"
#include "pages/page.h"
#include "pages/settings.h"
#include "settings/settings.h"

enum Program_state program_state;
Page pages[PROGRAM_STATES_COUNT] = {NULL};

void set_program_state(enum Program_state new_ps) {
    program_state = new_ps;
    glutPostRedisplay();
}

void exit_and_free(int exit_code) {
    for (int i = 0; i < PROGRAM_STATES_COUNT; i++) {
        Page p = pages[i];
        if (p.free_Page != NULL)
            p.free_Page();
    }
    settings_free();
    exit(exit_code);
}

void render() {
    set_window_size();
    if (program_state == Exit)
        exit_and_free(EXIT_SUCCESS);
    Page p = pages[program_state];
    if (p.render == NULL)
        exit_and_free(EXIT_FAILURE);
    p.render();
    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (state != GLUT_DOWN)
        return;

    float fx = 2 * x / (float)window_width() - 1.0;
    float fy = 1.0 - 2 * y / (float)window_height();
    Page p = pages[program_state];
    if (p.mouse != NULL)
        p.mouse(fx, fy);
}

void keyboard(unsigned char key, int x, int y) {
    Page p = pages[program_state];
    if (p.keyboard != NULL)
        p.keyboard(key, x, y);
}

void keyboard_special(int key, int x, int y) {
    Page p = pages[program_state];
    switch (key) {
    case GLUT_KEY_F5:
        glutFullScreenToggle();
        break;
    case GLUT_KEY_F12:
        exit_and_free(EXIT_SUCCESS);
        break;
    default:
        if (p.keyboard_special != NULL)
            p.keyboard_special(key, x, y);
        break;
    }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    glutCreateWindow("Pacman");
    glutFullScreen(); // now we can:)

    pages[Menu] = menu_Page();
    pages[About] = about_Page();
    pages[Game_page] = game_Page();
    pages[Settings] = settings_Page();

    settings_init();
    for (int i = 0; i < PROGRAM_STATES_COUNT; i++) {
        Page p = pages[i];
        if (p.init_Page != NULL)
            p.init_Page();
    }

    program_state = Menu;

    glutDisplayFunc(render);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboard_special);

    glutMainLoop();

    exit_and_free(EXIT_SUCCESS);
}
