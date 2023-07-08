#include "../../include/HMath.h"

HMat44::HMat44()
{
    HFloat m[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1};
    memcpy(m_Data, m, sizeof(HFloat) * 16);
}

HMat44::HMat44(const HFloat *data)
{
    for (int i = 0; i < 16; ++i)
    {
        m_Data[i] = data[i];
    }
}

HFloat HMat44::operator()(int i, int j) const
{
    return m_Data[i * 4 + j];
}

HMat44 HMat44::operator*(const HMat44 &second) const
{
    HMat44 result;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result.m_Data[i * 4 + j] = 0;

            for (int k = 0; k < 4; ++k)
            {
                result.m_Data[i * 4 + j] += m_Data[i * 4 + k] * second.m_Data[k * 4 + j];
            }
        }
    }

    return result;
}

bool HMat44::SetTranslation(const HVector3f &translate)
{
    m_Data[3] = translate.x;
    m_Data[7] = translate.y;
    m_Data[11] = translate.z;
    return true;
}

bool HMat44::SetScale(const HVector3f &scale)
{
    m_Data[0] = scale.x;
    m_Data[5] = scale.y;
    m_Data[10] = scale.z;
    return true;
}

bool HMat44::SetRotation(const HQuat &rotation)
{
    if (rotation[0] == 1)
        return false;
    return true;
}

bool HMat44::GetTranslation(HVector3f *translate) const
{
    translate->x = m_Data[3];
    translate->y = m_Data[7];
    translate->z = m_Data[11];
    return true;
}

bool HMat44::GetScale(HVector3f *scale) const
{
    scale->x = m_Data[0];
    scale->y = m_Data[5];
    scale->z = m_Data[10];
    return true;
}

bool HMat44::GetRotation(HQuat *rotation) const
{
    if (rotation == nullptr)
        return false;
    return true;
}