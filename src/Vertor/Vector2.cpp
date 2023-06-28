#include "../../include/HMath.h"
#include "../Utils/CompareUtils.h"
// HVector2f
HVector2f::HVector2f()
    : x(0), y(0)
{
}

HVector2f::HVector2f(HFloat value)
    : x(value), y(value)
{
}

HVector2f::HVector2f(HFloat X, HFloat Y)
    : x(X), y(Y)
{
}

HVector2f &HVector2f::operator=(const HVector2f &v)
{
    x = v.x;
    y = v.y;
    return *this;
}

bool HVector2f::operator==(const HVector2f &v) const
{
    return HMath::isEqual(x, v.x) && HMath::isEqual(y, v.y);
}

bool HVector2f::operator<(const HVector2f &v) const
{
    return x < v.x && y < v.y;
}

HFloat HVector2f::Dot(const HVector2f &v) const
{
    return x * v.x + y * v.y;
}

HVector2f HVector2f::operator-(const HVector2f &v) const
{
    return HVector2f(x - v.x, y - v.y);
}

HVector2f HVector2f::operator+(const HVector2f &v) const
{
    return HVector2f();
}

HVector2f HVector2f::operator*(const float &v) const
{
    return HVector2f();
}

HVector2f HVector2f::operator/(const HFloat &d) const
{
    return HVector2f();
}

HVector2f HVector2f::operator+=(const HVector2f &v)
{
    return HVector2f();
}

HFloat &HVector2f::operator[](const int i)
{
    // TODO: 在此处插入 return 语句
}

bool HVector2f::IsInPolygon(const std::vector<HVector2f> &polygon) const
{
    return false;
}

bool HVector2f::IsInPolygon(const std::vector<HVector2f> &polygonVertices, const std::vector<uint32_t> &polygonIndices) const
{
    return false;
}

bool HVector2f::IsInTriangle(HVector2f &a, HVector2f &b, HVector2f &c) const
{
    return false;
}

bool HVector2f::IsOnLine(HVector2f &a, HVector2f &b) const
{
    return false;
}

HFloat HVector2f::Length() const
{
    return HFloat();
}

HFloat HVector2f::LengthSqr() const
{
    return HFloat();
}

HVector2f &HVector2f::Normalize()
{
    // TODO: 在此处插入 return 语句
}
