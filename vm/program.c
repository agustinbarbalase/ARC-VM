
#include "program.h"
#include "hash.h"
#include <malloc.h>

typedef struct program {
  hash_t* instructions;
  size_t capacity;
  size_t size;
  size_t starting_point;
} program_t;

typedef struct program_iter {
  const program_t* program;
  size_t pos;
} program_iter_t;

program_t* program_init(size_t capacity, const size_t starting_point) {
  program_t* program = malloc(sizeof(program_t));
  program->starting_point = starting_point;
  program->instructions = hash_create(NULL);
  program->capacity = capacity;
  program->size = 0;
  return program;
}

size_t program_get_starting_point(const program_t* program) { return program->starting_point; }

bool program_add_instruction(program_t* program, instruction_t* instruction) {
  if (program->size >= program->capacity) {
    return false;
  }
  if (!hash_store(program->instructions, program->size, instruction)) {
    return false;
  }
  program->size++;
  return true;
}

void program_destroy(program_t* program) {
  hash_destroy(program->instructions);
  free(program);
}

program_iter_t* program_iter_create(const program_t* program) {
  program_iter_t* iter = malloc(sizeof(program_iter_t));
  if (!iter) return NULL;
  iter->program = program;
  iter->pos = 0;
  return iter;
}

instruction_t* program_iter_next(program_iter_t* iter) {
  if (iter->pos >= iter->program->size) {
    return NULL;
  }
  instruction_t* instr = hash_get(iter->program->instructions, iter->pos);
  iter->pos++;
  return instr;
}

void program_iter_destroy(program_iter_t* iter) { free(iter); }