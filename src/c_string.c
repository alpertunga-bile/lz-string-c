#include "c_string.h"

#include <stdlib.h>
#include <string.h>

#include "hash_map.h"

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
