#include "utility.h"

#include <stdio.h>
#include <stdlib.h>

void
read_data(const char* filepath, string_t* data)
{
  FILE* fileptr;

  fileptr = fopen(filepath, "r");

  fseek(fileptr, 0, SEEK_END);
  size_t file_size = ftell(fileptr);
  rewind(fileptr);

  char* buffer = calloc(file_size + 1, sizeof(char));

  fread(buffer, 1, file_size, fileptr);

  string_set_pointer(data, buffer);

  free(buffer);
  fclose(fileptr);
}