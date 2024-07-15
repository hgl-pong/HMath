#pragma once
#include <gtest/gtest.h>
#include <Math/Math.h>
#include <Math/Geometry/Triangulate/EarClip2D.h>

TEST(EarClip2DTest, TriangulateConcavePolygon) {
    MathLib::Geometry::Triangulate::EarClip2D<uint32_t> earClip;
    std::vector<MathLib::HVector2> points = { {0, 0}, {2, 0}, {2, 2}, {1, 1}, {0, 2} };
    earClip.SetPolygon(points);
    earClip.Triangulate();
    const std::vector<uint32_t>& triangles = earClip.GetTriangles();

    ASSERT_EQ(triangles.size(), 9);
}

TEST(EarClip2DTest, TriangulatePolygonWithHole) {
    MathLib::Geometry::Triangulate::EarClip2D<uint32_t> earClip;
    std::vector<MathLib::HVector2> outer = { {0, 0}, {4, 0}, {4, 4}, {0, 4} };
    std::vector<MathLib::HVector2> inner = { {1, 1}, {3, 1}, {3, 3}, {1, 3} };

    std::vector<MathLib::HVector2> points = outer;
    points.insert(points.end(), inner.begin(), inner.end());

    MathLib::Geometry::Triangulate::EarClip2D<uint32_t>::Polygon polygon(2);
    for(size_t i = 0; i < outer.size(); ++i) {
		polygon[0].vertices.push_back(i);
        polygon[1].vertices.push_back(4 + i);
	}   

    earClip.SetPolygon(points,polygon);
    earClip.Triangulate();
    const std::vector<uint32_t>& triangles = earClip.GetTriangles();

    ASSERT_EQ(triangles.size(), 24);
}

TEST(EarClip2DTest, TriangulateRandomPolygon) {
    MathLib::Geometry::Triangulate::EarClip2D<uint32_t> earClip;
    std::vector<MathLib::HVector2> points = {
        {0, 0}, {4, 0}, {5, 2}, {3, 5}, {1, 4}, {-1, 3}
    };
    earClip.SetPolygon(points);
    earClip.Triangulate();
    const std::vector<uint32_t>& triangles = earClip.GetTriangles();

    ASSERT_EQ(triangles.size(), 12);
}