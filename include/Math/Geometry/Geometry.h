#pragma once
#include <Math/Math.h>
namespace MathLib
{
    namespace Geometry
    {
        class Plane
        {
        public:
            Plane() = default;
            Plane(const HVector3& normal, HReal distance)
                : m_Normal(normal), m_Distance(distance)
            {
            }

            void Set(const HVector3& position, const HVector3& normal)
            {
                m_Normal = normal.normalized();
                m_Distance = m_Normal.dot(position);
            }

            const HVector3& GetNormal() const
            {
                return m_Normal;
            }

            HReal Distance(const HVector3& point) const
            {
                return m_Normal.dot(point) - m_Distance;
            }

        private:
            HVector3 m_Normal;
            HReal m_Distance = 0.f;
        };

        class Box
        {
        public:
            Box() = default;
            Box(const HVector3& halfSize)
                : m_HalfSize(halfSize)
            {
            }

            void Set(const HVector3& halfSize)
            {
                m_HalfSize = halfSize;
            }

            const HVector3& GetHalfSize() const
            {
                return m_HalfSize;
            }

            const HReal Distance(const HVector3& point) const
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

        private:
            HVector3 m_HalfSize = { 1, 1, 1 };
        };

        class Sphere
        {
        public:
            Sphere() = default;
            Sphere(HReal radius)
                : m_Radius(radius)
            {
            }

            void Set(HReal radius)
            {
                m_Radius = radius;
            }

            HReal GetRadius() const
            {
                return m_Radius;
            }

            HReal Distance(const HVector3& point) const
            {
                return (point - HVector3::Zero()).norm() - m_Radius;
            }

        private:
            HReal m_Radius = 1.f;
        };

        class Capsule
        {
        public:
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

            HReal GetRadius() const
            {
                return m_Radius;
            }

            HReal GetHalfHeight() const
            {
                return m_HalfHeight;
            }

            HReal Distance(const HVector3& point) const
            {
                HVector3 p = point;
                p[1] = std::max(p[1], -m_HalfHeight);
                p[1] = std::min(p[1], m_HalfHeight);
                return (p - HVector3(0, p[1], 0)).norm() - m_Radius;
            }

        private:
            HReal m_Radius;
            HReal m_HalfHeight;
        };

        class Tetraheron
        {
        public:
            Tetraheron() = default;
            Tetraheron(const HVector3& a, const HVector3& b, const HVector3& c, const HVector3& d)
                : m_A(a), m_B(b), m_C(c), m_D(d)
            {
			}

            void Set(const HVector3& a, const HVector3& b, const HVector3& c, const HVector3& d)
            {
				m_A = a;
				m_B = b;
				m_C = c;
				m_D = d;
			}

            HReal Distance(const HVector3& point) const
            {
				HVector3 p = point;
				HVector3 n = ((m_B - m_A).cross(m_C - m_A)).normalized();
				HReal d = n.dot(m_A);
				return n.dot(p) - d;
			}
        private:
            HVector3 m_A;
			HVector3 m_B;
			HVector3 m_C;
			HVector3 m_D;
        };
    }
}