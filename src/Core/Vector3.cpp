#include "../../include/HMath.h"
#include "../Utils/CompareUtils.h"

HVector3f::HVector3f()
    : x(0), y(0), z(0)
{
}

HVector3f::HVector3f(HFloat value)
    : x(value), y(value), z(value)
{
}

HVector3f::HVector3f(HFloat X, HFloat Y, HFloat Z)
    : x(X), y(Y), z(Z)
{
}

HVector3f &HVector3f::operator=(const HVector3f &v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

HVector3f &HVector3f::operator+=(const HVector3f &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

HVector3f &HVector3f::operator-=(const HVector3f &v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

HVector3f HVector3f::operator*=(const HFloat &d)
{
    x *= d;
    y *= d;
    z *= d;
    return *this;
}

HVector3f &HVector3f::operator/=(const HFloat d)
{
    x /= d;
    y /= d;
    z /= d;
    return *this;
}

HVector3f HVector3f::operator-(const HVector3f &v) const
{
    return HVector3f(x - v.x, y - v.y, z - v.z);
}

HVector3f HVector3f::operator+(const HVector3f &v) const
{
    return HVector3f(x + v.x, y + v.y, z + v.z);
}

HVector3f HVector3f::operator*(const HFloat d) const
{
    return HVector3f(x * d, y * d, z * d);
}

HVector3f HVector3f::operator*(const HVector3f &v) const
{
    return HVector3f(x * v.x, y * v.y, z * v.z);
}

HVector3f HVector3f::operator/(const HFloat d) const
{
    return HVector3f(x / d, y / d, z / d);
}

HVector3f HVector3f::operator/(const HVector3f &v) const
{
    return HVector3f(x / v.x, y / v.y, z / v.z);
}

int HVector3f::operator==(const HVector3f &v) const
{
    return HMath::isEqual(x, v.x) && HMath::isEqual(y, v.y) && HMath::isEqual(z, v.z);
}

bool HVector3f::operator<(const HVector3f &v) const
{
    return x < v.x && y < v.y && z < v.z;
}

HFloat HVector3f::operator[](const int i) const
{
    if (i == 0)
        return x;
    else if (i == 1)
        return y;
    else if (i == 2)
        return z;
    else
        throw("HVector3f out of range!");
}

bool HVector3f::IsOnSegment(HVector3f &p1, HVector3f &p2)
{
    auto m_Min = p1.Floor(p2);
    auto m_Max = p1.Ceiling(p2);
    if (std::max(x, m_Min.x) > std::min(x, m_Max.x) + FLOAT_EPSILON)
        return false;
    if (std::max(y, m_Min.y) > std::min(y, m_Max.y) + FLOAT_EPSILON)
        return false;
    if (std::max(z, m_Min.z) > std::min(z, m_Max.z) + FLOAT_EPSILON)
        return false;
    return IsOnLine(p1, p2);
}

bool HVector3f::IsOnLine(const HVector3f &p1, const HVector3f &p2) const
{
    return HMath::isZero(((x - p1.x) * (p1.y - p2.y)) - ((p1.x - p2.x) * (y - p1.y))) && HMath::isZero(((y - p1.y) * (p1.z - p2.z)) - ((p1.y - p2.y) * (z - p1.z)));
}

bool HVector3f::IsZero()
{
    return HMath::isZero(x) && HMath::isZero(y) && HMath::isZero(z);
}

HFloat HVector3f::Length() const
{
    return std::sqrt(LengthSqr());
}

HFloat HVector3f::LengthSqr() const
{
    return x * x + y * y + z * z;
}

HVector3f &HVector3f::Normalize()
{
    HFloat len = Length();
    if (HMath::isZero(len))
    {
        x = 0;
        y = 0;
        z = 0;
        return *this;
    }
    *this /= len;
    return *this;
}

HVector3f HVector3f::Cross(const HVector3f &v) const
{
    HVector3f tmp;
    tmp.x = y * v.z - z * v.y;
    tmp.y = z * v.x - x * v.z;
    tmp.z = x * v.y - y * v.x;
    return tmp;
}

HVector3f HVector3f::Prod(const HVector3f &v)
{
    return *this * v;
}

HFloat HVector3f::Dot(const HVector3f &v) const
{
    return v.x * x + v.y * y + v.z * z;
}

HFloat HVector3f::Distance(const HVector3f &v) const
{
    return std::sqrt(DistanceSqr(v));
}

HFloat HVector3f::DistanceSqr(const HVector3f &v) const
{
    return ((x - v.x) * (x - v.x) +
            (y - v.y) * (y - v.y) +
            (z - v.z) * (z - v.z));
}

HVector3f HVector3f::Ceiling(const HVector3f &v) const
{
    return HVector3f(std::max(x, v.x), std::max(y, v.y), std::max(z, v.z));
}

HVector3f HVector3f::Floor(const HVector3f &v) const
{
    return HVector3f(std::min(x, v.x), std::min(y, v.y), std::min(z, v.z));
}