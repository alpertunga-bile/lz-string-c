#define DEFINE_RKTEST_IMPLEMENTATION
#include "rktest.h"

#include "utility.h"

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

TEST(decompress_base64, hello_world)
{
  EXPECT_EQ(base64_decompress_result("test_src/data/hello_world/data.bin"), 0);
}

TEST(decompress_base64, lorem_ipsum)
{
  EXPECT_EQ(base64_decompress_result("test_src/data/lorem_ipsum/data.bin"), 0);
}

int
main(int argc, const char* argv[])
{
  return rktest_main(argc, argv);
}
