#include "registers.h"

int registers_values[NUM_REGS] = {0x10,  0x20,  0x30,  0x40,  0x50,  0x60,  0x70,  0x80,
                                  0x90,  0xA0,  0xB0,  0xC0,  0xD0,  0xE0,  0xF0,  0x100,
                                  0x110, 0x120, 0x130, 0x140, 0x150, 0x160, 0x170, 0x180,
                                  0x190, 0x1A0, 0x1B0, 0x1C0, 0x1D0, 0x1E0, 0x1F0, 0x200};

void draw_registers(WINDOW* w) {
  werase(w);
  int h, width;
  getmaxyx(w, h, width);
  int margin = 2;
  wattron(w, A_BOLD);
  mvwprintw(w, 0, margin, "Registers");
  wattroff(w, A_BOLD);
  for (int x = 0; x < width - margin; x++)
    mvwaddch(w, 1, x + margin, '-');

  const int REG_PER_COL = 8;
  const int NUM_COLS = 4;
  int col_width = (width - margin * 2) / NUM_COLS;

  for (int i = 0; i < NUM_REGS; i++) {
    int col = i / REG_PER_COL;
    int row = (i % REG_PER_COL) + 2;
    int x = col * col_width + margin;
    if (row < h) {
      // Register name (cyan)
      wattron(w, COLOR_PAIR(2) | A_BOLD);
      mvwprintw(w, row, x, "R%-2d", i);
      wattroff(w, COLOR_PAIR(2) | A_BOLD);

      // Register value (yellow)
      wattron(w, COLOR_PAIR(10));
      mvwprintw(w, row, x + 5, "= 0x%08X", registers_values[i]);
      wattroff(w, COLOR_PAIR(10));
    }
  }
  wrefresh(w);
}
