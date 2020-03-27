#include "gamemodel.h"

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

    game->pacman->animation_status = 0;
    game->pacman->direction = RIGHT;
    game->pacman->speed = 900;

    game->ghosts[0]->speed = 450;
    game->ghosts[0]->direction = LEFT;
    game->ghosts[0]->animation_status = 0;
}

void init_start_position(Game *game) {
    game->field[27][35].object = Wall;
    game->field[27][34].object = Wall;
    game->field[27][33].object = Wall;
    game->field[27][25].object = Wall;
    game->field[26][24].object = Wall;
    game->field[25][23].object = Wall;
    game->field[25][22].object = Wall;
    game->field[24][22].object = Wall;

    game->field[25][35].object = Food;
    game->field[25][35].food_type = LARGE;
    game->field[0][2].object = Food;
    game->field[0][1].food_type = MEDIUM;
    game->field[9][8].object = Food;
    game->field[9][8].food_type = SMALL;


    game->pacman = (struct creature *)malloc(sizeof(struct creature));
    game->ghosts[0] = (struct creature *)malloc(sizeof(struct creature));

    rebirth(game);
}

Game *new_Game() {
    Game *game = (Game *)malloc(sizeof(Game));

    game->alive = 0;
    game->lives = 3;
    game->countdown.ms = 1000;
    game->countdown.n = 10;
    start_countdown(game);

    set_ghost_count(game, 1);
    init_field(game, 28, 36);
    init_start_position(game);

    return game;
}
