#pragma once
#include <Math/Math.h>
namespace MathLib
{
    namespace Geometry2D
    {
        struct CircumCircle
        {
            HVector2 m_Center;
            HReal m_Radius;
            CircumCircle() {}

            CircumCircle(const std::vector<HVector2> &points, uint32_t v0, uint32_t v1, uint32_t v2)
            {
                Update(points, v0, v1, v2);
            }

            CircumCircle(const HVector2 &p0, const HVector2 &p1, const HVector2 &p2)
            {
                Update(p0, p1, p2);
            }

            void Update(const HVector2 &p0, const HVector2 &p1, const HVector2 &p2)
            {
                HMatrix2 A_matrix;
                HVector2 b_vector;

                A_matrix(0, 0) = 2 * (p1.x() - p0.x());
                A_matrix(0, 1) = 2 * (p1.y() - p0.y());
                A_matrix(1, 0) = 2 * (p2.x() - p0.x());
                A_matrix(1, 1) = 2 * (p2.y() - p0.y());

                b_vector(0) = p1.squaredNorm() - p0.squaredNorm();
                b_vector(1) = p2.squaredNorm() - p0.squaredNorm();

                m_Center = A_matrix.colPivHouseholderQr().solve(b_vector);
                m_Radius = (m_Center - p0).norm();
            }

            void Update(const std::vector<HVector2> &points, uint32_t v0, uint32_t v1, uint32_t v2)
            {
                const HVector2 &p0 = points[v0];
                const HVector2 &p1 = points[v1];
                const HVector2 &p2 = points[v2];
                Update(p0, p1, p2);
            }

            bool IsPointInside(const HVector2 &point) const
            {
                HVector2 diff = point - m_Center;
                HReal dist = diff.norm();
                return dist - m_Radius <= H_EPSILON;
            }
        };
    } // namespace Geometry2D
    namespace Geometry
    {
        struct Plane
        {
            Plane() = default;
            Plane(const HVector3 &normal, HReal distance)
                : m_Normal(normal), m_Distance(distance)
            {
            }

            void Set(const HVector3 &position, const HVector3 &normal)
            {
                m_Normal = normal.normalized();
                m_Distance = m_Normal.dot(position);
            }

            HReal Distance(const HVector3 &point) const
            {
                return m_Normal.dot(point) - m_Distance;
            }

            HVector3 m_Normal;
            HReal m_Distance = 0.f;
        };

        struct Box
        {
            Box() = default;
            Box(const HVector3 &halfSize)
                : m_HalfSize(halfSize)
            {
            }

            void Set(const HVector3 &halfSize)
            {
                m_HalfSize = halfSize;
            }

            const HReal Distance(const HVector3 &point) const
            {
                HReal distance = 0;
                for (int i = 0; i < 3; ++i)
                {
                    HReal d = std::abs(point[i]) - m_HalfSize[i];
                    if (d > 0)
                    {
                        distance += d * d;
                    }
                }
                return std::sqrt(distance);
            }

            HVector3 m_HalfSize = {1, 1, 1};
        };

        struct Sphere
        {
            Sphere() = default;
            Sphere(HReal radius)
                : m_Radius(radius)
            {
            }

            void Set(HReal radius)
            {
                m_Radius = radius;
            }

            HReal Distance(const HVector3 &point) const
            {
                return (point - HVector3::Zero()).norm() - m_Radius;
            }

            HReal m_Radius = 1.f;
        };

        struct Capsule
        {
            Capsule() = default;
            Capsule(HReal radius, HReal halfHeight)
                : m_Radius(radius), m_HalfHeight(halfHeight)
            {
            }

            void Set(HReal radius, HReal halfHeight)
            {
                m_Radius = radius;
                m_HalfHeight = halfHeight;
            }

            HReal Distance(const HVector3 &point) const
            {
                HVector3 p = point;
                p[1] = std::max(p[1], -m_HalfHeight);
                p[1] = std::min(p[1], m_HalfHeight);
                return (p - HVector3(0, p[1], 0)).norm() - m_Radius;
            }

            HReal m_Radius;
            HReal m_HalfHeight;
        };

        struct Tetraheron
        {
            Tetraheron() = default;
            Tetraheron(const HVector3 &a, const HVector3 &b, const HVector3 &c, const HVector3 &d)
                : m_A(a), m_B(b), m_C(c), m_D(d)
            {
            }

            void Set(const HVector3 &a, const HVector3 &b, const HVector3 &c, const HVector3 &d)
            {
                m_A = a;
                m_B = b;
                m_C = c;
                m_D = d;
            }

            HReal Distance(const HVector3 &point) const
            {
                HVector3 p = point;
                HVector3 n = ((m_B - m_A).cross(m_C - m_A)).normalized();
                HReal d = n.dot(m_A);
                return n.dot(p) - d;
            }

            HVector3 m_A;
            HVector3 m_B;
            HVector3 m_C;
            HVector3 m_D;
        };
    }
}