#include "instruction_set.h"
#include <malloc.h>

static void nop_execute(int* regs, bool* running, void* data) {
  (void)regs;
  (void)data;
  *running = false;
}

instruction_t* nop() { return create_instruction(nop_execute, NULL); }
