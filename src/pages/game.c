#include <GL/freeglut.h>
#include <stdio.h>
#include <time.h>

#include "main.h"
#include "pages/page.h"

#include "ui/gameui.h"
#include "ui/render.h"
#include "ui/windowsize.h"

#include "directions/directions.h"
#include "engine/engine.h"
#include "ui/gamesizing.h"
#include "pages/result.h"

#include "score/score.h"
#include "settings/settings.h"

static Game *game = NULL;

static char escape_btn_text[] = "escape";
static Button *escape_btn = NULL;

static int fps = 30;

static void animate_pacman() {
    if (!game || !game->alive)
        return;
    game->pacman->animation_status = !game->pacman->animation_status;
    glutTimerFunc(1000.0 / 3, animate_pacman, 0);
}

static void animate_Ghost(int id) {
    if (!game || !game->alive)
        return;
    game->ghosts[id]->animation_status = !game->ghosts[id]->animation_status;
    glutTimerFunc(1000.0 / 3, animate_Ghost, id);
}

static void escape_Game() {
    if (game->level > 10) {
        game->score += game->lives * 10000;
    }
    int place = add_to_scoreboard(game);
    int score = game->score;
    free_engine(game);
    free_Game(game);
    game = NULL;
    set_result(place, score);
    set_program_state(Result_page);
}

static void rebirth_game() { rebirth(game); }
static void set_level_game() { set_level(game); }

static int maybe_new_level() {
    if (game->foods.count_now == 0) {
        game->level++;
        if (game->level > 10) {
            game->alive = 0;
            escape_Game();
            return 1;
        }
        start_countdown(game);
        set_level_game();
        return 1;
    }
    return 0;
}

static void move_pacman() {
    if (!game || !game->alive || !game->lives)
        return;

    if (game->countdown.active || game->pause) {
        glutTimerFunc(60 * 1000.0 / game->pacman->speed, move_pacman, 0);
        return;
    }

    int new_x;
    int new_y;

    if (!set_new_xy(game, game->pacman, &new_x, &new_y)) { // new xy is new
        glutTimerFunc(60 * 1000.0 / game->pacman->speed, move_pacman, 0);
        return;
    }

    for (int i = 0; i < game->foods.next_fruit_index; i++) {
        if (game->fruits[i].is_eaten)
            continue;
        game->fruits[i].moves_uneaten++;
        if (game->fruits[i].moves_uneaten > (game->width + game->height)) {
            game->fruits[i].is_eaten = 1;
            game->foods.count_now--; // without score
            int x = game->fruits[i].x;
            int y = game->fruits[i].y;
            game->field[x][y].object = Eaten_Food;
            if (maybe_new_level()) {
                if (game && game->alive) {
                    glutTimerFunc(60 * 1000.0 / game->pacman->speed,
                                  move_pacman, 0);
                }
                return;
            }
        }
    }

    if (!game->extra_live.eaten && game->level == game->extra_live.when_level) {
        int x = game->extra_live.x;
        int y = game->extra_live.y;
        if (game->field[x][y].object == Eaten_Food) {
            game->extra_live.moves_unvisible++;
            if (game->extra_live.moves_unvisible >=
                (game->width + game->height)) {
                game->field[x][y].object = Food;
                game->foods.count_now++;
            }
        } else {
            game->extra_live.moves_uneaten++;
            if (game->extra_live.moves_uneaten >=
                (game->width + game->height) * 2 / 3) {
                game->field[x][y].object = Eaten_Food;
                game->extra_live.eaten = 1;
                game->foods.count_now--;
                if (maybe_new_level()) {
                    if (game && game->alive) {
                        glutTimerFunc(60 * 1000.0 / game->pacman->speed,
                                      move_pacman, 0);
                    }
                    return;
                }
            }
        }
    }

    if (game->fear_moves_now) {
        game->fear_moves_now--;
        if (!game->fear_moves_now) {
            for (int i = 0; i < game->ghost_count; i++) {
                game->ghost_fear[i] = 0;
            }
        }
    }

    if (game->field[new_x][new_y].object == Wall) {
        glutTimerFunc(60 * 1000.0 / game->pacman->speed, move_pacman, 0);
        return;
    }

    game->field[game->pacman->x][game->pacman->y] = game->pacman->under;
    game->pacman->x = new_x;
    game->pacman->y = new_y;

    game->pacman->under = game->field[new_x][new_y];
    game->field[new_x][new_y] = PACMAN_CELL;

    if (game->pacman->under.object == Pacman) { // not creatures under pacman
        game->pacman->under = NOTHING_CELL;     // can't be here(new xy is new)
    }

    else if (game->pacman->under.object == Ghost) {
        while (game->pacman->under.object == Ghost) {
            int id = game->pacman->under.ghost_id;
            game->pacman->under = game->ghosts[id]->under;

            if (game->ghost_fear[id]) {
                int start_x = game->ghosts[id]->start_position.x;
                int start_y = game->ghosts[id]->start_position.y;
                game->ghost_fear[id] = 0;

                if (start_x == new_x && start_y == new_y) { // lol
                    game->pacman->under = game->ghosts[id]->under;
                    continue;
                }

                game->ghosts[id]->under = game->field[start_x][start_y];
                game->field[start_x][start_y] = GHOST_CELL(id);
                game->ghosts[id]->x = start_x;
                game->ghosts[id]->y = start_y;

                int l = game->level;
                game->score += 5 * l * l;

                for (int i = 0; i < game->ghost_count; i++) {
                    if (game->ghosts[i]->x == game->pacman->x &&
                        game->ghosts[i]->y == game->pacman->y) {
                        game->pacman->under = GHOST_CELL(i);
                        break;
                    }
                }
            } else {
                game->field[new_x][new_y] = GHOST_CELL(id);
                game->lives--;

                if (game->lives) {
                    start_countdown(game);
                    rebirth_game();
                } else {
                    escape_Game();
                    return;
                }
                break;
            }
        }
    }
    // not else if because under feared may be Food.
    if (game->pacman->under.object == Food) {
        game->pacman->under.object = Eaten_Food;
        game->foods.count_now--;
        if (game->pacman->under.food_type == SMALL) {
            game->score += game->level;
        } else if (game->pacman->under.food_type == ENERGIZER) {
            game->score += game->level * game->level;
            int l = game->level;
            game->fear_moves_now = (int)(game->height + game->width) *
                                   (0.0089 * l * l - 0.208 * l + 1.2);
            if (game->fear_moves_now) {
                for (int i = 0; i < game->ghost_count; i++) {
                    game->ghost_fear[i] = 1;
                    game->ghosts[i]->direction =
                        opposite_direciton(game->ghosts[i]->direction);
                }
            }
        } else if (game->pacman->under.food_type == FRUIT) {
            int l = game->level;
            game->score += 44 * l * l + 56 * l + 0;
            for (int i = 0; i < game->foods.next_fruit_index; i++) {
                if (game->fruits[i].x == new_x && game->fruits[i].y == new_y) {
                    game->fruits[i].is_eaten = 1;
                }
            }
        } else if (game->pacman->under.food_type == EXTRALIVE &&
                   !game->extra_live.eaten) {
            game->extra_live.eaten = 1;
            game->lives++;
        }
        check_fruits(game);
        if (maybe_new_level()) {
            if (game && game->alive) {
                glutTimerFunc(60 * 1000.0 / game->pacman->speed, move_pacman,
                              0);
            }
            return;
        }
    }
    glutTimerFunc(60 * 1000.0 / game->pacman->speed, move_pacman, 0);
}

static void move_Ghost(int id) {
    if (!game || !game->alive || !game->lives)
        return;

    if (game->countdown.active || game->pause) {
        glutTimerFunc(60 * 1000.0 / game->ghosts[id]->speed, move_Ghost, id);
        return;
    }

    set_Ghost_direction(game, id);
    int new_x;
    int new_y;

    if (!set_new_xy(game, game->ghosts[id], &new_x, &new_y)) {
        glutTimerFunc(60 * 1000.0 / game->ghosts[id]->speed, move_Ghost, id);
        return;
    }

    if (game->field[new_x][new_y].object == Wall) {
        glutTimerFunc(60 * 1000.0 / game->ghosts[id]->speed, move_Ghost, id);
        return;
    }
    game->field[game->ghosts[id]->x][game->ghosts[id]->y] =
        game->ghosts[id]->under;

    if (game->ghost_fear[id] && game->field[new_x][new_y].object == Pacman) {
        new_x = game->ghosts[id]->start_position.x;
        new_y = game->ghosts[id]->start_position.y;
        game->ghost_fear[id] = 0;
        int l = game->level;
        game->score += 5 * l * l;
    }

    game->ghosts[id]->x = new_x;
    game->ghosts[id]->y = new_y;

    game->ghosts[id]->under = game->field[new_x][new_y];
    game->field[new_x][new_y] = GHOST_CELL(id);

    if (game->ghosts[id]->under.object == Pacman) { // not creatures under ghost
        game->ghosts[id]->under = game->pacman->under;
        game->lives--;

        if (game->lives) {
            start_countdown(game);
            rebirth_game();
        } else {
            escape_Game();
            return;
        }
    } else if (game->ghosts[id]->under.object == Ghost) {
        int ghost_under_id = game->ghosts[id]->under.ghost_id;
        game->ghosts[id]->under = game->ghosts[ghost_under_id]->under;
    }

    glutTimerFunc(60 * 1000.0 / game->ghosts[id]->speed, move_Ghost, id);
}

void countdown_tick() {
    if (!game || !game->alive)
        return;
    game->countdown.current_n++;
    game->countdown.runned = 0;
    if (game->countdown.current_n > game->countdown.n) {
        stop_countdown(game);
    }
}

static void frame() {
    if (!game || !game->alive)
        return;

    if (game->countdown.active && !game->countdown.runned) {
        game->countdown.runned = 1;
        glutTimerFunc(game->countdown.ms, countdown_tick, 0);
    }

    glClear(GL_COLOR_BUFFER_BIT);
    render_button(escape_btn, 1 - FONT_WIDTH * (sizeof escape_btn_text),
                  -1 + FONT_HEIGHT_UPPER_CASE, WHITE, WHITE);
    draw_game(game);

    glutTimerFunc(1000.0 / fps, frame, 0);
    glutSwapBuffers();
}

static void render() {
    if (game) {
        sync_sizing_props(game);
        return;
    }

    game = (Game *)calloc(1, sizeof(Game)); // NULL in pacman, ghosts, ...
    char *fname = settings_field(Load_file)->text;
    if (!file2Game(game, fname)) {
        fprintf(stderr, "Error while loading game from %s\n", fname);
        escape_Game();
        return;
    }
    sync_sizing_props(game);
    init_engine(game);
    game->alive = 1;
    animate_pacman();
    for (int i = 0; i < game->ghost_count; i++) {
        animate_Ghost(i);
    }
    frame();
    glutTimerFunc(60 * 1000.0 / game->pacman->speed, move_pacman, 0);
    for (int i = 0; i < game->ghost_count; i++) {
        glutTimerFunc(60 * 1000.0 / game->ghosts[i]->speed, move_Ghost, i);
    }
}

static void mouse(float x, float y) {
    // game->alive check prevents double click
    if (in_button(escape_btn, x, y) && game->alive) {
        game->alive = 0; // signal for stop
        escape_Game();
        return;
    }
}

static void keyboard_special(int key, int x, int y) {
    if (key == GLUT_KEY_F1 && game->pause) {
        time_t t = time(NULL);
        struct tm *t_info;
        time(&t);
        t_info = localtime(&t);
        size_t fname_len = settings_field(Load_file)->max_len + 40;
        char *fname = (char *)calloc(fname_len + 1, sizeof(char));
        snprintf(fname, fname_len, "%s%s.txt", settings_field(Load_file)->text,
                 asctime(t_info));
        if (!Game2file(game, fname)) {
            fprintf(stderr, "Error while saving game to %s\n", fname);
        }

        free(fname);
        return;
    }
    Direction new_direction = direction_from_special_key(key);
    if (new_direction) {
        game->pacman->direction = new_direction;
    }
}

static void keyboard(unsigned char key, int x, int y) {
    if (key == 'r' && game->countdown.active) {
        stop_countdown(game);
        return;
    }
    if (key == 'p') {
        if (game->pause) {
            stop_pause(game);
        } else {
            start_pause(game);
        }
        return;
    }
    Direction new_direction = direction_from_key(key);
    if (new_direction) {
        game->pacman->direction = new_direction;
        return;
    }
}
static void init_Game_Page() { escape_btn = new_Button(escape_btn_text); }

static void free_Game_Page() {
    free_Button(escape_btn);
    if (game != NULL) {
        game->alive = 0;
        escape_Game();
    }
}

Page game_Page() {
    return (Page){.render = render,
                  .mouse = mouse,
                  .keyboard = keyboard,
                  .keyboard_special = keyboard_special,
                  .init_Page = init_Game_Page,
                  .free_Page = free_Game_Page};
}