#define DEFINE_RKTEST_IMPLEMENTATION
#include "rktest.h"

#include "../src/lz_string.h"

#include "utility.h"

#include "c_string.h"

TEST(compress_base64, hello_world)
{
  string_t input;
  string_init(&input);

  read_data("test_src/data/hello_world/data.bin", &input);

  string_t compressed;
  string_init(&compressed);

  lz_compress_base64(&input, &compressed);

  string_t result;
  string_init(&result);

  read_data("test_src/data/hello_world/base64.bin", &result);

  EXPECT_EQ(string_compare_string(&result, &compressed), 0);

  string_free(&input);
  string_free(&compressed);
}

int
main(int argc, const char* argv[])
{
  return rktest_main(argc, argv);
}