#include <stdio.h>
#include <stdlib.h>

#include <GL/freeglut.h>

#include "main.h"
#include "about.h"
#include "menu.h"
#include "game.h"
#include "windowsize.h"

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
    set_window_size();
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
}

void mouse(int button, int state, int x, int y) {
    if (state != GLUT_DOWN) return;

    float fx = 2 * x / (float) window_width() - 1.0;
    float fy = 1.0 - 2 * y / (float) window_height();
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

void keyboard(unsigned char key, int x, int y) {
    switch (program_state)
    {
    case Menu:
        // keyboard_Menu()
        break;
        
    default:
        break;
    }
}

void keyboard_special(int key, int x, int y) {
    switch (key)
    {
    case GLUT_KEY_F5:
        glutFullScreenToggle();
        break;
    case GLUT_KEY_F12:
        exit_and_free(EXIT_SUCCESS);
        break;
    default:
        switch (program_state)
        {
        case Menu:
            // keyboard_special_Menu()
            break;
        
        case Game:
            keyboard_special_Game(key, x, y);
            break;
        
        default:
            break;
        }
        break;
    }    
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    
	glutCreateWindow("Pacman");
    glutFullScreen(); // now we can:)
    
    init_Menu();
    init_About();
    init_Game();

    program_state = Menu;

    glutDisplayFunc(render);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboard_special);

    glutMainLoop();

    exit_and_free(EXIT_SUCCESS);
}
