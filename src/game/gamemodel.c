#include "game/gamemodel.h"
#include <stdio.h>
#include <stdlib.h>

#include "settings/settings.h"

static void init_field(Game *game, int width, int height) {
    game->width = width;
    game->height = height;

    game->field = (struct cell **)malloc(sizeof(struct cell *) * width);
    for (int i = 0; i < width; i++) {
        game->field[i] = (struct cell *)calloc(
            height, sizeof(struct cell)); // initialized with Nothing
    }
}

static void init_countdown(Game *game) {
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

static void set_food_to_start(Game *game) {
    game->foods.count_now = game->foods.count_start;
}

void check_fruits(Game *game) {
    while (game->foods.next_fruit_index < game->fruits_count &&
           game->fruits[game->foods.next_fruit_index].when_food_count >=
               game->foods.count_now) {
        // maybe new fruit will appear
        int i = game->foods.next_fruit_index;
        game->foods.next_fruit_index++;
        if (game->fruits[i].is_eaten)
            continue;
        game->fruits[i].is_eaten = 0;
        game->fruits[i].moves_uneaten = 0;
        int x = game->fruits[i].x;
        int y = game->fruits[i].y;
        // Eaten food -> food
        if (game->field[x - 1][y - 1].object == Ghost) {
            int id = game->field[x - 1][y - 1].ghost_id;
            game->ghosts[id]->under.object = Food;
            game->foods.count_now++;
        } else if (game->field[x - 1][y - 1].object == Eaten_Food) {
            game->field[x - 1][y - 1].object = Food;
            game->foods.count_now++;
        }
    }
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
    // this file is not map, it is game saving.
    fprintf(f, "SAVE: 1\n");

    // sizing
    fprintf(f, "[width]: %i\n", game->width);
    fprintf(f, "[height]: %i\n", game->height);

    // data
    fprintf(f, "[lives]: %i\n", game->lives);

    // level, difficalty
    fprintf(f, "[level]: %i\n", game->level);
    fprintf(f, "[difficalty]: %i\n", game->difficalty);

    // countdown info
    fprintf(f, "[countdown.ms]: %i\n", game->countdown.ms);
    fprintf(f, "[countdown.n]: %i\n", game->countdown.n);

    // food
    fprintf(f, "[foods.next_fruit_index]: %i\n", game->foods.next_fruit_index);
    fprintf(f, "[fruits_count]: %i\n", game->fruits_count);
    for (int i = 0; i < game->fruits_count; i++) {
        fprintf(f, "[fruits.%i.x]: %i\n", i, game->fruits[i].x);
        fprintf(f, "[fruits.%i.y]: %i\n", i, game->fruits[i].y);
        fprintf(f, "[fruits.%i.when_food_count]: %i\n", i, game->fruits[i].when_food_count);
        fprintf(f, "[fruits.%i.is_eaten]: %i\n", i, game->fruits[i].is_eaten);
        fprintf(f, "[fruits.%i.moves_uneaten]: %i\n", i, game->fruits[i].moves_uneaten);
    }

    fprintf(f, "[extra_live.x]: %i\n", game->extra_live.x);
    fprintf(f, "[extra_live.y]: %i\n", game->extra_live.y);
    fprintf(f, "[extra_live.when_level]: %i\n", game->extra_live.when_level);
    fprintf(f, "[extra_live.moves_uneaten]: %i\n", game->extra_live.moves_uneaten);
    fprintf(f, "[extra_live.moves_unvisible]: %i\n", game->extra_live.moves_unvisible);
    fprintf(f, "[extra_live.moves_eaten]: %i\n", game->extra_live.eaten);

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
    if (result == EOF || result == 0) {                                        \
        fclose(f);                                                             \
        return 0;                                                              \
    }
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
    int is_save = 0;
    SCANF_WITH_CHECK(r, fscanf(f, "SAVE: %i\n", &is_save));

    // sizing
    SCANF_WITH_CHECK(r, fscanf(f, "[width]: %i\n", &game->width));
    SCANF_WITH_CHECK(r, fscanf(f, "[height]: %i\n", &game->height));
    
    // data
    // maybe it shouldn't be in the map, but maybe it should
    SCANF_WITH_CHECK(r, fscanf(f, "[lives]: %i\n", &game->lives));

    // level, difficalty
    // it definitely shoudn't be in the map
    if (is_save) {
        SCANF_WITH_CHECK(r, fscanf(f, "[level]: %i\n", &game->level));
        SCANF_WITH_CHECK(r, fscanf(f, "[difficalty]: %i\n", &game->difficalty));
    } else {
        game->level = 1;
        // without any validation, it's not worth an exception
        // 256 mod 4 == 0 and needs because (negative) % 4 = negative
        game->difficalty = (settings_field(Difficalty)->text[0] + 256 - '0') % 4;
    }

    // countdown info
    SCANF_WITH_CHECK(r, fscanf(f, "[countdown.ms]: %i\n", &game->countdown.ms));
    SCANF_WITH_CHECK(r, fscanf(f, "[countdown.n]: %i\n", &game->countdown.n));

    // food
    if (is_save) {
        SCANF_WITH_CHECK(r, fscanf(f, "[foods.next_fruit_index]: %i\n", &game->foods.next_fruit_index));
    } else {
        game->foods.next_fruit_index = 0;
    }
    SCANF_WITH_CHECK(r, fscanf(f, "[fruits_count]: %i\n", &game->fruits_count));
    game->fruits = (struct fruit *)malloc(game->fruits_count * sizeof(struct fruit));
    for (int i = 0; i < game->fruits_count; i++) {
        SCANF_WITH_CHECK(r, fscanf(f, "[fruits.%*i.x]: %i\n", &game->fruits[i].x));
        SCANF_WITH_CHECK(r, fscanf(f, "[fruits.%*i.y]: %i\n", &game->fruits[i].y));
        SCANF_WITH_CHECK(r, fscanf(f, "[fruits.%*i.when_food_count]: %i\n", &game->fruits[i].when_food_count));
        if (is_save) {
            SCANF_WITH_CHECK(r, fscanf(f, "[fruits.%*i.is_eaten]: %i\n", &game->fruits[i].is_eaten));
            SCANF_WITH_CHECK(r, fscanf(f, "[fruits.%*i.moves_uneaten]: %i\n", &game->fruits[i].moves_uneaten));
        } else {
            game->fruits[i].is_eaten = 0;
            game->fruits[i].moves_uneaten = 0;
        }
    }

    SCANF_WITH_CHECK(r, fscanf(f, "[extra_live.x]: %i\n", &game->extra_live.x));
    SCANF_WITH_CHECK(r, fscanf(f, "[extra_live.y]: %i\n", &game->extra_live.y));
    SCANF_WITH_CHECK(r, fscanf(f, "[extra_live.when_level]: %i\n", &game->extra_live.when_level));
    if (is_save) {
        SCANF_WITH_CHECK(r, fscanf(f, "[extra_live.moves_uneaten]: %i\n", &game->extra_live.moves_uneaten));
        SCANF_WITH_CHECK(r, fscanf(f, "[extra_live.moves_unvisible]: %i\n", &game->extra_live.moves_unvisible));
        SCANF_WITH_CHECK(r, fscanf(f, "[extra_live.moves_eaten]: %i\n", &game->extra_live.eaten));
    } else {
        game->extra_live.moves_uneaten = 0;
        game->extra_live.moves_unvisible = 0;
        game->extra_live.eaten = 0;
    }

    int temp = 0;

    game->foods.count_start = 0;
    game->foods.count_now = 0;

    // pacman
    game->pacman = (struct creature *)malloc(sizeof(struct creature));
    if (is_save) {
        SCANF_WITH_CHECK(r, fscanf(f, "[pacman.x]: %i\n", &game->pacman->x));
        SCANF_WITH_CHECK(r, fscanf(f, "[pacman.y]: %i\n", &game->pacman->y));
    }
    SCANF_WITH_CHECK(r, fscanf(f, "[pacman.direction]: %i\n", &temp));
    game->pacman->direction = temp;
    SCANF_WITH_CHECK(r, fscanf(f, "[pacman.speed]: %i\n", &game->pacman->speed));
    SCANF_WITH_CHECK(r, fscanf(f, "[pacman.start_position.x]: %i\n", &game->pacman->start_position.x));
    SCANF_WITH_CHECK(r, fscanf(f, "[pacman.start_position.y]: %i\n", &game->pacman->start_position.y));
    if (!is_save) {
        game->pacman->x = game->pacman->start_position.x;
        game->pacman->y = game->pacman->start_position.y;
    }
    if (is_save) {
        SCANF_WITH_CHECK(r, fscanf(f, "[pacman.under.object]: %i\n", &temp));
        game->pacman->under.object = temp;
        if (game->pacman->under.object == Ghost) {
            SCANF_WITH_CHECK(r, fscanf(f, "[pacman.under.ghost_id]: %i\n", &game->pacman->under.ghost_id));
        } else if (game->pacman->under.object == Food || game->pacman->under.object == Eaten_Food) {
            SCANF_WITH_CHECK(r, fscanf(f, "[pacman.under.food_type]: %i\n", &temp));
            game->pacman->under.food_type = temp;
            game->foods.count_start++;
        }
    } else {
        game->pacman->under = NOTHING_CELL;
    }
    game->pacman->animation_status = 1;

    // ghosts
    SCANF_WITH_CHECK(r, fscanf(f, "[ghost_count]: %i\n", &game->ghost_count));
    game->ghosts = (struct creature **)malloc(sizeof(struct creature *) * game->ghost_count);
    for (int ghost_id = 0; ghost_id < game->ghost_count; ghost_id++) {
        game->ghosts[ghost_id] = (struct creature *)malloc(sizeof(struct creature));
        if (is_save) {
            SCANF_WITH_CHECK(r, fscanf(f, "[ghosts.%*i.x]: %i\n", &game->ghosts[ghost_id]->x));
            SCANF_WITH_CHECK(r, fscanf(f, "[ghosts.%*i.y]: %i\n", &game->ghosts[ghost_id]->y));
        }
        SCANF_WITH_CHECK(r, fscanf(f, "[ghosts.%*i.direction]: %i\n", &temp));
        game->ghosts[ghost_id]->direction = temp;
        SCANF_WITH_CHECK(r, fscanf(f, "[ghosts.%*i.speed]: %i\n", &game->ghosts[ghost_id]->speed));
        SCANF_WITH_CHECK(r, fscanf(f, "[ghosts.%*i.start_position.x]: %i\n", &game->ghosts[ghost_id]->start_position.x));
        SCANF_WITH_CHECK(r, fscanf(f, "[ghosts.%*i.start_postition_y]: %i\n", &game->ghosts[ghost_id]->start_position.y));
        if (!is_save) {
            game->ghosts[ghost_id]->x = game->ghosts[ghost_id]->start_position.x;
            game->ghosts[ghost_id]->y = game->ghosts[ghost_id]->start_position.y;
        }

        if (is_save) {
            SCANF_WITH_CHECK(r, fscanf(f, "[ghosts.%*i.under.object]: %i\n", &temp));
            game->ghosts[ghost_id]->under.object = temp;
            if (game->ghosts[ghost_id]->under.object == Ghost) {
                SCANF_WITH_CHECK(r, fscanf(f, "[ghosts.%*i.under.ghost_id]: %i\n", &game->ghosts[ghost_id]->under.ghost_id));
            } else if (game->ghosts[ghost_id]->under.object == Food ||
                       game->ghosts[ghost_id]->under.object == Eaten_Food) {
                SCANF_WITH_CHECK(r, fscanf(f, "[ghosts.%*i.under.food_type]: %i\n", &temp));
                game->ghosts[ghost_id]->under.food_type = temp;
                game->foods.count_start++;
                if (game->ghosts[ghost_id]->under.object == Food) game->foods.count_now++;
            }
        } else {
            game->ghosts[ghost_id]->under = NOTHING_CELL;
        }

        game->ghosts[ghost_id]->animation_status = 1; // just initialization
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
                game->foods.count_start++;
                if (c->object == Food) game->foods.count_now++;
                SCANF_WITH_CHECK(r, fscanf(f, "[field.%*i.%*i.food_type]: %i\n", &temp));
                c->food_type = temp;
            }
        }
    }

    game->foods.count_start -= (game->fruits_count + 1); // minus fruits and extra live
    check_fruits(game);

    // clang-format on
    game->alive = 0; // we should run it
    init_countdown(game);
    stop_pause(game); // it will start countdown

    fclose(f);
    return 1;
}

#undef SCANF_WITH_CHECK
#undef CHECK

void free_Game(Game *game) {
    if (game->field) {
        for (int x = 0; x < game->width; x++) {
            if (game->field[x])
                free(game->field[x]);
        }
        free(game->field);
    }

    if (game->fruits) {
        free(game->fruits);
    }

    if (game->ghosts) {
        for (int ghost_id = 0; ghost_id < game->ghost_count; ghost_id++) {
            if (game->ghosts[ghost_id])
                free(game->ghosts[ghost_id]);
        }
        free(game->ghosts);
    }

    if (game->pacman)
        free(game->pacman);

    if (game)
        free(game);
}

void rebirth(Game *game) {
    //
    // creatures position <- start position
    //
    game->field[game->pacman->x - 1][game->pacman->y - 1] = game->pacman->under;
    game->pacman->under = NOTHING_CELL;
    for (int i = 0; i < game->ghost_count; i++) {
        game->field[game->ghosts[i]->x - 1][game->ghosts[i]->y - 1] =
            game->ghosts[i]->under;
        game->ghosts[i]->under = NOTHING_CELL;
    }
    game->field[game->pacman->start_position.x - 1]
               [game->pacman->start_position.y - 1] = PACMAN_CELL;
    for (int i = 0; i < game->ghost_count; i++) {
        game->field[game->ghosts[i]->start_position.x - 1]
                   [game->ghosts[i]->start_position.y - 1] = GHOST_CELL(i);
        game->ghosts[i]->x = game->ghosts[i]->start_position.x;
        game->ghosts[i]->y = game->ghosts[i]->start_position.y;
    }
    game->pacman->x = game->pacman->start_position.x;
    game->pacman->y = game->pacman->start_position.y;
}

void set_level(Game *game) {
    rebirth(game);
    set_food_to_start(game);
    game->foods.next_fruit_index = 0;
    for (int x = 0; x < game->width; x++) {
        for (int y = 0; y < game->height; y++) {
            if (game->field[x][y].object == Eaten_Food &&
                game->field[x][y].food_type != FRUIT &&
                game->field[x][y].food_type != EXTRALIVE) {
                game->field[x][y].object = Food;
            }
        }
    }

    for (int i = 0; i < game->ghost_count; i++) {
        game->ghosts[i]->speed = 450;
        game->ghosts[i]->direction = LEFT;
        game->ghosts[i]->animation_status = 0;
    }

    for (int i = 0; i < game->fruits_count; i++) {
        game->fruits[i].is_eaten = 0;
        game->fruits[i].moves_uneaten = 0;
    }

    game->pacman->animation_status = 0;
    game->pacman->direction = RIGHT;
    game->pacman->speed = 450;
}
