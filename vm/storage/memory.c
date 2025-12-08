#include "memory.h"
#include "hash.h"

#include <malloc.h>

typedef struct memory {
  size_t capacity;
  hash_t* storage;
} memory_t;

void hash_destroy_instruction(void* instr) { destroy_instruction((instruction_t*)instr); }

memory_t* memory_create(size_t capacity) {
  memory_t* memory = malloc(sizeof(memory_t));
  memory->capacity = capacity;
  memory->storage = hash_create(hash_destroy_instruction);
  return memory;
}

bool memory_store(memory_t* memory, size_t address, void* value) {
  if (address >= memory->capacity) {
    return false;
  }
  return hash_store(memory->storage, address, value);
}

void* memory_get(memory_t* memory, size_t address) {
  if (address >= memory->capacity) {
    return NULL;
  }
  return hash_get(memory->storage, address);
}

void memory_destroy(memory_t* memory) {
  hash_destroy(memory->storage);
  free(memory);
}
