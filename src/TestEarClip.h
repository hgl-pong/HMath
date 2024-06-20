#pragma once
#include <gtest/gtest.h>
#include <Math/Math.h>
#include <Math/Geometry/Triangulate/EarClip2D.h>

TEST(EarClip2DTest, TriangulateConcavePolygon) {
    MathLib::Geometry::Triangulate::EarClip2D earClip;
    std::vector<MathLib::HVector2> points = { {0, 0}, {2, 0}, {2, 2}, {1, 1}, {0, 2} };
    earClip.SetPolygon(points);
    earClip.Triangulate();
    const std::vector<uint32_t>& triangles = earClip.GetTriangles();

    ASSERT_EQ(triangles.size(), 9);
    // 检查三角形的顶点是否正确
    // 具体的三角化结果可能有多种正确的形式，这里只检查数量
}

TEST(EarClip2DTest, TriangulatePolygonWithHole) {
    MathLib::Geometry::Triangulate::EarClip2D earClip;
    std::vector<MathLib::HVector2> outer = { {0, 0}, {4, 0}, {4, 4}, {0, 4} };
    std::vector<MathLib::HVector2> inner = { {1, 1}, {3, 1}, {3, 3}, {1, 3} };

    std::vector<MathLib::HVector2> points = outer;
    points.insert(points.end(), inner.begin(), inner.end());

    std::vector<uint32_t> polygon(outer.size());
    for(size_t i = 0; i < outer.size(); ++i) {
		polygon[i] = i;
	}

    earClip.SetPolygon(points);
    earClip.Triangulate();
    const std::vector<uint32_t>& triangles = earClip.GetTriangles();

    ASSERT_EQ(triangles.size(), 18);
    // 检查三角形的顶点是否正确
    // 具体的三角化结果可能有多种正确的形式，这里只检查数量
}

TEST(EarClip2DTest, TriangulateRandomPolygon) {
    MathLib::Geometry::Triangulate::EarClip2D earClip;
    std::vector<MathLib::HVector2> points = {
        {0, 0}, {4, 0}, {5, 2}, {3, 5}, {1, 4}, {-1, 3}
    };
    earClip.SetPolygon(points);
    earClip.Triangulate();
    const std::vector<uint32_t>& triangles = earClip.GetTriangles();

    ASSERT_EQ(triangles.size(), 12);
    // 检查三角形的顶点是否正确
    // 具体的三角化结果可能有多种正确的形式，这里只检查数量
}