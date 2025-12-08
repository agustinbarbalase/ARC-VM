#ifndef TERMINAL_H
#define TERMINAL_H

#include <ncurses.h>

#define TERM_HISTORY 100

void draw_terminal(WINDOW* w, const char* input);
void add_to_history(const char* cmd);

#endif
