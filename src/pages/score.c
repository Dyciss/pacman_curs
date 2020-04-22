#include "ui/render.h"
#include "main.h"
#include "pages/page.h"
#include "score/score.h"
#include <GL/freeglut.h>

static Button *menu_btn = NULL;

static void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    render_button(menu_btn, 0, -1 + 2*FONT_HEIGHT_UPPER_CASE, WHITE, WHITE);
    print_scores();
}

static void mouse(float x, float y) {
    if (in_button(menu_btn, x, y)) {
        set_program_state(Menu);
    }
}

static void keyboard_special(int key, int x, int y) {}
static void keyboard(unsigned char key, int x, int y) {}

static void init_Score() { menu_btn = new_Button("Go to Menu"); }

static void free_Score() { free_Button(menu_btn); }

Page score_Page() {
    return (Page) {
        .render = render, .mouse = mouse, .keyboard = keyboard,
        .keyboard_special = keyboard_special, .init_Page = init_Score,
        .free_Page = free_Score
    };
}