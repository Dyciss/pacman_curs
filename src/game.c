#include <GL/freeglut.h>
#include "render.h"
#include <stdio.h>
#include "main.h"

int h = 36;
int w = 28;
int alpha = 20;

float game_h;
float game_w;

struct Pacman {
    float fps;
    int px;
    int x;
    int y;
    int mouth_runned;
} *pacman;

void map_xy_to_window_xy(int x, int y, float *xw, float *yw) {
    x *= alpha;
    y *= alpha;

    *xw = 2 * x / (float) glutGet(GLUT_WINDOW_WIDTH) - 1.0;
    *yw = 1.0 - 2 * y / (float) glutGet(GLUT_WINDOW_HEIGHT);
}

void grid() {
    glColor3ub(WHITE.r, WHITE.g, WHITE.b);
    glPointSize(3);
    for (int i = 1; i <= w; i++) {
        for (int j = 1; j <= h; j++) {
            float x;
            float y;
            map_xy_to_window_xy(i, j, &x, &y);
            glBegin(GL_POINTS);
                glVertex2f(x, y);
            glEnd();
        }
    }
}

void set_Wall(int map_x, int map_y){
    float game_x;
    float game_y;
    map_xy_to_window_xy(map_x, map_y, &game_x, &game_y);
    render_Wall(alpha*2, game_x, game_y, GAME_BG);
}

void set_Food(int  map_x, int map_y, enum Food food){
    float game_x;
    float game_y;
    map_xy_to_window_xy(map_x, map_y, &game_x, &game_y);
    render_Food(alpha*2-1, game_x, game_y, GOLD, food);
}

void pacman_run_mouth(int open) {
    float xw;
    float yw;
    map_xy_to_window_xy(pacman->x, pacman->y, &xw, &yw);
    if (open) {
        render_Pacman_mouth(pacman->px, xw, yw, GAME_BG, TOP);
        glutTimerFunc(pacman->fps, pacman_run_mouth, 0);
    }
    else {
        render_Pacman(pacman->px, xw, yw, GOLD);
        glutTimerFunc(pacman->fps, pacman_run_mouth, 1);
    }

    glutSwapBuffers();
}

void render_Game() {
    map_xy_to_window_xy(w, h, &game_w, &game_h);

    float alpha_relative_x = alpha / (float) glutGet(GLUT_WINDOW_WIDTH);
    float alpha_relative_y = alpha / (float) glutGet(GLUT_WINDOW_HEIGHT);

    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_POLYGON);
        glColor3ub(GAME_BG.r, GAME_BG.g, GAME_BG.b);
        glVertex2f(-1 + alpha_relative_x, 1 - alpha_relative_y);
        glVertex2f(game_w + alpha_relative_x, 1 - alpha_relative_y);
        glVertex2f(game_w + alpha_relative_x, game_h - alpha_relative_y);
        glVertex2f(-1 + alpha_relative_x, game_h - alpha_relative_y);
    glEnd();
    if (!pacman->mouth_runned) {
      pacman_run_mouth(0);
      pacman->mouth_runned = 1; 
    }
  //  grid();
    render_Ghost(alpha*2-10, -0.7, 0.1, RED, TOP);


    set_Wall(27,36);
    set_Wall(27,35);
    set_Wall(27,34);
    set_Wall(27,33);
    set_Wall(27,32);
    set_Wall(28,32);
    set_Wall(26,32);

    set_Food(28,35,SMALL);
    set_Food(28,34, MEDIUM);
    set_Food(28,33, LARGE);

}

void mouse_Game(float x, float y) {

}

void init_Game() {
    pacman = (struct Pacman*) malloc(sizeof(struct Pacman));
    pacman->fps = 1000/5.0;
    pacman->px = alpha * 2 - 10;
    pacman->x = 28;
    pacman->y = 36;
    pacman->mouth_runned = 0;
}

void free_Game() {
    free(pacman);
}


