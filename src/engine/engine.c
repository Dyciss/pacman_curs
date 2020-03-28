#include "gamemodel.h"
#include <string.h>

struct cell field[10][5] = {
    {GHOST_CELL(1), NOTHING_CELL, NOTHING_CELL, NOTHING_CELL, NOTHING_CELL,
     NOTHING_CELL, NOTHING_CELL, NOTHING_CELL, NOTHING_CELL, NOTHING_CELL},
    {NOTHING_CELL, NOTHING_CELL, NOTHING_CELL, NOTHING_CELL, NOTHING_CELL,
     NOTHING_CELL, NOTHING_CELL, NOTHING_CELL, NOTHING_CELL, NOTHING_CELL},
    {NOTHING_CELL, WALL_CELL, WALL_CELL, WALL_CELL, NOTHING_CELL, NOTHING_CELL,
     NOTHING_CELL, NOTHING_CELL, NOTHING_CELL, NOTHING_CELL},
    {PACMAN_CELL, WALL_CELL, WALL_CELL, WALL_CELL, NOTHING_CELL, NOTHING_CELL,
     NOTHING_CELL, NOTHING_CELL, NOTHING_CELL, NOTHING_CELL},
    {GHOST_CELL(0), WALL_CELL, WALL_CELL, WALL_CELL, NOTHING_CELL, NOTHING_CELL,
     NOTHING_CELL, NOTHING_CELL, NOTHING_CELL, NOTHING_CELL}};

struct start_position {
    int x;
    int y;
} pacman_sp, *ghost_sp;

void rebirth(Game *game) {
    //
    // creatures position <- start position
    // creatures speed, direction <- start values
    //
    int i = game->pacman->x - 1;
    int j = game->pacman->y - 1;

    if (i >= 0 && i <= game->width - 1 && j >= 0 && j <= game->height - 1)
        game->field[i][j] = NOTHING_CELL;

    game->field[0][5].object = Pacman;
    game->pacman->x = 1;
    game->pacman->y = 6;

    i = game->ghosts[0]->x - 1;
    j = game->ghosts[0]->y - 1;
    if (i >= 0 && i <= game->width - 1 && j >= 0 && j <= game->height - 1)
        game->field[i][j] = NOTHING_CELL;

    game->field[2][2].object = Ghost;
    game->field[2][2].ghost_id = 0;
    game->ghosts[0]->x = 3;
    game->ghosts[0]->y = 3;

    game->field[6][6].object = Ghost;
    game->field[6][6].ghost_id = 1;
    game->ghosts[1]->x = 1;
    game->ghosts[1]->y = 1;

    game->pacman->animation_status = 0;
    game->pacman->direction = RIGHT;
    game->pacman->speed = 200;

    game->ghosts[0]->speed = 900;
    game->ghosts[0]->direction = LEFT;
    game->ghosts[0]->animation_status = 0;

    game->ghosts[1]->speed = 450;
    game->ghosts[1]->direction = LEFT;
    game->ghosts[1]->animation_status = 0;
}

void set_level(Game *game) {
    for (int i = 0; i < game->width; i++) {
        memcpy(game->field[i], field[i], game->height);
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
    Game *game = (Game *)malloc(sizeof(Game));
    game->alive = 0;
    game->lives = 3;
    game->countdown.ms = 100;
    game->countdown.n = 10;
    init_field(game, 10, 5);
    int ghost_count = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 5; j++) {
            if (field[i][j].object = Ghost) {
                ghost_count++;
            } else if (field[i][j].object = Pacman) {
                pacman_sp.x = i + 1;
                pacman_sp.y = j + 1;
            }
        }
    }
    set_ghost_count(game, ghost_count);
    ghost_sp = (struct start_position *)malloc(sizeof(struct start_position) *
                                               ghost_count);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 5; j++) {
            if (field[i][j].object = Ghost) {
                ghost_sp[field[i][j].ghost_id].x = i + 1;
                ghost_sp[field[i][j].ghost_id].y = j + 1;
            }
        }
    }
    game->pacman = (struct creature *)malloc(sizeof(struct creature));
    for (int i = 0; i < ghost_count; i++) {
        game->ghosts[i] = (struct creature *)malloc(sizeof(struct creature));
    }
    set_level(game);
    start_countdown(game);

    return game;
}
