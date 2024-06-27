#pragma once
#include <Math/Math.h>
#include <Math/Geometry/Intersection.h>
#include <Math/Geometry/Triangulate/earcut.hpp>
#include <Math/ObjectPool.h>
#include <Math/Geometry/Orientaion.h>
namespace MathLib
{
	namespace Geometry
	{
		namespace Triangulate
		{
			template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
			class EarClip2D
			{
			private:
				struct Node
				{
					IntType vertexIndex;
					HVector2 vertex;
					Node *prev = nullptr;
					Node *next = nullptr;
					IntType z = 0;
					Node *prevZ = nullptr;
					Node *nextZ = nullptr;
					bool steiner = false;
					Node(IntType index, const HVector2 &v) : vertexIndex(index), vertex(v) {}
					Node(const Node &) = delete;
					Node &operator=(const Node &) = delete;
					Node(Node &&) = delete;
					Node &operator=(Node &&) = delete;
					bool operator==(const Node &rhs) const
					{
						return vertex == rhs.vertex;
					}
				};

			public:
				EarClip2D() {}
				~EarClip2D() {}

				void SetPolygon(const std::vector<HVector2> &points, const PolygonIndex<IntType> &polygon)
				{
					m_Points = points;
					m_Polygon = polygon;
				}

				void SetPolygon(const std::vector<HVector2> &points)
				{
					m_Points = points;
					m_Polygon.vertices.resize(points.size());
					for (size_t i = 0; i < points.size(); i++)
					{
						m_Polygon[i] = i;
					}
				}

				void Triangulate()
				{
				}

				const std::vector<uint32_t> &GetTriangles() const
				{
					return m_Triangles;
				}

			private:
				bool _IsConvex(const HVector2 &prev, const HVector2 &cur, const HVector2 &next) const
				{
					HVector2 preEdge = prev - cur;
					HVector2 nextEdge = next - cur;
					return preEdge[0] * nextEdge[1] - preEdge[1] * nextEdge[0] >= 0.0f;
				}

				bool _IsEar(Node *ear) const
				{
					const Node *a = ear->prev;
					const Node *b = ear;
					const Node *c = ear->next;

					if (GreaterEqual(OrientationUtils::TriangleArea(a->vertex, b->vertex, c->vertex), 0))
						return false; // reflex, can't be an ear

					Node *p = ear->next->next;

					while (p != ear->prev)
					{
						if (IntersectionUtils::IsPointInTriangle(p->vertex, a->vertex, b->vertex, c->vertex) &&
							GreaterEqual(OrientationUtils::TriangleArea(p->prev->vertex, p->vertex, p->next->vertex), 0))
							return false;
						p = p->next;
					}

					return true;
				}

				Node *_InsertNode(IntType index, const HVector2 &v, Node *last)
				{
					Node *p = m_NodesPool.construct(index, v);
					if (last == nullptr)
					{
						p->prev = p;
						p->next = p;
					}
					else
					{
						p->next = last->next;
						p->prev = last;
						last->next->prev = p;
						last->next = p;
					}
					return p;
				}

				void _RemoveNode(Node *p)
				{
					if (p == nullptr)
						return;
					p->next->prev = p->prev;
					p->prev->next = p->next;
					if (p->prevZ != nullptr)
						p->prevZ->nextZ = p->nextZ;
					if (p->nextZ != nullptr)
						p->nextZ->prevZ = p->prevZ;
				}

				Node *_LinkedList(const bool clockwise)
				{
					HReal sum = 0;
					const size_t len = m_Polygon.vertices.size();
					size_t i, j;
					Node *list = nullptr;

					for (i = 0, j = len > 0 ? len - 1 : 0; i < len; j = i++)
					{
						const HVector2 &p0 = m_Points[m_Polygon[i]];
						const HVector2 &p1 = m_Points[m_Polygon[j]];
						sum += (p1[0] - p0[0]) * (p1[1] + p0[1]);
					}

					if (clockwise == (sum > 0))
					{
						for (i = 0; i < len; i++)
							list = _InsertNode(m_Polygon[m_Vertices + i], m_Points[m_Polygon[i]], list);
					}
					else
					{
						for (i = len; i > 0; i--)
							list = _InsertNode(m_Polygon[m_Vertices + i], m_Points[m_Polygon[i]], list);
					}

					if (list && list->next && (*list == *(list->next)))
					{
						_RemoveNode(list);
						list = nullptr;
					}

					m_Vertices += len;
					return list;
				}

				Node *_FilterPoints(Node *start, Node *end)
				{
					if (end == nullptr)
						end = start;
					bool again = false;
					Node *p = start;
					do
					{
						again = false;
						if (!p->steiner && (*p == *(p->next) ||
											IsZero(OrientationUtils::TriangleArea(p->prev->vertex, p->vertex, p->next->vertex))))
						{
							_RemoveNode(p);
							p = end = p->next;
							if (p == p->next)
								break;
							again = true;
						}
						else
						{
							p = p->next;
						}
					} while (again || p != end);
					return end;
				}

				bool _LocallyInside(Node *a, Node *b)
				{
					return OrientationUtils::TriangleArea(a->prev->vertex, a->vertex, a->next->vertex) < 0
							   ? GreaterEqual(OrientationUtils::TriangleArea(a->vertex, b->vertex, a->next->vertex), 0) &&
									 GreaterEqual(OrientationUtils::TriangleArea(a->vertex, a->prev->vertex, b->vertex), 0)
							   : (OrientationUtils::TriangleArea(a->vertex, b->vertex, a->prev->vertex) < 0 ||
								  GreaterEqual(OrientationUtils::TriangleArea(a->vertex, a->next->vertex, b->vertex), 0)) &&
				}

				bool _MiddleInside(Node *a, Node *b)
				{
					const Node *p = a;
					bool inside = false;
					HVector2 c = (a->vertex + b->vertex) / 2;
					do
					{
						if (((p->vertex[1] > c[1]) != (p->next->vertex[1] > c[1])) && p->next->vertex[1] != p->vertex[1] &&
							(c[0] < (p->next->vertex[0] - p->vertex[0]) * (c[1] - p->vertex[1]) / (p->next->vertex[1] - p->vertex[1]) + p->vertex[0]))
							inside = !inside;
						p = p->next;
					} while (p != a);

					return inside;
				}

				Node *_SplitPolygon(Node *a, Node *b)
				{
					Node *a2 = m_NodesPool.construct(a->vertexIndex, a->vertex);
					Node *b2 = m_NodesPool.construct(b->vertexIndex, b->vertex);
					Node *an = a->next;
					Node *bp = b->prev;

					a->next = b;
					b->prev = a;

					a2->next = an;
					an->prev = a2;

					b2->next = a2;
					a2->prev = b2;

					bp->next = b2;
					b2->prev = bp;

					return b2;
				}

				Node *_FindHoleBridge(Node *hole, Node *outerNode)
				{
					Node *p = outerNode;
					const HVector2 &h = hole->vertex;
					HReal qx = H_REAL_MIN;
					Node *m = nullptr;

					// find a segment intersected by a ray from the hole's leftmost Vertex to the left;
					// segment's endpoint with lesser x will be potential connection Vertex
					do
					{
						if (h[1] <= p->vertex[1] && h[1] >= p->next->vertex[1] && p->next->vertex[1] != p->vertex[1])
						{
							HReal x = p->vertex[0] + (h[1] - p->vertex[1]) * (p->next->vertex[0] - p->vertex[0]) / (p->next->vertex[1] - p->vertex[1]);
							if (x <= h[0] && x > qx)
							{
								qx = x;
								if (x == h[0])
								{
									if (h[1] == p->vertex[1])
										return p;
									if (h[1] == p->next->vertex[1])
										return p->next;
								}
								m = p->vertex[0] < p->next->vertex[0] ? p : p->next;
							}
						}
						p = p->next;
					} while (p != outerNode);

					if (!m)
						return 0;

					if (h[0] == qx)
						return m; // hole touches outer segment; pick leftmost endpoint

					// look for points inside the triangle of hole Vertex, segment intersection and endpoint;
					// if there are no points found, we have a valid connection;
					// otherwise choose the Vertex of the minimum angle with the ray as connection Vertex

					const Node *stop = m;
					HReal tanMin = H_REAL_MIN;
					HReal tanCur = 0;

					p = m;
					const HVector2 &mp = m->vertex;

					do
					{
						if (h[0] >= p->vertex[0] && p->vertex[0] >= m[0] && h[0] != p->vertex[0] &&
							IntersectionUtils::IsPointInTriangle(p->vertex,
																 HVector2([1] < m[1] ? h[0] : qx, h[1]),
																 m,
																 HVector2(h[1] < m[1] ? qx : h[0], h[1])))
						{
							tanCur = std::abs(h[1] - p->vertex[1]) / (h[0] - p->[0]); // tangential

							if (_LocallyInside(p, hole) &&
								(tanCur < tanMin || (tanCur == tanMin && (p->vertex[0] > m->vertex[0] || _SectorContainsSector(m, p)))))
							{
								m = p;
								tanMin = tanCur;
							}
						}

						p = p->next;
					} while (p != stop);

					return m;
				}

				bool _SectorContainsSector(const Node *m, const Node *p)
				{
					return Less(OrientationUtils::TriangleArea(m->prev->vertex, m->vertex, p->prev->vertex), 0) &&
						   Less(OrientationUtils::TriangleArea(p->next->vertex, m->vertex, m->next->vertex), 0);
				}

			private:
				HAABBox2D m_BoundingBox;
				IntType m_Vertices = 0;
				std::vector<HVector2> m_Points;
				PolygonIndex<IntType> m_Polygon;
				std::vector<uint32_t> m_Triangles;
				ObjectPool<Node> m_NodesPool;
			};

			typedef EarClip2D<uint8_t> EarClip2DUI8;
			typedef EarClip2D<uint16_t> EarClip2DUI16;
			typedef EarClip2D<uint32_t> EarClip2DUI32;
			typedef EarClip2D<uint64_t> EarClip2DUI64;
		} // namespace Triangulate
	} // namespace Geometry
} // namespace MathLib