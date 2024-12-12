#include "char_dynamic_array.h"

#include <stdlib.h>
#include <string.h>

void
init_char_da(char_dynamic_array_t* da, size_t init_size, size_t increment_size)
{
  memset(da, 0, sizeof(char_dynamic_array_t));

  da->array          = calloc(init_size, sizeof(char));
  da->total_capacity = init_size;
  da->increment_size = increment_size;
}

void
add_char_da(char_dynamic_array_t* da, char value)
{
  if (da->element_size == da->total_capacity) {
    int type_size    = sizeof(da->array[0]);
    int current_size = da->element_size * type_size;

    da->array =
      realloc(da->array, current_size + (da->increment_size * type_size));
    da->total_capacity += da->increment_size;
  }

  da->array[da->element_size++] = value;
}

void
shrink_char_da(char_dynamic_array_t* da)
{
  if (da->element_size == da->total_capacity) {
    return;
  }

  int type_size    = sizeof(da->array[0]);
  int current_size = da->element_size * type_size;

  da->array          = realloc(da->array, current_size);
  da->total_capacity = da->element_size;
}

void
free_char_da(char_dynamic_array_t* da)
{
  free(da->array);
  da->array = NULL;
}
