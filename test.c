#include "src/lz_string.h"

#include <stdio.h>

#include "c_string.h"

int
main()
{
  string_t input;
  string_init(&input);
  string_set_pointer(&input, "Hello world");

  string_t compressed;
  string_init(&compressed);

  string_t decompressed;
  string_init(&decompressed);

  lz_compress_base64(&input, &compressed);
  lz_decompress_base64(&compressed, &decompressed);

  printf("Original     : %s\nCompressed   : %s\nDecompressed : %s\n",
         input.string,
         compressed.string,
         decompressed.string);

  string_free(&input);
  string_free(&compressed);
  string_free(&decompressed);

  return 0;
}