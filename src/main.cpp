extern "C" {
#include "ui/windowsize.h"
#include "settings/settings.h"
}

#include "main.hpp"

#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "pages/about.hpp"
#include "pages/game.hpp"
#include "pages/menu.hpp"
#include "pages/page.hpp"
#include "pages/settings.hpp"
#include "pages/result.hpp"
#include "pages/score.hpp"

enum Program_state program_state;
Page *pages[PROGRAM_STATES_COUNT] = {nullptr};

void set_program_state(enum Program_state new_ps) {
    program_state = new_ps;
    glutPostRedisplay();
}

void exit_and_free(int exit_code) {
    settings_free();
    exit(exit_code);
}

void render() {
    set_window_size();
    if (program_state == Exit)
        exit_and_free(EXIT_SUCCESS);
    
    Page *p = pages[program_state];
    if (p != nullptr) p->render();
    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (state != GLUT_DOWN)
        return;

    float fx = 2 * x / (float)window_width() - 1.0;
    float fy = 1.0 - 2 * y / (float)window_height();
    Page *p = pages[program_state];
    if (p != nullptr) p->mouse(fx, fy);
}

void keyboard(unsigned char key, int x, int y) {
    Page *p = pages[program_state];
    if (p != nullptr) p->keyboard(key, x, y);
}

void keyboard_special(int key, int x, int y) {
    Page *p = pages[program_state];
    switch (key) {
    case GLUT_KEY_F5:
        glutFullScreenToggle();
        break;
    case GLUT_KEY_F12:
        exit_and_free(EXIT_SUCCESS);
        break;
    default:
        if (p != nullptr) p->keyboard_special(key, x, y);
        break;
    }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    glutCreateWindow("Pacman");
    glutFullScreen();
    srand(time(NULL));

    pages[Menu] = new MenuPage();
    pages[About] = new AboutPage();
    pages[Game_page] = new GamePage();
    pages[Settings] = new SettingsPage();
    pages[Result_page] = new ResultPage();
    pages[Score_page] = new ScorePage();

    settings_init();
    program_state = Menu;

    glutDisplayFunc(render);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboard_special);

    glutMainLoop();

    exit_and_free(EXIT_SUCCESS);
}
