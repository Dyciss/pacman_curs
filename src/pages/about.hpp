#pragma once

#include "pages/page.hpp"
#include "ui/render.h"

class AboutPage: public Page {
private:
    Button *menu_btn;
public:
    void render();
    void mouse(float x, float y);
    AboutPage();
    ~AboutPage();
};
