#pragma once
#include <Math/Geometry/Orientaion.h>

namespace MathLib
{
	namespace IntersectionUtils
	{
		inline bool IsPointInTriangle(const HVector2& p, const HVector2& v0, const HVector2& v1, const HVector2& v2)
		{
			HReal a = OrientationUtils::Orientaion2D(v0, v1, p);
			HReal b = OrientationUtils::Orientaion2D(v1, v2, p);
			HReal c = OrientationUtils::Orientaion2D(v2, v0, p);
			return a >= 0 && b >= 0 && c >= 0;
		}

		inline bool IsPointInTriangle(const HVector3& p, const HVector3& v0, const HVector3& v1, const HVector3& v2)
		{
			HReal a = OrientationUtils::Orientation3D(v0, v1, v2, p);
			HReal b = OrientationUtils::Orientation3D(v1, v2, v0, p);
			HReal c = OrientationUtils::Orientation3D(v2, v0, v1, p);
			return a >= 0 && b >= 0 && c >= 0;
		}

		inline bool IsPointInTriangle(const HVector3& p, const HVector3& v0, const HVector3& v1, const HVector3& v2, const HVector3& v3)
		{
			HReal a = OrientationUtils::Orientation3D(v0, v1, v2, p);
			HReal b = OrientationUtils::Orientation3D(v1, v2, v3, p);
			HReal c = OrientationUtils::Orientation3D(v2, v3, v0, p);
			HReal d = OrientationUtils::Orientation3D(v3, v0, v1, p);
			return a >= 0 && b >= 0 && c >= 0 && d >= 0;
		}

		inline bool IsPointInTriangle(const HVector2& p, const HVector2& v0, const HVector2& v1, const HVector2& v2, HReal& w0, HReal& w1, HReal& w2)
		{
			HReal a = OrientationUtils::Orientaion2D(v0, v1, p);
			HReal b = OrientationUtils::Orientaion2D(v1, v2, p);
			HReal c = OrientationUtils::Orientaion2D(v2, v0, p);
			HReal sum = a + b + c;
			w0 = a / sum;
			w1 = b / sum;
			w2 = c / sum;
			return a >= 0 && b >= 0 && c >= 0;
		}

		inline bool RayIntersectTriangle(const HVector3& rayOrigin, const HVector3& rayDir, const HVector3& v0, const HVector3& v1, const HVector3& v2, HReal& t, HReal& u, HReal& v)
		{
			HVector3 e1 = v1 - v0;
			HVector3 e2 = v2 - v0;
			HVector3 p = rayDir.cross(e2);
			HReal det = e1.dot(p);
			if (det > -EPS && det < EPS) return false;
			HReal invDet = 1.0f / det;
			HVector3 tvec = rayOrigin - v0;
			u = tvec.dot(p) * invDet;
			if (u < 0 || u > 1) return false;
			HVector3 q = tvec.cross(e1);
			v = rayDir.dot(q) * invDet;
			if (v < 0 || u + v > 1) return false;
			t = e2.dot(q) * invDet;
			return t > EPS;
		}

		inline bool CheckMinMax(const HVector3& v0, const HVector3& v1, const HVector3& v2, const HVector3& min, const HVector3& max)
		{
			if (v0[0] < min[0] && v1[0] < min[0] && v2[0] < min[0]) return false;
			if (v0[0] > max[0] && v1[0] > max[0] && v2[0] > max[0]) return false;
			if (v0[1] < min[1] && v1[1] < min[1] && v2[1] < min[1]) return false;
			if (v0[1] > max[1] && v1[1] > max[1] && v2[1] > max[1]) return false;
			if (v0[2] < min[2] && v1[2] < min[2] && v2[2] < min[2]) return false;
			if (v0[2] > max[2] && v1[2] > max[2] && v2[2] > max[2]) return false;
			return true;
		}

		inline bool EdgeIntersectTriangle(const HVector2& v0, const HVector2& v1, const HVector2& u0, const HVector2& u1, const HVector2& u2)
		{
			HReal a = OrientationUtils::Orientaion2D(v0, v1, u0);
			HReal b = OrientationUtils::Orientaion2D(v0, v1, u1);
			HReal c = OrientationUtils::Orientaion2D(v0, v1, u2);
			if (a * b > 0 && a * c > 0) return false;
			a = OrientationUtils::Orientaion2D(v1, v0, u0);
			b = OrientationUtils::Orientaion2D(v1, v0, u1);
			c = OrientationUtils::Orientaion2D(v1, v0, u2);
			if (a * b > 0 && a * c > 0) return false;
			return true;
		}

		inline bool VertexIntersectTriangle(const HVector2& v0, const HVector2& v1, const HVector2& v2, const HVector2& u0)
		{
			HReal a = OrientationUtils::Orientaion2D(v0, v1, u0);
			HReal b = OrientationUtils::Orientaion2D(v1, v2, u0);
			HReal c = OrientationUtils::Orientaion2D(v2, v0, u0);
			return a >= 0 && b >= 0 && c >= 0;
		}

		inline bool TriTriIntersect2D(const HVector2& v0, const HVector2& v1, const HVector2& v2, const HVector2& u0, const HVector2& u1, const HVector2& u2)
		{
			HReal a = OrientationUtils::Orientaion2D(v0, v1, u0);
			HReal b = OrientationUtils::Orientaion2D(v0, v1, u1);
			HReal c = OrientationUtils::Orientaion2D(v0, v1, u2);
			if (a * b > 0 && a * c > 0) return false;
			a = OrientationUtils::Orientaion2D(v1, v2, u0);
			b = OrientationUtils::Orientaion2D(v1, v2, u1);
			c = OrientationUtils::Orientaion2D(v1, v2, u2);
			if (a * b > 0 && a * c > 0) return false;
			a = OrientationUtils::Orientaion2D(v2, v0, u0);
			b = OrientationUtils::Orientaion2D(v2, v0, u1);
			c = OrientationUtils::Orientaion2D(v2, v0, u2);
			if (a * b > 0 && a * c > 0) return false;
			return true;
		}

		inline bool TriTriOverlap2D(const HVector2& v0, const HVector2& v1, const HVector2& v2, const HVector2& u0, const HVector2& u1, const HVector2& u2)
		{
			if (OrientationUtils::Orientaion2D(v0, v1, u0) < 0.f)
				if (OrientationUtils::Orientaion2D(v0, v1, u1) < 0.f)
					return TriTriIntersect2D(v0, v1, v2, u0, u1, u2);
				else
					return TriTriIntersect2D(v0, v1, v2, u0, u2, u1);
			else
				if (OrientationUtils::Orientaion2D(v0, v1, u1) < 0.f)
					return TriTriIntersect2D(v0, v1, v2, u1, u0, u2);
				else
					return TriTriIntersect2D(v0, v1, v2, u1, u2, u0);
		}

		inline bool TriTriCoplanar3D(const HVector3& v0, const HVector3& v1, const HVector3& v2, const HVector3& u0, const HVector3& u1, const HVector3& u2)
		{
			HVector3 n = (v1 - v0).cross(v2 - v0);
			HReal d0 = n.dot(u0 - v0);
			HReal d1 = n.dot(u1 - v0);
			HReal d2 = n.dot(u2 - v0);
			if (d0 * d1 > 0 && d0 * d2 > 0) return false;
			n = (u1 - u0).cross(u2 - u0);
			d0 = n.dot(v0 - u0);
			d1 = n.dot(v1 - u0);
			d2 = n.dot(v2 - u0);
			if (d0 * d1 > 0 && d0 * d2 > 0) return false;
			return true;
		}

	} // namespace IntersectionUtils
} // namespace MathLib