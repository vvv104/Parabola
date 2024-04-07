#include <gtest/gtest.h>
#include "../Parabola.h"

// Test solving quadratic equation with two real roots
TEST(ParabolaTest, TwoRealRoots)
{
    Parabola::Definition p(1, -3, 2);  // x^2 - 3x + 2 = 0
    Parabola::Solution s = Parabola::Solve(p);
    ASSERT_TRUE(s.r1.has_value());
    ASSERT_TRUE(s.r2.has_value());
    EXPECT_DOUBLE_EQ(s.r1.value(), 1.0);  // Root 1
    EXPECT_DOUBLE_EQ(s.r2.value(), 2.0);  // Root 2
    ASSERT_TRUE(s.e.has_value());
    EXPECT_DOUBLE_EQ(s.e.value(), 1.5);   // Xmin
}

// Test solving quadratic equation with one real root
TEST(ParabolaTest, OneRealRoot)
{
    Parabola::Definition p(1, -2, 1);  // x^2 - 2x + 1 = 0
    Parabola::Solution s = Parabola::Solve(p);
    ASSERT_TRUE(s.r1.has_value());
    EXPECT_DOUBLE_EQ(s.r1.value(), 1.0);  // Root 1
    EXPECT_FALSE(s.r2.has_value());  // No other root
    ASSERT_TRUE(s.e.has_value());
    EXPECT_DOUBLE_EQ(s.e.value(), 1.0);   // Xmin
}

// Test solving quadratic equation with complex roots
TEST(ParabolaTest, ComplexRoots)
{
    Parabola::Definition p(1, 1, 1);  // x^2 + x + 1 = 0
    Parabola::Solution s = Parabola::Solve(p);
    EXPECT_FALSE(s.r1.has_value());  // No real roots
    ASSERT_TRUE(s.e.has_value());
    EXPECT_DOUBLE_EQ(s.e.value(), -0.5);  // Xmin
}

// Test solving quadratic equation with zero with x^2
TEST(ParabolaTest, LinearTest)
{
    Parabola::Definition p(0, 1, 1);  // x + 1 = 0
    Parabola::Solution s = Parabola::Solve(p);
    ASSERT_TRUE(s.r1.has_value());
    EXPECT_DOUBLE_EQ(s.r1.value(), -1.0);  // Root 1
    EXPECT_FALSE(s.r2.has_value());  // No other root
    EXPECT_FALSE(s.e.has_value()); // No Zmin
}
