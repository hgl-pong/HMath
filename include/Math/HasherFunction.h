#pragma once
#include <Math/Math.h>

namespace MathLib
{

};

namespace std
{	
	template <>
	struct hash<MathLib::HVector4>
	{
		std::size_t operator()(const MathLib::HVector4& v) const
		{
			return std::hash<MathLib::HReal>()(v(0)) ^ std::hash<MathLib::HReal>()(v(1)) ^ std::hash<MathLib::HReal>()(v(2)) ^ std::hash<MathLib::HReal>()(v(3));
		}
	};

	template <>
	struct equal_to<MathLib::HVector4>
	{
		bool operator()(const MathLib::HVector4& v1, const MathLib::HVector4& v2) const
		{
			return v1(0) == v2(0) && v1(1) == v2(1) && v1(2) == v2(2) && v1(3) == v2(3);
		}
	};

	template <>
	struct hash<MathLib::HVector3>
	{
		std::size_t operator()(const MathLib::HVector3& v) const
		{
			return std::hash<MathLib::HReal>()(v(0)) ^ std::hash<MathLib::HReal>()(v(1)) ^ std::hash<MathLib::HReal>()(v(2));
		}
	};

	template<>
	struct equal_to<MathLib::HVector3>
	{
		bool operator()(const MathLib::HVector3& v1, const MathLib::HVector3& v2) const
		{
			return v1(0) == v2(0) && v1(1) == v2(1) && v1(2) == v2(2);
		}
	};

	template <>
	struct hash<MathLib::HVector2>
	{
		std::size_t operator()(const MathLib::HVector2& v) const
		{
			return std::hash<MathLib::HReal>()(v(0)) ^ std::hash<MathLib::HReal>()(v(1));
		}
	};

	template<>
	struct equal_to<MathLib::HVector2>
	{
		bool operator()(const MathLib::HVector2& v1, const MathLib::HVector2& v2) const
		{
			return v1(0) == v2(0) && v1(1) == v2(1);
		}
	};

	template <>
	struct hash<MathLib::HVector2I>
	{
		std::size_t operator()(const MathLib::HVector2I& v) const
		{
			return std::hash<int>()(v(0)) ^ std::hash<int>()(v(1));
		}
	};

	template<>
	struct equal_to<MathLib::HVector2I>
	{
		bool operator()(const MathLib::HVector2I& v1, const MathLib::HVector2I& v2) const
		{
			return v1(0) == v2(0) && v1(1) == v2(1);
		}
	};

	template <>
	struct hash<MathLib::HVector3I>
	{
		std::size_t operator()(const MathLib::HVector3I& v) const
		{
			return std::hash<int>()(v(0)) ^ std::hash<int>()(v(1)) ^ std::hash<int>()(v(2));
		}
	};

	template<>
	struct equal_to<MathLib::HVector3I>
	{
		bool operator()(const MathLib::HVector3I& v1, const MathLib::HVector3I& v2) const
		{
			return v1(0) == v2(0) && v1(1) == v2(1) && v1(2) == v2(2);
		}
	};

	template <>
	struct hash<MathLib::HVector4I>
	{
		std::size_t operator()(const MathLib::HVector4I& v) const
		{
			return std::hash<int>()(v(0)) ^ std::hash<int>()(v(1)) ^ std::hash<int>()(v(2)) ^ std::hash<int>()(v(3));
		}
	};

	template<>
	struct equal_to<MathLib::HVector4I>
	{
		bool operator()(const MathLib::HVector4I& v1, const MathLib::HVector4I& v2) const
		{
			return v1(0) == v2(0) && v1(1) == v2(1) && v1(2) == v2(2) && v1(3) == v2(3);
		}
	};

	template <>
	struct hash<MathLib::HVector2UI>
	{
		std::size_t operator()(const MathLib::HVector2UI& v) const
		{
			return std::hash<unsigned int>()(v(0)) ^ std::hash<unsigned int>()(v(1));
		}
	};

	template<>
	struct equal_to<MathLib::HVector2UI>
	{
		bool operator()(const MathLib::HVector2UI& v1, const MathLib::HVector2UI& v2) const
		{
			return v1(0) == v2(0) && v1(1) == v2(1);
		}
	};

	template <>
	struct hash<MathLib::HVector3UI>
	{
		std::size_t operator()(const MathLib::HVector3UI& v) const
		{
			return std::hash<unsigned int>()(v(0)) ^ std::hash<unsigned int>()(v(1)) ^ std::hash<unsigned int>()(v(2));
		}
	};

	template<>
	struct equal_to<MathLib::HVector3UI>
	{
		bool operator()(const MathLib::HVector3UI& v1, const MathLib::HVector3UI& v2) const
		{
			return v1(0) == v2(0) && v1(1) == v2(1) && v1(2) == v2(2);
		}
	};

	template <>
	struct hash<MathLib::HVector4UI>
	{
		std::size_t operator()(const MathLib::HVector4UI& v) const
		{
			return std::hash<unsigned int>()(v(0)) ^ std::hash<unsigned int>()(v(1)) ^ std::hash<unsigned int>()(v(2)) ^ std::hash<unsigned int>()(v(3));
		}
	};

	template<>
	struct equal_to<MathLib::HVector4UI>
	{
		bool operator()(const MathLib::HVector4UI& v1, const MathLib::HVector4UI& v2) const
		{
			return v1(0) == v2(0) && v1(1) == v2(1) && v1(2) == v2(2) && v1(3) == v2(3);
		}
	};

}