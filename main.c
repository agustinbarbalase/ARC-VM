
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "instruction_set.h"

#define NUM_OF_INSTRUCTIONS 6

typedef enum { R1, NUM_REGISTERS } register_t;

int registers[NUM_REGISTERS] = {0};

instruction_t* program[NUM_OF_INSTRUCTIONS];

instruction_t* fetch(size_t pc) { return program[pc]; }

void execute(instruction_t* instr, bool* running) {
  execute_instruction(instr, registers, running);
}

int main(void) {
  size_t pc = 0;
  bool running = true;

  program[0] = addcc(1, 1, R1);
  program[1] = srl(1, 8, R1);
  program[2] = andcc(1, 1, R1);
  program[3] = orcc(1, 0, R1);
  program[4] = orncc(1, 0, R1);
  program[5] = nop(0, 0, 0);

  while (running && pc < NUM_OF_INSTRUCTIONS) {
    instruction_t* instr = fetch(pc);

    printf("PC: %zu ", pc);
    printf("Registers: R1=%d (%#x)\n", registers[R1], registers[R1]);
    execute(instr, &running);

    pc++;
  }

  return 0;
}
