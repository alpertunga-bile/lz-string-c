#include "src/lz_string.h"

#include <stdio.h>

#include "c_string.h"

int
main()
{
  string_t input;
  string_t compressed;
  string_t decompressed;

  string_init(&input);
  string_init(&compressed);
  string_init(&decompressed);

  string_set_pointer(&input, "Hello world");

  lz_compress_base64(&input, &compressed);

  printf("Compressed: %s\n", compressed.string);

  lz_decompress_base64(&compressed, &decompressed);

  printf("Decompressed: %s\n", decompressed.string);

  string_free(&input);
  string_free(&compressed);
  string_free(&decompressed);

  return 0;
}