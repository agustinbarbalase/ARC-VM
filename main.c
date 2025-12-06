#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "VM.h"
#include "compiler.h"
#include "program.h"

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

  vm_t* vm = vm_init();
  program_t* program = program_init(1024);

  while (true) {
    instruction_t* instr = fetch(file);
    if (!instr) {
      break;
    }
    if (!program_add_instruction(program, instr)) {
      fprintf(stderr, "Program capacity exceeded\n");
      break;
    }
  }
  
  close_file(file);

  vm_load_program(vm, program);
  vm_run_program(vm);
  
  program_destroy(program);
  vm_destroy(vm);

  return 0;
}
