#ifndef _LZ_STRING_SRC_STRING_DYNAMIC_ARRAY_H_
#define _LZ_STRING_SRC_STRING_DYNAMIC_ARRAY_H_

#include <stddef.h>
#include "c_string.h"

typedef struct
{
  unsigned long long element_size;
  unsigned long long total_capacity;
  unsigned int       increment_size;
  string_t*          array;
} string_dynamic_array_t;

void
da_string_init(string_dynamic_array_t* da,
               size_t init_size,
               size_t increment_size);

void
da_string_add_char(string_dynamic_array_t* da, char value);
void
da_string_add_pointer(string_dynamic_array_t* da, const char* value);
void
da_string_add_string(string_dynamic_array_t* da, string_t* value);

void
da_string_shrink(string_dynamic_array_t* da);
void
da_string_free(string_dynamic_array_t* da);

#endif
