#pragma once
#include <Math/Math.h>

namespace MathLib
{
	namespace Random
	{
		class RandomGenerator
		{
		public:
			virtual void Seed(uint32_t seed) = 0;
			virtual HReal GetReal(const HReal& min, const HReal& max) = 0;
			virtual HVector2 GetVector2(const HVector2& min, const HVector2& max) = 0;
			virtual HVector3 GetVector3(const HVector3& min, const HVector3& max) = 0;
			virtual HVector4 GetVector4(const HVector4& min, const HVector4& max) = 0;
		};
		
		class SimpleRandomGenerator : public RandomGenerator
		{
		public:
			void Seed(uint32_t seed) override
			{
				srand(seed);
			}

			HReal GetReal(const HReal& min, const HReal& max) override
			{
				return static_cast<HReal>(rand()) / RAND_MAX * (max - min) + min;
			}

			HVector2 GetVector2(const HVector2& min, const HVector2& max) override
			{
				return HVector2(GetReal(min[0], max[0]), GetReal(min[1], max[1]));
			}

			HVector3 GetVector3(const HVector3& min, const HVector3& max) override
			{
				return HVector3(GetReal(min[0], max[0]), GetReal(min[1], max[1]), GetReal(min[2], max[2]));
			}

			HVector4 GetVector4(const HVector4& min, const HVector4& max) override
			{
				return HVector4(GetReal(min[0], max[0]), GetReal(min[1], max[1]), GetReal(min[2], max[2]), GetReal(min[3], max[3]));
			}
		};

		SimpleRandomGenerator g_DefaultRandomGenerator;
	}
}//namespace MathLib