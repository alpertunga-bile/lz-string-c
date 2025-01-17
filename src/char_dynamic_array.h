#ifndef _LZ_STRING_SRC_CHAR_DYNAMIC_ARRAY_H_
#define _LZ_STRING_SRC_CHAR_DYNAMIC_ARRAY_H_

#include <stddef.h>

typedef struct
{
  unsigned long long element_size;
  unsigned long long total_capacity;
  unsigned int       increment_size;
  char*              array;
} char_dynamic_array_t;

void
init_char_da(char_dynamic_array_t* da,
             size_t init_size,
             size_t increment_size);
void
add_char_da(char_dynamic_array_t* da, char value);
void
shrink_char_da(char_dynamic_array_t* da);
void
free_char_da(char_dynamic_array_t* da);

#endif
