#include "program.h"

#include <malloc.h>

typedef struct program {
  instruction_t** instructions;
  size_t capacity;
  size_t size;
  size_t pc;
} program_t;

program_t* program_init(size_t capacity) {
  program_t* program = malloc(sizeof(program_t));
  program->instructions = malloc(sizeof(instruction_t*) * capacity);
  program->capacity = capacity;
  program->size = 0;
  program->pc = 0;
  return program;
}

bool program_add_instruction(program_t* program, instruction_t* instruction) {
  if (program->size >= program->capacity) {
    return false;
  }
  program->instructions[program->size++] = instruction;
  return true;
}

instruction_t* program_step(program_t* program) {
  if (program->pc >= program->size) {
    return NULL;
  }
  return program->instructions[program->pc++];
}

void program_destroy(program_t* program) {
  free(program->instructions);
  free(program);
}