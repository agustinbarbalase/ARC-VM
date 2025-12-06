#ifndef VM_H
#define VM_H

#include "program.h"

typedef struct vm vm_t;

vm_t* vm_init();

void vm_load_program(vm_t* vm, program_t* program);

void vm_run_program(vm_t* vm);

void vm_destroy(vm_t* vm);

#endif