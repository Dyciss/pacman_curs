extern "C" {
    #include "ui/render.h"
}

#include <GL/freeglut.h>
#include <stdio.h>

#include "main.hpp"
#include "pages/menu.hpp"

static char menu_str[] = "Menu page";

static Button *game_btn = NULL;
static Button *settings_btn = NULL;
static Button *about_btn = NULL;
static Button *exit_btn = NULL;
static Button *score_btn = NULL;

MenuPage::MenuPage() {
    game_btn = new_Button("New game");
    settings_btn = new_Button("Settings");
    about_btn = new_Button("About");
    exit_btn = new_Button("Exit");
    score_btn = new_Button("Scores");
}

MenuPage::~MenuPage() {
    free_Button(game_btn);
    free_Button(settings_btn);
    free_Button(about_btn);
    free_Button(exit_btn);
    free_Button(score_btn);
}

void MenuPage::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    float x = 0.0 - (sizeof menu_str - 1) * FONT_WIDTH; // center
    float y = 1.0 - FONT_HEIGHT_UPPER_CASE;             // on the top

    render_string(menu_str, x, y, FONT, WHITE);

    render_button(game_btn, 0, FONT_HEIGHT_UPPER_CASE * 2, WHITE, WHITE);
    render_button(settings_btn, 0, 0, WHITE, WHITE);
    render_button(score_btn, 0, -FONT_HEIGHT_UPPER_CASE * 2, WHITE, WHITE);
    render_button(about_btn, 0, -FONT_HEIGHT_UPPER_CASE * 4, WHITE, WHITE);
    render_button(exit_btn, 0, -FONT_HEIGHT_UPPER_CASE * 6, WHITE, WHITE);
}

void MenuPage::mouse(float x, float y) {
    if (in_button(about_btn, x, y)) {
        set_program_state(About);
    } else if (in_button(settings_btn, x, y)) {
        set_program_state(Settings);
    } else if (in_button(game_btn, x, y)) {
        set_program_state(Game_page);
    } else if (in_button(exit_btn, x, y)) {
        set_program_state(Exit);
    } else if (in_button(score_btn, x, y)) {
        set_program_state(Score_page);
    }
}

void MenuPage::keyboard_special(int key, int x, int y) {}
void MenuPage::keyboard(unsigned char key, int x, int y) {}
