#include "gamemodel.h"
#include "render.h"
#include "directions.h"

#define CELL_CREATURE_PADDING 10

void map_xy_to_window_xy(int alpha, int x, int y, float *xw, float *yw) {
    x *= alpha;
    y *= alpha;

    *xw = 2 * x / (float) window_width() - 1.0;
    *yw = 1.0 - 2 * y / (float) window_height();
}

void _set_alpha(Game *game) {
    int a1 = window_height()*(1 - FONT_HEIGHT) / game->height;
    int a2 = window_width()*(1 - FONT_WIDTH*30) / game->width; // 30 symbols in right part of screen
    game->alpha = a1 < a2 ? a1 : a2;
}

void _set_px_creature(Game *game) {
    game->px_creature = game->alpha * 2 - CELL_CREATURE_PADDING;
}

void _set_window_coordinates(Game *g, struct creature *c) {
    map_xy_to_window_xy(g->alpha, c->x, c->y, &(c->window_x), &(c->window_y));
}

void _set_window_xy(Game *g) {
    map_xy_to_window_xy(g->alpha, g->width, g->height, &g->window_x, &g->window_y);
}

void sync_sizing_props(Game *game) {
    _set_alpha(game);
    _set_window_xy(game);
    _set_px_creature(game);
    _set_window_coordinates(game, game->pacman);
    for (int ghost_id = 0; ghost_id < game->ghost_count; ghost_id++) {
        _set_window_coordinates(game, game->ghosts[ghost_id]);
    }
}

void set_ghost_count(Game *game, int count) {
    game->ghost_count = count;
    game->ghosts = (struct creature **) malloc(sizeof(struct creature)*count);
}

void init_field(Game *game, int width, int height) {
    game->width = width;
    game->height = height;

    game->field = (struct cell **) malloc(sizeof(struct cell *) * width);
    for (int i=0; i<width; i++) {
        game->field[i] = (struct cell *) calloc(height, sizeof(struct cell)); // initialized with Nothing
    }
}

Game *canonical_game() {
    Game *game = (Game*) malloc(sizeof(Game));

    game->alive = 0;
    set_ghost_count(game, 1);
    init_field(game, 28, 36);
    
    game->field[27][35].object = Wall;
    game->field[25][35].object = Food;
    game->field[25][35].food_type = LARGE;

    game->field[0][5].object = Pacman;
    game->pacman = (struct creature *) malloc(sizeof(struct creature));
    game->pacman->x = 1;
    game->pacman->y = 6;
    game->pacman->animation_status = 0;
    game->pacman->direction = RIGHT;
    game->pacman->speed = 900;

    game->field[2][2].object = Ghost;
    game->field[2][2].ghost_id = 0;
    game->ghosts[0] = (struct creature *) malloc(sizeof(struct creature));
    game->ghosts[0]->x = 3;
    game->ghosts[0]->y = 3;
    game->ghosts[0]->speed = 450;
    game->ghosts[0]->direction = LEFT;
    game->ghosts[0]->animation_status = 0;

    return game;
}
