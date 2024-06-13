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
					bool IsPointInside(const HVector2 &point)
					{
						HVector2 diff = point - center;
						HReal dist = diff.norm();
						return dist < radius;
					}
				};
				struct Triangle
				{
					uint32_t vertices[3];
					Triangle(uint32_t p1, uint32_t p2, uint32_t p3)
					{
						vertices[0] = p1;
						vertices[1] = p2;
						vertices[2] = p3;
					}
				};
				struct EdgeHash
				{
					size_t operator()(const EdgeIndex& edge) const
					{
						return std::hash<uint32_t>()(edge.v0) ^ std::hash<uint32_t>()(edge.v1);
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
				void InsertPoint(const HVector2 &p)
				{
				}
				void RemovePoint(const HVector2 &p)
				{
				}
				void SetPoints(const std::vector<HVector2> &points)
				{
					m_Points = points;
					if (m_Points.size() < 3)
						return;
					HAABBox2D aabb;
					for (const HVector2 &p : m_Points)
					{
						aabb.extend(p);
					}
					HVector2 size = aabb.sizes();
					HVector2 center = aabb.center();
					const HReal maxXY = std::max(size[0], size[1]);

					HVector2 p0(center[0] - 2.5 * maxXY, center[1] - maxXY);
					HVector2 p1(center[0], center[1] + 5 * maxXY);
					HVector2 p2(center[0] + 2.5 * maxXY, center[1] - maxXY);

					CircumCircle superCircle = _CalculateCircumCircle(p0, p1, p2);
					std::vector<uint32_t> badTriangles;
					const uint32_t pointCount = m_Points.size();
					std::vector<CircumCircle> circumCircles;
					std::vector<Triangle> triangleSet = m_Triangles;
					circumCircles.push_back(superCircle);
					triangleSet.push_back(Triangle(pointCount, pointCount + 1, pointCount + 2));
					for (int l = 0; l < pointCount; l++)
					{
						auto &p = m_Points[l];
						std::unordered_set<EdgeIndex,EdgeHash> edges;
						std::vector<Triangle> triangles;
						for (int i = 0; i < triangleSet.size(); i++)
						{
							const Triangle &t = triangleSet[i];
							CircumCircle &circle = circumCircles[i];
							if (circle.IsPointInside(p))
							{
								// if (t.vertices[0] >= pointCount || t.vertices[1] >= pointCount || t.vertices[2] >= pointCount)
								//	continue;
								edges.emplace(EdgeIndex(t.vertices[0], t.vertices[1]));
								edges.emplace(EdgeIndex(t.vertices[1], t.vertices[2]));
								edges.emplace(EdgeIndex(t.vertices[2], t.vertices[0]));
								badTriangles.push_back(i);
								// triangleSet.push_back(t);
								// circumCircles.push_back(_CalculateCircumCircle(m_Points, t.vertices[0], t.vertices[1],t.vertices[2]));
							}
						}

						for (auto &edge : edges)
						{
							triangleSet.push_back(Triangle(edge.v0, edge.v1, l));
							const HVector2 v0 = edge.v0 < pointCount ? m_Points[edge.v0] : (edge.v0 == pointCount ? p0 : (edge.v0 == pointCount + 1 ? p1 : p2));
							const HVector2 v1 = edge.v1 < pointCount ? m_Points[edge.v1] : (edge.v1 == pointCount ? p0 : (edge.v1 == pointCount + 1 ? p1 : p2));
							const HVector2 v2 = m_Points[l];
							circumCircles.push_back(_CalculateCircumCircle(v0, v1, v2));
						}

						for (int i = 0; i < badTriangles.size(); i++)
						{
							triangleSet.erase(triangleSet.begin() + badTriangles[i]);
						}
						badTriangles.clear();
					}
					triangleSet.erase(
						std::remove_if(triangleSet.begin(), triangleSet.end(),
							[&](auto const& tri) {
								return tri.vertices[0] >= pointCount || tri.vertices[1] >= pointCount || tri.vertices[2] >= pointCount;
							}),
						triangleSet.end());
					m_Triangles = std::move(triangleSet);
					std::swap(m_Triangles.front(), m_Triangles.back());
					m_Triangles.pop_back();
				}

				std::vector<uint32_t> GetTriangles() const
				{
					std::vector<uint32_t> indices;
					indices.reserve(m_Triangles.size() * 3);
					for(auto& t : m_Triangles)
					{
						indices.push_back(t.vertices[0]);
						indices.push_back(t.vertices[1]);
						indices.push_back(t.vertices[2]);
					}
					return indices;
				}

			private:
				CircumCircle _CalculateCircumCircle(const std::vector<HVector2> &points, uint32_t v0, uint32_t v1, uint32_t v2)
				{
					HVector2 p0 = points[v0];
					HVector2 p1 = points[v1];
					HVector2 p2 = points[v2];
					return _CalculateCircumCircle(p0, p1, p2);
				}

				CircumCircle _CalculateCircumCircle(const HVector2 &p0, const HVector2 &p1, const HVector2 &p2)
				{
					CircumCircle circle;
					HVector2 center = (p0 + p1 + p2) / 3;
					HReal radius = (p0 - center).norm();
					circle.center = center;
					circle.radius = radius;
					return circle;
				}

			private:
				HashGrid2D<HVector2> m_HashGrid;
				std::vector<HVector2> m_Points;
				std::vector<Triangle> m_Triangles;
			};
		}
	}
}