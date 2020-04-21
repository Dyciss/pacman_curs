#include "ui/render.h"
#include "game/gamemodel.h"

#define CELL_CREATURE_PADDING 10

void map_xy_to_window_xy(int alpha, int x, int y, float *xw, float *yw) {
    x *= alpha;
    y *= alpha;

    *xw = 2 * x / (float)window_width() - 1.0;
    *yw = 1.0 - 2 * y / (float)window_height();
}

static void set_alpha(Game *game) {
    // +1 is game margin;
    int a1 = window_height() * (1 - FONT_HEIGHT) / (game->height + 1);
    int a2 = window_width() * (1 - FONT_WIDTH * 30) /
             (game->width + 1); // 30 symbols in right part of screen
    game->alpha = a1 < a2 ? a1 : a2;
}

static void set_px_creature(Game *game) {
    game->px_creature = game->alpha * 2 - CELL_CREATURE_PADDING;
}

static void set_window_xy(Game *g) {
    map_xy_to_window_xy(g->alpha, g->width, g->height, &g->window_x,
                        &g->window_y);
}

void sync_sizing_props(Game *game) {
    set_alpha(game);
    set_window_xy(game);
    set_px_creature(game);
}