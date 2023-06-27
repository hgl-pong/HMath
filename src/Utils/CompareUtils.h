#include "../../include/HMath.h"
#include <cmath>
namespace HMath
{
    inline bool isZero(HFloat number)
    {
        return std::abs(number) <= HFLOAT_EPSILON;
    }

    inline bool isEqual(HFloat a, HFloat b)
    {
        return isZero(a - b);
    }

    inline bool isWeakZero(HFloat number)
    {
        return std::abs(number) <= 1e-5f;
    }

    inline bool isWeakEqual(HFloat a, HFloat b)
    {
        return isWeakZero(a - b);
    }

} // namespace FFLOAT