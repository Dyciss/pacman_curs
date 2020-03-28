#include "gamemodel.h"
#include <stdio.h>
#include <string.h>

#define X 30
#define Y 30
struct cell field[Y][X] = { 0 };

struct start_position {
    int x;
    int y;
} pacman_sp, *ghost_sp;

void rebirth(Game *game) {
    //
    // creatures position <- start position
    //
    for (int ghost_id = 0; ghost_id < game->ghost_count; ghost_id++) {
        struct creature *ghost = game->ghosts[ghost_id];
        game->field[ghost->x - 1][ghost->y - 1] = game->ghosts_under[ghost_id];
        ghost->x = ghost_sp[ghost_id].x;
        ghost->y = ghost_sp[ghost_id].y;
        game->field[ghost->x - 1][ghost->y - 1] = GHOST_CELL(ghost_id);
    }
    game->field[game->pacman->x - 1][game->pacman->y - 1] = NOTHING_CELL;
    game->pacman->x = pacman_sp.x;
    game->pacman->y = pacman_sp.y;
    game->field[game->pacman->x - 1][game->pacman->y - 1] = PACMAN_CELL;
}

void set_level(Game *game) {
    for (int x = 0; x < game->width; x++) {
        for (int y = 0; y < game->height; y++) {
            game->field[x][y] = field[y][x];
        }
    }
    game->pacman->x = pacman_sp.x;
    game->pacman->y = pacman_sp.y;
    for (int i = 0; i < game->ghost_count; i++) {
        game->ghosts[i]->x = ghost_sp[i].x;
        game->ghosts[i]->y = ghost_sp[i].y;
        game->ghosts[i]->speed = 450;
        game->ghosts[i]->direction = LEFT;
        game->ghosts[i]->animation_status = 0;
    }
    game->pacman->animation_status = 0;
    game->pacman->direction = RIGHT;
    game->pacman->speed = 200;
}

Game *new_Game() {
    field[0][0] = GHOST_CELL(0);
    field[4][5] = GHOST_CELL(1);
    field[3][3] = PACMAN_CELL;
    field[3][4] = FOOD_CELL(SMALL);
    field[3][5] = WALL_CELL;

    Game *game = (Game *)malloc(sizeof(Game));
    game->alive = 0;
    game->lives = 3;
    game->countdown.ms = 1000;
    game->countdown.n = 10;

    init_field(game, X, Y);
    int ghost_count = 0;
    for (int x = 0; x < X; x++) {
        for (int y = 0; y < Y; y++) {
            if (field[y][x].object == Ghost) {
                ghost_count++;
            } else if (field[y][x].object == Pacman) {
                pacman_sp.x = x + 1;
                pacman_sp.y = y + 1;
            }
        }
    }

    set_ghost_count(game, ghost_count);
    ghost_sp = (struct start_position *)malloc(sizeof(struct start_position) *
                                               ghost_count);

    game->pacman = (struct creature *)malloc(sizeof(struct creature));
    for (int i = 0; i < ghost_count; i++) {
        game->ghosts[i] = (struct creature *)malloc(sizeof(struct creature));
    }

    for (int x = 0; x < X; x++) {
        for (int y = 0; y < Y; y++) {
            if (field[y][x].object == Ghost) {
                ghost_sp[field[y][x].ghost_id].x = x + 1;
                ghost_sp[field[y][x].ghost_id].y = y + 1;
            }
        }
    }

    set_level(game);
    start_countdown(game);

    return game;
}
