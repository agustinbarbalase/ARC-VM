
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_OF_INSTRUCTIONS 2

typedef enum {
  NOP,
  ADDCC,
} instruction_t;

typedef enum { R1, NUM_REGISTERS } register_t;

typedef struct {
  instruction_t opcode;
  int src1;
  int src2;
  register_t dest;
} instr_t;

const instr_t program[NUM_OF_INSTRUCTIONS] = {{ADDCC, 2, 2, R1}, {NOP, 0, 0, 0}};

register_t registers[NUM_REGISTERS] = {0};

instr_t fetch(size_t pc) { return program[pc]; }

void execute(instr_t instr, bool* running) {
  switch (instr.opcode) {
  case NOP:
    *running = false;
    break;
  case ADDCC:
    registers[instr.dest] = instr.src1 + instr.src2;
    break;
  default:
    *running = false;
    break;
  }
}

int main(void) {
  size_t pc = 0;
  bool running = true;

  while (running && pc < NUM_OF_INSTRUCTIONS) {
    instr_t instr = fetch(pc);

    printf("PC: %zu, INSTR: %d\n", pc, instr.opcode);
    printf("Registers: R1=%d\n", registers[R1]);
    execute(instr, &running);

    pc++;
  }

  return 0;
}
