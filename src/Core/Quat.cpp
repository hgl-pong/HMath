#include "../../include/HMath.h"

HQuat HQuat::operator+(const HQuat& quat) const
{
    HQuat result;
	result.x =x+ quat.x;
	return HQuat();
}

HFloat HQuat::operator[](const int i) const
{
    if (i == 0)
        return x;
    else if (i == 1)
        return y;
    else if (i == 2)
        return z;
    else if (i == 3)
        return z;
    else
        throw("HQuat out of range!");
}