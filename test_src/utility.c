#include "utility.h"

#include "../src/lz_string.h"

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

int
base64_compress_result(const char* data_filepath, const char* result_filepath)
{
  string_t input;
  string_init(&input);

  read_data(data_filepath, &input);

  string_t compressed;
  string_init(&compressed);

  lz_compress_base64(&input, &compressed);

  string_t result;
  string_init(&result);

  read_data(result_filepath, &result);

  int cmp_result = string_compare_string(&result, &compressed);

  string_free(&input);
  string_free(&compressed);
  string_free(&result);

  return cmp_result;
}

int
base64_decompress_result(const char* data_filepath)
{
  string_t input;
  string_init(&input);

  read_data(data_filepath, &input);

  string_t compressed;
  string_init(&compressed);

  lz_compress_base64(&input, &compressed);

  string_t result;
  string_init(&result);

  lz_decompress_base64(&compressed, &result);

  int cmp_result = string_compare_string(&result, &input);

  string_free(&input);
  string_free(&compressed);
  string_free(&result);

  return cmp_result;
}