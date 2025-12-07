#include "hash.h"

#include <malloc.h>
#include <string.h>

#define INIT_CAPACITY 10

#define MIN_LOAD_FACTOR 0.3f
#define MAX_LOAD_FACTOR 0.7f
#define RESIZE_FACTOR 2
#define REDUCTION_FACTOR 2

typedef enum { EMPTY, OCCUPIED, DELETED } hash_node_status_t;

typedef struct hash_node {
  hash_node_status_t status;
  size_t key;
  void* data;
} hash_node_t;

typedef struct hash {
  hash_node_t* table;
  size_t capacity;
  size_t count;
  size_t deleted_count;
  hash_destroy_t destroy_data;
} hash_t;

size_t hash_function(size_t key, size_t capacity) {
  // Thomas Wang’s 64-bit integer hash
  key = (~key) + (key << 21);
  key = key ^ (key >> 24);
  key = (key + (key << 3)) + (key << 8);
  key = key ^ (key >> 14);
  key = (key + (key << 2)) + (key << 4);
  key = key ^ (key >> 28);
  key = key + (key << 31);
  return key % capacity;
}

size_t hash_position(hash_t* hash, size_t key) {
  size_t index = hash_function(key, hash->capacity);

  while (hash->table[index].status != EMPTY) {
    if (hash->table[index].status == OCCUPIED && hash->table[index].key == key) {
      return index;
    }
    index++;
    if (index == hash->capacity) index = 0;
  }

  return index;
}

float hash_load_factor(hash_t* hash) {
  return (float)(hash->count + hash->deleted_count) / (float)hash->capacity;
}

bool hash_resize(hash_t* hash, size_t new_capacity) {
  hash_node_t* new_table = malloc(new_capacity * sizeof(hash_node_t));
  if (!new_table) return false;

  for (size_t i = 0; i < new_capacity; i++) {
    new_table[i].status = EMPTY;
  }

  hash_node_t* old_table = hash->table;
  size_t old_capacity = hash->capacity;

  hash->table = new_table;
  hash->capacity = new_capacity;
  hash->count = 0;
  hash->deleted_count = 0;

  for (size_t i = 0; i < old_capacity; i++) {
    if (old_table[i].status == OCCUPIED) {
      hash_store(hash, old_table[i].key, old_table[i].data);
      free(old_table[i].key);
    }
  }

  free(old_table);

  return true;
}

hash_t* hash_create(hash_destroy_t destroy_data) {
  hash_t* hash = malloc(sizeof(hash_t));

  hash->capacity = INIT_CAPACITY;
  hash->count = 0;
  hash->destroy_data = destroy_data;
  hash->table = malloc(hash->capacity * sizeof(hash_node_t));

  return hash;
}

bool hash_store(hash_t* hash, size_t key, void* data) {
  if (hash_load_factor(hash) > MAX_LOAD_FACTOR) {
    if (!hash_resize(hash, hash->capacity * RESIZE_FACTOR)) {
      return false;
    }
  }

  size_t index = hash_position(hash, key);

  if (hash->table[index].status == EMPTY) {
    hash->table[index].status = OCCUPIED;
    hash->table[index].key = key;
    hash->count++;
  } else if (hash->destroy_data) {
    hash->destroy_data(hash->table[index].data);
  }

  hash->table[index].data = data;

  return true;
}

void* hash_delete(hash_t* hash, size_t key) {
  if (!hash_contains(hash, key)) return NULL;

  size_t index = hash_position(hash, key);

  void* data = hash->table[index].data;
  hash->table[index].status = DELETED;
  hash->count--;
  hash->deleted_count++;

  if (hash_load_factor(hash) < MIN_LOAD_FACTOR && hash->capacity > INIT_CAPACITY) {
    hash_resize(hash, hash->capacity / REDUCTION_FACTOR);
  }

  return data;
}

void* hash_get(const hash_t* hash, size_t key) {
  if (!hash_contains(hash, key)) return NULL;
  size_t index = hash_position((hash_t*)hash, key);
  return ((hash_t*)hash)->table[index].data;
}

bool hash_contains(const hash_t* hash, size_t key) {
  size_t index = hash_function(key, hash->capacity);
  return hash->table[index].status == OCCUPIED && hash->table[index].key == key;
}

size_t hash_count(const hash_t* hash) { return hash->count; }

void hash_destroy(hash_t* hash) {
  for (size_t i = 0; i < hash->capacity; i++) {
    if (hash->table[i].status == OCCUPIED) {
      if (hash->destroy_data) {
        hash->destroy_data(hash->table[i].data);
      }
    }
  }
  free(hash->table);
  free(hash);
}
