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
    return HVector2f(x + v.x, y + v.y);
}

HVector2f HVector2f::operator*(const float &v) const
{
    return HVector2f(x * v, y * v);
}

HVector2f HVector2f::operator/(const HFloat &d) const
{
    return HVector2f(x / d, y / d);
}

HVector2f HVector2f::operator+=(const HVector2f &v)
{
    x += v.x;
    y += v.y;
    return *this;
}

HFloat &HVector2f::operator[](const int i)
{
    if (i == 0)
        return x;
    else if (i == 1)
        return y;
    else
        throw("HVector2f out of range!");
}

bool HVector2f::IsInPolygon(const std::vector<HVector2f> &polygon) const
{
    bool inside = false;
    auto i = 0; 
    auto j=polygon.size() - 1;
    for (; i < polygon.size(); j = i++)
    {
        if (((polygon[i].y > y) != (polygon[j].y > y)) &&
            (x < (polygon[j].x - polygon[i].x) * (y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x))
        {
            inside = !inside;
        }
    }
    return inside;
}

bool HVector2f::IsInPolygon(const std::vector<HVector2f> &polygonVertices, const std::vector<uint32_t> &polygonIndices) const
{
    bool inside = false;
    auto i = 0;
    auto j = polygonIndices.size() - 1;
    for (; i < polygonIndices.size(); j = i++)
    {
        if (((polygonVertices[polygonIndices[i]].y > y) != (polygonVertices[polygonIndices[j]].y > y)) &&
            (x < (polygonVertices[polygonIndices[j]].x - polygonVertices[polygonIndices[i]].x) * (y - polygonVertices[polygonIndices[i]].y) / (polygonVertices[polygonIndices[j]].y - polygonVertices[polygonIndices[i]].y) + polygonVertices[polygonIndices[i]].y))
        {
            inside = !inside;
        }
    }
    return inside;
}

HVector2f BarycentricCoordinates(const HVector2f &a, const HVector2f &b, const HVector2f &c, const HVector2f &point)
{
    auto v0 = c - a;
    auto v1 = b - a;
    auto v2 = point - a;

    auto dot00 = v0.Dot(v0);
    auto dot01 = v0.Dot(v1);
    auto dot02 = v0.Dot(v2);
    auto dot11 = v1.Dot(v1);
    auto dot12 = v1.Dot(v2);

    auto invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    auto alpha = (dot11 * dot02 - dot01 * dot12) * invDenom;
    auto beta = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return HVector2f(alpha, beta);
}
bool HVector2f::IsInTriangle(HVector2f &a, HVector2f &b, HVector2f &c) const
{
    auto alphaAndBeta = BarycentricCoordinates(a, b, c, *this);
    if (alphaAndBeta.x < 0)
        return false;
    if (alphaAndBeta.y < 0)
        return false;
    if ((1.0 - (alphaAndBeta.x + alphaAndBeta.y)) < 0)
        return false;
    return true;
}

bool HVector2f::IsOnLine(HVector2f &a, HVector2f &b) const
{
    HFloat d = (y - a.y) * (b.x - a.x) - (x - a.x) * (b.y - a.y);
    return HMath::isZero(d);
}

HFloat HVector2f::Length() const
{
    return std::sqrt(LengthSqr());
}

HFloat HVector2f::LengthSqr() const
{
    return x * x + y * y;
}

HVector2f &HVector2f::Normalize()
{
    HFloat l = 1 / Length();
    x *= l;
    y *= l;
    return *this;
}

// HVector2i

HVector2i::HVector2i()
    : x(0), y(0)
{
}

HVector2i::HVector2i(HUInt X, HUInt Y)
    : x(X), y(Y)
{
}
