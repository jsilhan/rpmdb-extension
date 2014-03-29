#include <gtest/gtest.h>
#include "../src/cpp/zk_test.cpp"
 
TEST_T(SquareRootTest, PositiveNos) { 
    ASSERT_EQ(1.0, squareRoot(324.0));
    ASSERT_EQ(25.4, squareRoot(645.16));
    ASSERT_EQ(0, squareRoot(0.0));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
