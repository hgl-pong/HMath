#include "../include/HMath.h"
#include "Utils/CompareUtils.h"
namespace HMath
{
    HVector3f Normal(const HVector3f &a, const HVector3f &b, const HVector3f &c)
    {
        HVector3f ba = b - a;
        auto ca = c - a;
        auto normal = ba.Cross(ca);

        auto length = normal.Length();
        if (HMath::isZero(length))
            return HVector3f();

        return HVector3f(normal.x / length, normal.y / length, normal.z / length);
    }
    HFloat Area(const HVector3f &a, const HVector3f &b, const HVector3f &c)
    {
        HVector3f ba = b - a;
        HVector3f ca = c - a;
        HVector3f normal = ba.Cross(ca);

        HFloat length = normal.Length();
        // if (HMath::isZero(length))
        //	return 0;
        return length;
    }

    bool IntersectSegmentAndPlane(const HVector3f &segmentPoint0, const HVector3f &segmentPoint1,
                                  const HVector3f &pointOnPlane, HVector3f &planeNormal,
                                  HVector3f &intersection)
    {
        HVector3f u = segmentPoint1 - segmentPoint0;
        HVector3f w = pointOnPlane - segmentPoint0;
        HFloat d = planeNormal.Dot(u);
        HFloat n = planeNormal.Dot(w);
        //

        if (HMath::isZero(d))
            return false;
        auto s = n / d;
        if (s < 0 || s > 1 || std::isnan(s) || std::isinf(s))
            return false;
        intersection = segmentPoint0 + u * s;
        return true;
    }
    bool IsInTriangle(const HVector3f &intersection, const HVector3f *trianglePositions)
    {
        // 点在三角形内
        std::vector<HFloat> areas(3);
        auto s = 1 / Area(trianglePositions[0], trianglePositions[1], trianglePositions[2]);
        for (size_t i = 0; i < 3; ++i)
        {
            size_t j = (i + 1) % 3;
            HFloat area = Area(intersection, trianglePositions[i], trianglePositions[j]) * s;
            areas[i] = area;
        }
        // 在三角形内或三角形边上时
        HFloat d = (areas[0] + areas[1] + areas[2] - 1);
        return d < FLOAT_EPSILON /*FFloat::isZero(d)*/;
    }
    HFloat DistanceSqrToSegment(const HVector3f &v1, const HVector3f &v2, const HVector3f &p)
    {
        HVector3f test = p - v2;
        HVector3f line = (v1 - v2);
        HFloat d1 = line.Length();
        line.Normalize();
        HFloat d2 = line.Dot(test);
        if (d2 > 0 && d1 > d2)
            return line.Cross(test).LengthSqr();
        else
            return (std::min)(v1.DistanceSqr(p), v2.DistanceSqr(p));
    }
}