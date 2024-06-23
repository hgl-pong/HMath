#pragma once
#include <Math/Math.h>

#ifndef H_EPSILON
#define H_EPSILON 1e-6
#endif

#define H_REAL_MAX std::numeric_limits<MathLib::HReal>::max()
#define H_REAL_MIN std::numeric_limits<MathLib::HReal>::lowest()

namespace MathLib
{
	inline bool Equal(const HReal& a, const HReal& b)
	{
		return std::abs(a - b) < H_EPSILON;
	}

	inline bool Less(const HReal& a, const HReal& b)
	{
		return a < b - H_EPSILON;
	}

	inline bool LessEqual(const HReal& a, const HReal& b)
	{
		return a < b + H_EPSILON;
	}

	inline bool Greater(const HReal& a, const HReal& b)
	{
		return a > b + H_EPSILON;
	}

	inline bool GreaterEqual(const HReal& a, const HReal& b)
	{
		return a > b - H_EPSILON;
	}

	inline bool NotEqual(const HReal& a, const HReal& b)
	{
		return std::abs(a - b) > H_EPSILON;
	}

	inline bool IsZero(const HReal& a)
	{
		return std::abs(a) < H_EPSILON;
	}

	inline bool NotZero(const HReal& a)
	{
		return std::abs(a) > H_EPSILON;
	}
} // namespace MathLib