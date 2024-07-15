#include <gtest/gtest.h>
#include <Math/Geometry/Orientaion.h>

TEST(TriangleAreaTest, TestCase1)
{
    MathLib::HVector2 v0(0, 0);
    MathLib::HVector2 v1(1, 0);
    MathLib::HVector2 v2(0, 1);

    EXPECT_TRUE(MathLib::Equal(MathLib::OrientationUtils::TriangleArea(v0, v1, v2), 0.5));
}

TEST(TriangleAreaTest, TestCase2)
{
    MathLib::HVector2 v0(0, 0);
    MathLib::HVector2 v1(1, 0);
    MathLib::HVector2 v2(1, 1);

    EXPECT_TRUE(MathLib::Equal(MathLib::OrientationUtils::TriangleArea(v0, v1, v2), 0.5));
}

TEST(TriangleAreaTest, TestCase3)
{
    MathLib::HVector2 v0(0, 0);
    MathLib::HVector2 v1(0, 1);
    MathLib::HVector2 v2(1, 1);

    EXPECT_TRUE(MathLib::Equal(MathLib::OrientationUtils::TriangleArea(v0, v1, v2), -0.5));
}

TEST(TriangleAreaTest, TestCase4)
{
    MathLib::HVector2 v0(0, 0);
    MathLib::HVector2 v1(1, 1);
    MathLib::HVector2 v2(1, 0);

    EXPECT_TRUE(MathLib::Equal(MathLib::OrientationUtils::TriangleArea(v0, v1, v2), -0.5));
}

TEST(TriangleAreaTest, TestCase5)
{
    MathLib::HVector2 v0(0, 0);
    MathLib::HVector2 v1(0, 1);
    MathLib::HVector2 v2(1, 0);

    EXPECT_TRUE(MathLib::Equal(MathLib::OrientationUtils::TriangleArea(v0, v1, v2), -0.5));
}
