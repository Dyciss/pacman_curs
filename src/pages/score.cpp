extern "C" {
    #include "score/score.h"
    #include "ui/render.h"
}

#include "main.hpp"
#include "pages/score.hpp"
#include <GL/freeglut.h>

static Button *menu_btn = NULL;

void ScorePage::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    render_button(menu_btn, 0, -1 + 2 * FONT_HEIGHT_UPPER_CASE, WHITE, WHITE);
    print_scores();
}

void ScorePage::mouse(float x, float y) {
    if (in_button(menu_btn, x, y)) {
        set_program_state(Menu);
    }
}

void ScorePage::keyboard_special(int key, int x, int y) {}
void ScorePage::keyboard(unsigned char key, int x, int y) {}

ScorePage::ScorePage() { menu_btn = new_Button("Go to Menu"); }

ScorePage::~ScorePage() { free_Button(menu_btn); }
