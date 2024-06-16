#pragma once
#include <Math/Math.h>
#include <Math/HashGrid.h>
#include <Math/GraphicUtils/MeshCommon.h>
#include <Math/Geometry/Geometry.h>
#ifdef USE_OPENMP
#include <omp.h>
#endif
namespace MathLib
{
	namespace Geometry
	{
		namespace Triangulate
		{
			class Delaunay2D
			{
			private:
				struct DTriangle
				{
					TriangleIndex ti;
					Geometry2D::CircumCircle circumCircle;
					DTriangle() {}
					DTriangle(const std::vector<HVector2> &points, uint32_t v0, uint32_t v1, uint32_t v2)
						: ti(v0, v1, v2), circumCircle(points, v0, v1, v2)
					{
					}

					DTriangle(uint32_t v0, uint32_t v1, uint32_t v2, const Geometry2D::CircumCircle &circle)
						: ti(v0, v1, v2), circumCircle(circle)
					{
					}
					void UpdateCircumCircle(const std::vector<HVector2> &points)
					{
						HVector2 p0(-2.5, -1);
						HVector2 p1(0, 5);
						HVector2 p2(2.5, -1);
						const HVector2 v0 = ti.vertices[0] < UINT32_MAX - 2 ? points[ti.vertices[0]] : (ti.vertices[0] == UINT32_MAX - 2 ? p0 : (ti.vertices[0] == UINT32_MAX - 1 ? p1 : p2));
						const HVector2 v1 = ti.vertices[1] < UINT32_MAX - 2 ? points[ti.vertices[1]] : (ti.vertices[1] == UINT32_MAX - 2 ? p0 : (ti.vertices[1] == UINT32_MAX - 1 ? p1 : p2));
						const HVector2 v2 = ti.vertices[2] < UINT32_MAX - 2 ? points[ti.vertices[2]] : (ti.vertices[2] == UINT32_MAX - 2 ? p0 : (ti.vertices[2] == UINT32_MAX - 1 ? p1 : p2));
						circumCircle.Update(v0, v1, v2);
					}
				};

			public:
				Delaunay2D()
				{
					HVector2 p0(-2.5, -1);
					HVector2 p1(0, 5);
					HVector2 p2(2.5, -1);
					m_SuperDTriangle = DTriangle(UINT32_MAX - 2, UINT32_MAX - 1, UINT32_MAX, Geometry2D::CircumCircle(p0, p1, p2));
				}
				~Delaunay2D() {}
				void Clear()
				{
					m_Points.clear();
					m_DTriangles.clear();
					m_SuperDTriangles.clear();
					m_BoundBox.setEmpty();
				}
				void InsertPoint(const HVector2 &point)
				{
					if (m_DTriangles.empty())
					{
						m_Points.push_back(point);
						if (m_Points.size() < 3)
							return;
						SetPoints({m_Points[0], m_Points[1], m_Points[2]});
						return;
					}
					HVector2 size0 = m_BoundBox.sizes();
					HVector2 center0 = m_BoundBox.center();
					const HReal maxXY0 = std::max(size0[0], size0[1]);
					m_BoundBox.extend(point);
					m_Points.push_back((point - center0) / maxXY0);

					HVector2 size = m_BoundBox.sizes();
					HVector2 center = m_BoundBox.center();
					const HReal maxXY = std::max(size[0], size[1]);

					HVector2 p = (point - center) / maxXY;

					for (size_t i = 0; i < m_Points.size(); i++)
					{
						auto &pt = m_Points[i];
						pt = ((pt * maxXY0 + center0) - center) / maxXY;
					}

					for (size_t i = 0; i < m_DTriangles.size(); i++)
					{
						auto &tri = m_DTriangles[i];
						tri.UpdateCircumCircle(m_Points);
					}

					for (size_t i = 0; i < m_SuperDTriangles.size(); i++)
					{
						auto &tri = m_SuperDTriangles[i];
						tri.UpdateCircumCircle(m_Points);
					}

					const uint32_t pointCount = m_Points.size();
					std::vector<DTriangle> triangles;
					std::vector<DTriangle> superDTriangles;
					triangles.reserve(pointCount);
					_CalculateDTriangles(pointCount - 1, triangles, superDTriangles);
					m_DTriangles = std::move(triangles);
					m_SuperDTriangles = std::move(superDTriangles);
				}

				void InsertPoints(std::vector<HVector2> points)
				{
					uint32_t startIndex = 0;
					for (; startIndex < points.size(); startIndex++)
					{
						InsertPoint(points[startIndex]);
						if (!m_DTriangles.empty())
							break;
					}
					HVector2 size0 = m_BoundBox.sizes();
					HVector2 center0 = m_BoundBox.center();
					const HReal maxXY0 = std::max(size0[0], size0[1]);
					m_Points.reserve(points.size() - startIndex);
					for (uint32_t i = startIndex + 1; i < points.size(); i++)
					{
						m_BoundBox.extend(points[i] - center0);
						m_Points.push_back((points[i] - center0) / maxXY0);
					}
					HVector2 size = m_BoundBox.sizes();
					HVector2 center = m_BoundBox.center();
					const HReal maxXY = std::max(size[0], size[1]);

					for (size_t i = 0; i < m_Points.size(); i++)
					{
						auto &pt = m_Points[i];
						pt = ((pt * maxXY0 + center0) - center) / maxXY;
					}

					for (size_t i = 0; i < m_DTriangles.size(); i++)
					{
						auto &tri = m_DTriangles[i];
						tri.UpdateCircumCircle(m_Points);
					}

					for (size_t i = 0; i < m_SuperDTriangles.size(); i++)
					{
						auto &tri = m_SuperDTriangles[i];
						tri.UpdateCircumCircle(m_Points);
					}

					const uint32_t pointCount = m_Points.size();
					std::vector<DTriangle> triangles;
					std::vector<DTriangle> superDTriangles;
					for (int l = pointCount - (points.size() - startIndex); l < pointCount; l++)
					{
						triangles.reserve(pointCount);
						_CalculateDTriangles(l, triangles, superDTriangles);
						m_DTriangles = std::move(triangles);
						m_SuperDTriangles = std::move(superDTriangles);
					}
				}

				void RemovePoint(const HVector2 &p)
				{
				}

				void SetPoints(const std::vector<HVector2> &points)
				{
					Clear();
					m_Points = points;
					if (m_Points.size() < 3)
						return;
					for (const HVector2 &p : m_Points)
					{
						m_BoundBox.extend(p);
					}
					HVector2 size = m_BoundBox.sizes();
					HVector2 center = m_BoundBox.center();
					const HReal maxXY = std::max(size[0], size[1]);

					for (auto &p : m_Points)
					{
						p = (p - center) / maxXY;
					}

					const uint32_t pointCount = m_Points.size();
					std::vector<DTriangle> triangles;
					std::vector<DTriangle> superDTriangles;
					m_SuperDTriangles.push_back(m_SuperDTriangle);
					for (int l = 0; l < pointCount; l++)
					{
						triangles.reserve(pointCount);
						_CalculateDTriangles(l, triangles, superDTriangles);
						m_DTriangles = std::move(triangles);
						m_SuperDTriangles = std::move(superDTriangles);
					}
				}

				std::vector<uint32_t> GetTriangles() const
				{
					std::vector<uint32_t> indices;
					indices.reserve(m_DTriangles.size() * 3);
					for (auto &t : m_DTriangles)
					{
						indices.push_back(t.ti.vertices[0]);
						indices.push_back(t.ti.vertices[1]);
						indices.push_back(t.ti.vertices[2]);
					}
					return indices;
				}

			private:
				void _CalculateDTriangles(const uint32_t index, std::vector<DTriangle> &triangles, std::vector<DTriangle> &superDTriangles)
				{
					triangles.clear();
					superDTriangles.clear();
					const HVector2 &p = m_Points[index];
					HVector2 p0(-2.5, -1);
					HVector2 p1(0, 5);
					HVector2 p2(2.5, -1);
					std::unordered_map<EdgeIndex, bool> edges;
					const uint32_t pointCount = m_Points.size();
					// edges.reserve(l+1);
					triangles.reserve(pointCount);

					for (auto const &tri : m_DTriangles)
					{
						if (tri.circumCircle.IsPointInside(p))
						{
							EdgeIndex e0(tri.ti.vertices[0], tri.ti.vertices[1]);
							EdgeIndex e1(tri.ti.vertices[1], tri.ti.vertices[2]);
							EdgeIndex e2(tri.ti.vertices[2], tri.ti.vertices[0]);

							const bool duplicate0 = edges.find(e0) != edges.end();
							const bool duplicate1 = edges.find(e1) != edges.end();
							const bool duplicate2 = edges.find(e2) != edges.end();

							if (duplicate0)
								m_DuplicateEdges[index].push_back(e0);
							if (duplicate1)
								m_DuplicateEdges[index].push_back(e1);
							if (duplicate2)
								m_DuplicateEdges[index].push_back(e2);

							edges[e0] = !duplicate0;
							edges[e1] = !duplicate1;
							edges[e2] = !duplicate2;
						}
						else
						{
							triangles.push_back(tri);
						}
					}

					for (auto const &tri : m_SuperDTriangles)
					{
						if (tri.circumCircle.IsPointInside(p))
						{
							EdgeIndex e0(tri.ti.vertices[0], tri.ti.vertices[1]);
							EdgeIndex e1(tri.ti.vertices[1], tri.ti.vertices[2]);
							EdgeIndex e2(tri.ti.vertices[2], tri.ti.vertices[0]);

							edges[e0] = edges.find(e0) == edges.end();
							edges[e1] = edges.find(e1) == edges.end();
							edges[e2] = edges.find(e2) == edges.end();
						}
						else
						{
							superDTriangles.push_back(tri);
						}
					}

					for (auto &edgeIt : edges)
					{
						const EdgeIndex &edge = edgeIt.first;
						if (!edgeIt.second)
							continue;

						const bool isSuper = edge.v0 >= UINT32_MAX - 2 || edge.v1 >= UINT32_MAX - 2;
						if (isSuper)
						{
							const HVector2 &v0 = edge.v0 < UINT32_MAX - 2 ? m_Points[edge.v0] : (edge.v0 == UINT32_MAX - 2 ? p0 : (edge.v0 == UINT32_MAX - 1 ? p1 : p2));
							const HVector2 &v1 = edge.v1 < UINT32_MAX - 2 ? m_Points[edge.v1] : (edge.v1 == UINT32_MAX - 2 ? p0 : (edge.v1 == UINT32_MAX - 1 ? p1 : p2));
							const HVector2 &v2 = p;
							superDTriangles.push_back(DTriangle(edge.v0, edge.v1, index, Geometry2D::CircumCircle(v0, v1, v2)));
						}
						else
							triangles.push_back(DTriangle(m_Points, edge.v0, edge.v1, index));
					}
				}

				void _CalculateEdges(const uint32_t &index, std::vector<EdgeIndex> &edges)
				{
				}

			private:
				DTriangle m_SuperDTriangle;
				std::vector<HVector2> m_Points;
				HAABBox2D m_BoundBox;
				std::unordered_map<uint32_t, std::vector<EdgeIndex>> m_DuplicateEdges;
				std::vector<DTriangle> m_DTriangles;
				std::vector<DTriangle> m_SuperDTriangles;
			};
		}
	}
}