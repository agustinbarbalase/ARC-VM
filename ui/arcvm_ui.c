#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define NUM_INS 12
#define NUM_REGS 32
#define TERM_HISTORY 100

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
const char* mnemonic[NUM_INS] = {"push %rbp",   "mov %rsp,%rbp",         "sub $0x10,%rsp",
                                 "mov $1,%rdi", "lea hello_string,%rsi", "mov $14,%rdx",
                                 "call print",  "xor %rdi,%rdi",         "call exit",
                                 "ret",         "add $1,%rax",           "int3"};
int registers_values[NUM_REGS] = {0x10,  0x20,  0x30,  0x40,  0x50,  0x60,  0x70,  0x80,
                                  0x90,  0xA0,  0xB0,  0xC0,  0xD0,  0xE0,  0xF0,  0x100,
                                  0x110, 0x120, 0x130, 0x140, 0x150, 0x160, 0x170, 0x180,
                                  0x190, 0x1A0, 0x1B0, 0x1C0, 0x1D0, 0x1E0, 0x1F0, 0x200};

// Terminal history
char history[TERM_HISTORY][256];
int history_count = 0;

// Draw disassembly
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

// Draw registers
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

// Draw vertical border
void draw_border(WINDOW* w) {
  int h, width;
  getmaxyx(w, h, width);
  for (int y = 0; y < h; y++)
    mvwaddch(w, y, 0, ACS_VLINE);
  wrefresh(w);
}

// Draw terminal with prompt and history
void draw_terminal(WINDOW* w, const char* input) {
  werase(w);
  int h, width;
  getmaxyx(w, h, width);
  wattron(w, A_BOLD);
  mvwprintw(w, 0, 1, "Terminal");
  wattroff(w, A_BOLD);
  for (int x = 0; x < width - 2; x++)
    mvwaddch(w, 1, x + 1, '-');

  // Show history
  int start = history_count > h - 4 ? history_count - (h - 4) : 0;
  for (int i = start; i < history_count; i++) {
    mvwprintw(w, 2 + (i - start), 1, "%s", history[i]);
  }

  // Prompt with a space after '>'
  mvwprintw(w, h - 1, 1, "> %s", input);
  wmove(w, h - 1, 3 + strlen(input)); // move cursor after '> '
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
        strncpy(history[history_count % TERM_HISTORY], input, 255);
        history[history_count % TERM_HISTORY][255] = '\0';
        history_count++;
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
