#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdbool.h>
#include <stdlib.h>

#include "instruction_set.h"

typedef struct program program_t;

program_t* program_init(size_t capacity);

bool program_add_instruction(program_t* program, instruction_t* instruction);

instruction_t* program_step(program_t* program);

void program_destroy(program_t* program);

#endif