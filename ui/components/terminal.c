#include "terminal.h"
#include <string.h>

char history[TERM_HISTORY][256];
int history_count = 0;

void add_to_history(const char* cmd) {
    strncpy(history[history_count % TERM_HISTORY], cmd, 255);
    history[history_count % TERM_HISTORY][255] = '\0';
    history_count++;
}

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
