
#include "VM.h"

#include <malloc.h>
#include <stdio.h>

#include "storage/memory.h"

typedef struct vm {
  size_t program_counter;
  memory_t* memory;
  int* registers;
  bool running;
} vm_t;

vm_t* vm_init() {
  vm_t* vm = malloc(sizeof(vm_t));
  vm->memory = memory_create(1024);
  vm->registers = malloc(sizeof(int) * 8);
  for (int i = 0; i < 8; i++) {
    vm->registers[i] = 0;
  }
  vm->running = false;
  vm->program_counter = 0;
  return vm;
}

void vm_load_program(vm_t* vm, program_t* program) {
  program_iter_t* iter = program_iter_create(program);
  size_t addr = program_get_starting_point(program);
  instruction_t* instr = NULL;

  while ((instr = program_iter_next(iter)) != NULL) {
    memory_store(vm->memory, addr, instr);
    addr++;
  }

  vm->program_counter = program_get_starting_point(program);
  program_iter_destroy(iter);
  program_destroy(program);
}

void vm_run_program(vm_t* vm) {
  vm->running = true;
  instruction_t* instr = NULL;

  while (vm->running) {
    instr = memory_get(vm->memory, vm->program_counter);
    if (!instr) break;
    vm->program_counter += 1;
    execute_instruction(instr, vm->registers, &vm->running);
    printf("Registers: R1=%d (%#x) \n", vm->registers[1], vm->registers[1]);
  }
}

void vm_destroy(vm_t* vm) {
  memory_destroy(vm->memory);
  free(vm->registers);
  free(vm);
}
