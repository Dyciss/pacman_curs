#include "gamemodel.h"

Game *new_Game() {
    Game *game = (Game *)malloc(sizeof(Game));

    game->alive = 0;
    set_ghost_count(game, 1);
    init_field(game, 28, 36);

    game->field[27][35].object = Wall;
    game->field[25][35].object = Food;
    game->field[25][35].food_type = LARGE;

    game->field[0][5].object = Pacman;
    game->pacman = (struct creature *)malloc(sizeof(struct creature));
    game->pacman->x = 1;
    game->pacman->y = 6;
    game->pacman->animation_status = 0;
    game->pacman->direction = RIGHT;
    game->pacman->speed = 900;

    game->field[2][2].object = Ghost;
    game->field[2][2].ghost_id = 0;
    game->ghosts[0] = (struct creature *)malloc(sizeof(struct creature));
    game->ghosts[0]->x = 3;
    game->ghosts[0]->y = 3;
    game->ghosts[0]->speed = 450;
    game->ghosts[0]->direction = LEFT;
    game->ghosts[0]->animation_status = 0;

    return game;
}
