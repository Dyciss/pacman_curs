#pragma once
#include "pages/page.hpp"

class ScorePage: public Page {
public:
    ScorePage();
    ~ScorePage();
    void render();
    void mouse(float x, float y);
    void keyboard_special(int key, int x, int y);
    void keyboard(unsigned char key, int x, int y);
};
