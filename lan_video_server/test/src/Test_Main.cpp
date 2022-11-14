#include <gtest/gtest.h>

int add(int a, int b)
{
    return a + b;
}

TEST(fun, add)
{
    EXPECT_EQ(1, add(2, -1));
    EXPECT_EQ(5, add(2, 3));
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}