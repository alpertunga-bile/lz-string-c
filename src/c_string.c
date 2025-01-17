#include "c_string.h"

#include <stdlib.h>
#include <string.h>

void
string_init(string_t* str)
{
  memset(str, 0, sizeof(string_t));

  str->string = NULL;
  str->length = 0;
}

void
string_set_char(string_t* str, char value)
{
  string_free(str);

  if (value == 0) {
    return;
  }

  char temp[2] = { '\0' };
  temp[0]      = value;

  size_t value_length = strlen(temp);

  str->string = calloc(value_length + 1, sizeof(char));
  strcpy(str->string, temp);

  str->length = value_length;
}

void
string_set_pointer(string_t* str, const char* value)
{
  string_free(str);

  if (value == NULL) {
    return;
  }

  size_t value_length = strlen(value);

  str->string = calloc(value_length + 1, sizeof(char));
  strcpy(str->string, value);

  str->length = value_length;
}

void
string_set_string(string_t* str, string_t* value)
{
  string_free(str);

  if (value == NULL) {
    return;
  }

  if (value->string == NULL) {
    return;
  }

  str->string = calloc(value->length + 1, sizeof(char));
  strcpy(str->string, value->string);

  str->length = value->length;
}

void
string_append_char(string_t* str, char value)
{
  char temp[2] = { '\0' };
  temp[0]      = value;

  if (str->string == NULL) {
    string_set_char(str, value);
    return;
  }

  size_t total_length = str->length + strlen(temp);
  char*  result       = calloc(str->length + 1, sizeof(char));
  strcpy(result, str->string);

  string_free(str);

  str->string = calloc(total_length + 1, sizeof(char));
  strcpy(str->string, result);
  strcat(str->string, temp);

  free(result);

  str->length = total_length;
}

void
string_append_pointer(string_t* str, const char* value)
{
  if (str->string == NULL) {
    string_set_pointer(str, value);
    return;
  }

  size_t total_length = str->length + strlen(value);
  char*  result       = calloc(str->length + 1, sizeof(char));
  strcpy(result, str->string);

  string_free(str);

  str->string = calloc(total_length + 1, sizeof(char));
  strcpy(str->string, result);
  strcat(str->string, value);

  free(result);

  str->length = total_length;
}

void
string_append_string(string_t* str, string_t* value)
{
  if (str->string == NULL) {
    if (value->string != NULL) {
      string_set_string(str, value);
      return;
    }

    return;
  }

  size_t total_length = str->length + value->length;
  char*  result       = calloc(str->length + 1, sizeof(char));
  strcpy(result, str->string);

  string_free(str);

  str->string = calloc(total_length + 1, sizeof(char));
  strcpy(str->string, result);
  strcat(str->string, value->string);

  free(result);

  str->length = total_length;
}

int
string_compare_string(string_t* a, string_t* b)
{
  if (a->length != b->length) {
    return -1;
  }

  return strcmp(a->string, b->string);
}

int
string_compare_pointer(string_t* a, const char* b)
{
  if (a->length != strlen(b)) {
    return -1;
  }

  return strcmp(a->string, b);
}

int
string_compare_char(string_t* a, char b)
{
  if (a->length != 1) {
    return -1;
  }

  char temp[2] = { '\0' };
  temp[0]      = b;

  return strcmp(a->string, temp);
}

void
join_string_array(string_t*          result,
                  string_t*          array,
                  size_t array_size)
{
  int i = 0;

  for (i = 0; i < array_size; ++i) {
    string_append_string(result, &array[i]);
  }
}

size_t
hash_string(const char* str)
{
  unsigned long hash = 5381;
  int           c    = *str++;

  while (c != 0) {
    hash = ((hash << 5) + hash) + c;
    c    = *str++;
  }

  return hash;
}

unsigned long long
string_hash(string_t* str)
{
  if (str->string == NULL) {
    return 0;
  }

  return hash_string(str->string);
}

void
string_free(string_t* str)
{
  if (str->string == NULL) {
    return;
  }

  free(str->string);
  str->string = NULL;
  str->length = 0;
}

unsigned long long
string_size(const char* value)
{
  return strlen(value);
}

char
string_char_at(string_t* str, int index)
{
  if (index >= str->length || index < 0 || str->string == NULL) {
    return '\0';
  }

  return str->string[index];
}
