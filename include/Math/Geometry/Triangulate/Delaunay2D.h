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
				static const HVector2 m_SuperP0;
				static const HVector2 m_SuperP1;
				static const HVector2 m_SuperP2;
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
						const HVector2 v0 = ti.vertices[0] < UINT32_MAX - 2 ? points[ti.vertices[0]] : (ti.vertices[0] == UINT32_MAX - 2 ? m_SuperP0 : (ti.vertices[0] == UINT32_MAX - 1 ? m_SuperP1 : m_SuperP2));
						const HVector2 v1 = ti.vertices[1] < UINT32_MAX - 2 ? points[ti.vertices[1]] : (ti.vertices[1] == UINT32_MAX - 2 ? m_SuperP0 : (ti.vertices[1] == UINT32_MAX - 1 ? m_SuperP1 : m_SuperP2));
						const HVector2 v2 = ti.vertices[2] < UINT32_MAX - 2 ? points[ti.vertices[2]] : (ti.vertices[2] == UINT32_MAX - 2 ? m_SuperP0 : (ti.vertices[2] == UINT32_MAX - 1 ? m_SuperP1 : m_SuperP2));
						circumCircle.Update(v0, v1, v2);
					}
				};
				static const DTriangle m_SuperTriangle;
			public:
				Delaunay2D()
				{
				}
				~Delaunay2D() {}
				void Clear()
				{
					m_Points.clear();
					m_Triangles.clear();
					m_SuperTriangles.clear();
					m_BoundBox.setEmpty();
				}
				void InsertPoint(const HVector2 &point)
				{
					if (m_Triangles.empty())
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

					for (size_t i = 0; i < m_Triangles.size(); i++)
					{
						auto &tri = m_Triangles[i];
						tri.UpdateCircumCircle(m_Points);
					}

					for (size_t i = 0; i < m_SuperTriangles.size(); i++)
					{
						auto &tri = m_SuperTriangles[i];
						tri.UpdateCircumCircle(m_Points);
					}

					const uint32_t pointCount = m_Points.size();
					std::vector<DTriangle> triangles;
					std::vector<DTriangle> superDTriangles;
					triangles.reserve(pointCount);
					_CalculateDTriangles(pointCount - 1, triangles, superDTriangles);
					m_Triangles = std::move(triangles);
					m_SuperTriangles = std::move(superDTriangles);
				}

				void InsertPoints(std::vector<HVector2> points)
				{
					uint32_t startIndex = 0;
					for (; startIndex < points.size(); startIndex++)
					{
						InsertPoint(points[startIndex]);
						if (!m_Triangles.empty())
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

					for (size_t i = 0; i < m_Triangles.size(); i++)
					{
						auto &tri = m_Triangles[i];
						tri.UpdateCircumCircle(m_Points);
					}

					for (size_t i = 0; i < m_SuperTriangles.size(); i++)
					{
						auto &tri = m_SuperTriangles[i];
						tri.UpdateCircumCircle(m_Points);
					}
					
					const uint32_t pointCount = m_Points.size();
					for (int l = pointCount - (points.size() - startIndex); l < pointCount; l++)
					{					
						std::vector<DTriangle> triangles;
						std::vector<DTriangle> superTriangles;
						triangles.reserve(pointCount);
						_CalculateDTriangles(l, triangles, superTriangles);
						m_Triangles = std::move(triangles);
						m_SuperTriangles = std::move(superTriangles);
					}
				}

				void ErasePoint(const HVector2 &p)
				{
					uint32_t pointIndex = UINT32_MAX;
					for (uint32_t i = 0; i < m_Points.size(); i++)
					{
						if (m_Points[i] == p)
						{
							pointIndex = i;
							break;
						}
					}
					if(pointIndex==UINT32_MAX)
						return;
					ErasePoint(pointIndex);
				}

				void ErasePoint(const uint32_t& pointIndex)
				{
					if (pointIndex >= m_Points.size())
						return;
					m_Points.erase(m_Points.begin() + pointIndex);
					std::vector<HVector2> points=std::move(m_Points);
					HVector2 size0 = m_BoundBox.sizes();
					HVector2 center0 = m_BoundBox.center();
					const HReal maxXY0 = std::max(size0[0], size0[1]);
					for (uint32_t i = 0; i < points.size(); i++)
					{
						points[i] = points[i] * maxXY0 + center0;
					}
					SetPoints(points);
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
					m_SuperTriangles.push_back(m_SuperTriangle);
					for (int l = 0; l < pointCount; l++)
					{
						triangles.reserve(pointCount);
						_CalculateDTriangles(l, triangles, superDTriangles);
						std::swap(triangles, m_Triangles);
						std::swap(superDTriangles, m_SuperTriangles);
					}
				}

				std::vector<uint32_t> GetTriangles() const
				{
					std::vector<uint32_t> indices;
					indices.reserve(m_Triangles.size() * 3);
					for (auto &t : m_Triangles)
					{
						indices.push_back(t.ti.vertices[0]);
						indices.push_back(t.ti.vertices[1]);
						indices.push_back(t.ti.vertices[2]);
					}
					return indices;
				}

				std::unordered_set<uint32_t> FindNeighbors(const uint32_t& pointIndex) const
				{
					if (pointIndex >= m_Points.size())
						return std::unordered_set<uint32_t>();
					std::unordered_set<uint32_t> neighbors;
					for (auto const& tri : m_Triangles)
					{
						if (tri.ti.vertices[0] == pointIndex || tri.ti.vertices[1] == pointIndex || tri.ti.vertices[2] == pointIndex)
						{
							for (int i = 0; i < 3; i++)
							{
								if(tri.ti.vertices[i]!=pointIndex)
									neighbors.insert(tri.ti.vertices[i]);
							}
						}
					}
					return neighbors;
				}

				std::vector < std::unordered_set<uint32_t>> Neighbors() const
				{
					std::vector<std::unordered_set<uint32_t>> neighbors;
					neighbors.resize(m_Points.size());
					for (auto const& tri : m_Triangles)
					{
						const uint32_t& v0=tri.ti.vertices[0];
						const uint32_t& v1=tri.ti.vertices[1];
						const uint32_t& v2=tri.ti.vertices[2];

						neighbors[v0].insert(v1);
						neighbors[v0].insert(v2);
						neighbors[v1].insert(v0);
						neighbors[v1].insert(v2);
						neighbors[v2].insert(v0);
						neighbors[v2].insert(v1);
					}
					return neighbors;
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
					triangles.reserve(pointCount);

					for (auto const &tri : m_Triangles)
					{
						if (tri.circumCircle.IsPointInside(p))
						{
							EdgeIndex e0(tri.ti.vertices[0], tri.ti.vertices[1]);
							EdgeIndex e1(tri.ti.vertices[1], tri.ti.vertices[2]);
							EdgeIndex e2(tri.ti.vertices[2], tri.ti.vertices[0]);

							const bool duplicate0 = edges.find(e0) != edges.end();
							const bool duplicate1 = edges.find(e1) != edges.end();
							const bool duplicate2 = edges.find(e2) != edges.end();

							edges[e0] = !duplicate0;
							edges[e1] = !duplicate1;
							edges[e2] = !duplicate2;
						}
						else
						{
							triangles.push_back(tri);
						}
					}

					for (auto const &tri : m_SuperTriangles)
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

			private:
				std::vector<HVector2> m_Points;
				HAABBox2D m_BoundBox;
				std::vector<DTriangle> m_Triangles;
				std::vector<DTriangle> m_SuperTriangles;
			};
			const HVector2 Delaunay2D::m_SuperP0 = HVector2(-2.5, -1);
			const HVector2 Delaunay2D::m_SuperP1 = HVector2(0, 5);
			const HVector2 Delaunay2D::m_SuperP2 = HVector2(2.5, -1);
			const Delaunay2D::DTriangle Delaunay2D::m_SuperTriangle = Delaunay2D::DTriangle(UINT32_MAX - 2, UINT32_MAX - 1, UINT32_MAX, Geometry2D::CircumCircle(m_SuperP0, m_SuperP1, m_SuperP2));
		}
	}
}