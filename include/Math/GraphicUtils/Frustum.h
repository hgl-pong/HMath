#pragma once
#include <Math/Math.h>
#include <Math/Geometry/Geometry.h>
#include <Math/GraphicUtils/Camara.h>
namespace MathLib
{
	namespace GraphicUtils
	{
		class Frustum
		{
        public:
            Frustum(Camera& camera) : m_Camera(camera)
            {
            }
            void UpdateFrustum()
            {
                const HVector3& eye = m_Camera.GetEye();
                const HVector3& dir = m_Camera.GetDir();
                const HVector3& right = HVector3(0, 1, 0).cross(-dir).normalized();
                const HVector3& up = (-dir).cross(right);
                const HReal aspectRatio = m_Camera.GetAspectRatio();
                const HReal nearClip = m_Camera.GetNearClip();
                const HReal farClip = m_Camera.GetFarClip();
                const HReal fov = m_Camera.GetFOV();

                const HReal halfHSize = std::tan(fov / 2 * 4 / 3 * H_PI / 180.f) * farClip;
                const HReal halfVSize = halfHSize / aspectRatio;
                const HVector3 frontMultFar = dir * farClip;

                m_Planes[FRUSTUM_PLANE_NEAR].Set(eye + dir * nearClip, dir);
                m_Planes[FRUSTUM_PLANE_FAR].Set(eye + frontMultFar, -dir);
                m_Planes[FRUSTUM_PLANE_RIGHT].Set(eye, -(frontMultFar + right * halfHSize).cross(up));
                m_Planes[FRUSTUM_PLANE_LEFT].Set(eye, -up.cross(frontMultFar - right * halfHSize));
                m_Planes[FRUSTUM_PLANE_TOP].Set(eye, -right.cross(frontMultFar + up * halfVSize));
                m_Planes[FRUSTUM_PLANE_BOTTOM].Set(eye, -(frontMultFar - up * halfVSize).cross(right));
            }

            bool IsPointInFrustum(const HVector3& point) const
            {
                for (int i = 0; i < FRUSTUM_PLANE_COUNT; i++)
                {
                    if (m_Planes[i].Distance(point) < 0)
                    {
                        return false;
                    }
                }
                return true;
            }

            bool IsSphereInFrustum(const HVector3& center, float radius) const
            {
                for (int i = 0; i < FRUSTUM_PLANE_COUNT; i++)
                {
                    if (m_Planes[i].Distance(center) < -radius)
                    {
                        return false;
                    }
                }
                return true;
            }

            bool IsAABBInFrustum(const HAABBox3D& aabb) const
            {
                Eigen::Vector3f center = aabb.center();
                Eigen::Vector3f extents = aabb.sizes() / 2.0f;

                for (int i = 0; i < FRUSTUM_PLANE_COUNT; ++i)
                {
                    const Eigen::Vector3f& normal = m_Planes[i].GetNormal();
                    float distance = m_Planes[i].Distance(center);
                    float r = extents.x() * std::abs(normal.x()) +
                        extents.y() * std::abs(normal.y()) +
                        extents.z() * std::abs(normal.z());

                    if (distance < -r)
                    {
                        return false;
                    }
                }

                return true;
            }

            const HReal Distance(const HVector3& point) const
            {
                return (m_Camera.GetEye() - point).norm();
            };

        private:
            enum FrustumPlane
            {
                FRUSTUM_PLANE_NEAR = 0,
                FRUSTUM_PLANE_FAR,
                FRUSTUM_PLANE_LEFT,
                FRUSTUM_PLANE_RIGHT,
                FRUSTUM_PLANE_TOP,
                FRUSTUM_PLANE_BOTTOM,
                FRUSTUM_PLANE_COUNT
            };
            Camera& m_Camera;
            Geometry::Plane m_Planes[FRUSTUM_PLANE_COUNT];
        };

        class CullingManager
        {
        public:
            CullingManager(Camera& camera) : m_Frustum(camera)
            {
            }

            void UpdateFrustum()
            {
                m_Frustum.UpdateFrustum();
            }

            bool CullingObject(const HAABBox3D& worldBox)
            {
                if (m_Frustum.Distance(worldBox.center()) > m_MaxDistance)
                    return true;
                if (m_Frustum.IsAABBInFrustum(worldBox))
                {
                    return false;
                }
                return true;
            }

            void SetCullingDistance(HReal distance)
            {
                m_MaxDistance = distance;
            }

        private:
            HReal m_MaxDistance = std::numeric_limits<HReal>::max();
            Frustum m_Frustum;
        };
	}
}