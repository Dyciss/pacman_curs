#include <GL/freeglut.h>

#include "directions.h"
#include "gamemodel.h"
#include "gamesizing.h"
#include "render.h"
#include <stdio.h>

void draw_right_part(Game *game) {
    float alpha_relative_x = game->alpha / (float)window_width();
    float alpha_relative_y = game->alpha / (float)window_height();

    render_string("PACMAN GAME!",
                  game->window_x + alpha_relative_x + FONT_WIDTH,
                  1 - alpha_relative_y - FONT_HEIGHT_UPPER_CASE, FONT, WHITE);
}

void draw_background(Game *game) {
    float alpha_relative_x = game->alpha / (float)window_width();
    float alpha_relative_y = game->alpha / (float)window_height();

    glBegin(GL_POLYGON);
    glColor3ub(GAME_BG.r, GAME_BG.g, GAME_BG.b);
    glVertex2f(-1 + alpha_relative_x, 1 - alpha_relative_y);
    glVertex2f(game->window_x + alpha_relative_x, 1 - alpha_relative_y);
    glVertex2f(game->window_x + alpha_relative_x,
               game->window_y - alpha_relative_y);
    glVertex2f(-1 + alpha_relative_x, game->window_y - alpha_relative_y);
    glEnd();
}

void draw_grid(Game *game) {
    glColor3ub(WHITE.r, WHITE.g, WHITE.b);
    glPointSize(3);
    for (int i = 1; i <= game->width; i++) {
        for (int j = 1; j <= game->height; j++) {
            float x;
            float y;
            map_xy_to_window_xy(game->alpha, i, j, &x, &y);
            glBegin(GL_POINTS);
            glVertex2f(x, y);
            glEnd();
        }
    }
}

void draw_Wall(Game *game, float x, float y) {
    render_Wall(game->alpha * 2, x, y, GAME_BG);
}

void draw_Food(Game *game, float x, float y, enum Food type) {
    render_Food(game->alpha * 2 - 1, x, y, GOLD, type);
}

void draw_Pacman(Game *game, float x, float y) {
    render_Pacman(game->px_creature, x, y, GOLD);
    int mouth_opened = game->pacman->animation_status;
    if (mouth_opened) {
        render_Pacman_mouth(game->px_creature, x, y, GAME_BG,
                            game->pacman->direction);
    }
}

void draw_Ghost(Game *game, float x, float y, int ghost_id) {
    render_Ghost(game->px_creature, x, y, RED,
                 game->ghosts[ghost_id]->direction);
}

void draw_game(Game *game) {
    draw_background(game);
    draw_right_part(game);
    for (int x = 0; x < game->width; x++) {
        for (int y = 0; y < game->height; y++) {
            struct cell c = game->field[x][y];
            float x_window;
            float y_window;
            map_xy_to_window_xy(game->alpha, x + 1, y + 1, &x_window,
                                &y_window);
            switch (c.object) {
            case Nothing:
                break;
            case Wall:
                draw_Wall(game, x_window, y_window);
                break;
            case Food:
                draw_Food(game, x_window, y_window, c.food_type);
                break;
            case Pacman:
                draw_Pacman(game, x_window, y_window);
                break;
            case Ghost:
                draw_Ghost(game, x_window, y_window, c.ghost_id);
                break;
            default:
                break;
            }
        }
    }
    draw_grid(game);
}