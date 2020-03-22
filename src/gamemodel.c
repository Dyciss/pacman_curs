#include "gamemodel.h"

void set_ghost_count(Game *game, int count) {
    game->ghost_count = count;
    game->ghosts = (struct creature **) malloc(sizeof(struct creature *)*count);
}

void init_field(Game *game, int width, int height) {
    game->width = width;
    game->height = height;

    game->field = (struct cell **) malloc(sizeof(struct cell *) * width);
    for (int i=0; i<width; i++) {
        game->field[i] = (struct cell *) calloc(height, sizeof(struct cell)); // initialized with Nothing
    }
}
