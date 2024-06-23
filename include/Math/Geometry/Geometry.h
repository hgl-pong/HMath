#pragma once
#include <Math/Math.h>
#include <Math/GraphicUtils/MeshCommon.h>
namespace MathLib
{
    namespace Geometry2D
    {
        template<typename IntType,typename = std::enable_if_t<std::is_integral<IntType>::value>>
        struct CircumCircle
        {
            HVector2 m_Center;
            HReal m_Radius;
            CircumCircle() {}

            CircumCircle(const std::vector<HVector2> &points, const TriangleIndex<IntType> &triangle)
            {
                Update(points, triangle[0], triangle[1], triangle[2]);
            }

            CircumCircle(const std::vector<HVector2> &points, IntType v0, IntType v1, IntType v2)
            {
                Update(points, v0, v1, v2);
            }

            CircumCircle(const HVector2 &p0, const HVector2 &p1, const HVector2 &p2)
            {
                Update(p0, p1, p2);
            }

            void Update(const HVector2& p0, const HVector2& p1, const HVector2& p2)
            {
                HMatrix2 A_matrix;
                HVector2 b_vector;

                A_matrix(0, 0) = 2 * (p1[0] - p0[0]);
                A_matrix(0, 1) = 2 * (p1[1] - p0[1]);
                A_matrix(1, 0) = 2 * (p2[0] - p0[0]);
                A_matrix(1, 1) = 2 * (p2[1] - p0[1]);

                b_vector(0) = p1.squaredNorm() - p0.squaredNorm();
                b_vector(1) = p2.squaredNorm() - p0.squaredNorm();

                m_Center = A_matrix.colPivHouseholderQr().solve(b_vector);
                m_Radius = (m_Center - p0).norm();
            }

            void Update(const std::vector<HVector2> &points, IntType v0, IntType v1, IntType v2)
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
        typedef CircumCircle<uint8_t> CircumCircle8;
        typedef CircumCircle<uint16_t> CircumCircle16; 
        typedef CircumCircle<uint32_t> CircumCircle32;
        typedef CircumCircle<uint64_t> CircumCircle64;
    } // namespace Geometry2D
    namespace Geometry
    {
        struct CircumSphere
        {
            HVector3 m_Center;
            HReal m_Radius;
            CircumSphere() {}
            template <typename IntType>
            CircumSphere(const std::vector<HVector3> &points, const TetrahedronIndex<IntType> &tetrahedron)
            {
                Update(points, tetrahedron[0], tetrahedron[1], tetrahedron[2], tetrahedron[3]);
            }

            CircumSphere(const std::vector<HVector3> &points, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3)
            {
                Update(points, v0, v1, v2, v3);
            }

            CircumSphere(const HVector3 &p0, const HVector3 &p1, const HVector3 &p2, const HVector3 &p3)
            {
                Update(p0, p1, p2, p3);
            }

            void Update(const HVector3 &p0, const HVector3 &p1, const HVector3 &p2, const HVector3 &p3)
            {
                HMatrix4 A_matrix;
                HVector4 b_vector;

                A_matrix(0, 0) = 2 * (p1[0] - p0[0]);
                A_matrix(0, 1) = 2 * (p1[1] - p0[1]);
                A_matrix(0, 2) = 2 * (p1[2] - p0[2]);
                A_matrix(1, 0) = 2 * (p2[0] - p0[0]);
                A_matrix(1, 1) = 2 * (p2[1] - p0[1]);
                A_matrix(1, 2) = 2 * (p2[2] - p0[2]);
                A_matrix(2, 0) = 2 * (p3[0] - p0[0]);
                A_matrix(2, 1) = 2 * (p3[1] - p0[1]);
                A_matrix(2, 2) = 2 * (p3[2] - p0[2]);

                // b_vector(0) = p1.squaredNorm() - p0.squaredNorm();
                // b_vector(1) = p2.squaredNorm() - p0.squaredNorm();
                // b_vector(2) = p3.squaredNorm() - p0.squaredNorm();

                // m_Center = A_matrix.colPivHouseholderQr().solve(b_vector);
                // m_Radius = (m_Center - p0).norm();
            }

            void Update(const std::vector<HVector3> &points, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3)
            {
                const HVector3 &p0 = points[v0];
                const HVector3 &p1 = points[v1];
                const HVector3 &p2 = points[v2];
                const HVector3 &p3 = points[v3];
                Update(p0, p1, p2, p3);
            }
        };
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