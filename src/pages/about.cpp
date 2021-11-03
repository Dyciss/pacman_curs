extern "C" {
    #include "ui/render.h"
}

#include "main.hpp"
#include "pages/about.hpp"
#include <GL/freeglut.h>

static char about_text[] = "About program:\n"
                           "There is smt interesting I think\n"
                           "PACMAN!!";


void AboutPage::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    render_string(about_text, -1 + FONT_WIDTH, 1.0 - FONT_HEIGHT_UPPER_CASE,
                  FONT, WHITE);
    render_button(menu_btn, 0, 0, WHITE, WHITE);
}

void AboutPage::mouse(float x, float y) {
    if (in_button(menu_btn, x, y)) {
        set_program_state(Menu);
    }
}

AboutPage::AboutPage() { menu_btn = new_Button("Go to Menu"); }
AboutPage::~AboutPage() { free_Button(menu_btn); }
