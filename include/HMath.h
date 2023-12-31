#ifndef H_MATH_H
#define H_MATH_H
#include <vector>
#ifndef HAPI
#define HAPI __declspec(dllexport)
#endif

#if USE_DOUBLE_PRECISION
typedef double HFloat;
#else
typedef float HFloat;
#endif

#if USE_LONG_INT
typedef unsigned long HUInt;
#else
typedef unsigned short HUInt;
#endif

// 不使用默认精度必须设定计算精度
#ifndef USE_DEFAULT_EPSILON
#define USE_DEFAULT_EPSILON 1
#endif

#if USE_DEFAULT_EPSILON
#define FLOAT_EPSILON 1e-6f
#define SCALE 1000000
#define ROUND 5e-7f
#endif

#define FLOAT_MAX std::numeric_limits<HFloat>::max()
#define FLOAT_MIN std::numeric_limits<HFloat>::lowest()

// HFloat Vector
class HAPI HVector2f
{
public:
    HFloat x, y;

public:
    HVector2f();
    HVector2f(HFloat);
    HVector2f(HFloat, HFloat);
    ~HVector2f() = default;

    // operator
    HVector2f &operator=(const HVector2f &v);
    bool operator==(const HVector2f &v) const;
    bool operator<(const HVector2f &v) const;

    HVector2f operator-(const HVector2f &v) const;
    HVector2f operator+(const HVector2f &v) const;
    HVector2f operator*(const float &v) const;
    HVector2f operator/(const HFloat &d) const;
    HVector2f operator+=(const HVector2f &v);
    HFloat &operator[](const int i);

    // detect
    bool IsInPolygon(const std::vector<HVector2f> &polygon) const;
    bool IsInPolygon(const std::vector<HVector2f> &polygonVertices, const std::vector<uint32_t> &polygonIndices) const;
    bool IsInTriangle(HVector2f &a, HVector2f &b, HVector2f &c) const;
    bool IsOnLine(HVector2f &a, HVector2f &b) const;

    // other
    HFloat Length() const;
    HFloat LengthSqr() const;
    HVector2f &Normalize();
    HFloat Dot(const HVector2f &v) const;
};

class HAPI HVector3f
{
public:
    HFloat x, y, z;

public:
    HVector3f();
    HVector3f(HFloat);
    HVector3f(HFloat, HFloat, HFloat);
    ~HVector3f() = default;

    // operator
    HVector3f &operator=(const HVector3f &v);
    HVector3f &operator+=(const HVector3f &v);
    HVector3f &operator-=(const HVector3f &v);
    HVector3f operator*=(const HFloat &d);
    HVector3f &operator/=(const HFloat d);
    HVector3f operator-(const HVector3f &v) const;
    HVector3f operator+(const HVector3f &v) const;
    HVector3f operator*(const HFloat d) const;
    HVector3f operator*(const HVector3f &v) const;
    HVector3f operator/(const HFloat d) const;
    HVector3f operator/(const HVector3f &d) const;
    int operator==(const HVector3f &v) const;
    bool operator<(const HVector3f &v) const;
    HFloat operator[](const int i) const;

    // detect
    bool IsOnSegment(HVector3f &a, HVector3f &b);
    bool IsOnLine(const HVector3f &a, const HVector3f &b) const;
    bool IsZero();

    // other
    HFloat Length() const;
    HFloat LengthSqr() const;
    HVector3f &Normalize();
    HVector3f Cross(const HVector3f &v) const;
    HVector3f Prod(const HVector3f &v);
    HFloat Dot(const HVector3f &v) const;
    HFloat Distance(const HVector3f &v) const;
    HFloat DistanceSqr(const HVector3f &v) const;
    HVector3f Ceiling(const HVector3f &v) const;
    HVector3f Floor(const HVector3f &v) const;
};

class HAPI HVector4f
{
public:
    HFloat x, y, z, w;

public:
    HVector4f();
    HVector4f(HFloat);
    HVector4f(HFloat, HFloat, HFloat, HFloat);
};

// HUint Vector
class HAPI HVector2i
{
public:
    HUInt x, y;

public:
    HVector2i();
    HVector2i(HUInt, HUInt);
};

class HAPI HVector3i
{
public:
    HUInt x, y, z;

public:
    HVector3i();
    HVector3i(HUInt, HUInt, HUInt);
};

class HAPI HVector4i
{
public:
    HUInt x, y, z, w;

public:
    HVector4i();
    HVector4i(HUInt, HUInt, HUInt, HUInt);
};

class HQuat
{
public:
    HQuat operator+(const HQuat &quat) const;
    HFloat operator[](const int i) const;

private:
    HFloat x, y, z, w;
};

class HMat44
{
public:
    HMat44();
    HMat44(const HFloat *data);
    HFloat operator()(int i, int j) const;
    HMat44 operator*(const HMat44 &second) const;
    bool SetTranslation(const HVector3f &translate);
    bool SetScale(const HVector3f &scale);
    bool SetRotation(const HQuat &rotation);
    bool GetTranslation(HVector3f *translate) const;
    bool GetScale(HVector3f *scale) const;
    bool GetRotation(HQuat *rotation) const;

private:
    HFloat m_Data[16];
};

class HTransform
{
public:
    HTransform();
    HTransform(const HVector3f &translate, const HVector3f scale = {0, 0, 0});
    HTransform(const HVector3f &translate, const HVector3f &scale, const HQuat &rotation);
    HTransform(const HMat44 &matrix);

    HTransform operator+(const HTransform &transform) const;

    bool SetTranslate(const HVector3f &translate);
    bool SetScale(const HVector3f &scale);
    bool SetRotation(const HQuat &rotation);

    bool GetTranslate(HVector3f *translate);
    bool GetScale(HVector3f *scale);
    bool GetRotation(HQuat *rotation);
    bool GetMatrix(HMat44 *matrix);

private:
    HVector3f m_Translate;
    HVector3f m_Scale;
    HQuat m_Rotation;
};

namespace HMath
{
    HAPI HVector3f Normal(const HVector3f &a, const HVector3f &b, const HVector3f &c);
    HAPI HFloat Area(const HVector3f &a, const HVector3f &b, const HVector3f &c);
    HAPI bool IntersectSegmentAndPlane(const HVector3f &segmentPoint0, const HVector3f &segmentPoint1,
                                       const HVector3f &pointOnPlane, HVector3f &planeNormal,
                                       HVector3f &intersection);
    HAPI bool IsInTriangle(const HVector3f &intersection, const HVector3f *trianglePositions);
    HAPI HFloat DistanceSqrToSegment(const HVector3f &v1, const HVector3f &v2, const HVector3f &p);
}

#endif /* H_MATH_H */