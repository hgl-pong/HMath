#pragma once
#include <Math/Procedural/Erosion.h>

namespace MathLib
{
	namespace Procedural
	{
		class HydraulicErosion : public Erosion
		{
		public:
			struct Params
			{
				int32_t mErosionRadius = 3;
				HReal mInertia = 0.05f;
				HReal mSedimentCapacityFactor = 4.f;
				HReal mMinSedimentCapacity = 0.01f;
				HReal mErodeSpeed = 0.3f;
				HReal mDespiteSpeed = 0.3f;
				HReal mEvaporateSpeed = 0.01f;
				HReal mGravity = 4;
				HReal mMaxDropletLifeTime = 30;
				HReal mInitialWaterVolume = 1;
				HReal mInitialSpeed = 1.f;
			};

		public:
			HydraulicErosion() = delete;
			HydraulicErosion(const Array2D<HReal> &heightMap, Params &params, Random::RandomGenerator* randomGenerator = nullptr)
				: Erosion(heightMap, params.mErosionRadius, randomGenerator)
			{				
				m_Params = params;
			}

			void Erode(const uint32_t numIterations) override
			{
				const HVector2I& resolution = m_Resolution;
				Parallel::ParallelFunction<uint32_t> fn = [&](uint32_t i)
					{				
						HVector2 particlePosition = m_RandomGenerator->GetVector2(HVector2(0,0), HVector2(resolution[0]-1, resolution[1]-1));
						HVector2 velocity;
						velocity.setZero();
						HReal speed = m_Params.mInitialSpeed;
						HReal waterVolume = m_Params.mInitialWaterVolume;
						HReal sediment = 0;

						for (uint32_t lifeTime = 0; lifeTime < m_Params.mMaxDropletLifeTime; lifeTime++)
						{
							HVector2I ipos = HVector2I(static_cast<int32_t>(particlePosition[0]), static_cast<int32_t>(particlePosition[1]));

							HVector2 offset = HVector2(particlePosition[0] - ipos[0], particlePosition[1] - ipos[1]);

							HVector2 gradient = _CalculateGradient(particlePosition);
							HReal height = m_ErosionResult.Sample(particlePosition);

							velocity[0] = velocity[0] * m_Params.mInertia - gradient[0] * (1 - m_Params.mInertia);
							velocity[1] = velocity[1] * m_Params.mInertia - gradient[1] * (1 - m_Params.mInertia);
							if (velocity.norm() != 0)
								velocity.normalize();
							particlePosition += velocity;
							if ((velocity[0] == 0 && velocity[1] == 0) ||
								particlePosition[0] < 0 || particlePosition[0] >= resolution[0] - 1 ||
								particlePosition[1] < 0 || particlePosition[1] >= resolution[1] - 1)
								break;
							HReal newHeight = m_ErosionResult.Sample(particlePosition);
							HReal deltaHeight = newHeight - height;
							HReal sedimentCapacity = std::max(-deltaHeight * speed * waterVolume *m_Params.mSedimentCapacityFactor, m_Params.mMinSedimentCapacity);

							if (sediment > sedimentCapacity || deltaHeight > 0)
							{
								HReal sedimentToDeposit = deltaHeight > 0 ? std::min(sediment, deltaHeight) : (sediment - sedimentCapacity) * m_Params.mDespiteSpeed;
								sediment -= sedimentToDeposit;

								if(ipos[0]>=0 && ipos[0]<resolution[0] && ipos[1]>=0 && ipos[1]<resolution[1])
								m_ErosionResult(ipos[0], ipos[1]) += sedimentToDeposit * (1 - offset[0]) * (1 - offset[1]);
								if(ipos[0]>=0 && ipos[0]<resolution[0] - 1 && ipos[1]>=0 && ipos[1]<resolution[1])
								m_ErosionResult(ipos[0] + 1, ipos[1]) += sedimentToDeposit* offset[0] * (1 - offset[1]);
								if(ipos[0]>=0 && ipos[0]<resolution[0] && ipos[1]>=0 && ipos[1]<resolution[1] - 1)
								m_ErosionResult(ipos[0], ipos[1] + 1) += sedimentToDeposit* (1 - offset[0]) * offset[1];
								if(ipos[0]>=0 && ipos[0]<resolution[0] - 1 && ipos[1]>=0 && ipos[1]<resolution[1] - 1)
								m_ErosionResult(ipos[0] + 1, ipos[1] + 1) += sedimentToDeposit* offset[0] * offset[1];
							}
							else
							{
								HReal sedimentToErode = std::min((sedimentCapacity - sediment) * m_Params.mErodeSpeed, -deltaHeight);

								for (uint32_t brushPosIndex = 0; brushPosIndex < m_BrushPositions(ipos[0], ipos[1]).size(); brushPosIndex++)
								{
									HVector2I brushPos = m_BrushPositions(ipos[0], ipos[1])[brushPosIndex];
									HReal weighedErodeAmount = m_BrushWeights(ipos[0], ipos[1])[brushPosIndex] * sedimentToErode;
									HReal deltaSediment = std::min(m_ErosionResult(brushPos[0], brushPos[1]), weighedErodeAmount);
									m_ErosionResult(brushPos[0], brushPos[1]) -= deltaSediment;
									sediment += deltaSediment;
								}

							}
							speed = std::sqrt(speed * speed + std::abs(deltaHeight) * m_Params.mGravity);
							waterVolume *= (1-m_Params.mEvaporateSpeed);
						}

					};
				Parallel::ParallelFor<uint32_t>(0, numIterations, fn);
			}

		private:
			Params m_Params;
		};
	} // namespace Procedural
} // namespace MathLib