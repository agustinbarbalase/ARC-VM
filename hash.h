#ifndef HASH_H
#define HASH_H

#include <stdbool.h>
#include <stddef.h>

typedef struct hash hash_t;
typedef struct hash_iter hash_iter_t;

typedef void (*hash_destroy_t)(void*);

hash_t* hash_create(hash_destroy_t destroy_data);

bool hash_store(hash_t* hash, size_t key, void* data);

void* hash_delete(hash_t* hash, size_t key);

void* hash_get(const hash_t* hash, size_t key);

bool hash_contains(const hash_t* hash, size_t key);

size_t hash_count(const hash_t* hash);

void hash_destroy(hash_t* hash);

#endif
