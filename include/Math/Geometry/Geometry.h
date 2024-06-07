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
            Plane(const MathLib::HVector3& normal, MathLib::HReal distance)
                : m_Normal(normal), m_Distance(distance)
            {
            }

            void Set(const MathLib::HVector3& position, const MathLib::HVector3& normal)
            {
                m_Normal = normal.normalized();
                m_Distance = m_Normal.dot(position);
            }

            const MathLib::HVector3& GetNormal() const
            {
                return m_Normal;
            }

            MathLib::HReal Distance(const MathLib::HVector3& point) const
            {
                return m_Normal.dot(point) - m_Distance;
            }

        private:
            MathLib::HVector3 m_Normal;
            MathLib::HReal m_Distance = 0.f;
        };

        class Box
        {
        public:
            Box() = default;
            Box(const Hvector3& halfSize)
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
        private:
            HVector3 m_HalfSize = { 1,1,1 };
        };
    }
}