#include <gtest/gtest.h>

int Add(int a, int b)
{
    return a + b;
}

class AddTest : public testing::Test
{
public:
    virtual void SetUp() override {

    }
    virtual void TearDown() override {

    }
};

TEST_F(AddTest, addtest_one)
{
    ASSERT_EQ(Add(1, 2), 3); // 故意的
    ASSERT_GT(Add(4, 5), 6); // 也是故意的
}