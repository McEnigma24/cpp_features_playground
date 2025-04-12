// clang-format off
#include <gtest/gtest.h> // must be first
// clang-format on
#include "__preprocessor__.h"
#include "ram_access.hpp"

TEST(SampleTest, Sanity) { EXPECT_EQ(1, 1); }

TEST(SampleTest, memcpy_Sanity)
{
    size_t size = 1024 * 1024 * 1024; // 1GB

    void* src = malloc(size);
    void* dst = malloc(size);

    memset(src, 1, size);
    memset(dst, 0, size);

    memcpy(dst, src, size);

    EXPECT_FALSE(memcmp(src, dst, size)); // returns 0 if equal
    free(src);
    free(dst);
}

TEST(SampleTest, no_cache_polutions_memcpy_1)
{
    size_t size = 1024 * 1024 * 1024; // 1 GB

    void* src = malloc(size);
    void* dst = malloc(size);

    memset(src, 1, size);
    memset(dst, 0, size);

    no_write_allocate_memcpy(dst, src, size);

    EXPECT_FALSE(memcmp(src, dst, size)); // returns 0 if equal
    free(src);
    free(dst);
}

TEST(SampleTest, no_cache_polutions_memcpy_2)
{
    size_t size = 1024 * 1024 * 3; // 3 MB

    void* src = malloc(size);
    void* dst = malloc(size);

    memset(src, 1, size);
    memset(dst, 0, size);

    no_write_allocate_memcpy(dst, src, size);

    EXPECT_FALSE(memcmp(src, dst, size)); // returns 0 if equal
    free(src);
    free(dst);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
