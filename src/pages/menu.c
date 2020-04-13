#include <GL/freeglut.h>
#include <stdio.h>

#include "./ui/render.h"
#include "main.h"
#include "page.h"

static char menu_str[] = "Menu page";

static Button *game_btn = NULL;
static Button *settings_btn = NULL;
static Button *about_btn = NULL;
static Button *exit_btn = NULL;

static void init() {
    game_btn = new_Button("New game");
    settings_btn = new_Button("Settings");
    about_btn = new_Button("About");
    exit_btn = new_Button("Exit");
}

static void free_Menu() {
    free_Button(game_btn);
    free_Button(settings_btn);
    free_Button(about_btn);
    free_Button(exit_btn);
}

static void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    float x = 0.0 - (sizeof menu_str - 1) * FONT_WIDTH; // center
    float y = 1.0 - FONT_HEIGHT_UPPER_CASE;             // on the top

    render_string(menu_str, x, y, FONT, WHITE);

    render_button(game_btn, 0, FONT_HEIGHT_UPPER_CASE * 2, WHITE, WHITE);
    render_button(settings_btn, 0, 0, WHITE, WHITE);
    render_button(about_btn, 0, -FONT_HEIGHT_UPPER_CASE * 2, WHITE, WHITE);
    render_button(exit_btn, 0, -FONT_HEIGHT_UPPER_CASE * 4, WHITE, WHITE);
}

static void mouse(float x, float y) {
    if (in_button(about_btn, x, y)) {
        set_program_state(About);
    } else if (in_button(settings_btn, x, y)) {
        set_program_state(Settings);
    } else if (in_button(game_btn, x, y)) {
        set_program_state(Game_page);
    } else if (in_button(exit_btn, x, y)) {
        set_program_state(Exit);
    }
}

static void keyboard_special(int key, int x, int y) {}
static void keyboard(unsigned char key, int x, int y) {}

Page menu_Page() {
    return (Page){.render = render,
                  .mouse = mouse,
                  .keyboard = keyboard,
                  .keyboard_special = keyboard_special,
                  .init_Page = init,
                  .free_Page = free_Menu};
}