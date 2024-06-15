#pragma once
#include <Math/Math.h>

namespace MathLib
{
	struct EdgeIndex
	{
		uint32_t v0;
		uint32_t v1;
		EdgeIndex() = default;
		EdgeIndex(uint32_t v0, uint32_t v1) : v0(v0), v1(v1) {}
		bool operator==(const EdgeIndex &rhs) const
		{
			return (v0 == rhs.v0 && v1 == rhs.v1) || (v0 == rhs.v1 && v1 == rhs.v0);
		}
		bool operator!=(const EdgeIndex &rhs) const
		{
			return !(v0 == rhs.v0 && v1 == rhs.v1) || (v0 == rhs.v1 && v1 == rhs.v0);
		}
	};

	struct TriangleIndex
	{
		uint32_t vertices[3];
		TriangleIndex() {}
		TriangleIndex(uint32_t p1, uint32_t p2, uint32_t p3)
		{
			vertices[0] = p1;
			vertices[1] = p2;
			vertices[2] = p3;
		}
		bool operator==(const TriangleIndex &rhs) const
		{
			return (vertices[0] == rhs.vertices[0] && vertices[1] == rhs.vertices[1] && vertices[2] == rhs.vertices[2]) ||
				   (vertices[0] == rhs.vertices[1] && vertices[1] == rhs.vertices[2] && vertices[2] == rhs.vertices[0]) ||
				   (vertices[0] == rhs.vertices[2] && vertices[1] == rhs.vertices[0] && vertices[2] == rhs.vertices[1]);
		}
		bool operator!=(const TriangleIndex &rhs) const
		{
			return !(vertices[0] == rhs.vertices[0] && vertices[1] == rhs.vertices[1] && vertices[2] == rhs.vertices[2]) &&
				   !(vertices[0] == rhs.vertices[1] && vertices[1] == rhs.vertices[2] && vertices[2] == rhs.vertices[0]) &&
				   !(vertices[0] == rhs.vertices[2] && vertices[1] == rhs.vertices[0] && vertices[2] == rhs.vertices[1]);
		}
	};

} // namespace MathLib

namespace std
{
	template <>
	struct hash<MathLib::EdgeIndex>
	{
		size_t operator()(const MathLib::EdgeIndex &edge) const
		{
			return std::hash<uint32_t>()(edge.v0) ^ std::hash<uint32_t>()(edge.v1);
		}
	};

	template <>
	struct hash<MathLib::TriangleIndex>
	{
		size_t operator()(const MathLib::TriangleIndex &tri) const
		{
			return std::hash<uint32_t>()(tri.vertices[0]) ^ std::hash<uint32_t>()(tri.vertices[1]) ^ std::hash<uint32_t>()(tri.vertices[2]);
		}
	};

}