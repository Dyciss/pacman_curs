#include "page.h"

static void render() {}

static void mouse(float x, float y) {}

static void keyboard_special(int key, int x, int y) {}
static void keyboard(unsigned char key, int x, int y) {}

static void init() {}

static void free_Settings() {}

Page settings_Page() {
    return (Page){.render = render,
                  .mouse = mouse,
                  .keyboard = keyboard,
                  .keyboard_special = keyboard_special,
                  .init_Page = init,
                  .free_Page = free_Settings};
}