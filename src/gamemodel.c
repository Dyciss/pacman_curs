#include "gamemodel.h"
#include <stdlib.h>

void set_ghost_count(Game *game, int count) {
    game->ghost_count = count;
    game->ghosts =
        (struct creature **)malloc(sizeof(struct creature *) * count);
}

void init_field(Game *game, int width, int height) {
    game->width = width;
    game->height = height;

    game->field = (struct cell **)malloc(sizeof(struct cell *) * width);
    for (int i = 0; i < width; i++) {
        game->field[i] = (struct cell *)calloc(
            height, sizeof(struct cell)); // initialized with Nothing
    }
}

void init_countdown(Game *game) { game->countdown.runned = 0; } // current tick is not run

void start_countdown(Game *game) {
    game->countdown.active = 1;
    game->countdown.current_n = 1;
}

void stop_countdown(Game *game) { game->countdown.active = 0; }

void start_pause(Game *game) {
    game->pause = 1;
    stop_countdown(game);
}

void stop_pause(Game *game) {
    start_countdown(game);
    game->pause = 0;
}