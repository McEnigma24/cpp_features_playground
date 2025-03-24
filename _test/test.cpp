// clang-format off
#include <gtest/gtest.h> // must be first
// clang-format on
#include "__preprocessor__.h"

TEST(SampleTest, Sanity) { EXPECT_EQ(1, 1); }

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
