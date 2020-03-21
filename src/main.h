#pragma once

enum Program_state{
    Menu,
    Game_page,
    Settings,
    Scores,
    About,
    Exit
};

void set_program_state(enum Program_state new_ps);