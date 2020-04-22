#pragma once

enum Program_state { Menu, Game_page, Settings, Scores, About, Exit, Result_page, PROGRAM_STATES_COUNT };

void set_program_state(enum Program_state new_ps);