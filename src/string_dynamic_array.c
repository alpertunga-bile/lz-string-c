#include "string_dynamic_array.h"

#include <stdlib.h>
#include <string.h>

void
da_string_init(string_dynamic_array_t* da,
               size_t                  init_size,
               size_t                  increment_size)
{
  memset(da, 0, sizeof(string_dynamic_array_t));

  da->array          = calloc(init_size, sizeof(string_t));
  da->total_capacity = init_size;
  da->increment_size = increment_size;
}

void
da_string_check_sizes(string_dynamic_array_t* da)
{
  if (da->element_size < da->total_capacity) {
    return;
  }

  int type_size    = sizeof(da->array[0]);
  int current_size = da->element_size * type_size;

  da->array =
    realloc(da->array, current_size + (da->increment_size * type_size));
  da->total_capacity += da->increment_size;
}

void
da_string_add_char(string_dynamic_array_t* da, char value)
{
  da_string_check_sizes(da);

  int current_index = da->element_size;

  string_init(&da->array[current_index]);
  string_set_char(&da->array[current_index], value);

  da->element_size++;
}

void
da_string_add_pointer(string_dynamic_array_t* da, const char* value)
{
  da_string_check_sizes(da);

  int current_index = da->element_size;

  string_init(&da->array[current_index]);
  string_set_pointer(&da->array[current_index], value);

  da->element_size++;
}

void
da_string_add_string(string_dynamic_array_t* da, string_t* value)
{
  da_string_check_sizes(da);

  int current_index = da->element_size;

  string_init(&da->array[current_index]);
  string_set_string(&da->array[current_index], value);

  da->element_size++;
}

void
shrink_string_da(string_dynamic_array_t* da)
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
da_string_free(string_dynamic_array_t* da)
{
  int i = 0;

  for (i = 0; i < da->element_size; ++i) {
    string_free(&da->array[i]);
  }

  free(da->array);
  da->array = NULL;
}
