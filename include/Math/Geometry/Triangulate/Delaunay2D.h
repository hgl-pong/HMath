#pragma once
#include <Math/Math.h>
#include <Math/HashGrid.h>
#include <Math/GraphicUtils/MeshCommon.h>

namespace MathLib
{
	namespace Geometry
	{
		namespace Triangulate
		{
			class Delaunay2D
			{
			private:
				struct CircumCircle
				{
					HVector2 center;
					HReal radius;

					CircumCircle(const std::vector<HVector2>& points, uint32_t v0, uint32_t v1, uint32_t v2)
					{
						HVector2 p0 = points[v0];
						HVector2 p1 = points[v1];
						HVector2 p2 = points[v2];
						CircumCircle(p0, p1, p2);
					}

					CircumCircle(const HVector2& p0, const HVector2& p1, const HVector2& p2) {
						HMatrix2 A_matrix;
						HVector2 b_vector;

						A_matrix(0, 0) = 2 * (p1.x() - p0.x());
						A_matrix(0, 1) = 2 * (p1.y() - p0.y());
						A_matrix(1, 0) = 2 * (p2.x() - p0.x());
						A_matrix(1, 1) = 2 * (p2.y() - p0.y());

						b_vector(0) = p1.squaredNorm() - p0.squaredNorm();
						b_vector(1) = p2.squaredNorm() - p0.squaredNorm();

						center = A_matrix.colPivHouseholderQr().solve(b_vector);
						radius = (center - p0).norm();
					}

					bool IsPointInside(const HVector2& point)const
					{
						HVector2 diff = point - center;
						HReal dist = diff.norm();
						return dist <= radius;
					}
				};

				struct TriangleIndex
				{
					uint32_t vertices[3];
					TriangleIndex(uint32_t p1, uint32_t p2, uint32_t p3)
					{
						vertices[0] = p1;
						vertices[1] = p2;
						vertices[2] = p3;
					}
				};

				struct Triangle
				{
					TriangleIndex ti;
					CircumCircle circumCircle;
					Triangle(const std::vector<HVector2>& points, uint32_t v0, uint32_t v1, uint32_t v2)
						: ti(v0, v1, v2)
						, circumCircle(points, v0, v1, v2)
					{
					}

					Triangle(uint32_t v0, uint32_t v1, uint32_t v2, const CircumCircle& circle)
						: ti(v0, v1, v2), circumCircle(circle)
					{
					}
				};
			public:
				Delaunay2D() {}
				~Delaunay2D() {}
				void Clear()
				{
					m_Points.clear();
					m_Triangles.clear();
				}
				void InsertPoint(const HVector2& p)
				{
				}
				void RemovePoint(const HVector2& p)
				{
				}
				void SetPoints(const std::vector<HVector2>& points)
				{
					m_Triangles.clear();
					m_Points = points;
					if (m_Points.size() < 3)
						return;
					HAABBox2D aabb;
					for (const HVector2& p : m_Points)
					{
						aabb.extend(p);
					}
					HVector2 size = aabb.sizes();
					HVector2 center = aabb.center();
					const HReal maxXY = std::max(size[0], size[1]);

					HVector2 p0(center[0] - 2.5 * maxXY, center[1] - maxXY);
					HVector2 p1(center[0], center[1] + 5 * maxXY);
					HVector2 p2(center[0] + 2.5 * maxXY, center[1] - maxXY);

					const uint32_t pointCount = m_Points.size();
					std::vector<Triangle> triangleSet;
					triangleSet.push_back(Triangle(pointCount, pointCount + 1, pointCount + 2, CircumCircle(p0, p1, p2)));
					for (int l = 0; l < pointCount; l++)
					{
						auto& p = m_Points[l];
						std::vector<EdgeIndex> edges;
						std::vector<Triangle> triangles;
						for (auto const& tri : triangleSet)
						{
							if (tri.circumCircle.IsPointInside(p))
							{
								edges.push_back(EdgeIndex(tri.ti.vertices[0], tri.ti.vertices[1]));
								edges.push_back(EdgeIndex(tri.ti.vertices[1], tri.ti.vertices[2]));
								edges.push_back(EdgeIndex(tri.ti.vertices[2], tri.ti.vertices[0]));
							}
							else
							{
								triangles.push_back(tri);
							}
						}

						///* Delete duplicate edges. */
						std::vector<bool> remove(edges.size(), false);
						for (auto it1 = edges.begin(); it1 != edges.end(); ++it1) {
							for (auto it2 = edges.begin(); it2 != edges.end(); ++it2) {
								if (it1 == it2) {
									continue;
								}
								if (*it1 == *it2) {
									remove[std::distance(edges.begin(), it1)] = true;
									remove[std::distance(edges.begin(), it2)] = true;
								}
							}
						}

						edges.erase(
							std::remove_if(edges.begin(), edges.end(),
								[&](auto const& e) { return remove[&e - &edges[0]]; }),
							edges.end());

						for (auto& edge : edges)
						{
							const HVector2 v0 = edge.v0 < pointCount ? m_Points[edge.v0] : (edge.v0 == pointCount ? p0 : (edge.v0 == pointCount + 1 ? p1 : p2));
							const HVector2 v1 = edge.v1 < pointCount ? m_Points[edge.v1] : (edge.v1 == pointCount ? p0 : (edge.v1 == pointCount + 1 ? p1 : p2));
							const HVector2 v2 = p;
							triangles.push_back(Triangle(edge.v0, edge.v1, l, CircumCircle(v0, v1, v2)));
						}

						triangleSet = triangles;
					}
					triangleSet.erase(
						std::remove_if(triangleSet.begin(), triangleSet.end(),
							[&](auto const& tri) {
								return tri.ti.vertices[0] >= pointCount || tri.ti.vertices[1] >= pointCount || tri.ti.vertices[2] >= pointCount;
							}),
						triangleSet.end());
					m_Triangles = std::move(triangleSet);
				}

				std::vector<uint32_t> GetTriangles() const
				{
					std::vector<uint32_t> indices;
					indices.reserve(m_Triangles.size() * 3);
					for (auto& t : m_Triangles)
					{
						indices.push_back(t.ti.vertices[0]);
						indices.push_back(t.ti.vertices[1]);
						indices.push_back(t.ti.vertices[2]);
					}
					return indices;
				}

			private:
				HashGrid2D<HVector2> m_HashGrid;
				std::vector<HVector2> m_Points;
				std::vector<Triangle> m_Triangles;
			};
		}
	}
}