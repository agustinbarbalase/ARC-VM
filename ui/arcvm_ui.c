#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "components/disassembly.h"
#include "components/registers.h"
#include "components/terminal.h"

// Draw vertical border
void draw_border(WINDOW* w) {
  int h, width;
  getmaxyx(w, h, width);
  for (int y = 0; y < h; y++)
    mvwaddch(w, y, 0, ACS_VLINE);
  wrefresh(w);
}

int main(void) {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(1);
  if (!has_colors()) {
    endwin();
    fprintf(stderr, "No color support\n");
    return 1;
  }
  start_color();
  use_default_colors();
  init_pair(10, COLOR_YELLOW, -1);
  init_pair(2, COLOR_CYAN, -1);
  init_pair(3, COLOR_GREEN, -1);
  init_pair(4, COLOR_WHITE, -1);
  init_pair(5, COLOR_MAGENTA, -1);
  init_pair(6, COLOR_WHITE, COLOR_BLUE);

  int h, w;
  getmaxyx(stdscr, h, w);
  int left_w = w / 2;
  int right_w = w - left_w - 1;
  int reg_h = h * 0.6;
  int term_h = h - reg_h;

  WINDOW* left = newwin(h, left_w, 0, 0);
  WINDOW* border = newwin(h, 1, 0, left_w);
  WINDOW* right_regs = newwin(reg_h, right_w, 0, left_w + 1);
  WINDOW* terminal = newwin(term_h, right_w, reg_h, left_w + 1);

  int current = 0;
  char input[256];
  int input_len = 0;
  memset(input, 0, sizeof(input));

  while (1) {
    draw_disassembly(left, current);
    draw_registers(right_regs);
    draw_border(border);
    draw_terminal(terminal, input);

    int ch = wgetch(terminal);
    if (ch == KEY_UP && current > 0)
      current--;
    else if (ch == KEY_DOWN && current < NUM_INS - 1)
      current++;
    else if (ch == 10 || ch == KEY_ENTER) { // Enter pressed
      input[input_len] = '\0';
      if (input_len > 0) {
        add_to_history(input);
      }
      if (strcmp(input, "quit") == 0) break;
      input_len = 0;
      memset(input, 0, sizeof(input));
    } else if (ch == KEY_BACKSPACE || ch == 127) {
      if (input_len > 0) input_len--;
      input[input_len] = '\0';
    } else if (ch >= 32 && ch <= 126) {
      if (input_len < 255) {
        input[input_len] = ch;
        input_len++;
        input[input_len] = '\0';
      }
    }
  }

  delwin(left);
  delwin(right_regs);
  delwin(border);
  delwin(terminal);
  endwin();
  return 0;
}
