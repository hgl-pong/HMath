#pragma once
#include <Math/Math.h>
#include <Math/HashGrid.h>
#include <Math/GraphicUtils/MeshCommon.h>
#include <Math/Geometry/Geometry.h>
#include <unordered_set>
namespace MathLib
{
	namespace Geometry
	{
		namespace Triangulate
		{
			template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
			class Delaunay2D
			{
			private:
				typedef EdgeIndex<IntType> InternalEdgeIndex;
				typedef TriangleIndex<IntType> InternalTriangleIndex;
				typedef Geometry2D::CircumCircle<IntType> InternnalCircumCircle;
				static const IntType Max_IntType = std::numeric_limits<IntType>::max();

				struct DTriangle
				{
					InternalTriangleIndex ti;
					InternnalCircumCircle circumCircle;
					DTriangle() {}
					DTriangle(const std::vector<HVector2> &points, IntType v0, IntType v1, IntType v2)
						: ti(v0, v1, v2), circumCircle(points, v0, v1, v2)
					{
					}

					DTriangle(IntType v0, IntType v1, IntType v2, const InternnalCircumCircle &circle)
						: ti(v0, v1, v2), circumCircle(circle)
					{
					}
					void UpdateCircumCircle(const std::vector<HVector2> &points)
					{
						const HVector2 v0 = ti[0] < Max_IntType - 2 ? points[ti[0]] : (ti[0] == Max_IntType - 2 ? m_SuperP0 : (ti[0] == Max_IntType - 1 ? m_SuperP1 : m_SuperP2));
						const HVector2 v1 = ti[1] < Max_IntType - 2 ? points[ti[1]] : (ti[1] == Max_IntType - 2 ? m_SuperP0 : (ti[1] == Max_IntType - 1 ? m_SuperP1 : m_SuperP2));
						const HVector2 v2 = ti[2] < Max_IntType - 2 ? points[ti[2]] : (ti[2] == Max_IntType - 2 ? m_SuperP0 : (ti[2] == Max_IntType - 1 ? m_SuperP1 : m_SuperP2));
						circumCircle.Update(v0, v1, v2);
					}
					IntType operator[](size_t i) const
					{
						return ti.vertices[i];
					}
				};

				inline static HVector2 m_SuperP0{-2.5, -1};
				inline static HVector2 m_SuperP1{0, 5};
				inline static HVector2 m_SuperP2{2.5, -1};
				inline static DTriangle m_SuperTriangle = DTriangle(Max_IntType - 2, Max_IntType - 1, Max_IntType,
															 InternnalCircumCircle(m_SuperP0, m_SuperP1, m_SuperP2));

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

					const IntType pointCount = m_Points.size();
					std::vector<DTriangle> triangles;
					std::vector<DTriangle> superDTriangles;
					triangles.reserve(pointCount);
					_CalculateDTriangles(pointCount - 1, triangles, superDTriangles);
					m_Triangles = std::move(triangles);
					m_SuperTriangles = std::move(superDTriangles);
				}

				void InsertPoints(std::vector<HVector2> points)
				{
					IntType startIndex = 0;
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
					for (IntType i = startIndex + 1; i < points.size(); i++)
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

					const IntType pointCount = m_Points.size();
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
					IntType pointIndex = Max_IntType;
					for (IntType i = 0; i < m_Points.size(); i++)
					{
						if (m_Points[i] == p)
						{
							pointIndex = i;
							break;
						}
					}
					if (pointIndex == Max_IntType)
						return;
					ErasePoint(pointIndex);
				}

				void ErasePoint(const IntType &pointIndex)
				{
					if (pointIndex >= m_Points.size())
						return;
					m_Points.erase(m_Points.begin() + pointIndex);
					std::vector<HVector2> points = std::move(m_Points);
					HVector2 size0 = m_BoundBox.sizes();
					HVector2 center0 = m_BoundBox.center();
					const HReal maxXY0 = std::max(size0[0], size0[1]);
					for (IntType i = 0; i < points.size(); i++)
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

					const IntType pointCount = m_Points.size();
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

				std::vector<IntType> GetTriangles() const
				{
					std::vector<IntType> indices;
					indices.reserve(m_Triangles.size() * 3);
					for (auto &t : m_Triangles)
					{
						indices.push_back(t[0]);
						indices.push_back(t[1]);
						indices.push_back(t[2]);
					}
					return indices;
				}

				std::unordered_set<IntType> FindNeighbors(const IntType &pointIndex) const
				{
					if (pointIndex >= m_Points.size())
						return std::unordered_set<IntType>();
					std::unordered_set<IntType> neighbors;
					for (auto const &tri : m_Triangles)
					{
						if (tri[0] == pointIndex || tri[1] == pointIndex || tri[2] == pointIndex)
						{
							for (int i = 0; i < 3; i++)
							{
								if (tri[i] != pointIndex)
									neighbors.insert(tri[i]);
							}
						}
					}
					return neighbors;
				}

				std::vector<std::unordered_set<IntType>> Neighbors() const
				{
					std::vector<std::unordered_set<IntType>> neighbors;
					neighbors.resize(m_Points.size());
					for (auto const &tri : m_Triangles)
					{
						const IntType &v0 = tri[0];
						const IntType &v1 = tri[1];
						const IntType &v2 = tri[2];

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
				void _CalculateDTriangles(const IntType index, std::vector<DTriangle> &triangles, std::vector<DTriangle> &superDTriangles)
				{
					triangles.clear();
					superDTriangles.clear();
					const HVector2 &p = m_Points[index];
					HVector2 p0(-2.5, -1);
					HVector2 p1(0, 5);
					HVector2 p2(2.5, -1);
					std::unordered_map<InternalEdgeIndex, bool> edges;
					const IntType pointCount = m_Points.size();
					triangles.reserve(pointCount);

					for (auto const &tri : m_Triangles)
					{
						if (tri.circumCircle.IsPointInside(p))
						{
							InternalEdgeIndex e0(tri[0], tri[1]);
							InternalEdgeIndex e1(tri[1], tri[2]);
							InternalEdgeIndex e2(tri[2], tri[0]);

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
							InternalEdgeIndex e0(tri[0], tri[1]);
							InternalEdgeIndex e1(tri[1], tri[2]);
							InternalEdgeIndex e2(tri[2], tri[0]);

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
						const InternalEdgeIndex&edge = edgeIt.first;
						if (!edgeIt.second)
							continue;

						const bool isSuper = edge.v0 >= Max_IntType - 2 || edge.v1 >= Max_IntType - 2;
						if (isSuper)
						{
							const HVector2 &v0 = edge.v0 < Max_IntType - 2 ? m_Points[edge.v0] : (edge.v0 == Max_IntType - 2 ? p0 : (edge.v0 == Max_IntType - 1 ? p1 : p2));
							const HVector2 &v1 = edge.v1 < Max_IntType - 2 ? m_Points[edge.v1] : (edge.v1 == Max_IntType - 2 ? p0 : (edge.v1 == Max_IntType - 1 ? p1 : p2));
							const HVector2 &v2 = p;
							superDTriangles.push_back(DTriangle(edge.v0, edge.v1, index, InternnalCircumCircle(v0, v1, v2)));
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

			typedef Delaunay2D<uint8_t> Delaunay2DUI8;
			typedef Delaunay2D<uint16_t> Delaunay2DUI16;
			typedef Delaunay2D<uint32_t> Delaunay2DUI32;
			typedef Delaunay2D<uint64_t> Delaunay2DUI64;
		}
	}
}