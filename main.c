

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"

#define NUM_OF_INSTRUCTIONS 6

typedef enum { R1, NUM_REGISTERS } register_t;

int registers[NUM_REGISTERS] = {0};

instruction_t* program[NUM_OF_INSTRUCTIONS];

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <program_file>\n", argv[0]);
    return 1;
  }

  file_t* file = init_file(argv[1]);
  if (!file) {
    fprintf(stderr, "Failed to open file: %s\n", argv[1]);
    return 1;
  }

  size_t pc = 0;
  bool running = true;
  instruction_t* instr = NULL;

  while (running && (instr = fetch(file)) != NULL) {
    execute_instruction(instr, registers, &running);
    printf("Current PC: %zu ", pc);
    printf("Registers: R1=%d (%#x) \n", registers[R1], registers[R1]);
    pc++;
  }

  close_file(file);
  return 0;
}
