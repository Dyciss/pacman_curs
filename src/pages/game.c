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

void animate_Ghost(int id) {
    if (!game->alive)
        return;
    game->ghosts[id]->animation_status = !game->ghosts[id]->animation_status;
    glutTimerFunc(1000.0 / 3, animate_Ghost, id);
}

void rebirth_game() { rebirth(game); }

void move_pacman() {
    if (!game->alive || !game->lives)
        return;

    if (game->countdown.active) {
        glutTimerFunc(60 * 1000.0 / game->pacman->speed, move_pacman, 0);
        return;
    }

    int new_x;
    int new_y;

    if (!set_new_xy(game, game->pacman, &new_x, &new_y)){
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
        game->lives--;

        if (game->lives) {
            start_countdown(game);
            glutTimerFunc(game->countdown.ms, rebirth_game, 0);
        }

        break;

    default: // cant be here
        break;
    }

    glutTimerFunc(60 * 1000.0 / game->pacman->speed, move_pacman, 0);
}

void move_Ghost(int id) {
    if (!game->alive || !game->lives)
        return;

    if (game->countdown.active) {
        glutTimerFunc(60 * 1000.0 / game->ghosts[id]->speed, move_Ghost, id);
        return;
    }
    int new_x;
    int new_y;

    if (!set_new_xy(game, game->ghosts[id], &new_x, &new_y)){
        glutTimerFunc(60 * 1000.0 / game->ghosts[id]->speed, move_pacman, id);
        return;  
    }

    if (game->field[new_x - 1][new_y - 1].object == Wall) {
        glutTimerFunc(60 * 1000.0 / game->ghosts[id]->speed, move_Ghost, id);
        return;
    }
    game->field[game->ghosts[id]->x - 1][game->ghosts[id]->y - 1] =
        game->ghosts_under[id];

    game->ghosts[id]->x = new_x;
    game->ghosts[id]->y = new_y;

    game->ghosts_under[id] = game->field[new_x - 1][new_y - 1];
    game->field[new_x - 1][new_y - 1] = GHOST_CELL(id);

    if (game->ghosts_under[id].object == Pacman) { // not creatures under ghost
        game->ghosts_under[id] = NOTHING_CELL;
        game->lives--;

        if (game->lives) {
            start_countdown(game);
            glutTimerFunc(game->countdown.ms, rebirth_game, 0);
        }
    } else if (game->ghosts_under[id].object == Ghost) {
        game->ghosts_under[id] = NOTHING_CELL;
    }

    glutTimerFunc(60 * 1000.0 / game->ghosts[id]->speed, move_Ghost, id);
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

void think_Ghost(int ghost_id) {
    set_Ghost_direction(game, ghost_id);
    glutTimerFunc(60 * 1000.0 / game->ghosts[ghost_id]->speed / 2, think_Ghost, ghost_id);
}
void render_Game() {
    sync_sizing_props(game);
    if (!game->alive) {
        game->alive = 1;
        animate_pacman();
        for (int i = 0; i < game->ghost_count; i++) {
            animate_Ghost(i);
            glutTimerFunc(60 * 1000.0 / game->ghosts[i]->speed / 2, think_Ghost, i);
        }
        frame();
        glutTimerFunc(60 * 1000.0 / game->pacman->speed, move_pacman, 0);
        for (int i = 0; i < game->ghost_count; i++) {
            glutTimerFunc(60 * 1000.0 / game->ghosts[i]->speed, move_Ghost, i);
        }
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