#include "instruction_set.h"
#include <malloc.h>

typedef struct instruction {
  execute_fn_t execute;
  void* data;
} instruction_t;

instruction_t* create_instruction(execute_fn_t execute, void* data) {
  instruction_t* inst = malloc(sizeof(instruction_t));
  inst->execute = execute;
  inst->data = data;
  return inst;
}

void destroy_instruction(instruction_t* instr) {
  if (instr->data) {
    free(instr->data);
  }
  free(instr);
}

void execute_instruction(instruction_t* instr, int* registers, bool* running) {
  instr->execute(registers, running, instr->data);
  destroy_instruction(instr);
}
