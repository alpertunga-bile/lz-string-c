#ifndef _LZ_STRING_TEST_SRC_UTILIY_H_
#define _LZ_STRING_TEST_SRC_UTILIY_H_

#include "../src/c_string.h"

void
read_data(const char* filepath, string_t* data);

int
base64_compress_result(const char* data_filepath, const char* result_filepath);

int
base64_decompress_result(const char* data_filepath);

#endif