#include <GL/freeglut.h>
#include "render.h"
#include <stdio.h>
#include <time.h>

#include "main.h"
#include "windowsize.h"

int game_alive = 0;
int fps = 30;
int frame_n = 0;

int h = 36;
int w = 28;
int alpha = 20;

float game_h;
float game_w;

struct Pacman {
    int px;
    int pos_x;
    int pos_y;
    float x;
    float y;
    int alive;
    int mouth_opened;
    int mouth_count_in_frames;
    int move_count_in_frames;
    enum Direction direction;
} *pacman;

void map_xy_to_window_xy(int x, int y, float *xw, float *yw) {
    x *= alpha;
    y *= alpha;

    *xw = 2 * x / (float) window_width() - 1.0;
    *yw = 1.0 - 2 * y / (float) window_height();
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

void pacman_move() {
    switch (pacman->direction)
    {
    case TOP:
        pacman->pos_y = (h + pacman->pos_y-1 - 1) % h + 1;
        break;
    case BOTTOM:
        pacman->pos_y = (h + pacman->pos_y-1 + 1) % h + 1;
        break;
    case LEFT:
        pacman->pos_x = (w + pacman->pos_x-1 - 1) % w + 1;
        break;
    case RIGHT:
        pacman->pos_x = (w + pacman->pos_x-1 + 1) % w + 1;
        break;
    default:
        break;
    }

    map_xy_to_window_xy(pacman->pos_x, pacman->pos_y, &pacman->x, &pacman->y);
}

void set_Pacman() {
    if (!(frame_n % pacman->mouth_count_in_frames)) {
        pacman->mouth_opened = !pacman->mouth_opened;
    }

    if (!(frame_n % pacman->move_count_in_frames)) {
        pacman_move();
    }

    render_Pacman(pacman->px, pacman->x, pacman->y, GOLD);

    if (!pacman->mouth_opened) {
        render_Pacman_mouth(pacman->px, pacman->x, pacman->y, GAME_BG, pacman->direction);
    }
}

void frame() {
    clock_t t1 = clock();
    frame_n = (frame_n + 1) % (pacman->mouth_count_in_frames * pacman->move_count_in_frames);

    float alpha_relative_x = alpha / (float) window_width();
    float alpha_relative_y = alpha / (float) window_height();

    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_POLYGON);
        glColor3ub(GAME_BG.r, GAME_BG.g, GAME_BG.b);
        glVertex2f(-1 + alpha_relative_x, 1 - alpha_relative_y);
        glVertex2f(game_w + alpha_relative_x, 1 - alpha_relative_y);
        glVertex2f(game_w + alpha_relative_x, game_h - alpha_relative_y);
        glVertex2f(-1 + alpha_relative_x, game_h - alpha_relative_y);
    glEnd();
    
    render_Ghost(alpha*2-10, -0.7, 0.1, RED, BOTTOM);


    set_Wall(27,36);
    set_Wall(27,35);
    set_Wall(27,34);
    set_Wall(27,33);
    set_Wall(27,32);
    set_Wall(28,32);
    set_Wall(26,32);

    set_Food(28,35, SMALL);
    set_Food(28,34, MEDIUM);
    set_Food(28,33, LARGE);
    set_Pacman();

    grid();
    glutTimerFunc(1000.0/fps, frame, 0);
    clock_t t2 = clock();
    printf("%lf\n", 1.0/((double)(t2 - t1) / CLOCKS_PER_SEC));
    glutSwapBuffers();
}


void render_Game() {
    map_xy_to_window_xy(pacman->pos_x, pacman->pos_y, &pacman->x, &pacman->y);
    map_xy_to_window_xy(w, h, &game_w, &game_h);

    if (!game_alive) {
      frame();
      game_alive = 1; 
    }
}

void mouse_Game(float x, float y) {

}

void init_Game() {
    pacman = (struct Pacman*) malloc(sizeof(struct Pacman));
    pacman->px = alpha * 2 - 10;
    pacman->alive = 0;
    pacman->mouth_opened = 0;
    pacman->mouth_count_in_frames = 2;
    pacman->move_count_in_frames = 2;
    pacman->direction = LEFT;
    pacman->pos_x = 28;
    pacman->pos_y = 36;
}

void free_Game() {
    free(pacman);
}


