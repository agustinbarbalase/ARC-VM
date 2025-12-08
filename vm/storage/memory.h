#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

#include "instruction_set.h"

typedef struct memory memory_t;

memory_t* memory_create(size_t capacity);

bool memory_store(memory_t* memory, size_t address, void* value);

void* memory_get(memory_t* memory, size_t address);

void memory_destroy(memory_t* memory);

#endif
