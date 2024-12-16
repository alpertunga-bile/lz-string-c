#define DEFINE_RKTEST_IMPLEMENTATION
#include "rktest.h"

#include "../src/lz_string.h"

#include "utility.h"

#include "c_string.h"

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

TEST(compress_base64, all_ascii)
{
  EXPECT_EQ(base64_compress_result("test_src/data/all_ascii/data.bin",
                                   "test_src/data/all_ascii/base64.bin"),
            0);
}

TEST(compress_base64, hello_world)
{
  EXPECT_EQ(base64_compress_result("test_src/data/hello_world/data.bin",
                                   "test_src/data/hello_world/base64.bin"),
            0);
}

TEST(compress_base64, lorem_ipsum)
{
  EXPECT_EQ(base64_compress_result("test_src/data/lorem_ipsum/data.bin",
                                   "test_src/data/lorem_ipsum/base64.bin"),
            0);
}

TEST(compress_base64, pi)
{
  EXPECT_EQ(base64_compress_result("test_src/data/pi/data.bin",
                                   "test_src/data/pi/base64.bin"),
            0);
}

TEST(compress_base64, repeated)
{
  EXPECT_EQ(base64_compress_result("test_src/data/repeated/data.bin",
                                   "test_src/data/repeated/base64.bin"),
            0);
}

TEST(compress_base64, tattoo)
{
  EXPECT_EQ(base64_compress_result("test_src/data/tattoo/data.bin",
                                   "test_src/data/tattoo/base64.bin"),
            0);
}

TEST(decompress_base64, all_ascii)
{
  EXPECT_EQ(base64_decompress_result("test_src/data/all_ascii/data.bin"), 0);
}

int
main(int argc, const char* argv[])
{
  return rktest_main(argc, argv);
}