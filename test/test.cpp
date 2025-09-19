#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(SampleTest, BasicAssertions)
{
    // Expect two strings to be equal.
    EXPECT_EQ("hello", "hello");
    // Expect a boolean condition to be true.
    EXPECT_TRUE(1 + 1 == 2);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}