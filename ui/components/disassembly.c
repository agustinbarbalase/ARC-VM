#include "disassembly.h"
#include <string.h>

// Sample data
const char* addresses[NUM_INS] = {"0x0000", "0x0004", "0x0008", "0x000C", "0x0010", "0x0014",
                                  "0x0018", "0x001C", "0x0020", "0x0024", "0x0028", "0x002C"};
const char* bytes_str[NUM_INS] = {"55",
                                  "48 89 E5",
                                  "48 83 EC 10",
                                  "48 C7 C0 01 00",
                                  "48 8D 35 12 00",
                                  "48 C7 C2 0E 00",
                                  "E8 00 00 00 00",
                                  "48 31 FF",
                                  "E8 00 00 00 00",
                                  "C3",
                                  "48 83 C0 01",
                                  "FF"};
const char* mnemonic[NUM_INS] = {"push %rbp",   "mov %rsp,%rbp",         "sub /bin/bashx10,%rsp",
                                 "mov ,%rdi", "lea hello_string,%rsi", "mov 4,%rdx",
                                 "call print",  "xor %rdi,%rdi",         "call exit",
                                 "ret",         "add ,%rax",           "int3"};

void draw_disassembly(WINDOW* w, int current_index) {
  werase(w);
  int h, wid;
  getmaxyx(w, h, wid);
  wattron(w, A_BOLD);
  mvwprintw(w, 0, 2, "Disassembly");
  wattroff(w, A_BOLD);
  for (int x = 0; x < wid; x++)
    mvwaddch(w, 1, x, '-');
  int start_row = 3;
  for (int i = 0; i < NUM_INS; i++) {
    int row = start_row + i;
    if (row >= h) break;
    if (i == current_index) {
      wattron(w, COLOR_PAIR(10) | A_BOLD);
      mvwprintw(w, row, 0, ">");
      mvwprintw(w, row, 2, "%s", addresses[i]);
      mvwprintw(w, row, 14, "%-20s", bytes_str[i]);
      mvwprintw(w, row, 36, "%s", mnemonic[i]);
      wattroff(w, COLOR_PAIR(10) | A_BOLD);
    } else {
      wattron(w, COLOR_PAIR(2));
      mvwprintw(w, row, 2, "%s", addresses[i]);
      wattroff(w, COLOR_PAIR(2));
      wattron(w, COLOR_PAIR(3));
      mvwprintw(w, row, 14, "%-20s", bytes_str[i]);
      wattroff(w, COLOR_PAIR(3));
      wattron(w, COLOR_PAIR(4));
      mvwprintw(w, row, 36, "%s", mnemonic[i]);
      wattroff(w, COLOR_PAIR(4));
    }
  }
  wrefresh(w);
}
