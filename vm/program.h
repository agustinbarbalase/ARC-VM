#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdbool.h>
#include <stddef.h>

#include "instruction_set.h"

typedef struct program program_t;
typedef struct program_iter program_iter_t;

program_t* program_init(size_t capacity, size_t starting_point);

size_t program_get_starting_point(const program_t* program);

bool program_add_instruction(program_t* program, instruction_t* instruction);

void program_destroy(program_t* program);

program_iter_t* program_iter_create(const program_t* program);

instruction_t* program_iter_next(program_iter_t* iter);

void program_iter_destroy(program_iter_t* iter);

#endif
