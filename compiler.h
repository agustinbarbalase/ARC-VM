#ifndef COMPILER_H
#define COMPILER_H

#include "instruction_set.h"

typedef struct file file_t;

file_t* init_file(const char* filename);

instruction_t* fetch(file_t* file);

void close_file(file_t* file);

#endif