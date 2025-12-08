#ifndef DISASSEMBLY_H
#define DISASSEMBLY_H

#include <ncurses.h>

#define NUM_INS 12

void draw_disassembly(WINDOW* w, int current_index);

#endif
