#ifndef _LZ_SRC_C_STRING_H_
#define _LZ_SRC_C_STRING_H_

typedef struct
{
  char*              string;
  unsigned long long length;
} string_t;

void
string_init(string_t* str);

/*
 * String set functions
 */

void
string_set_char(string_t* str, char value);
void
string_set_pointer(string_t* str, const char* value);
void
string_set_string(string_t* str, string_t* value);

/*
 * String append functions
 */

void
string_append_char(string_t* str, char value);
void
string_append_pointer(string_t* str, const char* value);
void
string_append_string(string_t* str, string_t* value);

/*
 * String compare functions
 */

int
string_compare_string(string_t* a, string_t* b);
int
string_compare_pointer(string_t* a, const char* b);
int
string_compare_char(string_t* a, char b);

/*
 * String utility functions
 */

void
join_string_array(string_t*          result,
                  string_t*          array,
                  unsigned long long array_size);

void
string_free(string_t* str);

unsigned long long
string_hash(string_t* str);

unsigned long long
string_size(const char* value);

char
string_char_at(string_t* str, int index);

#endif