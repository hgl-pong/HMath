#pragma once
#include <Math/Geometry/Orientaion.h>

namespace MathLib
{
	namespace IntersectionUtils
	{
		inline bool IsPointInTriangle2D(const HVector2& p, const HVector2& v0, const HVector2& v1, const HVector2& v2)
		{
			if (Less(OrientationUtils::Orientation2D(v0, v1, p), 0))
				return false;
			if (Less(OrientationUtils::Orientation2D(v1, v2, p), 0))
				return false;
			if (Less(OrientationUtils::Orientation2D(v2, v0, p), 0))
				return false;
			return true;
		}

		inline bool IsPointInTriangle3D(const HVector3& p, const HVector3& v0, const HVector3& v1, const HVector3& v2)
		{
			HReal s = OrientationUtils::Orientation3D(v0, v1, v2);
			HReal a = OrientationUtils::Orientation3D(v0, v1, p);
			HReal b = OrientationUtils::Orientation3D(v1, v2, p);
			HReal c = OrientationUtils::Orientation3D(v2, v0, p);
			HReal sum = a + b + c;
			if (IsZero(sum) && IsZero(s))
				return true;
			return Equal(sum / s, 1);
		}

		inline bool IsPointInTriangle3D(const HVector3& p, const HVector3& v0, const HVector3& v1, const HVector3& v2, const HVector3& v3)
		{
			HReal a = OrientationUtils::Orientation3D(v0, v1, v2, p);
			HReal b = OrientationUtils::Orientation3D(v1, v2, v3, p);
			HReal c = OrientationUtils::Orientation3D(v2, v3, v0, p);
			HReal d = OrientationUtils::Orientation3D(v3, v0, v1, p);
			return a >= 0 && b >= 0 && c >= 0 && d >= 0;
		}

		inline bool IsPointInTriangle3D(const HVector2& p, const HVector2& v0, const HVector2& v1, const HVector2& v2, HReal& w0, HReal& w1, HReal& w2)
		{
			HReal a = OrientationUtils::Orientation2D(v0, v1, p);
			HReal b = OrientationUtils::Orientation2D(v1, v2, p);
			HReal c = OrientationUtils::Orientation2D(v2, v0, p);
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
			if (det > -H_EPSILON && det < H_EPSILON) return false;
			HReal invDet = 1.0f / det;
			HVector3 tvec = rayOrigin - v0;
			u = tvec.dot(p) * invDet;
			if (u < 0 || u > 1) return false;
			HVector3 q = tvec.cross(e1);
			v = rayDir.dot(q) * invDet;
			if (v < 0 || u + v > 1) return false;
			t = e2.dot(q) * invDet;
			return t > H_EPSILON;
		}

		inline bool IsOnLine(const HVector2& p, const HVector2& v0, const HVector2& v1)
		{
			HReal a = OrientationUtils::Orientation2D(v0, v1, p);
			return IsZero(a);
		}

		inline bool IsOnSegment(const HVector2& p, const HVector2& v0, const HVector2& v1)
		{
			HReal a = OrientationUtils::Orientation2D(v0, v1, p);
			if (!IsZero(a)) return false;
			HReal b = (p - v0).dot(p - v1);
			return b <= 0;
		}

		inline bool IsOnLine(const HVector3& p, const HVector3& v0, const HVector3& v1)
		{
			HReal a = OrientationUtils::Orientation3D(v0, v1, p);
			return IsZero(a);
		}

		inline bool IsOnSegment(const HVector3& p, const HVector3& v0, const HVector3& v1)
		{
			HReal a = OrientationUtils::Orientation3D(v0, v1, p);
			if (!IsZero(a)) return false;
			HReal b = (p - v0).dot(p - v1);
			return b <= 0;
		}

		inline bool EdgeIntersectEdge2D(const HVector2& v0, const HVector2& v1, const HVector2& u0, const HVector2& u1)
		{
			HReal d1 = OrientationUtils::Orientation2D(v0, v1, u0);
			HReal d2 = OrientationUtils::Orientation2D(v0, v1, u1);
			HReal d3 = OrientationUtils::Orientation2D(u0, u1, v0);
			HReal d4 = OrientationUtils::Orientation2D(u0, u1, v1);

			if (d1 * d2 < 0 && d3 * d4 < 0) return true;

			if (IsZero(d1) && IsOnSegment(u0, v0, v1)) return true;
			if (IsZero(d2) && IsOnSegment(u1, v0, v1)) return true;
			if (IsZero(d3) && IsOnSegment(v0, u0, u1)) return true;
			if (IsZero(d4) && IsOnSegment(v1, u0, u1)) return true;

			return false;
		}

		inline bool EdgeIntersectTriangle2D(const HVector2& v0, const HVector2& v1, const HVector2& u0, const HVector2& u1, const HVector2& u2)
		{			
			if( IsPointInTriangle2D(v0, u0, u1, u2)) return true;
			if( IsPointInTriangle2D(v1, u0, u1, u2)) return true;
			if (EdgeIntersectEdge2D(v0, v1, u0, u1)) return true;
			if (EdgeIntersectEdge2D(v0, v1, u1, u2)) return true;
			if (EdgeIntersectEdge2D(v0, v1, u2, u0)) return true;
			return false;
		}

		inline bool VertexIntersectTriangle(const HVector2& v0, const HVector2& v1, const HVector2& v2, const HVector2& u0)
		{
			HReal a = OrientationUtils::Orientation2D(v0, v1, u0);
			HReal b = OrientationUtils::Orientation2D(v1, v2, u0);
			HReal c = OrientationUtils::Orientation2D(v2, v0, u0);
			return a >= 0 && b >= 0 && c >= 0;
		}

		//inline bool TriTriIntersect2D(const HVector2& v0, const HVector2& v1, const HVector2& v2, const HVector2& u0, const HVector2& u1, const HVector2& u2)
		//{
		//	HReal a = OrientationUtils::Orientation2D(v0, v1, u0);
		//	HReal b = OrientationUtils::Orientation2D(v0, v1, u1);
		//	HReal c = OrientationUtils::Orientation2D(v0, v1, u2);
		//	if (a * b > 0 && a * c > 0) return false;
		//	a = OrientationUtils::Orientation2D(v1, v2, u0);
		//	b = OrientationUtils::Orientation2D(v1, v2, u1);
		//	c = OrientationUtils::Orientation2D(v1, v2, u2);
		//	if (a * b > 0 && a * c > 0) return false;
		//	a = OrientationUtils::Orientation2D(v2, v0, u0);
		//	b = OrientationUtils::Orientation2D(v2, v0, u1);
		//	c = OrientationUtils::Orientation2D(v2, v0, u2);
		//	if (a * b > 0 && a * c > 0) return false;
		//	return true;
		//}
		inline bool TriTriIntersect2D(const HVector2& v0, const HVector2& v1, const HVector2& v2,
			const HVector2& u0, const HVector2& u1, const HVector2& u2) 
		{
			if (IsPointInTriangle2D(v0, u0, u1, u2)) return true;
			if (IsPointInTriangle2D(v1, u0, u1, u2)) return true;
			if (IsPointInTriangle2D(v2, u0, u1, u2)) return true;
			if (IsPointInTriangle2D(u0, v0, v1, v2)) return true;
			if (IsPointInTriangle2D(u1, v0, v1, v2)) return true;
			if (IsPointInTriangle2D(u2, v0, v1, v2)) return true;

			if (EdgeIntersectEdge2D(v0, v1, u0, u1)) return true;
			if (EdgeIntersectEdge2D(v0, v1, u1, u2)) return true;
			if (EdgeIntersectEdge2D(v0, v1, u2, u0)) return true;
			if (EdgeIntersectEdge2D(v1, v2, u0, u1)) return true;
			if (EdgeIntersectEdge2D(v1, v2, u1, u2)) return true;
			if (EdgeIntersectEdge2D(v1, v2, u2, u0)) return true;
			if (EdgeIntersectEdge2D(v2, v0, u0, u1)) return true;
			if (EdgeIntersectEdge2D(v2, v0, u1, u2)) return true;
			if (EdgeIntersectEdge2D(v2, v0, u2, u0)) return true;
			return false;
		}

		inline bool TriTriOverlap2D(const HVector2& v0, const HVector2& v1, const HVector2& v2, const HVector2& u0, const HVector2& u1, const HVector2& u2)
		{
			if (OrientationUtils::Orientation2D(v0, v1, u0) < 0.f)
				if (OrientationUtils::Orientation2D(v0, v1, u1) < 0.f)
					return TriTriIntersect2D(v0, v1, v2, u0, u1, u2);
				else
					return TriTriIntersect2D(v0, v1, v2, u0, u2, u1);
			else
				if (OrientationUtils::Orientation2D(v0, v1, u1) < 0.f)
					return TriTriIntersect2D(v0, v1, v2, u1, u0, u2);
				else
					return TriTriIntersect2D(v0, v1, v2, u1, u2, u0);
		}

		inline bool TriTriCoplanar3D(const HVector3& v0, const HVector3& v1, const HVector3& v2, const HVector3& u0, const HVector3& u1, const HVector3& u2,const HVector3&n1, const HVector3& n2)
		{
			HReal d1 = n1.dot(v0);
			HReal d2 = n2.dot(u0);
			HReal d = n1.dot(u0) - d1;
			if (d < -H_EPSILON || d > H_EPSILON) return false;
			HVector3 e1 = v1 - v0;
			HVector3 e2 = v2 - v0;
			HVector3 e3 = u1 - u0;
			HVector3 e4 = u2 - u0;
			HVector3 p = e1.cross(e2);
			HReal det = e3.dot(p);
			if (det > -H_EPSILON && det < H_EPSILON) return false;
			HReal invDet = 1.0f / det;
			HVector3 tvec = u0 - v0;
			HReal u = tvec.dot(p) * invDet;
			if (u < 0 || u > 1) return false;
			HVector3 q = tvec.cross(e3);
			HReal v = e1.dot(q) * invDet;
			if (v < 0 || u + v > 1) return false;
			return true;
		}

		inline bool TriTriIntersect3D(const HVector3& v0, const HVector3& v1, const HVector3& v2, const HVector3& u0, const HVector3& u1, const HVector3& u2, HVector3& p0, HVector3& p1)
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
			HVector3 e1 = v1 - v0;
			HVector3 e2 = v2 - v0;
			HVector3 e3 = u1 - u0;
			HVector3 e4 = u2 - u0;
			HVector3 p = e1.cross(e2);
			HReal det = e3.dot(p);
			if (det > -H_EPSILON && det < H_EPSILON) return false;
			HReal invDet = 1.0f / det;
			HVector3 tvec = u0 - v0;
			HReal u = tvec.dot(p) * invDet;
			if (u < 0 || u > 1) return false;
			HVector3 q = tvec.cross(e3);
			HReal v = e1.dot(q) * invDet;
			if (v < 0 || u + v > 1) return false;
			HReal t = e2.dot(q) * invDet;
			if (t < 0) return false;
			p0 = v0 + e1 * u + e2 * v;
			p1 = u0 + e3 * t;
			return true;
		}

	} // namespace IntersectionUtils
} // namespace MathLib