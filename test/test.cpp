#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(SampleTest, BasicAssertions)
{
    // Expect two strings to be equal.
    EXPECT_EQ(std::string("hello"), std::string("hello"));
    // Expect a boolean condition to be true.
    EXPECT_TRUE(1 + 1 == 2);
}