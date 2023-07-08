#include "../../include/HMath.h"
namespace HMath
{
    bool PutTranslate(const HVector3f &translate, HMat44 *matrix)
    {
        matrix->SetTranslation(translate);
    }
    bool PutScale(const HVector3f &scale, HMat44 *matrix)
    {
        matrix->SetScale(scale);
    }
    bool PutRotation(const HQuat &rotation, HMat44 *matrix)
    {
        matrix->SetRotation(rotation);
    }

    bool StoreTranslate(HVector3f *translate, const HMat44 &matrix)
    {
        matrix.GetTranslation(translate);
    }
    bool StoreScale(HVector3f *scale, const HMat44 &matrix)
    {
        matrix.GetScale(scale);
    }
    bool StoreRotation(HQuat *rotation, const HMat44 &matrix)
    {
        matrix.GetRotation(rotation);
    }

    bool ComposeMatrix(const HVector3f &translate, const HVector3f &scale, const HQuat &rotation, HMat44 *matrix)
    {
        PutTranslate(translate, matrix);
        PutScale(scale, matrix);
        PutRotation(rotation, matrix);
    }
    bool DeComposeMatrix(HVector3f *translate, HVector3f *scale, HQuat *rotation, const HMat44 &matrix)
    {
        StoreTranslate(translate, matrix);
        StoreScale(scale, matrix);
        StoreRotation(rotation, matrix);
    }

};