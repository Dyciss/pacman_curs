#pragma once

#include "pages/page.hpp"

void set_result(int place_value, int score_value);
class ResultPage: public Page {
public:
    ResultPage();
    ~ResultPage();
    void render();
    void mouse(float x, float y);
    void keyboard_special(int key, int x, int y);
    void keyboard(unsigned char key, int x, int y);
};
