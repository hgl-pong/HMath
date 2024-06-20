#pragma once
#include <Math/Math.h>
#include <Math/Geometry/Intersection.h>
namespace MathLib
{
	namespace Geometry
	{
		namespace Triangulate
		{
			class EarClip2D
			{
			public:
				EarClip2D() {}
				~EarClip2D() {}

				void SetPolygon(const std::vector<HVector2>& points, const std::vector<uint32_t>& polygon)
				{
					m_Points = points;
					m_Polygon = polygon;
				}

				void SetPolygon(const std::vector<HVector2>& points)
				{
					m_Points = points;
					m_Polygon.resize(points.size());
					for (size_t i = 0; i < points.size(); i++)
					{
						m_Polygon[i] = i;
					}
				}

				void Triangulate()
				{
					m_Triangles.clear();
					if (m_Polygon.size() < 3)
					{
						return;
					}
					if (!_IsClockwise())
					{
						std::reverse(m_Polygon.begin(), m_Polygon.end());
					}
					while (m_Polygon.size() > 3)
					{
						bool found = false;
						for (size_t i = 0; i < m_Polygon.size(); i++)
						{
							const uint32_t preIndex = (i + m_Polygon.size() - 1) % m_Polygon.size();
							const uint32_t curIndex = i;
							const uint32_t nextIndex = (i + 1) % m_Polygon.size();
							if (_IsEar(preIndex, curIndex, nextIndex))
							{
								m_Triangles.push_back(m_Polygon[preIndex]);
								m_Triangles.push_back(m_Polygon[curIndex]);
								m_Triangles.push_back(m_Polygon[nextIndex]);
								m_Polygon.erase(m_Polygon.begin() + curIndex);
								found = true;
								break;
							}
						}
						if (!found)
						{
							break;
						}
					}
					m_Triangles.push_back(m_Polygon[0]);
					m_Triangles.push_back(m_Polygon[1]);
					m_Triangles.push_back(m_Polygon[2]);
				}

				const std::vector<uint32_t>& GetTriangles() const
				{
					return m_Triangles;
				}

			private:
				bool _IsClockwise() const
				{
					float sum = 0.0f;
					size_t n = m_Polygon.size();
					for (size_t i = 0; i < n; i++)
					{
						const HVector2& cur = m_Points[m_Polygon[i]];
						const HVector2& next = m_Points[m_Polygon[(i + 1) % n]];
						sum += (next[0] - cur[0]) * (next[1] + cur[1]);
					}
					return sum > 0.0f;
				}

				bool _IsConvex(const HVector2& prev, const HVector2& cur, const HVector2& next) const
				{
					HVector2 preEdge = prev - cur;
					HVector2 nextEdge = next - cur;
					return preEdge[0] * nextEdge[1] - preEdge[1] * nextEdge[0] >= 0.0f;
				}

				bool _IsEar(const uint32_t&preIndex, const uint32_t& curIndex, const uint32_t& nextIndex) const
				{
					const HVector2& prev = m_Points[m_Polygon[preIndex]];
					const HVector2& cur = m_Points[m_Polygon[curIndex]];
					const HVector2& next = m_Points[m_Polygon[nextIndex]];
					if (!_IsConvex(prev, cur, next))
					{
						return false;
					}
					for (size_t i = 0; i < m_Polygon.size(); i++)
					{
						if (i == preIndex || i == curIndex || i == nextIndex)
						{
							continue;
						}
						if (IntersectionUtils::IsPointInTriangle2D(m_Points[m_Polygon[i]], prev, cur, next))
						{
							return false;
						}
					}
					return true;
				}
			private:
				std::vector<HVector2> m_Points;
				std::vector<uint32_t> m_Polygon;
				std::vector<uint32_t> m_Triangles;
			};
		}// namespace Triangulate
	}// namespace Geometry
}// namespace MathLib