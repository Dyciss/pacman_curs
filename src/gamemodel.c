#include "gamemodel.h"
#include <stdio.h>
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

void init_countdown(Game *game) {
    game->countdown.runned = 0;
} // current tick is not run

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

int Game2file(Game *game, char *fname) {
    //
    // if success returns 1 else 0
    //
    FILE *f = fopen(fname, "w");
    if (f == NULL)
        return 0;

    // if you need to change something you should change it in file2Game also

    // clang-format off
    // sizing
    fprintf(f, "[width]: %i\n", game->width);
    fprintf(f, "[height]: %i\n", game->height);

    // data
    fprintf(f, "[lives]: %i\n", game->lives);

    // countdown, pause state
    fprintf(f, "[countdown.active]: %i\n", game->countdown.active);
    fprintf(f, "[countdown.ms]: %i\n", game->countdown.ms);
    fprintf(f, "[countdown.n]: %i\n", game->countdown.n);
    fprintf(f, "[countdown.current_n]: %i\n", game->countdown.current_n);
    fprintf(f, "[pause]: %i\n", game->pause);

    // pacman
    fprintf(f, "[pacman.x]: %i\n", game->pacman->x);
    fprintf(f, "[pacman.y]: %i\n", game->pacman->y);
    fprintf(f, "[pacman.direction]: %i\n", game->pacman->direction);
    fprintf(f, "[pacman.speed]: %i\n", game->pacman->speed);
    fprintf(f, "[pacman.start_position.x]: %i\n", game->pacman->start_position.x);
    fprintf(f, "[pacman.start_position.y]: %i\n", game->pacman->start_position.y);
    fprintf(f, "[pacman.under.object]: %i\n", game->pacman->under.object);
    if (game->pacman->under.object == Ghost) {
        fprintf(f, "[pacman.under.ghost_id]: %i\n", game->pacman->under.ghost_id);
    } else if (game->pacman->under.object == Food ||
               game->pacman->under.object == Eaten_Food) {
        fprintf(f, "[pacman.under.food_type]: %i\n", game->pacman->under.food_type);
    }

    // ghosts
    fprintf(f, "[ghost_count]: %i\n", game->ghost_count);
    for (int ghost_id = 0; ghost_id < game->ghost_count; ghost_id++) {
        fprintf(f, "[ghosts.%i.x]: %i\n", ghost_id, game->ghosts[ghost_id]->x);
        fprintf(f, "[ghosts.%i.y]: %i\n", ghost_id, game->ghosts[ghost_id]->y);
        fprintf(f, "[ghosts.%i.direction]: %i\n", ghost_id, game->ghosts[ghost_id]->direction);
        fprintf(f, "[ghosts.%i.speed]: %i\n", ghost_id, game->ghosts[ghost_id]->speed);
        fprintf(f, "[ghosts.%i.start_position.x]: %i\n", ghost_id, game->ghosts[ghost_id]->start_position.x);
        fprintf(f, "[ghosts.%i.start_postition_y]: %i\n", ghost_id, game->ghosts[ghost_id]->start_position.y);
        fprintf(f, "[ghosts.%i.under.object]: %i\n", ghost_id, game->ghosts[ghost_id]->under.object);
        if (game->ghosts[ghost_id]->under.object == Ghost) {
            fprintf(f, "[ghosts.%i.under.ghost_id]: %i\n", ghost_id, game->ghosts[ghost_id]->under.ghost_id);
        } else if (game->ghosts[ghost_id]->under.object == Food ||
                   game->ghosts[ghost_id]->under.object == Eaten_Food) {
            fprintf(f, "[ghosts.%i.under.food_type]: %i\n", ghost_id, game->ghosts[ghost_id]->under.food_type);
        }
    }

    // field
    for (int x = 0; x < game->width; x++) {
        for (int y = 0; y < game->height; y++) {
            struct cell c = game->field[x][y];
            fprintf(f, "[field.%i.%i.object]: %i\n", x, y, c.object);
            if (c.object == Ghost) {
                fprintf(f, "[field.%i.%i.ghost_id]: %i\n", x, y, c.ghost_id);
            } else if (c.object == Food || c.object == Eaten_Food) {
                fprintf(f, "[field.%i.%i.food_type]: %i\n", x, y, c.food_type);
            }
        }
    }

    // clang-format on
    fclose(f);
    return 1;
}

#define CHECK(result)                                                          \
    if (result == EOF || result == 0)                                          \
        return 0;
#define SCANF_WITH_CHECK(result, scanf)                                        \
    result = scanf;                                                            \
    CHECK(result);

int file2Game(Game *game, char *fname) {
    //
    // if success returns 1 else 0
    // memory for game should be allocated
    // memory for pacman, ghosts, etc shouldn't be allocated
    //

    FILE *f = fopen(fname, "r");
    if (f == NULL)
        return 0;

    int r = 0;

    // clang-format off
    // sizing
    SCANF_WITH_CHECK(r, fscanf(f, "[width]: %i\n", &game->width));
    SCANF_WITH_CHECK(r, fscanf(f, "[height]: %i\n", &game->height));
    
    // data
    SCANF_WITH_CHECK(r, fscanf(f, "[lives]: %i\n", &game->lives));

    // countdown, pause state
    SCANF_WITH_CHECK(r, fscanf(f, "[countdown.active]: %i\n", &game->countdown.active));
    SCANF_WITH_CHECK(r, fscanf(f, "[countdown.ms]: %i\n", &game->countdown.ms));
    SCANF_WITH_CHECK(r, fscanf(f, "[countdown.n]: %i\n", &game->countdown.n));
    SCANF_WITH_CHECK(r, fscanf(f, "[countdown.current_n]: %i\n", &game->countdown.current_n));
    SCANF_WITH_CHECK(r, fscanf(f, "[pause]: %i\n", &game->pause));
    
    int temp = 0;
    // pacman
    game->pacman = (struct creature *)malloc(sizeof(struct creature));
    SCANF_WITH_CHECK(r, fscanf(f, "[pacman.x]: %i\n", &game->pacman->x));
    SCANF_WITH_CHECK(r, fscanf(f, "[pacman.y]: %i\n", &game->pacman->y));
    SCANF_WITH_CHECK(r, fscanf(f, "[pacman.direction]: %i\n", &temp));
    game->pacman->direction = temp;
    SCANF_WITH_CHECK(r, fscanf(f, "[pacman.speed]: %i\n", &game->pacman->speed));
    SCANF_WITH_CHECK(r, fscanf(f, "[pacman.start_position.x]: %i\n", &game->pacman->start_position.x));
    SCANF_WITH_CHECK(r, fscanf(f, "[pacman.start_position.y]: %i\n", &game->pacman->start_position.y));
    SCANF_WITH_CHECK(r, fscanf(f, "[pacman.under.object]: %i\n", &temp));
    game->pacman->under.object = temp;
    if (game->pacman->under.object == Ghost) {
        SCANF_WITH_CHECK(r, fscanf(f, "[pacman.under.ghost_id]: %i\n", &game->pacman->under.ghost_id));
    } else if (game->pacman->under.object == Food || game->pacman->under.object == Eaten_Food) {
        SCANF_WITH_CHECK(r, fscanf(f, "[pacman.under.food_type]: %i\n", &temp));
        game->pacman->under.food_type = temp;
    }

    // ghosts
    SCANF_WITH_CHECK(r, fscanf(f, "[ghost_count]: %i\n", &game->ghost_count));
    game->ghosts = (struct creature **)malloc(sizeof(struct creature *) * game->ghost_count);
    for (int ghost_id = 0; ghost_id < game->ghost_count; ghost_id++) {
        game->ghosts[ghost_id] = (struct creature *)malloc(sizeof(struct creature));
        SCANF_WITH_CHECK(r, fscanf(f, "[ghosts.%*i.x]: %i\n", &game->ghosts[ghost_id]->x));
        SCANF_WITH_CHECK(r, fscanf(f, "[ghosts.%*i.y]: %i\n", &game->ghosts[ghost_id]->y));
        SCANF_WITH_CHECK(r, fscanf(f, "[ghosts.%*i.direction]: %i\n", &temp));
        game->ghosts[ghost_id]->direction = temp;
        SCANF_WITH_CHECK(r, fscanf(f, "[ghosts.%*i.speed]: %i\n", &game->ghosts[ghost_id]->speed));
        SCANF_WITH_CHECK(r, fscanf(f, "[ghosts.%*i.start_position.x]: %i\n", &game->ghosts[ghost_id]->start_position.x));
        SCANF_WITH_CHECK(r, fscanf(f, "[ghosts.%*i.start_postition_y]: %i\n", &game->ghosts[ghost_id]->start_position.y));
        SCANF_WITH_CHECK(r, fscanf(f, "[ghosts.%*i.under.object]: %i\n", &temp));
        game->ghosts[ghost_id]->under.object = temp;
        if (game->ghosts[ghost_id]->under.object == Ghost) {
            SCANF_WITH_CHECK(r, fscanf(f, "[ghosts.%*i.under.ghost_id]: %i\n", &game->ghosts[ghost_id]->under.ghost_id));
        } else if (game->ghosts[ghost_id]->under.object == Food ||
                   game->ghosts[ghost_id]->under.object == Eaten_Food) {
            SCANF_WITH_CHECK(r, fscanf(f, "[ghosts.%*i.under.food_type]: %i\n", &temp));
            game->ghosts[ghost_id]->under.food_type = temp;
        }
    }

    // field
    init_field(game, game->width, game->height);
    for (int x = 0; x < game->width; x++) {
        for (int y = 0; y < game->height; y++) {
            struct cell *c = &game->field[x][y];
            SCANF_WITH_CHECK(r, fscanf(f, "[field.%*i.%*i.object]: %i\n", &temp));
            c->object = temp;
            if (c->object == Ghost) {
                SCANF_WITH_CHECK(r, fscanf(f, "[field.%*i.%*i.ghost_id]: %i\n", &c->ghost_id));
            } else if (c->object == Food || c->object == Eaten_Food) {
                SCANF_WITH_CHECK(r, fscanf(f, "[field.%*i.%*i.food_type]: %i\n", &temp));
                c->food_type = temp;
            }
        }
    }

    // clang-format on
    fclose(f);
    return 1;
}

#undef SCANF_WITH_CHECK
#undef CHECK
