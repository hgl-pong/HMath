#pragma once
#include <Math/Math.h>
#include <Math/GraphicUtils/MeshCommon.h>
#include <vector>
#include <Math/Geometry/Intersection.h>
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
			public:
				typedef std::vector<PolygonIndex<IntType>> Polygon;
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
					Node() = delete;
					Node(IntType index, const HVector2 &v) : vertexIndex(index), vertex(v) {}
					Node(const Node &) = delete;
					Node &operator=(const Node &) = delete;
					Node(Node &&) = delete;
					Node &operator=(Node &&) = delete;
					bool operator==(const Node &rhs) const
					{
						return vertex == rhs.vertex;
					}
					bool EqualTo(const Node &rhs) const
					{
						return vertex == rhs.vertex;
					}
					HReal operator[](size_t i) const
					{
						return vertex[i];
					}

				};

				HReal _Area(const Node *p, const Node *q, const Node *r) const
				{
					HReal area = OrientationUtils::TriangleArea(p->vertex, q->vertex, r->vertex);
					return IsZero(area) ? 0 : -area;
				}

				bool _IsPointInTriangle(const Node *p, const Node *a, const Node *b, const Node *c) const
				{
					return IntersectionUtils::IsPointInTriangle(p->vertex, a->vertex, b->vertex, c->vertex);
				}

				bool _IsEdgeEdgeIntersect(const Node *p1, const Node *q1, const Node *p2, const Node *q2) const
				{
					return IntersectionUtils::EdgeIntersectEdge(p1->vertex, q1->vertex, p2->vertex, q2->vertex);
				}

			public:
				EarClip2D() {}
				~EarClip2D() {}

				void SetPolygon(const std::vector<HVector2> &points, const Polygon &polygon)
				{
					m_Points = points;
					m_Polygon = polygon;
				}

				void SetPolygon(const std::vector<HVector2> &points)
				{
					m_Points = points;
					m_Polygon.resize(1);
					PolygonIndex<IntType> &polygon = m_Polygon[0];
					polygon.vertices.resize(points.size());
					for (size_t i = 0; i < points.size(); i++)
					{
						polygon[i] = i;
					}
				}

				static std::vector<IntType> Triangulate(const std::vector<HVector2> &points, const Polygon &polygon)
				{
					EarClip2D<IntType> earClip;
					earClip.SetPolygon(points, polygon);
					earClip.Triangulate();
					return earClip.GetTriangles();
				}

				static std::vector<IntType> Triangulate(const std::vector<HVector2> &points)
				{
					EarClip2D<IntType> earClip;
					earClip.SetPolygon(points);
					earClip.Triangulate();
					return earClip.GetTriangles();
				}

				void Triangulate()
				{
					m_Triangles.clear();
					m_Vertices = 0;

					if (m_Polygon.empty())
						return;

					HReal x;
					HReal y;
					int threshold = 80;
					std::size_t len = 0;

					for (size_t i = 0; threshold >= 0 && i < m_Polygon.size(); i++) {
						threshold -= static_cast<int>(m_Polygon[i].vertices.size());
						len += m_Polygon[i].vertices.size();
					}

					// estimate size of nodes and indices
					m_NodesPool.reset(len * 3 / 2);
					m_Triangles.reserve(len + m_Polygon[0].vertices.size());

					Node *outerNode = _LinkedList(m_Polygon[0], true);
					if (!outerNode || outerNode->prev == outerNode->next)
						return;

					if (m_Polygon.size() > 1)
						outerNode = _EliminateHoles(outerNode);

					// if the shape is not too simple, we'll use z-order curve hash later; calculate polygon bbox
					m_bIsHashing = threshold < 0;
					if (m_bIsHashing)
					{
						Node *p = outerNode->next;
						m_BoundingBox.extend(outerNode->vertex);
						do
						{
							m_BoundingBox.extend(p->vertex);
							p = p->next;
						} while (p != outerNode);
					}

					_EarcutLinked(outerNode);

					m_NodesPool.clear();
				}

				const std::vector<uint32_t> &GetTriangles() const
				{
					return m_Triangles;
				}

			private:
				bool _IsEar(Node *ear) const
				{
					const Node *a = ear->prev;
					const Node *b = ear;
					const Node *c = ear->next;

					if (GreaterEqual(_Area(a, b, c), 0))
						return false; // reflex, can't be an ear

					Node *p = ear->next->next;

					while (p != ear->prev)
					{
						if (_IsPointInTriangle(p, a, b, c) &&
							GreaterEqual(_Area(p->prev, p, p->next), 0))
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

				Node *_LinkedList(const PolygonIndex<IntType>& ring, const bool clockwise)
				{
					HReal sum = 0;
					const size_t len = ring.vertices.size();
					size_t i, j;
					Node *list = nullptr;

					for (i = 0, j = len > 0 ? len - 1 : 0; i < len; j = i++)
					{
						const HVector2 &p0 = m_Points[ring[i]];
						const HVector2 &p1 = m_Points[ring[j]];
						sum += (p1[0] - p0[0]) * (p1[1] + p0[1]);
					}

					if (clockwise == (sum > 0))
					{
						for (i = 0; i < len; i++)
							list = _InsertNode(m_Vertices + i, m_Points[ring[i]], list);
					}
					else
					{
						for (i = len; i-- > 0; )
							list = _InsertNode(m_Vertices + i, m_Points[ring[i]], list);
					}

					if (list && list->next && list->EqualTo( *(list->next)))
					{
						_RemoveNode(list);
						list = list->next;
					}

					m_Vertices += len;
					return list;
				}

				Node *_FilterPoints(Node *start, Node *end = nullptr)
				{
					if (end == nullptr)
						end = start;
					bool again = false;
					Node *p = start;
					do
					{
						again = false;
						if (!p->steiner && (p->EqualTo(*(p->next)) ||
											IsZero(_Area(p->prev, p, p->next))))
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
					return Less(_Area(a->prev, a, a->next) ,0)
							   ? (GreaterEqual(_Area(a, b, a->next), 0) &&
									 GreaterEqual(_Area(a, a->prev, b), 0))
							   : (Less(_Area(a, b, a->prev) , 0) ||
								  Less(_Area(a, a->next, b), 0));
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
						if (h[0] >= p->vertex[0] 
							&& p->vertex[0] >= m->vertex[0]
							&& h[0] != p->vertex[0] 
							&& IntersectionUtils::IsPointInTriangle(p->vertex,
																 HVector2(h[1] < m->vertex[1] ? h[0] : qx, h[1]),
																 m->vertex,
																 HVector2(h[1] < m->vertex[1] ? qx : h[0], h[1])))
						{
							tanCur = std::abs(h[1] - p->vertex[1]) / (h[0] - p->vertex[0]); // tangential

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
					return Less(_Area(m->prev, m, p->prev), 0) &&
						   Less(_Area(p->next, m, m->next), 0);
				}

				Node *_SortLinked(Node *list)
				{
					assert(list);
					Node *p;
					Node *q;
					Node *e;
					Node *tail;
					int i, numMerges, pSize, qSize;
					int inSize = 1;

					for (;;)
					{
						p = list;
						list = nullptr;
						tail = nullptr;
						numMerges = 0;

						while (p)
						{
							numMerges++;
							q = p;
							pSize = 0;
							for (i = 0; i < inSize; i++)
							{
								pSize++;
								q = q->nextZ;
								if (!q)
									break;
							}

							qSize = inSize;

							while (pSize > 0 || (qSize > 0 && q))
							{

								if (pSize == 0)
								{
									e = q;
									q = q->nextZ;
									qSize--;
								}
								else if (qSize == 0 || !q)
								{
									e = p;
									p = p->nextZ;
									pSize--;
								}
								else if (p->z <= q->z)
								{
									e = p;
									p = p->nextZ;
									pSize--;
								}
								else
								{
									e = q;
									q = q->nextZ;
									qSize--;
								}

								if (tail)
									tail->nextZ = e;
								else
									list = e;

								e->prevZ = tail;
								tail = e;
							}

							p = q;
						}

						tail->nextZ = nullptr;

						if (numMerges <= 1)
							return list;

						inSize *= 2;
					}
				}

				uint32_t _ZOrder(const HVector2 &p)
				{
					const HVector2 &min = m_BoundingBox.min();
					const HVector2 &sizes = m_BoundingBox.sizes();
					HReal inv_size = 1.0f / std::max(sizes[0], sizes[1]);
					if (std::isnan(inv_size) || std::isinf(inv_size))
						inv_size = 0.0f;
					uint32_t x = static_cast<uint32_t>(32767.0 * (p[0] - min[0]) * inv_size);
					uint32_t y = static_cast<uint32_t>(32767.0 * (p[1] - min[1]) * inv_size);

					x = (x | (x << 8)) & 0x00FF00FF;
					x = (x | (x << 4)) & 0x0F0F0F0F;
					x = (x | (x << 2)) & 0x33333333;
					x = (x | (x << 1)) & 0x55555555;

					y = (y | (y << 8)) & 0x00FF00FF;
					y = (y | (y << 4)) & 0x0F0F0F0F;
					y = (y | (y << 2)) & 0x33333333;
					y = (y | (y << 1)) & 0x55555555;

					return x | (y << 1);
				}

				bool _IsEarHashed(Node *ear)
				{
					const Node *a = ear->prev;
					const Node *b = ear;
					const Node *c = ear->next;

					if (_Area(a, b, c) >= 0)
						return false; // reflex, can't be an ear

					const HReal minTX = (std::min)((*a)[0], (std::min)((*b)[0], (*c)[0]));
					const HReal minTY = (std::min)((*a)[1], (std::min)((*b)[1], (*c)[1]));
					const HReal maxTX = (std::max)((*a)[0], (std::max)((*b)[0], (*c)[0]));
					const HReal maxTY = (std::max)((*a)[1], (std::max)((*b)[1], (*c)[1]));

					const uint32_t minZ = _ZOrder(HVector2(minTX, minTY));
					const uint32_t maxZ = _ZOrder(HVector2(maxTX, maxTY));

					Node *p = ear->nextZ;

					while (p && p->z <= maxZ)
					{
						if (p != ear->prev && p != ear->next &&
							_IsPointInTriangle(p, a, b, c) &&
							_Area(p->prev, p, p->next) >= 0)
							return false;
						p = p->nextZ;
					}

					// then look for points in decreasing z-order
					p = ear->prevZ;

					while (p && p->z >= minZ)
					{
						if (p != ear->prev && p != ear->next &&
							_IsPointInTriangle(p, a, b, c) &&
							_Area(p->prev, p, p->next) >= 0)
							return false;
						p = p->prevZ;
					}

					return true;
				}

				Node *_CureLocalIntersections(Node *start)
				{
					Node *p = start;
					do
					{
						Node *a = p->prev;
						Node *b = p->next->next;

						// a self-intersection where edge (v[i-1],v[i]) intersects (v[i+1],v[i+2])
						if (!a ->EqualTo(*b) && _IsEdgeEdgeIntersect(a, p, p->next, b) && _LocallyInside(a, b) && _LocallyInside(b, a))
						{
							m_Triangles.emplace_back(a->vertexIndex);
							m_Triangles.emplace_back(p->vertexIndex);
							m_Triangles.emplace_back(b->vertexIndex);

							// remove two nodes involved
							_RemoveNode(p);
							_RemoveNode(p->next);

							p = start = b;
						}
						p = p->next;
					} while (p != start);

					return _FilterPoints(p);
				}

				void _IndexCurve(Node *start)
				{
					assert(start);
					Node *p = start;

					do
					{
						p->z = p->z ? p->z : _ZOrder(p->vertex);
						p->prevZ = p->prev;
						p->nextZ = p->next;
						p = p->next;
					} while (p != start);

					p->prevZ->nextZ = nullptr;
					p->prevZ = nullptr;

					_SortLinked(p);
				}

				void _EarcutLinked(Node *ear, int pass = 0 )
				{
					if (!ear)
						return;

					// interlink polygon nodes in z-order
					if (!pass && m_bIsHashing)
						_IndexCurve(ear);

					Node *stop = ear;
					Node *prev;
					Node *next;

					int iterations = 0;

					// iterate through ears, slicing them one by one
					while (ear->prev != ear->next)
					{
						iterations++;
						prev = ear->prev;
						next = ear->next;

						if (m_bIsHashing ? _IsEarHashed(ear) : _IsEar(ear))
						{
							// cut off the triangle
							m_Triangles.emplace_back(prev->vertexIndex);
							m_Triangles.emplace_back(ear->vertexIndex);
							m_Triangles.emplace_back(next->vertexIndex);

							_RemoveNode(ear);

							// skipping the next vertices leads to less sliver triangles
							ear = next->next;
							stop = next->next;

							continue;
						}

						ear = next;

						// if we looped through the whole remaining polygon and can't find any more ears
						if (ear == stop)
						{
							// try filtering points and slicing again
							if (!pass)
								_EarcutLinked(_FilterPoints(ear), 1);

							// if this didn't work, try curing all small self-intersections locally
							else if (pass == 1)
							{
								ear = _CureLocalIntersections(_FilterPoints(ear));
								_EarcutLinked(ear, 2);

								// as a last resort, try splitting the remaining polygon into two
							}
							else if (pass == 2)
								_SplitEarcut(ear);

							break;
						}
					}
				}

				Node *_EliminateHoles(Node *outerNode)
				{
					const size_t len = m_Polygon.size();

					std::vector<Node *> queue;
					for (size_t i = 1; i < len; i++)
					{
						Node *list = _LinkedList(m_Polygon[i], false);
						if (list)
						{
							if (list == list->next)
								list->steiner = true;
							queue.push_back(_GetLeftmost(list));
						}
					}
					std::sort(queue.begin(), queue.end(), [](const Node *a, const Node *b)
							  { return a->vertex[0] < b->vertex[0]; });

					// process holes from left to right
					for (size_t i = 0; i < queue.size(); i++)
					{
						_EliminateHole(queue[i], outerNode);
						outerNode = _FilterPoints(outerNode, outerNode->next);
					}

					return outerNode;
				}

				Node *_GetLeftmost(Node *start)
				{
					Node *p = start;
					Node *leftmost = start;
					do
					{
						const HVector2 &v = p->vertex;
						const HVector2 &lv = leftmost->vertex;
						if (v[0] < lv[0] || (v[0]==lv[0] && v[1] < lv[1]))
							leftmost = p;
						p = p->next;
					} while (p != start);

					return leftmost;
				}

				void _EliminateHole(Node *hole, Node *outerNode)
				{
					outerNode = _FindHoleBridge(hole, outerNode);
					if (outerNode)
					{
						Node *b = _SplitPolygon(outerNode, hole);

						// filter out colinear points around cuts
						_FilterPoints(outerNode, outerNode->next);
						_FilterPoints(b, b->next);
					}
				}

				void _SplitEarcut(Node* start) {
					// look for a valid diagonal that divides the polygon into two
					Node* a = start;
					do {
						Node* b = a->next->next;
						while (b != a->prev) {
							if (a->vertexIndex != b->vertexIndex && _IsValidDiagonal(a, b)) {
								// split the polygon in two by the diagonal
								Node* c = _SplitPolygon(a, b);

								// filter colinear points around the cuts
								a = _FilterPoints(a, a->next);
								c = _FilterPoints(c, c->next);

								// run earcut on each half
								_EarcutLinked(a);
								_EarcutLinked(c);
								return;
							}
							b = b->next;
						}
						a = a->next;
					} while (a != start);
				}

				bool _IsValidDiagonal(Node* a, Node* b) {
					return a->next->vertexIndex != b->vertexIndex && a->prev->vertexIndex != b->vertexIndex && !_IntersectsPolygon(a, b) && // dones't intersect other edges
						((_LocallyInside(a, b) && _LocallyInside(b, a) && _MiddleInside(a, b) && // locally visible
							(_Area(a->prev, a, b->prev) != 0.0 || _Area(a, b->prev, b) != 0.0)) || // does not create opposite-facing sectors
							(*a==*b && _Area(a->prev, a, a->next) > 0 && _Area(b->prev, b, b->next) > 0)); // special zero-length case
				}

				bool _IntersectsPolygon(const Node* a, const Node* b) {
					const Node* p = a;
					do {
						if (p->vertexIndex != a->vertexIndex 
							&& p->next->vertexIndex != a->vertexIndex
							&& p->vertexIndex != b->vertexIndex 
							&& p->next->vertexIndex != b->vertexIndex 
							&&_IsEdgeEdgeIntersect(p, p->next, a, b)) return true;
						p = p->next;
					} while (p != a);

					return false;
				}

			private:
				bool m_bIsHashing = false;
				HAABBox2D m_BoundingBox;
				IntType m_Vertices = 0;
				std::vector<HVector2> m_Points;
				Polygon m_Polygon;
				std::vector<IntType> m_Triangles;
				ObjectPool<Node> m_NodesPool;
			};

			typedef EarClip2D<uint8_t> EarClip2DUI8;
			typedef EarClip2D<uint16_t> EarClip2DUI16;
			typedef EarClip2D<uint32_t> EarClip2DUI32;
			typedef EarClip2D<uint64_t> EarClip2DUI64;
		} // namespace Triangulate
	} // namespace Geometry
} // namespace MathLib