#ifndef _LZ_STRING_SRC_HASH_MAP_H_
#define _LZ_STRING_SRC_HASH_MAP_H_

#define HASH_MAP_SIZE 1024

struct hash_map_ll_node;

typedef struct
{
  struct hash_map_ll_node* map[HASH_MAP_SIZE];
} hash_map_t;

void
hash_map_init(hash_map_t* hash_map);
int
hash_map_get_value(hash_map_t*        hash_map,
                   unsigned long long key,
                   unsigned int*      value);
int
hash_map_contains_key(hash_map_t* hash_map, unsigned long long key);
void
hash_map_add_value(hash_map_t*        hash_map,
                   unsigned long long key,
                   unsigned int       value);
void
hash_map_remove_value(hash_map_t* hash_map, unsigned long long key);
void
hash_map_free(hash_map_t* hash_map);

#endif