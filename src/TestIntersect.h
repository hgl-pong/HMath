#pragma once
#include <gtest/gtest.h>
#include <Math/Math.h>
#include <Math/Geometry/Intersection.h>

TEST(IntersectionUtilsTest, IsPointInTriangle2D)
{
    {
        MathLib::HVector2 p(0.5, 0.5);
        MathLib::HVector2 v0(0, 0);
        MathLib::HVector2 v1(1, 0);
        MathLib::HVector2 v2(0, 1);

        EXPECT_TRUE(MathLib::IntersectionUtils::IsPointInTriangle(p, v0, v1, v2)) << "Point should be inside the triangle.";
    }

    {
        MathLib::HVector2 p_out(1.5, 1.5);
        MathLib::HVector2 v0(0, 0);
        MathLib::HVector2 v1(1, 0);
        MathLib::HVector2 v2(0, 1);

        EXPECT_FALSE(MathLib::IntersectionUtils::IsPointInTriangle(p_out, v0, v1, v2)) << "Point should be outside the triangle.";
    }

    {
        MathLib::HVector2 p_vertex(0, 0);
        MathLib::HVector2 v0(0, 0);
        MathLib::HVector2 v1(1, 0);
        MathLib::HVector2 v2(0, 1);

        EXPECT_TRUE(MathLib::IntersectionUtils::IsPointInTriangle(p_vertex, v0, v1, v2)) << "Point should be on the triangle vertex.";
    }

    {
        MathLib::HVector2 p_edge(0.5, 0);
        MathLib::HVector2 v0(0, 0);
        MathLib::HVector2 v1(1, 0);
        MathLib::HVector2 v2(0, 1);

        EXPECT_TRUE(MathLib::IntersectionUtils::IsPointInTriangle(p_edge, v0, v1, v2)) << "Point should be on the triangle edge.";
    }

    {
        MathLib::HVector2 p_near(1, 1);
        MathLib::HVector2 v0(0, 0);
        MathLib::HVector2 v1(1, 0);
        MathLib::HVector2 v2(0, 1);

        EXPECT_FALSE(MathLib::IntersectionUtils::IsPointInTriangle(p_near, v0, v1, v2)) << "Point should be near but outside the triangle.";
    }

    {
        MathLib::HVector2 p_degenerate(0.5, 0);
        MathLib::HVector2 v0(0, 0);
        MathLib::HVector2 v1(1, 0);
        MathLib::HVector2 v2(2, 0);

        EXPECT_TRUE(MathLib::IntersectionUtils::IsPointInTriangle(p_degenerate, v0, v1, v2)) << "Point should be on the degenerate triangle (line segment).";
    }
}

TEST(IntersectionUtilsTest, IsPointInTriangle3D)
{
    {
        MathLib::HVector3 p(0.5, 0.5, 0.0);
        MathLib::HVector3 v0(0, 0, 0);
        MathLib::HVector3 v1(1, 0, 0);
        MathLib::HVector3 v2(0, 1, 0);

        EXPECT_TRUE(MathLib::IntersectionUtils::IsPointInTriangle(p, v0, v1, v2)) << "Point should be inside the triangle.";
    }

    {
        MathLib::HVector3 p_out(1.5, 1.5, 0.0);
        MathLib::HVector3 v0(0, 0, 0);
        MathLib::HVector3 v1(1, 0, 0);
        MathLib::HVector3 v2(0, 1, 0);

        EXPECT_FALSE(MathLib::IntersectionUtils::IsPointInTriangle(p_out, v0, v1, v2)) << "Point should be outside the triangle.";
    }

    {
        MathLib::HVector3 p_vertex(0, 0, 0);
        MathLib::HVector3 v0(0, 0, 0);
        MathLib::HVector3 v1(1, 0, 0);
        MathLib::HVector3 v2(0, 1, 0);

        EXPECT_TRUE(MathLib::IntersectionUtils::IsPointInTriangle(p_vertex, v0, v1, v2)) << "Point should be on the triangle vertex.";
    }

    {
        MathLib::HVector3 p_edge(0.5, 0, 0);
        MathLib::HVector3 v0(0, 0, 0);
        MathLib::HVector3 v1(1, 0, 0);
        MathLib::HVector3 v2(0, 1, 0);

        EXPECT_TRUE(MathLib::IntersectionUtils::IsPointInTriangle(p_edge, v0, v1, v2)) << "Point should be on the triangle edge.";
    }

    {
        MathLib::HVector3 p_near(1, 1, 0);
        MathLib::HVector3 v0(0, 0, 0);
        MathLib::HVector3 v1(1, 0, 0);
        MathLib::HVector3 v2(0, 1, 0);

        EXPECT_FALSE(MathLib::IntersectionUtils::IsPointInTriangle(p_near, v0, v1, v2)) << "Point should be near but outside the triangle.";
    }

    {
        MathLib::HVector3 p_out_of_plane(0.5, 0.5, 1.0);
        MathLib::HVector3 v0(0, 0, 0);
        MathLib::HVector3 v1(1, 0, 0);
        MathLib::HVector3 v2(0, 1, 0);

        EXPECT_FALSE(MathLib::IntersectionUtils::IsPointInTriangle(p_out_of_plane, v0, v1, v2)) << "Point should be outside the triangle plane.";
    }

    {
        MathLib::HVector3 p_degenerate(0.5, 0, 0);
        MathLib::HVector3 v0(0, 0, 0);
        MathLib::HVector3 v1(1, 0, 0);
        MathLib::HVector3 v2(2, 0, 0);

        EXPECT_TRUE(MathLib::IntersectionUtils::IsPointInTriangle(p_degenerate, v0, v1, v2)) << "Point should be on the degenerate triangle (line segment).";
    }
}

TEST(IntersectionUtilsTest, RayIntersectTriangle2D)
{
    MathLib::HReal t, u, v;

    // 测试射线与三角形相交的情况
    {
        MathLib::HVector2 rayOrigin(0.5, 0.5);
        MathLib::HVector2 rayDir(0, -1);
        MathLib::HVector2 v0(0, 0);
        MathLib::HVector2 v1(1, 0);
        MathLib::HVector2 v2(0, 1);

        EXPECT_TRUE(MathLib::IntersectionUtils::RayIntersectTriangle(rayOrigin, rayDir, v0, v1, v2, t, u, v));
        EXPECT_NEAR(t, 0.5, H_EPSILON);
        EXPECT_NEAR(u, 0.5, H_EPSILON);
        EXPECT_NEAR(v, 0.0, H_EPSILON);
    }

    // 测试射线与三角形不相交的情况
    {
        MathLib::HVector2 rayOrigin(1.5, 1.5);
        MathLib::HVector2 rayDir(0, -1);
        MathLib::HVector2 v0(0, 0);
        MathLib::HVector2 v1(1, 0);
        MathLib::HVector2 v2(0, 1);

        EXPECT_FALSE(MathLib::IntersectionUtils::RayIntersectTriangle(rayOrigin, rayDir, v0, v1, v2, t, u, v));
    }

    // 测试射线平行于三角形边的情况
    {
        MathLib::HVector2 rayOrigin(0.5, 0.5);
        MathLib::HVector2 rayDir(1, 0);
        MathLib::HVector2 v0(0, 0);
        MathLib::HVector2 v1(1, 0);
        MathLib::HVector2 v2(0, 1);

        EXPECT_FALSE(MathLib::IntersectionUtils::RayIntersectTriangle(rayOrigin, rayDir, v0, v1, v2, t, u, v));
    }

    // 测试射线起点在三角形内部的情况
    {
        MathLib::HVector2 rayOrigin(0.25, 0.25);
        MathLib::HVector2 rayDir(0, -1);
        MathLib::HVector2 v0(0, 0);
        MathLib::HVector2 v1(1, 0);
        MathLib::HVector2 v2(0, 1);

        EXPECT_TRUE(MathLib::IntersectionUtils::RayIntersectTriangle(rayOrigin, rayDir, v0, v1, v2, t, u, v));
        EXPECT_NEAR(t, 0.25, H_EPSILON);
        EXPECT_NEAR(u, 0.25, H_EPSILON);
        EXPECT_NEAR(v, 0.0, H_EPSILON);
    }

    // 测试射线方向为零向量的情况
    {
        MathLib::HVector2 rayOrigin(0.5, 0.5);
        MathLib::HVector2 rayDir(0, 0);
        MathLib::HVector2 v0(0, 0);
        MathLib::HVector2 v1(1, 0);
        MathLib::HVector2 v2(0, 1);

        EXPECT_FALSE(MathLib::IntersectionUtils::RayIntersectTriangle(rayOrigin, rayDir, v0, v1, v2, t, u, v));
    }
}

TEST(IntersectionUtilsTest, RayIntersectTriangle3D)
{
    {
        MathLib::HVector3 rayOrigin(0.5, 0.5, -1);
        MathLib::HVector3 rayDir(0, 0, 1);
        MathLib::HVector3 v0(0, 0, 0);
        MathLib::HVector3 v1(1, 0, 0);
        MathLib::HVector3 v2(0, 1, 0);
        MathLib::HReal t, u, v;

        EXPECT_TRUE(MathLib::IntersectionUtils::RayIntersectTriangle(rayOrigin, rayDir, v0, v1, v2, t, u, v));
        EXPECT_NEAR(t, 1.0, H_EPSILON);
        EXPECT_NEAR(u, 0.5, H_EPSILON);
        EXPECT_NEAR(v, 0.5, H_EPSILON);
    }

    {
        MathLib::HVector3 rayOriginMiss(1.5, 1.5, -1);
        MathLib::HVector3 rayDir(0, 0, 1);
        MathLib::HVector3 v0(0, 0, 0);
        MathLib::HVector3 v1(1, 0, 0);
        MathLib::HVector3 v2(0, 1, 0);
        MathLib::HReal t, u, v;

        EXPECT_FALSE(MathLib::IntersectionUtils::RayIntersectTriangle(rayOriginMiss, rayDir, v0, v1, v2, t, u, v));
    }

    {
        MathLib::HVector3 rayOrigin(0.5, 0.5, 1);
        MathLib::HVector3 rayDir(0, 0, 1);
        MathLib::HVector3 v0(0, 0, 0);
        MathLib::HVector3 v1(1, 0, 0);
        MathLib::HVector3 v2(0, 1, 0);
        MathLib::HReal t, u, v;

        EXPECT_FALSE(MathLib::IntersectionUtils::RayIntersectTriangle(rayOrigin, rayDir, v0, v1, v2, t, u, v));
    }

    {
        MathLib::HVector3 rayOrigin(1.5, 1.5, 0);
        MathLib::HVector3 rayDir(0, 0, 1);
        MathLib::HVector3 v0(0, 0, 0);
        MathLib::HVector3 v1(1, 0, 0);
        MathLib::HVector3 v2(0, 1, 0);
        MathLib::HReal t, u, v;

        EXPECT_FALSE(MathLib::IntersectionUtils::RayIntersectTriangle(rayOrigin, rayDir, v0, v1, v2, t, u, v));
    }

    {
        MathLib::HVector3 rayOrigin(0.5, 0.5, 0);
        MathLib::HVector3 rayDir(1, 1, 0);
        MathLib::HVector3 v0(0, 0, 0);
        MathLib::HVector3 v1(1, 0, 0);
        MathLib::HVector3 v2(0, 1, 0);
        MathLib::HReal t, u, v;

        EXPECT_FALSE(MathLib::IntersectionUtils::RayIntersectTriangle(rayOrigin, rayDir, v0, v1, v2, t, u, v));
    }
}

TEST(IntersectionUtilsTest, IsOnLine2D)
{
    MathLib::HVector2 v0(0, 0);
    MathLib::HVector2 v1(1, 1);

    MathLib::HVector2 p_on_line(0.5, 0.5);
    EXPECT_TRUE(MathLib::IntersectionUtils::IsOnLine(p_on_line, v0, v1));

    MathLib::HVector2 p_off_line(1, 0);
    EXPECT_FALSE(MathLib::IntersectionUtils::IsOnLine(p_off_line, v0, v1));

    // 边界情况
    MathLib::HVector2 p_at_v0(0, 0);
    EXPECT_TRUE(MathLib::IntersectionUtils::IsOnLine(p_at_v0, v0, v1));

    MathLib::HVector2 p_at_v1(1, 1);
    EXPECT_TRUE(MathLib::IntersectionUtils::IsOnLine(p_at_v1, v0, v1));
}

TEST(IntersectionUtilsTest, IsOnSegment2D)
{
    MathLib::HVector2 v0(0, 0);
    MathLib::HVector2 v1(1, 1);

    MathLib::HVector2 p_on_segment(0.5, 0.5);
    EXPECT_TRUE(MathLib::IntersectionUtils::IsOnSegment(p_on_segment, v0, v1));

    MathLib::HVector2 p_on_line_but_not_segment(2, 2);
    EXPECT_FALSE(MathLib::IntersectionUtils::IsOnSegment(p_on_line_but_not_segment, v0, v1));

    MathLib::HVector2 p_off_line(1, 0);
    EXPECT_FALSE(MathLib::IntersectionUtils::IsOnSegment(p_off_line, v0, v1));

    // 边界情况
    MathLib::HVector2 p_at_v0(0, 0);
    EXPECT_TRUE(MathLib::IntersectionUtils::IsOnSegment(p_at_v0, v0, v1));

    MathLib::HVector2 p_at_v1(1, 1);
    EXPECT_TRUE(MathLib::IntersectionUtils::IsOnSegment(p_at_v1, v0, v1));

    // 浮点数精度
    MathLib::HVector2 p_near_line(0.5000001, 0.4999999);
    EXPECT_TRUE(MathLib::IntersectionUtils::IsOnSegment(p_near_line, v0, v1));
}

TEST(IntersectionUtilsTest, IsOnLine3D)
{
    MathLib::HVector3 v0(0, 0, 0);
    MathLib::HVector3 v1(1, 1, 1);

    MathLib::HVector3 p_on_line(0.5, 0.5, 0.5);
    EXPECT_TRUE(MathLib::IntersectionUtils::IsOnLine(p_on_line, v0, v1));

    MathLib::HVector3 p_off_line(1, 0, 0);
    EXPECT_FALSE(MathLib::IntersectionUtils::IsOnLine(p_off_line, v0, v1));

    // 边界情况
    MathLib::HVector3 p_at_v0(0, 0, 0);
    EXPECT_TRUE(MathLib::IntersectionUtils::IsOnLine(p_at_v0, v0, v1));

    MathLib::HVector3 p_at_v1(1, 1, 1);
    EXPECT_TRUE(MathLib::IntersectionUtils::IsOnLine(p_at_v1, v0, v1));
}

TEST(IntersectionUtilsTest, IsOnSegment3D)
{
    MathLib::HVector3 v0(0, 0, 0);
    MathLib::HVector3 v1(1, 1, 1);

    MathLib::HVector3 p_on_segment(0.5, 0.5, 0.5);
    EXPECT_TRUE(MathLib::IntersectionUtils::IsOnSegment(p_on_segment, v0, v1));

    MathLib::HVector3 p_on_line_but_not_segment(2, 2, 2);
    EXPECT_FALSE(MathLib::IntersectionUtils::IsOnSegment(p_on_line_but_not_segment, v0, v1));

    MathLib::HVector3 p_off_line(1, 0, 0);
    EXPECT_FALSE(MathLib::IntersectionUtils::IsOnSegment(p_off_line, v0, v1));

    // 边界情况
    MathLib::HVector3 p_at_v0(0, 0, 0);
    EXPECT_TRUE(MathLib::IntersectionUtils::IsOnSegment(p_at_v0, v0, v1));

    MathLib::HVector3 p_at_v1(1, 1, 1);
    EXPECT_TRUE(MathLib::IntersectionUtils::IsOnSegment(p_at_v1, v0, v1));

    // 浮点数精度
    MathLib::HVector3 p_near_segment(0.5000001, 0.5000001, 0.4999999);
    EXPECT_TRUE(MathLib::IntersectionUtils::IsOnSegment(p_near_segment, v0, v1));
}

TEST(IntersectionUtilsTest, EdgeIntersectEdge2D)
{
    MathLib::HVector2 v0(0, 0);
    MathLib::HVector2 v1(1, 1);
    MathLib::HVector2 u0(0, 1);
    MathLib::HVector2 u1(1, 0);

    EXPECT_TRUE(MathLib::IntersectionUtils::EdgeIntersectEdge(v0, v1, u0, u1));

    MathLib::HVector2 u2(2, 2);
    EXPECT_FALSE(MathLib::IntersectionUtils::EdgeIntersectEdge(v0, v1, u0, u2));

    MathLib::HVector2 v2(0, 0);
    MathLib::HVector2 v3(2, 2);
    MathLib::HVector2 u3(1, 1);
    MathLib::HVector2 u4(3, 3);
    EXPECT_TRUE(MathLib::IntersectionUtils::EdgeIntersectEdge(v2, v3, u3, u4));

    MathLib::HVector2 v4(0, 0);
    MathLib::HVector2 v5(1, 1);
    MathLib::HVector2 u5(1, 1);
    MathLib::HVector2 u6(2, 2);
    EXPECT_TRUE(MathLib::IntersectionUtils::EdgeIntersectEdge(v4, v5, u5, u6));
}

TEST(IntersectionUtilsTest, EdgeIntersectTriangle2D)
{
    {
        MathLib::HVector2 v0(0, 0);
        MathLib::HVector2 v1(2, 2);
        MathLib::HVector2 u0(1, 0);
        MathLib::HVector2 u1(3, 0);
        MathLib::HVector2 u2(2, 3);

        EXPECT_TRUE(MathLib::IntersectionUtils::EdgeIntersectTriangle2D(v0, v1, u0, u1, u2)) << "Edge should intersect with the triangle.";
    }

    {
        MathLib::HVector2 v0(0, 0);
        MathLib::HVector2 v1(1, 1);
        MathLib::HVector2 u0(2, 0);
        MathLib::HVector2 u1(3, 0);
        MathLib::HVector2 u2(2, 3);

        EXPECT_FALSE(MathLib::IntersectionUtils::EdgeIntersectTriangle2D(v0, v1, u0, u1, u2)) << "Edge should not intersect with the triangle.";
    }

    {
        MathLib::HVector2 v0(0, 0);
        MathLib::HVector2 v1(1, 1);
        MathLib::HVector2 u0(1, 1);
        MathLib::HVector2 u1(2, 0);
        MathLib::HVector2 u2(2, 2);

        EXPECT_TRUE(MathLib::IntersectionUtils::EdgeIntersectTriangle2D(v0, v1, u0, u1, u2)) << "Edge should intersect at the triangle vertex.";
    }

    {
        MathLib::HVector2 v0(1, 1);
        MathLib::HVector2 v1(2, 2);
        MathLib::HVector2 u0(1, 1);
        MathLib::HVector2 u1(2, 2);
        MathLib::HVector2 u2(3, 0);

        EXPECT_TRUE(MathLib::IntersectionUtils::EdgeIntersectTriangle2D(v0, v1, u0, u1, u2)) << "Edge should coincide with the triangle edge.";
    }

    {
        MathLib::HVector2 v0(1.5, 1.5);
        MathLib::HVector2 v1(2, 2);
        MathLib::HVector2 u0(1, 1);
        MathLib::HVector2 u1(3, 1);
        MathLib::HVector2 u2(2, 3);

        EXPECT_TRUE(MathLib::IntersectionUtils::EdgeIntersectTriangle2D(v0, v1, u0, u1, u2)) << "Edge should be inside the triangle.";
    }
}

TEST(IntersectionUtilsTest, TriTriIntersect2D)
{
    // 原始测试用例
    MathLib::HVector2 v0(0, 0);
    MathLib::HVector2 v1(1, 0);
    MathLib::HVector2 v2(0, 1);
    MathLib::HVector2 u0(0.5, 0.5);
    MathLib::HVector2 u1(1.5, 0.5);
    MathLib::HVector2 u2(0.5, 1.5);

    EXPECT_TRUE(MathLib::IntersectionUtils::TriTriIntersect2D(v0, v1, v2, u0, u1, u2));

    MathLib::HVector2 u0_out(2, 2);
    MathLib::HVector2 u1_out(3, 2);
    MathLib::HVector2 u2_out(2, 3);
    EXPECT_FALSE(MathLib::IntersectionUtils::TriTriIntersect2D(v0, v1, v2, u0_out, u1_out, u2_out));

    // 三角形完全重叠
    MathLib::HVector2 v0_overlap(0, 0);
    MathLib::HVector2 v1_overlap(1, 0);
    MathLib::HVector2 v2_overlap(0, 1);
    EXPECT_TRUE(MathLib::IntersectionUtils::TriTriIntersect2D(v0, v1, v2, v0_overlap, v1_overlap, v2_overlap));

    // 三角形共享一条边
    MathLib::HVector2 u0_shared_edge(1, 0);
    MathLib::HVector2 u1_shared_edge(2, 0);
    MathLib::HVector2 u2_shared_edge(1, 1);
    EXPECT_TRUE(MathLib::IntersectionUtils::TriTriIntersect2D(v0, v1, v2, u0_shared_edge, u1_shared_edge, u2_shared_edge));

    // 三角形相交但没有共享边
    MathLib::HVector2 u0_intersect(0.5, -0.5);
    MathLib::HVector2 u1_intersect(1.5, -0.5);
    MathLib::HVector2 u2_intersect(0.5, 0.5);
    EXPECT_TRUE(MathLib::IntersectionUtils::TriTriIntersect2D(v0, v1, v2, u0_intersect, u1_intersect, u2_intersect));

    // 三角形在不同象限
    MathLib::HVector2 u0_diff_quadrant(-1, -1);
    MathLib::HVector2 u1_diff_quadrant(-2, -1);
    MathLib::HVector2 u2_diff_quadrant(-1, -2);
    EXPECT_FALSE(MathLib::IntersectionUtils::TriTriIntersect2D(v0, v1, v2, u0_diff_quadrant, u1_diff_quadrant, u2_diff_quadrant));

    // 一个三角形完全包含在另一个三角形中
    MathLib::HVector2 u0_inside(0.1, 0.1);
    MathLib::HVector2 u1_inside(0.9, 0.1);
    MathLib::HVector2 u2_inside(0.1, 0.9);
    EXPECT_TRUE(MathLib::IntersectionUtils::TriTriIntersect2D(v0, v1, v2, u0_inside, u1_inside, u2_inside));

    // 三角形仅在顶点处接触
    MathLib::HVector2 u0_touch_vertex(1, 1);
    MathLib::HVector2 u1_touch_vertex(1, 0);
    MathLib::HVector2 u2_touch_vertex(2, 0);
    EXPECT_TRUE(MathLib::IntersectionUtils::TriTriIntersect2D(v0, v1, v2, u0_touch_vertex, u1_touch_vertex, u2_touch_vertex));
}

TEST(IntersectionUtilsTest, RayIntersectAABBox2D)
{
    MathLib::HVector2 rayOrigin(0, 0);
    MathLib::HVector2 rayDir(1, 1);
    MathLib::HAABBox2D aabb(MathLib::HVector2(-1, -1), MathLib::HVector2(1, 1));
    MathLib::HReal tmin =0, tmax= H_REAL_MAX;

    // 测试射线从原点出发，方向为(1,1)，与AABB相交
    EXPECT_TRUE(MathLib::IntersectionUtils::RayIntersectAABBox(rayOrigin, rayDir, aabb, tmin, tmax));
    EXPECT_GE(tmax, tmin);

    // 测试射线从AABB外部出发，方向为(1,1)，不与AABB相交
    MathLib::HVector2 rayOrigin_outside(2, 2);
    EXPECT_FALSE(MathLib::IntersectionUtils::RayIntersectAABBox(rayOrigin_outside, rayDir, aabb, tmin, tmax));

    rayOrigin_outside = MathLib::HVector2(-10, -7);
    EXPECT_FALSE(MathLib::IntersectionUtils::RayIntersectAABBox(rayOrigin_outside, rayDir, aabb, tmin, tmax));

    // 新增测试用例：射线平行于AABB的边
    MathLib::HVector2 rayDir_parallel(1, 0);
    EXPECT_TRUE(MathLib::IntersectionUtils::RayIntersectAABBox(rayOrigin, rayDir_parallel, aabb, tmin, tmax));
    EXPECT_GE(tmax, tmin);

    // 新增测试用例：射线起点在AABB内部
    MathLib::HVector2 rayOrigin_inside(0, 0);
    EXPECT_TRUE(MathLib::IntersectionUtils::RayIntersectAABBox(rayOrigin_inside, rayDir, aabb, tmin, tmax));
    EXPECT_GE(tmax, tmin);

    // 新增测试用例：射线起点在AABB边界上
    MathLib::HVector2 rayOrigin_on_edge(1, 0);
    EXPECT_TRUE(MathLib::IntersectionUtils::RayIntersectAABBox(rayOrigin_on_edge, rayDir, aabb, tmin, tmax));
    EXPECT_GE(tmax, tmin);

    // 新增测试用例：射线与AABB相交于一个顶点
    MathLib::HVector2 rayDir_to_vertex(-1, -1);
    EXPECT_TRUE(MathLib::IntersectionUtils::RayIntersectAABBox(rayOrigin, rayDir_to_vertex, aabb, tmin, tmax));
    EXPECT_GE(tmax, tmin);

    // 新增测试用例：射线方向为零向量
    MathLib::HVector2 rayDir_zero(0, 0);
    EXPECT_TRUE(MathLib::IntersectionUtils::RayIntersectAABBox(rayOrigin, rayDir_zero, aabb, tmin, tmax));
    EXPECT_GE(tmax, tmin);
}

TEST(IntersectionUtilsTest, RayIntersectAABBox3D)
{
    MathLib::HVector3 rayOrigin(0, 0, 0);
    MathLib::HVector3 rayDir(1, 1, 1);
    MathLib::HAABBox3D aabb(MathLib::HVector3(-1, -1, -1), MathLib::HVector3(1, 1, 1));
    MathLib::HReal tmin, tmax;

    // 测试射线从原点出发，方向为(1,1,1)，与AABB相交
    EXPECT_TRUE(MathLib::IntersectionUtils::RayIntersectAABBox(rayOrigin, rayDir, aabb, tmin, tmax));
    EXPECT_GE(tmax, tmin);

    // 测试射线从AABB外部出发，方向为(1,1,1)，不与AABB相交
    MathLib::HVector3 rayOrigin_outside(2, 2, 2);
    EXPECT_FALSE(MathLib::IntersectionUtils::RayIntersectAABBox(rayOrigin_outside, rayDir, aabb, tmin, tmax));

    rayOrigin_outside = MathLib::HVector3(-10, -7, 0);
    EXPECT_FALSE(MathLib::IntersectionUtils::RayIntersectAABBox(rayOrigin_outside, rayDir, aabb, tmin, tmax));

    // 新增测试用例：射线平行于AABB的边
    MathLib::HVector3 rayDir_parallel(1, 0, 0);
    EXPECT_TRUE(MathLib::IntersectionUtils::RayIntersectAABBox(rayOrigin, rayDir_parallel, aabb, tmin, tmax));
    EXPECT_GE(tmax, tmin);

    // 新增测试用例：射线起点在AABB内部
    MathLib::HVector3 rayOrigin_inside(0, 0, 0);
    EXPECT_TRUE(MathLib::IntersectionUtils::RayIntersectAABBox(rayOrigin_inside, rayDir, aabb, tmin, tmax));
    EXPECT_GE(tmax, tmin);

    // 新增测试用例：射线起点在AABB边界上
    MathLib::HVector3 rayOrigin_on_edge(1, 0, 0);
    EXPECT_TRUE(MathLib::IntersectionUtils::RayIntersectAABBox(rayOrigin_on_edge, rayDir, aabb, tmin, tmax));
    EXPECT_GE(tmax, tmin);

    // 新增测试用例：射线与AABB相交于一个顶点
    MathLib::HVector3 rayDir_to_vertex(-1, -1, -1);
    EXPECT_TRUE(MathLib::IntersectionUtils::RayIntersectAABBox(rayOrigin, rayDir_to_vertex, aabb, tmin, tmax));
    EXPECT_GE(tmax, tmin);

    // 新增测试用例：射线方向为零向量
    MathLib::HVector3 rayDir_zero(0, 0, 0);
    EXPECT_TRUE(MathLib::IntersectionUtils::RayIntersectAABBox(rayOrigin, rayDir_zero, aabb, tmin, tmax));
}
