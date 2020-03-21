#include <GL/freeglut.h>
#include <stdio.h>

#include "render.h"
#include "main.h"

char menu_str[] = "Menu page";

Button *game_btn = NULL;
Button *settings_btn = NULL; 
Button *about_btn = NULL;
Button *exit_btn = NULL;

void init_Menu() {
    game_btn = new_Button("New game");
    settings_btn = new_Button("Settings");
    about_btn = new_Button("About");
    exit_btn = new_Button("Exit");
}

void free_Menu() {
    free_Button(game_btn);
    free_Button(settings_btn);
    free_Button(about_btn);
    free_Button(exit_btn);
}

void render_Menu() {
    glClear(GL_COLOR_BUFFER_BIT);
    float x = 0.0 - (sizeof menu_str - 1) * FONT_WIDTH; // center
    float y = 1.0 - FONT_HEIGHT_UPPER_CASE; // on the top

    render_string(menu_str, x, y, FONT, WHITE);

    render_button(game_btn, 0, FONT_HEIGHT_UPPER_CASE*2, WHITE, WHITE);
    render_button(settings_btn, 0, 0, WHITE, WHITE);
    render_button(about_btn, 0, -FONT_HEIGHT_UPPER_CASE*2, WHITE, WHITE);
    render_button(exit_btn, 0, -FONT_HEIGHT_UPPER_CASE*4, WHITE, WHITE);
}

void mouse_Menu(float x, float y) {
    if (in_button(about_btn, x, y)) {
        set_program_state(About);
    }
    else if (in_button(settings_btn, x, y)) {
        set_program_state(Settings);
    }
    else if (in_button(game_btn, x, y)) {
        set_program_state(Game_page);
    }
    else if (in_button(exit_btn, x, y)) {
        set_program_state(Exit);
    }
}