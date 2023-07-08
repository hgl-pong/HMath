#include "../Utils/TransformUtils.h"

HTransform::HTransform()
{
}

HTransform::HTransform(const HVector3f &translate, const HVector3f scale)
    : m_Translate(translate),
      m_Scale(scale)
{
}

HTransform::HTransform(const HVector3f &translate, const HVector3f &scale, const HQuat &rotation)
    : m_Translate(translate),
      m_Scale(scale),
      m_Rotation(rotation)
{
}

HTransform::HTransform(const HMat44 &matrix)
{
    HMath::DeComposeMatrix(&m_Translate, &m_Scale, &m_Rotation, matrix);
}

HTransform HTransform::operator+(const HTransform &transform) const
{
    HTransform newTrans;
    newTrans.m_Translate = m_Translate + transform.m_Translate;
    newTrans.m_Scale = m_Scale + transform.m_Scale;
    newTrans.m_Rotation = m_Rotation + transform.m_Rotation;
    return newTrans;
}

bool HTransform::SetTranslate(const HVector3f &translate)
{
    m_Translate = translate;
    return true;
}

bool HTransform::SetScale(const HVector3f &scale)
{
    m_Scale = scale;
    return true;
}

bool HTransform::SetRotation(const HQuat &rotation)
{
    m_Rotation = rotation;
    return true;
}

bool HTransform::GetTranslate(HVector3f *translate)
{
    if (translate == nullptr)
        return false;
    *translate = m_Translate;
    return true;
}

bool HTransform::GetScale(HVector3f *scale)
{
    if (scale == nullptr)
        return false;
    *scale = m_Scale;
    return true;
}

bool HTransform::GetRotation(HQuat *rotation)
{
    if (rotation == nullptr)
        return false;
    *rotation = m_Rotation;
    return true;
}

bool HTransform::GetMatrix(HMat44 *matrix)
{
    if (matrix == nullptr)
        return false;
    HMath::ComposeMatrix(m_Translate, m_Scale, m_Rotation, matrix);
    return true;
}