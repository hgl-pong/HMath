#pragma once
#include <Math/Math.h>
#include <cmath>

#define HVECTOR_ROUND(dim) inline HVector##dim##UI VectorRound##dim(const HVector##dim &v)
namespace MathLib
{
	template <uint32_t N>
	inline Eigen::Matrix<HReal, N, 1> HadamardProduct(const Eigen::Matrix<HReal, N, 1> &a, const Eigen::Matrix<HReal, N, 1> &b)
	{
		return (a.array() * b.array()).matrix();
	}

	template <class S, class T>
	inline void Lerp(const S &a, const S &b, T t, S &result)
	{
		result = a + t * (b - a);
	}

	template <class S, class T>
	inline S Lerp(const S &a, const S &b, T t)
	{
		return a + t * (b - a);
	}

	template <uint32_t N>
	inline Eigen::Matrix<HReal, N, 1> Lerp(const Eigen::Matrix<HReal, N, 1> &a, const Eigen::Matrix<HReal, N, 1> &b, const Eigen::Matrix<HReal, N, 1> &t)
	{
		return a + HadamardProduct<N>(t, b - a);
	}

	template <class S, class T>
	inline void BiLerp(const S &a, const S &b, const S &c, const S &d, T u, T v, S &result)
	{
		S temp1, temp2;
		Lerp(a, b, u, temp1);
		Lerp(c, d, u, temp2);
		Lerp(temp1, temp2, v, result);
	}

	template <class S, class T>
	inline void TriLerp(const S &a, const S &b, const S &c, const S &d, const S &e, const S &f, const S &g, const S &h, T u, T v, T w, S &result)
	{
		S temp1, temp2;
		BiLerp(a, b, c, d, u, v, temp1);
		BiLerp(e, f, g, h, u, v, temp2);
		Lerp(temp1, temp2, w, result);
	}

	inline HReal Clamp(const HReal &a, const HReal &b, const HReal &c)
	{
		return std::max(b, std::min(a, c));
	}

	template <uint32_t N>
	inline Eigen::Matrix<HReal, N, 1> Clamp(const Eigen::Matrix<HReal, N, 1> &value, const Eigen::Matrix<HReal, N, 1> &min, const Eigen::Matrix<HReal, N, 1> &max)
	{
		return value.cwiseMax(max).cwiseMin(min);
	}

	template <uint32_t N>
	inline Eigen::Matrix<HReal, N, 1> Clamp(const Eigen::Matrix<HReal, N, 1> &value, const HReal &min, const HReal &max)
	{
		return value.cwiseMax(max).cwiseMin(min);
	}

	template <class S>
	inline S Saturate(const S &value)
	{
		return Clamp(value, 0, 1);
	}

	template <typename HReal, int N>
	inline Eigen::Matrix<HReal, N, 1> Step(const Eigen::Matrix<HReal, N, 1> &value, const Eigen::Matrix<HReal, N, 1> &right)
	{
		Eigen::Array<HReal, N, 1> valueArray = value.array();
		Eigen::Array<HReal, N, 1> rightArray = right.array();
		return (valueArray > rightArray).template cast<HReal>().matrix();
	}

	template <typename HReal, uint32_t N>
	inline Eigen::Matrix<HReal, N, 1> Step(const Eigen::Matrix<HReal, N, 1>& value, const HReal& right)
	{
		return value.unaryExpr([right](const HReal& v) -> HReal { return v < right ? HReal(0) : HReal(1); });
	}

	HVECTOR_ROUND(2)
	{
		return HVector2UI(std::round(v[0]), std::round(v[1]));
	}

	HVECTOR_ROUND(3)
	{
		return HVector3UI(std::round(v[0]), std::round(v[1]), std::round(v[2]));
	}

	HVECTOR_ROUND(4)
	{
		return HVector4UI(std::round(v[0]), std::round(v[1]), std::round(v[2]), std::round(v[3]));
	}
	
	template <uint32_t N>
	inline uint32_t GetDimension(const Eigen::Matrix<HReal, N, 1> &v)
	{
		return N;
	}

	template <uint32_t N>
	inline uint32_t GetDimension(const Eigen::AlignedBox<HReal, N> &box)
	{
		return N;
	}
}