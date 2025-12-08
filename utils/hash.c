#define _POSIX_C_SOURCE 200809L
#include "hash.h"
#include <stdlib.h>
#include <string.h>

#define INIT_CAPACITY 10
#define RESIZE_FACTOR 2
#define REDUCTION_FACTOR 2
#define MAX_LOAD_FACTOR 0.7f
#define MIN_LOAD_FACTOR 0.3f

typedef enum { EMPTY, DELETED, BUSY } hash_node_state_t;

typedef struct hash_node {
  hash_node_state_t state;
  size_t key;
  void* data;
} hash_node_t;

struct hash {
  hash_node_t* table;
  size_t size;
  size_t deleted_count;
  size_t capacity;
  hash_destroy_t destroy_fn;
};

size_t hash_function(size_t key, size_t length) {
  key = (~key) + (key << 21);
  key = key ^ (key >> 24);
  key = (key + (key << 3)) + (key << 8);
  key = key ^ (key >> 14);
  key = (key + (key << 2)) + (key << 4);
  key = key ^ (key >> 28);
  key = key + (key << 31);
  return key % length;
}

void set_table_empty(hash_node_t* table, size_t capacity) {
  for (size_t i = 0; i < capacity; i++) {
    table[i].state = EMPTY;
  }
}

size_t hash_find(const hash_t* hash, size_t key) {
  size_t index = hash_function(key, hash->capacity);
  
  while (hash->table[index].state != EMPTY) {
    if (hash->table[index].state == BUSY) {
      if (hash->table[index].key == key) break;
    }
    index++;
    if (index == hash->capacity) index = 0;
  }
  
  return index;
}

bool hash_resize(hash_t* hash, size_t new_capacity) {
  hash_node_t* new_table = malloc(new_capacity * sizeof(hash_node_t));
  if (new_table == NULL) return false;

  set_table_empty(new_table, new_capacity);

  hash_node_t* old_table = hash->table;
  hash->table = new_table;
  hash->size = 0;
  hash->deleted_count = 0;

  size_t old_capacity = hash->capacity;
  hash->capacity = new_capacity;

  for (size_t i = 0; i < old_capacity; i++) {
    if (old_table[i].state == BUSY) {
      hash_store(hash, old_table[i].key, old_table[i].data);
    }
  }

  free(old_table);
  return true;
}

float hash_load_factor(hash_t* hash) {
  return ((float)hash->size + (float)hash->deleted_count) / (float)hash->capacity;
}

hash_t* hash_create(hash_destroy_t destroy_fn) {
  hash_t* new_hash = malloc(sizeof(hash_t));
  if (!new_hash) return NULL;

  new_hash->capacity = INIT_CAPACITY;
  new_hash->size = 0;
  new_hash->destroy_fn = destroy_fn;
  new_hash->deleted_count = 0;

  hash_node_t* new_table = malloc(INIT_CAPACITY * sizeof(hash_node_t));
  if (!new_table) {
    free(new_hash);
    return NULL;
  }

  new_hash->table = new_table;
  set_table_empty(new_table, INIT_CAPACITY);

  return new_hash;
}

bool hash_store(hash_t* hash, size_t key, void* data) {
  if (hash_load_factor(hash) > MAX_LOAD_FACTOR) {
    if (!hash_resize(hash, hash->capacity * RESIZE_FACTOR)) return false;
  }

  size_t index = hash_find(hash, key);

  if (hash->table[index].state == EMPTY) {
    hash->table[index].key = key;
    hash->table[index].state = BUSY;
    hash->size++;
  } else if (hash->destroy_fn) {
    hash->destroy_fn(hash->table[index].data);
  }

  hash->table[index].data = data;
  return true;
}

void* hash_delete(hash_t* hash, size_t key) {
  if (!hash_contains(hash, key)) return NULL;

  size_t index = hash_find(hash, key);

  void* data = hash->table[index].data;
  hash->table[index].state = DELETED;
  hash->size--;
  hash->deleted_count++;

  if (hash->capacity > INIT_CAPACITY && hash_load_factor(hash) < MIN_LOAD_FACTOR) {
    hash_resize(hash, hash->capacity / REDUCTION_FACTOR);
  }

  return data;
}

void* hash_get(const hash_t* hash, size_t key) {
  size_t index = hash_find(hash, key);
  if (!hash_contains(hash, key)) return NULL;
  return hash->table[index].data;
}

bool hash_contains(const hash_t* hash, size_t key) {
  size_t index = hash_find(hash, key);
  return hash->table[index].state == BUSY;
}

size_t hash_size(const hash_t* hash) { return hash->size; }

void hash_destroy(hash_t* hash) {
  for (size_t i = 0; i < hash->capacity; i++) {
    hash_node_t current = hash->table[i];
    if (current.state == BUSY) {
      if (hash->destroy_fn) hash->destroy_fn(current.data);
    }
  }

  free(hash->table);
  free(hash);
}
