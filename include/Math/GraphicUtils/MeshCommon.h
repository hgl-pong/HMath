#pragma once
#include <Math/Math.h>

namespace MathLib
{
	template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
	struct EdgeIndex
	{
		IntType v0;
		IntType v1;
		EdgeIndex() = default;
		EdgeIndex(IntType v0, IntType v1) : v0(v0), v1(v1) {}
		bool operator==(const EdgeIndex &rhs) const
		{
			return (v0 == rhs.v0 && v1 == rhs.v1) || (v0 == rhs.v1 && v1 == rhs.v0);
		}
		bool operator!=(const EdgeIndex &rhs) const
		{
			return !(v0 == rhs.v0 && v1 == rhs.v1) || (v0 == rhs.v1 && v1 == rhs.v0);
		}
		IntType operator[](size_t i) const
		{
			return i == 0 ? v0 : v1;
		}
		IntType &operator[](size_t i)
		{
			return i == 0 ? v0 : v1;
		}
	};

	template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
	struct TriangleIndex
	{
		IntType vertices[3];
		TriangleIndex() {}
		TriangleIndex(IntType p1, IntType p2, IntType p3)
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
		IntType operator[](size_t i) const
		{
			return vertices[i];
		}
		IntType &operator[](size_t i)
		{
			return vertices[i];
		}
	};

	template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
	struct TetrahedronIndex
	{
		IntType vertices[4];
		IntType neighbors[4];
		TetrahedronIndex() {}
		TetrahedronIndex(IntType v0, IntType v1, IntType v2, IntType v3)
		{
			vertices[0] = v0;
			vertices[1] = v1;
			vertices[2] = v2;
			vertices[3] = v3;
		}

		bool operator==(const TetrahedronIndex &rhs) const
		{
			return (vertices[0] == rhs.vertices[0] && vertices[1] == rhs.vertices[1] && vertices[2] == rhs.vertices[2] && vertices[3] == rhs.vertices[3]) ||
				   (vertices[0] == rhs.vertices[1] && vertices[1] == rhs.vertices[2] && vertices[2] == rhs.vertices[3] && vertices[3] == rhs.vertices[0]) ||
				   (vertices[0] == rhs.vertices[2] && vertices[1] == rhs.vertices[3] && vertices[2] == rhs.vertices[0] && vertices[3] == rhs.vertices[1]) ||
				   (vertices[0] == rhs.vertices[3] && vertices[1] == rhs.vertices[0] && vertices[2] == rhs.vertices[1] && vertices[3] == rhs.vertices[2]);
		}

		bool operator!=(const TetrahedronIndex &rhs) const
		{
			return !(vertices[0] == rhs.vertices[0] && vertices[1] == rhs.vertices[1] && vertices[2] == rhs.vertices[2] && vertices[3] == rhs.vertices[3]) &&
				   !(vertices[0] == rhs.vertices[1] && vertices[1] == rhs.vertices[2] && vertices[2] == rhs.vertices[3] && vertices[3] == rhs.vertices[0]) &&
				   !(vertices[0] == rhs.vertices[2] && vertices[1] == rhs.vertices[3] && vertices[2] == rhs.vertices[0] && vertices[3] == rhs.vertices[1]) &&
				   !(vertices[0] == rhs.vertices[3] && vertices[1] == rhs.vertices[0] && vertices[2] == rhs.vertices[1] && vertices[3] == rhs.vertices[2]);
		};

		IntType operator[](size_t i) const
		{
			return vertices[i];
		}
		IntType &operator[](size_t i)
		{
			return vertices[i];
		}
	};

	template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
	struct PolygonIndex
	{
		std::vector<IntType> vertices;
		PolygonIndex() {}
		PolygonIndex(const std::vector<IntType> &v) : vertices(v) {}
		bool operator==(const PolygonIndex &rhs) const
		{
			if (vertices.size() != rhs.vertices.size())
				return false;
			for (size_t i = 0; i < vertices.size(); i++)
			{
				if (vertices[i] != rhs.vertices[i])
					return false;
			}
			return true;
		}
		bool operator!=(const PolygonIndex &rhs) const
		{
			if (vertices.size() != rhs.vertices.size())
				return true;
			for (size_t i = 0; i < vertices.size(); i++)
			{
				if (vertices[i] != rhs.vertices[i])
					return true;
			}
			return false;
		}
		IntType operator[](size_t i) const
		{
			return vertices[i];
		}
		IntType &operator[](size_t i)
		{
			return vertices[i];
		}
	};
	typedef EdgeIndex<uint8_t> EdgeIndexUI8;
	typedef EdgeIndex<uint16_t> EdgeIndexUI16;
	typedef EdgeIndex<uint32_t> EdgeIndexUI32;
	typedef EdgeIndex<uint64_t> EdgeIndexUI64;

	typedef TriangleIndex<uint8_t> TriangleIndexUI8;
	typedef TriangleIndex<uint16_t> TriangleIndexUI16;
	typedef TriangleIndex<uint32_t> TriangleIndexUI32;
	typedef TriangleIndex<uint64_t> TriangleIndexUI64;

	typedef TetrahedronIndex<uint8_t> TetrahedronIndexUI8;
	typedef TetrahedronIndex<uint16_t> TetrahedronIndexUI16;
	typedef TetrahedronIndex<uint32_t> TetrahedronIndexUI32;
	typedef TetrahedronIndex<uint64_t> TetrahedronIndexUI64;

	typedef PolygonIndex<uint8_t> PolygonIndexUI8;
	typedef PolygonIndex<uint16_t> PolygonIndexUI16;
	typedef PolygonIndex<uint32_t> PolygonIndexUI32;
	typedef PolygonIndex<uint64_t> PolygonIndexUI64;

} // namespace MathLib

namespace std
{
	template <typename IntType>
	struct hash<MathLib::EdgeIndex<IntType>>
	{
		size_t operator()(const MathLib::EdgeIndex<IntType> &edge) const
		{
			return std::hash<IntType>()(edge.v0) ^ std::hash<IntType>()(edge.v1);
		}
	};

	template <typename IntType>
	struct hash<MathLib::TriangleIndex<IntType>>
	{
		size_t operator()(const MathLib::TriangleIndex<IntType> &tri) const
		{
			return std::hash<IntType>()(tri[0]) ^ std::hash<IntType>()(tri[1]) ^ std::hash<IntType>()(tri[2]);
		}
	};

	template <typename IntType>
	struct hash<MathLib::TetrahedronIndex<IntType>>
	{
		size_t operator()(const MathLib::TetrahedronIndex<IntType> &tet) const
		{
			return std::hash<IntType>()(tet[0]) ^ std::hash<IntType>()(tet[1]) ^ std::hash<IntType>()(tet[2]) ^ std::hash<IntType>()(tet[3]);
		}
	};
}