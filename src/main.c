#include <stdio.h>
#include <stdlib.h>

#include <GL/freeglut.h>

#include "main.h"
#include "about.h"
#include "menu.h"
#include "game.h"

enum Program_state program_state;

void set_program_state(enum Program_state new_ps) {
    program_state = new_ps;
    glutPostRedisplay();
}

void exit_and_free(int exit_code) {
    free_Menu();
    free_About();
    free_Game();
    exit(exit_code);
}

void render() {
    switch (program_state) {
    case Menu:
        render_Menu();
        break;
    
    case About:
        render_About();
        break;

    case Exit:
        exit_and_free(EXIT_SUCCESS);
        break;
    
    case Game:
        render_Game();
        break;
    
    default:
        exit_and_free(EXIT_FAILURE); // we cannot be here
        break;

    }
    glutSwapBuffers();
    //glFlush();
}

void mouse(int button, int state, int x, int y) {
    float fx = 2 * x / (float) glutGet(GLUT_WINDOW_WIDTH) - 1.0;
    float fy = 1.0 - 2 * y / (float) glutGet(GLUT_WINDOW_HEIGHT);
    
    if (state != GLUT_DOWN) return;
    switch (program_state) {
    case Menu:
        mouse_Menu(fx, fy);
        break;

    case About:
        mouse_About(fx, fy);
    
    case Game:
        mouse_Game(fx, fy);

    default:
        break;
    }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    
	glutCreateWindow("Pacman");
    glutFullScreen();
    
    init_Menu();
    init_About();
    init_Game();

    program_state = Menu;

    glutDisplayFunc(render);
    //glutTimerFunc(1000.0 / 20.0, render, 1);
    glutMouseFunc(mouse);

    glutMainLoop();

    exit_and_free(EXIT_SUCCESS);
}
