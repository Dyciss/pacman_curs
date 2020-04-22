#include "ui/render.h"
#include "main.h"
#include "pages/page.h"
#include <GL/freeglut.h>
#include <stdio.h>

static int place = 0;
static int score = 0;

static Button *menu_btn = NULL;
static Button *game_btn = NULL;
static Button *settings_btn = NULL;

#define str_len 64
#define current_y (1 - line_number * FONT_HEIGHT_UPPER_CASE)
static void render() {
    char str[str_len+1] = {'\0'};
    int line_number = 1;

    glClear(GL_COLOR_BUFFER_BIT);
    snprintf(str, str_len, "Score: %i", score);
    render_string(str, 0, current_y, FONT, WHITE);
    line_number++;
    if (place) {
        snprintf(str, str_len, "Place: %i!", place);
        render_string(str, 0, current_y, FONT, GOLD);
    }
    
    render_button(menu_btn, 0, 0, WHITE, WHITE);
    render_button(game_btn, 0, 0 - 2*FONT_HEIGHT_UPPER_CASE, WHITE, WHITE);
    render_button(settings_btn, 0, 0 - 4*FONT_HEIGHT_UPPER_CASE, WHITE, WHITE);
}

static void mouse(float x, float y) {
    if (in_button(menu_btn, x, y)) {
        set_program_state(Menu);
    } else if (in_button(game_btn, x, y)) {
        set_program_state(Game_page);
    } else if (in_button(settings_btn, x, y)) {
        set_program_state(Settings);
    }
}

static void keyboard_special(int key, int x, int y) {}
static void keyboard(unsigned char key, int x, int y) {}

static void init_Result() { 
    menu_btn = new_Button("Go to Menu");
    game_btn = new_Button("New game");
    settings_btn = new_Button("Settings");
}

static void free_Result() { 
    free_Button(menu_btn);
    free_Button(game_btn);
    free_Button(settings_btn);
}

void set_result(int place_value, int score_value) {
    place = place_value;
    score = score_value;
} 

Page result_Page() {
    return (Page) {
        .render = render, .mouse = mouse, .keyboard = keyboard,
        .keyboard_special = keyboard_special, .init_Page = init_Result,
        .free_Page = free_Result
    };
}