#include <GL/freeglut.h>
#include <stdio.h>
#include <time.h>

#include "main.h"

#include "./ui/gameui.h"
#include "./ui/render.h"
#include "./ui/windowsize.h"

#include "./engine/engine.h"
#include "./ui/gamesizing.h"
#include "directions.h"

Game *game = NULL;

char escape_btn_text[] = "escape";
Button *escape_btn = NULL;

int fps = 30;

void animate_pacman() {
    if (!game->alive)
        return;
    game->pacman->animation_status = !game->pacman->animation_status;
    glutTimerFunc(1000.0 / 3, animate_pacman, 0);
}

void move_pacman() {
    if (!game->alive)
        return;

    int new_x = game->pacman->x;
    int new_y = game->pacman->y;

    switch (game->pacman->direction) {
    case TOP:
        new_y = (game->height + game->pacman->y - 1 - 1) % game->height + 1;
        break;
    case BOTTOM:
        new_y = (game->height + game->pacman->y - 1 + 1) % game->height + 1;
        break;
    case LEFT:
        new_x = (game->width + game->pacman->x - 1 - 1) % game->width + 1;
        break;
    case RIGHT:
        new_x = (game->width + game->pacman->x - 1 + 1) % game->width + 1;
        break;
    default:
        glutTimerFunc(60 * 1000.0 / game->pacman->speed, move_pacman, 0);
        return;
    }

    if (game->field[new_x - 1][new_y - 1].object == Wall) {
        glutTimerFunc(60 * 1000.0 / game->pacman->speed, move_pacman, 0);
        return;
    }

    game->field[game->pacman->x - 1][game->pacman->y - 1] = NOTHING_CELL;
    game->pacman->x = new_x;
    game->pacman->y = new_y;

    switch (game->field[new_x - 1][new_y - 1].object) {
    case Nothing:
        game->field[new_x - 1][new_y - 1] = PACMAN_CELL;
        break;
    case Food:
        game->field[new_x - 1][new_y - 1] = PACMAN_CELL;
        // smt ++
        break;
    case Ghost:
        // no pacman
        game->pacman->x = 0;
        game->pacman->y = 0;
        game->alive = 0; // maybe game->end = 1
        // THE END
        break;

    default: // cant be here
        break;
    }

    map_xy_to_window_xy(game->alpha, game->pacman->x, game->pacman->y,
                        &game->pacman->window_x, &game->pacman->window_y);
    printf("PACMAN -- (%i, %i) -- (%f, %f)\n", game->pacman->x, game->pacman->y,
           game->pacman->window_x, game->pacman->window_y);

    glutTimerFunc(60 * 1000.0 / game->pacman->speed, move_pacman, 0);
}

void frame() {
    if (!game->alive)
        return;

    glClear(GL_COLOR_BUFFER_BIT);
    puts("draw");
    render_button(escape_btn, 1 - FONT_WIDTH * (sizeof escape_btn_text),
                  -1 + FONT_HEIGHT_UPPER_CASE, WHITE, WHITE);
    draw_game(game);

    glutTimerFunc(1000.0 / fps, frame, 0);
    glutSwapBuffers();
}

void render_Game() {
    sync_sizing_props(game);
    if (!game->alive) {
        game->alive = 1;
        animate_pacman();
        frame();
        glutTimerFunc(60 * 1000.0 / game->pacman->speed, move_pacman, 0);
    }
}

void mouse_Game(float x, float y) {
    if (in_button(escape_btn, x, y)) {
        game->alive = 0;
        set_program_state(Menu);
        return;
    }
}

void keyboard_special_Game(int key, int x, int y) {
    Direction new_direction = direction_from_special_key(key);
    if (new_direction) {
        game->pacman->direction = new_direction;
    }
}

void init_Game() {
    game = new_Game();
    escape_btn = new_Button(escape_btn_text);
}

void free_Game() { free_Button(escape_btn); }