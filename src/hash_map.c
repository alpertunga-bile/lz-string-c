#include "hash_map.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct hash_map_ll_node
{
  size_t   key;
  uint32_t value;
  void*    next;
};

uint32_t
hash_number(uint32_t x)
{
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = (x >> 16) ^ x;

  return x;
}

void
hash_map_init(hash_map_t* hash_map)
{
  memset(hash_map, 0, sizeof(hash_map_t));
}

int
hash_map_get_value(hash_map_t* hash_map, size_t key, uint32_t* value)
{
  size_t hash_value = hash_number(key);
  size_t index      = hash_value % HASH_MAP_SIZE;

  struct hash_map_ll_node* node = hash_map->map[index];

  while (NULL != node) {
    if (key == node->key) {
      *value = node->value;
      return 0;
    }

    node = (struct hash_map_ll_node*)node->next;
  }

  return -1;
}

int
hash_map_contains_key(hash_map_t* hash_map, size_t key)
{
  size_t hash_value = hash_number(key);
  size_t index      = hash_value % HASH_MAP_SIZE;

  struct hash_map_ll_node* node = hash_map->map[index];

  while (NULL != node) {
    if (key == node->key) {
      return 0;
    }

    node = (struct hash_map_ll_node*)node->next;
  }

  return -1;
}

void
hash_map_add_value(hash_map_t* hash_map, size_t key, uint32_t value)
{
  size_t hash_value = hash_number(key);
  size_t index      = hash_value % HASH_MAP_SIZE;

  struct hash_map_ll_node* current_node = hash_map->map[index];
  struct hash_map_ll_node* prev_node    = NULL;

  while (NULL != current_node) {
    prev_node = current_node;

    if (key == current_node->key) {
      return;
    }

    current_node = (struct hash_map_ll_node*)current_node->next;
  }

  struct hash_map_ll_node* new_node = malloc(sizeof(struct hash_map_ll_node));
  memset(new_node, 0, sizeof(struct hash_map_ll_node));
  new_node->key   = key;
  new_node->value = value;

  if (NULL == hash_map->map[index]) {
    hash_map->map[index] = new_node;
  } else {
    prev_node->next = new_node;
  }
}

void
hash_map_remove_value(hash_map_t* hash_map, size_t key)
{
  size_t hash_value = hash_number(key);
  size_t index      = hash_value % HASH_MAP_SIZE;

  struct hash_map_ll_node* current_node = hash_map->map[index];
  struct hash_map_ll_node* prev_node    = NULL;

  while (NULL != current_node) {
    if (key == current_node->key) {
      break;
    }

    prev_node    = current_node;
    current_node = (struct hash_map_ll_node*)current_node->next;
  }

  if (NULL == current_node) {
    return;
  }

  if (NULL == prev_node) {
    hash_map->map[index] = current_node->next;
  } else {
    prev_node->next = current_node->next;
  }

  free(current_node);
  current_node = NULL;
}

void
hash_map_free(hash_map_t* hash_map)
{
  int i = 0;

  for (i = 0; i < HASH_MAP_SIZE; ++i) {
    struct hash_map_ll_node* current_node = hash_map->map[i];

    if (NULL == current_node) {
      continue;
    }

    struct hash_map_ll_node* prev_node = NULL;

    while (NULL != current_node) {
      prev_node    = current_node;
      current_node = (struct hash_map_ll_node*)current_node->next;

      free(prev_node);
    }

    hash_map->map[i] = NULL;
  }
}