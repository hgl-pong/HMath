#pragma once
#include <Math/Procedural/Erosion.h>

namespace MathLib
{
	namespace Procedural
	{
		class WindErosion : public Erosion
		{
		public:
			struct Params
			{
				HReal mErosionRadius = 3;
				HReal mMaxLifeTime = 512;
				HReal mGravity = 4.0;
				HVector3 mInitWindSpeed = HVector3(1, 0, 0);
				HReal mBoundatLayer = 2;
				HReal mSuspension = 0.05f;
			};

		public:
			WindErosion() = delete;
			WindErosion(const Array2D<HReal>& heightMap, Params& params, Random::RandomGenerator* randomGenerator = nullptr)
				: Erosion(heightMap, params.mErosionRadius, randomGenerator)
			{
				m_Params = params;
			}

			void Erode(const uint32_t numIterations) override
			{
				const HVector2I& resolution = m_Resolution;
				Parallel::ParallelFunction<uint32_t> fn = [&](uint32_t i)
					{
						HVector3 particlePosition(m_RandomGenerator->GetReal(0, resolution[0] - 1), 1.5, m_RandomGenerator->GetReal(0, resolution[1] - 1));
						HVector3 velocity;
						velocity.setZero();
						HVector3 windSpeed = m_Params.mInitWindSpeed;
						HReal sediment = 0;

						for (uint32_t lifeTime = 0; lifeTime < m_Params.mMaxLifeTime; lifeTime++)
						{
							HVector2I ipos = HVector2I(static_cast<int32_t>(particlePosition[0]), static_cast<int32_t>(particlePosition[2]));

							HVector2 offset = HVector2(particlePosition[0] - ipos[0], particlePosition[2] - ipos[1]);

							HVector2 gradient = _CalculateGradient(HVector2(particlePosition[0], particlePosition[2]));
							HVector3 normal(gradient[0], 1, gradient[1]);
							normal.normalize();

							HReal height = m_ErosionResult.Sample(HVector2(particlePosition[0], particlePosition[2]));

							HReal hFac =std::exp(-(particlePosition[1] - height) * m_Params.mBoundatLayer);
							if (hFac < 0)
								hFac = 0;

							HReal shadow = normal.dot(windSpeed.normalized());
							if(shadow < 0)
								shadow = 0;
							shadow = 1 - shadow;

							velocity += 0.05f * ((0.1f + 0.9f * shadow) * windSpeed - velocity);

							if (particlePosition[1] > height)
								velocity[1] -= m_Params.mGravity * sediment;

							HReal collision = -normal.dot(velocity.normalized());
							if(collision < 0)
								collision = 0;

							HVector3 rSpeed = normal.cross(normal.cross((1 - collision) * velocity));

							velocity += 0.9 * (shadow * Lerp(windSpeed, rSpeed, shadow * hFac) - velocity);

							velocity += 0.1f * hFac * collision * m_RandomGenerator->GetVector3(HVector3(-0.5f, -0.5f, -0.5f), HVector3(0.5f, 0.5f, 0.5f));

							velocity *= (1.0f - 0.3 * sediment);

							particlePosition += velocity;
							if ((velocity[0] == 0 && velocity[1] == 0) ||
								particlePosition[0] < 0 || particlePosition[0] >= resolution[0] - 1 ||
								particlePosition[2] < 0 || particlePosition[2] >= resolution[1] - 1)
								break;

							HReal force = - normal.dot(velocity.normalized()) * velocity.norm();
							if (force < 0)
								force = 0;

							HReal lift = (1.0f - collision) * velocity.norm();

							HReal sedimentCapacity = force * hFac + 0.02f * lift * hFac;

							//if (sediment > sedimentCapacity)
							//{
							//	HReal sedimentToDeposit = (sediment - sedimentCapacity) * m_Params.mSuspension;
							//	sediment -= sedimentToDeposit;

							//	if (ipos[0] >= 0 && ipos[0] < resolution[0] && ipos[1] >= 0 && ipos[1] < resolution[1])
							//		m_ErosionResult(ipos[0], ipos[1]) += sedimentToDeposit * (1 - offset[0]) * (1 - offset[1]);
							//	if (ipos[0] >= 0 && ipos[0] < resolution[0] - 1 && ipos[1] >= 0 && ipos[1] < resolution[1])
							//		m_ErosionResult(ipos[0] + 1, ipos[1]) += sedimentToDeposit * offset[0] * (1 - offset[1]);
							//	if (ipos[0] >= 0 && ipos[0] < resolution[0] && ipos[1] >= 0 && ipos[1] < resolution[1] - 1)
							//		m_ErosionResult(ipos[0], ipos[1] + 1) += sedimentToDeposit * (1 - offset[0]) * offset[1];
							//	if (ipos[0] >= 0 && ipos[0] < resolution[0] - 1 && ipos[1] >= 0 && ipos[1] < resolution[1] - 1)
							//		m_ErosionResult(ipos[0] + 1, ipos[1] + 1) += sedimentToDeposit * offset[0] * offset[1];
							//}
							//else
							{
								HReal sedimentToErode = (sedimentCapacity - sediment) * m_Params.mSuspension;

								for (uint32_t brushPosIndex = 0; brushPosIndex < m_BrushPositions(ipos[0], ipos[1]).size(); brushPosIndex++)
								{
									HVector2I brushPos = m_BrushPositions(ipos[0], ipos[1])[brushPosIndex];
									HReal weighedErodeAmount = m_BrushWeights(ipos[0], ipos[1])[brushPosIndex] * sedimentToErode;
									HReal deltaSediment = std::min(m_ErosionResult(brushPos[0], brushPos[1]), weighedErodeAmount);
									m_ErosionResult(brushPos[0], brushPos[1]) -= deltaSediment;
									sediment += deltaSediment;
								}

							}
						}

					};
				for(uint32_t i = 0; i < numIterations; i++)
					fn(i);
				//Parallel::ParallelFor<uint32_t>(0, numIterations, fn);
			}
		private:
			 Params m_Params;
		};
	}
}// namespace MathLib