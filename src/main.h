#pragma once

enum Program_state{
    Menu,
    Game,
    Settings,
    Scores,
    About,
    Exit
};

void set_program_state(enum Program_state new_ps);