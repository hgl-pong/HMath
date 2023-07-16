#pragma once
#include "../HMath.h"
#include <cmath>
namespace HMath
{

    inline HFloat HashVector3f(const HVector3f &v)
    {
        HFloat vx, vy, vz;
        HVector3f temp = v / v.LengthSqr();
        if (v.x >= 0)
            vx = ((long)((temp.x + ROUND) * SCALE)) / SCALE;
        else
            vx = ((long)((temp.x - ROUND) * SCALE)) / SCALE;

        if (v.y >= 0)
            vy = ((long)((temp.y + ROUND) * SCALE)) / SCALE;
        else
            vy = ((long)((temp.y - ROUND) * SCALE)) / SCALE;

        if (v.z >= 0)
            vz = ((long)((temp.z + ROUND) * SCALE)) / SCALE;
        else
            vz = ((long)((temp.z - ROUND) * SCALE)) / SCALE;
        HFloat key = vx * (0.1 + vy) * (0.2 + vz);
        return key;
    }

    inline HFloat HashVector2f(const HVector2f &v)
    {
        HFloat vx, vy;
        HFloat l = std::sqrt(v.x * v.x + v.y * v.y);
        HVector2f temp = {v.x / l, v.y / l};
        if (v.x >= 0)
            vx = ((long)((temp.x + ROUND) * SCALE)) / SCALE;
        else
            vx = ((long)((temp.x - ROUND) * SCALE)) / SCALE;

        if (v.y >= 0)
            vy = ((long)((temp.y + ROUND) * SCALE)) / SCALE;
        else
            vy = ((long)((temp.y - ROUND) * SCALE)) / SCALE;
        HFloat key = vx * (0.1 + vy);
        return key;
    }

#define HSCALE 100000000
#define HROUND 5e-9f

    inline size_t HashHFloat(const HFloat &v)
    {
        long x;
        if (v >= 0)
            x = ((long)((v + HROUND) * HSCALE)) / HSCALE;
        else
            x = ((long)((v - HROUND) * HSCALE)) / HSCALE;
        return std::hash<long>()(x);
    }

#define WEAK_SCALE 10000
#define WEAK_ROUND 1e-5f
    inline HFloat WeakHashHVector3f(const HVector3f &v)
    {
        HFloat vx, vy, vz;
        HVector3f temp = v / v.LengthSqr();
        if (v.x >= 0)
            vx = ((long)((temp.x + WEAK_ROUND) * WEAK_SCALE)) / WEAK_SCALE;
        else
            vx = ((long)((temp.x - WEAK_ROUND) * WEAK_SCALE)) / WEAK_SCALE;

        if (v.y >= 0)
            vy = ((long)((temp.y + WEAK_ROUND) * WEAK_SCALE)) / WEAK_SCALE;
        else
            vy = ((long)((temp.y - WEAK_ROUND) * WEAK_SCALE)) / WEAK_SCALE;

        if (v.z >= 0)
            vz = ((long)((temp.z + WEAK_ROUND) * WEAK_SCALE)) / WEAK_SCALE;
        else
            vz = ((long)((temp.z - WEAK_ROUND) * WEAK_SCALE)) / WEAK_SCALE;
        HFloat key = vx * (0.1 + vy) * (0.2 + vz);
        return key;
    }

    inline size_t WeakHashHFloat(const HFloat &v)
    {
        long x;
        if (v >= 0)
            x = ((long)((v + WEAK_ROUND) * WEAK_SCALE)) / WEAK_SCALE;
        else
            x = ((long)((v - WEAK_ROUND) * WEAK_SCALE)) / WEAK_SCALE;
        return std::hash<long>()(x);
    }

}
namespace std
{

    template <>
    struct hash<HVector3f>
    {
        size_t operator()(const HVector3f &v) const
        {

            return hash<HFloat>()(HMath::HashVector3f(v));
        }
    };

    template <>
    struct hash<HVector2f>
    {
        size_t operator()(const HVector2f &v) const
        {
            return hash<HFloat>()(HMath::HashVector2f(v));
        }
    };

}
