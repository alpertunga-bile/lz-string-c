#ifndef _LZ_STRING_SRC_LZ_STRING_H_
#define _LZ_STRING_SRC_LZ_STRING_H_

#include "c_string.h"

void
lz_compress_base64(string_t* input, string_t* compressed);

void
lz_decompress_base64(string_t* compressed, string_t* uncompressed);

#endif