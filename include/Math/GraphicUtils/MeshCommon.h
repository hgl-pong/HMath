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
		bool operator==(const EdgeIndex& rhs) const
		{
			return v0 == rhs.v0 && v1 == rhs.v1;
		}
		bool operator!=(const EdgeIndex& rhs) const
		{
			return v0 != rhs.v0 || v1 != rhs.v1;
		}
	};
}// namespace MathLib