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

void animate_Ghost() {
    if (!game->alive)
        return;
    game->ghosts[0]->animation_status = !game->ghosts[0]->animation_status;
    glutTimerFunc(1000.0 / 3, animate_Ghost, 0);
}

void rebirth_game() { rebirth(game); }

void move_pacman() {
    if (!game->alive || !game->lives)
        return;

    if (game->countdown.active) {
        glutTimerFunc(60 * 1000.0 / game->pacman->speed, move_pacman, 0);
        return;
    }

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
        game->lives--;

        if (game->lives) {
            start_countdown(game);
            glutTimerFunc(game->countdown.ms, rebirth_game, 0);
        }

        break;

    default: // cant be here
        break;
    }

    map_xy_to_window_xy(game->alpha, game->pacman->x, game->pacman->y,
                        &game->pacman->window_x, &game->pacman->window_y);

    glutTimerFunc(60 * 1000.0 / game->pacman->speed, move_pacman, 0);
}

void move_Ghost() {
    if (!game->alive || !game->lives)
        return;

    if (game->countdown.active) {
        glutTimerFunc(60 * 1000.0 / game->ghosts[0]->speed, move_Ghost, 0);
        return;
    }
    int new_x = game->ghosts[0]->x;
    int new_y = game->ghosts[0]->y;

    switch (game->ghosts[0]->direction) {
    case TOP:
        new_y = (game->height + game->ghosts[0]->y - 1 - 1) % game->height + 1;
        break;
    case BOTTOM:
        new_y = (game->height + game->ghosts[0]->y - 1 + 1) % game->height + 1;
        break;
    case LEFT:
        new_x = (game->width + game->ghosts[0]->x - 1 - 1) % game->width + 1;
        break;
    case RIGHT:
        new_x = (game->width + game->ghosts[0]->x - 1 + 1) % game->width + 1;
        break;
    default:
        glutTimerFunc(60 * 1000.0 / game->ghosts[0]->speed, move_Ghost, 0);
        return;
    }

    if (game->field[new_x - 1][new_y - 1].object == Wall) {
        glutTimerFunc(60 * 1000.0 / game->ghosts[0]->speed, move_Ghost, 0);
        return;
    }

    game->field[game->ghosts[0]->x - 1][game->ghosts[0]->y - 1] = NOTHING_CELL;
    game->ghosts[0]->x = new_x;
    game->ghosts[0]->y = new_y;

    switch (game->field[new_x - 1][new_y - 1].object) {
    case Nothing:
        game->field[new_x - 1][new_y - 1] = GHOST_CELL(0);
        break;
    case Food:
        game->field[new_x - 1][new_y - 1] = GHOST_CELL(0);
        // smt ++
        break;
        // case ghost???????????
    case Pacman:
        game->field[new_x - 1][new_y - 1] = GHOST_CELL(0);
        game->pacman->x = 0;
        game->pacman->y = 0;
        game->lives--;

        if (game->lives) {
            start_countdown(game);
            glutTimerFunc(game->countdown.ms, rebirth_game, 0);
        }
        break;

    default: // cant be here
        break;
    }
    map_xy_to_window_xy(game->alpha, game->ghosts[0]->x, game->ghosts[0]->y,
                        &game->ghosts[0]->window_x, &game->ghosts[0]->window_y);

    glutTimerFunc(60 * 1000.0 / game->ghosts[0]->speed, move_Ghost, 0);
}

void countdown() {
    game->countdown.current_n++;
    if (game->countdown.current_n > game->countdown.n) {
        stop_countdown(game);
        return;
    }

    glutTimerFunc(game->countdown.ms, countdown, 0);
}

void frame() {
    if (!game->alive)
        return;

    if (game->countdown.active && !game->countdown.runned) {
        game->countdown.runned = 1;
        glutTimerFunc(game->countdown.ms, countdown, 0);
    }

    glClear(GL_COLOR_BUFFER_BIT);
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
        animate_Ghost();
        frame();
        glutTimerFunc(60 * 1000.0 / game->pacman->speed, move_pacman, 0);
        glutTimerFunc(60 * 1000.0 / game->ghosts[0]->speed, move_Ghost, 0);
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