#ifndef _LZ_SRC_C_STRING_H_
#define _LZ_SRC_C_STRING_H_

typedef struct
{
  char*              string;
  unsigned long long length;
} string_t;

void
string_init(string_t* str);

void
string_set_char(string_t* str, char value);
void
string_set_pointer(string_t* str, const char* value);
void
string_set_string(string_t* str, string_t* value);

void
string_append_char(string_t* str, char value);
void
string_append_pointer(string_t* str, const char* value);
void
string_append_string(string_t* str, string_t* value);

void
string_free(string_t* str);

unsigned long long
string_hash(string_t* str);

unsigned long long
string_size(const char* value);

char
string_char_at(string_t* str, int index);

#endif