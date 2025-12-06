#include "VM.h"

#include <malloc.h>
#include <stdio.h>

typedef struct vm {
  program_t* program;
  int* registers;
  bool running;
} vm_t;

vm_t* vm_init() {
  vm_t* vm = malloc(sizeof(vm_t));
  vm->program = NULL;
  vm->registers = malloc(sizeof(int) * 8);
  for (int i = 0; i < 8; i++) {
    vm->registers[i] = 0;
  }
  vm->running = false;
  return vm;
}

void vm_load_program(vm_t* vm, program_t* program) { vm->program = program; }

void vm_run_program(vm_t* vm) {
  vm->running = true;
  instruction_t* instr = NULL;

  while (vm->running && (instr = program_step(vm->program)) != NULL) {
    execute_instruction(instr, vm->registers, &vm->running);
    printf("Registers: R1=%d (%#x) \n", vm->registers[0], vm->registers[0]);
  }
}

void vm_destroy(vm_t* vm) {
  free(vm->registers);
  free(vm);
}