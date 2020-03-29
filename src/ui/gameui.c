#include <GL/freeglut.h>

#include "directions.h"
#include "gamemodel.h"
#include "gamesizing.h"
#include "render.h"
#include <stdio.h>

void draw_right_part(Game *game) {
    float alpha_relative_x = game->alpha / (float)window_width();
    float alpha_relative_y = game->alpha / (float)window_height();
    float right_part_starts_x = game->window_x + alpha_relative_x + FONT_WIDTH;
    float top_starts_y = 1 - alpha_relative_y;

    int line_number = 1;

    render_string("PACMAN GAME!", right_part_starts_x,
                  top_starts_y - line_number * FONT_HEIGHT_UPPER_CASE, FONT,
                  WHITE);

    line_number++;

    char str[64] = {'\0'};
    sprintf(str, "Your lifes count: %i", game->lives);
    render_string(str, right_part_starts_x,
                  top_starts_y - line_number * FONT_HEIGHT_UPPER_CASE, FONT,
                  WHITE);

    line_number++;

    if (game->countdown.active) {
        sprintf(str, "We want you to calm down: %i -- %i",
                game->countdown.current_n, game->countdown.n);
        render_string(str, right_part_starts_x,
                      top_starts_y - line_number * FONT_HEIGHT_UPPER_CASE, FONT,
                      WHITE);
        line_number++;
    }
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
    Color Ghost_colors[] = {(Color){250, 120, 120}, (Color){120, 120, 210}, (Color){230, 160, 160}, (Color){120, 210, 110}};
    int Ghost_colors_len = sizeof(Ghost_colors)/sizeof(Ghost_colors[0]);
    render_Ghost(game->px_creature, x, y, (Ghost_colors[ghost_id % Ghost_colors_len]),
                 game->ghosts[ghost_id]->direction);
    int eyes_opened = game->ghosts[0]->animation_status;
    if (eyes_opened){
        render_Eyes(game->px_creature, x, y,
                 game->ghosts[ghost_id]->direction);
    }
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