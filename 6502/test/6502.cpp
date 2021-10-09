#include <gtest/gtest.h>
#include <iostream>

TEST(ADC, ACCUMULATOR) {
    ASSERT_EQ(0.0, 0.0);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}