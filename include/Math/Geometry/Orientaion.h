#pragma once
#include <Math/Math.h>

namespace MathLib
{
	namespace OrientationUtils
	{
		inline HReal Orientation1D(const HReal &v0, const HReal &v1)
		{
			return v0 - v1;
		}

		inline HReal Orientation2D(const HVector2 &v0, const HVector2 &v1, const HVector2 &v2)
		{
			HVector2 v01 = v1 - v0;
			HVector2 v02 = v2 - v0;
			return v01[0] * v02[1] - v01[1] * v02[0];
		}

		inline HReal TriangleArea(const HVector2 &v0, const HVector2 &v1, const HVector2 &v2)
		{
			return Orientation2D(v0, v1, v2) * HReal(0.5);
		}

		inline HReal Orientation3D(const HVector3 &v0, const HVector3 &v1, const HVector3 &v2)
		{
			HVector3 v01 = v1 - v0;
			HVector3 v02 = v2 - v0;
			return v01.cross(v02).norm();
		}

		inline HReal TriangleArea(const HVector3 &v0, const HVector3 &v1, const HVector3 &v2)
		{
			return Orientation3D(v0, v1, v2) * HReal(0.5);
		}

		inline HReal Orientation3D(const HVector3 &v0, const HVector3 &v1, const HVector3 &v2, const HVector3 &v3)
		{
			HVector3 v01 = v1 - v0;
			HVector3 v02 = v2 - v0;
			HVector3 v03 = v3 - v0;
			return v01.dot(v02.cross(v03));
		}

		inline void IntervalOrientation2D(const HVector2 &v0, const HVector2 &v1, const HVector2 &v2, HReal &min, HReal &max)
		{
			HReal a = Orientation2D(v0, v1, v2);
			min = std::min(min, a);
			max = std::max(max, a);
		}

		inline void IntervalOrientation3D(const HVector3 &v0, const HVector3 &v1, const HVector3 &v2, const HVector3 &v3, HReal &min, HReal &max)
		{
			HReal a = Orientation3D(v0, v1, v2, v3);
			min = std::min(min, a);
			max = std::max(max, a);
		}

		template <class Priority, class VectorType>
		inline void SortPoints(Priority &p1, VectorType &v1, Priority &p2, VectorType &v2, int &sign)
		{
			if (p1 < p2)
			{
				std::swap(p1, p2);
				std::swap(v1, v2);
				sign = -sign;
			}
		}

		template <class Priority = unsigned int>
		inline int SoSOrientation1D(Priority &p1, const HReal &v0,
									Priority &p2, const HReal &v1, HReal &o)
		{
			o = Orientation1D(v0, v1);
			if (o != 0)
				return o < 0 ? -1 : 1;
			if (p1 < p2)
				return -1;
			else if (p1 > p2)
				return 1;
			return 0;
		}

		template <class Priority = unsigned int>
		inline int SoSOrientation2D(Priority &p0, HVector2 &v0,
									Priority &p1, HVector2 &v1,
									Priority &p2, HVector2 &v2, HReal &o)
		{
			o = Orientaion2D(v0, v1, v2);
			if (o != 0)
				return o < 0 ? -1 : 1;
			int sign = 1;
			SortPoints(p0, v0, p1, v1, sign);
			SortPoints(p0, v0, p2, v2, sign);
			SortPoints(p1, v1, p2, v2, sign);

			o = Orientation1D(v1[1], v2[1]);
			if (o < 0)
				return -sign;
			else if (o > 0)
				return sign;
			o = -Orientation1D(v0[0], v2[0]);
			if (o < 0)
				return -sign;
			else if (o > 0)
				return sign;
			o = -Orientation1D(v1[0], v2[0]);
			if (o < 0)
				return -sign;
			else if (o > 0)
				return sign;
			return 0;
		}

		template <class Priority = unsigned int>
		inline int SoSOrientation3D(Priority &p0, HVector3 &v0,
									Priority &p1, HVector3 &v1,
									Priority &p2, HVector3 &v2,
									Priority &p3, HVector3 &v3, HReal &o)
		{
			o = Orientation3D(v0, v1, v2, v3);
			if (o != 0)
				return o < 0 ? -1 : 1;
			int sign = 1;
			SortPoints(p0, v0, p1, v1, sign);
			SortPoints(p0, v0, p2, v2, sign);
			SortPoints(p0, v0, p3, v3, sign);
			SortPoints(p1, v1, p2, v2, sign);
			SortPoints(p1, v1, p3, v3, sign);
			SortPoints(p2, v2, p3, v3, sign);

			o = Orientaion2D(v1, v2, v3);
			if (o < 0)
				return -sign;
			else if (o > 0)
				return sign;
			o = -Orientaion2D(v0, v2, v3);
			if (o < 0)
				return -sign;
			else if (o > 0)
				return sign;
			o = Orientaion2D(v0, v1, v3);
			if (o < 0)
				return -sign;
			else if (o > 0)
				return sign;
			o = -Orientaion2D(v0, v1, v2);
			if (o < 0)
				return -sign;
			else if (o > 0)
				return sign;
			return 0;
		}
	}
}