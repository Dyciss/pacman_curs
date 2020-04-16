#include <GL/freeglut.h>

#include "game/gamemodel.h"
#include "render.h"
#include "ui/gamesizing.h"
#include <stdio.h>
#include <string.h>

#define str_len 64
#define current_y (top_starts_y - line_number * FONT_HEIGHT_UPPER_CASE)
#define render_str(str)                                                        \
    (render_string((str), right_part_starts_x, current_y, FONT, WHITE));

void draw_right_part(Game *game) {
    float alpha_relative_x = game->alpha / (float)window_width();
    float alpha_relative_y = game->alpha / (float)window_height();
    float right_part_starts_x = game->window_x + alpha_relative_x + FONT_WIDTH;
    float top_starts_y = 1 - alpha_relative_y;

    int line_number = 1;

    char str[str_len] = {'\0'};
    strncpy(str, "PACMAN GAME!", str_len);
    render_str(str);

    line_number++;

    snprintf(str, str_len, "Your lifes count: %i", game->lives);
    render_str(str);

    line_number++;

    snprintf(str, str_len, "Level: %i", game->level);
    render_str(str);

    line_number++;

    snprintf(str, str_len, "Score: %i", game->score);
    render_str(str);

    line_number++;

    snprintf(str, str_len, "Difficalty: %i", game->difficalty);
    render_str(str);

    line_number++;

    snprintf(str, str_len, "Food: %i", game->foods.count_now);
    render_str(str);

    line_number++;

    if (game->level == game->extra_live.when_level && !game->extra_live.eaten) {
        snprintf(str, str_len, "Extra life in this level!");
        render_str(str);
        line_number++;

        int a = game->width + game->height - game->extra_live.moves_unvisible;
        if (a > 0) {
            snprintf(str, str_len, "%i moves before appearance", a);
        } else {
            a = (game->width + game->height) * 2 / 3 -
                game->extra_live.moves_uneaten;
            ;
            snprintf(str, str_len, "%i moves before expires", a);
        }
        render_str(str);
        line_number++;
    }

    if (game->fear_moves_now) {
        snprintf(str, str_len, "Fear! Moves you have: %i",
                 game->fear_moves_now);
        render_str(str);
        line_number++;
    }

    if (game->countdown.active) {
        snprintf(str, str_len, "We want you to calm down: %i -- %i",
                 game->countdown.current_n, game->countdown.n);
        render_str(str);
        line_number++;
        strncpy(str, "Click [r] to start immediately", str_len);
        render_str(str);
        line_number++;
    }

    if (game->pause) {
        strncpy(str, "Click [p] to stop pause", str_len);
        render_str(str);
        line_number++;
        strncpy(str, "Click [F1] to save the game", str_len);
        render_str(str);
        line_number++;
    } else {
        strncpy(str, "Click [p] to pause the game", str_len);
        render_str(str);
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
    Color c = type == EXTRALIVE ? (Color){42, 199, 199} : GOLD;
    render_Food(game->alpha * 2 - 1, x, y, c, type);
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
    Color Ghost_colors[] = {(Color){250, 60, 60}, (Color){120, 120, 210},
                            (Color){250, 20, 147}, (Color){120, 210, 110}};
    int Ghost_colors_len = sizeof(Ghost_colors) / sizeof(Ghost_colors[0]);
    int gray =
        (127 + 40) * (game->fear_moves_now) / (game->height + game->width) +
        (128 - 40);
    Color fear_color = {gray, gray, gray};
    Color c = game->ghost_fear[ghost_id]
                  ? (fear_color)
                  : Ghost_colors[ghost_id % Ghost_colors_len];
    render_Ghost(game->px_creature, x, y, c, game->ghosts[ghost_id]->direction);
    int eyes_opened = game->ghosts[0]->animation_status;
    if (eyes_opened) {
        render_Eyes(game->px_creature, x, y, game->ghosts[ghost_id]->direction);
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
            case Eaten_Food:
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
    // draw_grid(game);
}