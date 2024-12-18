#pragma once
#include <Math/Math.h>
#include <Math/Core/Random.h>
#include <Math/Array2D.h>
#include <Math/MathUtils.h>

namespace MathLib
{
	namespace Procedural
	{
		class HydraulicErosion
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
			{				
				m_ErosionResult = heightMap;
				m_RandomGenerator = randomGenerator;
				if (m_RandomGenerator == nullptr)
					m_RandomGenerator = &Random::g_DefaultRandomGenerator;
				m_Params = params;
				m_Resolution =HVector2I((int32_t)heightMap.GetSizeX(), (int32_t)heightMap.GetSizeY());
				_InitBrush();
			}

			void Erode(const uint32_t numIterations)
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

			Array2D<HReal> Result() const
			{
				return m_ErosionResult;
			}

		private:
			void _InitBrush()
			{
				const int32_t& erosionRadius = m_Params.mErosionRadius;
				const HVector2I& resolution = m_Resolution;
				m_BrushPositions.ReSize(resolution[0], resolution[1]);
				m_BrushWeights.ReSize(resolution[0], resolution[1]);

				Parallel::ParallelFunction2<int32_t> fn = [&](int32_t x, int32_t y)
				{
					std::vector<int32_t> xOffset(erosionRadius * erosionRadius * 4);
					std::vector<int32_t> yOffset(erosionRadius * erosionRadius * 4);
					std::vector<HReal> weights(erosionRadius * erosionRadius * 4);

					HReal weightSum = 0.f;
					uint32_t addIndex = 0;
					{
						for (int32_t j = -erosionRadius; j <= erosionRadius; j++)
						{
							for (int32_t i = -erosionRadius; i <= erosionRadius; i++)
							{
								HReal sqrtDist = HReal(i * i + j * j);
								if (sqrtDist < erosionRadius * erosionRadius)
								{
									int32_t xPos = x + i;
									int32_t yPos = y + j;
									if (xPos >= 0 && xPos < resolution[0] && yPos >= 0 && yPos < resolution[1])
									{
										HReal weight = HReal(1.f - std::sqrt(sqrtDist) / erosionRadius);
										if (weight > 0)
										{
											weightSum += weight;
											xOffset[addIndex] = i;
											yOffset[addIndex] = j;
											weights[addIndex] = weight;
											addIndex++;
										}
									}
								}
							}
						}
					}
					uint32_t numEntries = addIndex;
					m_BrushPositions(x, y).reserve(numEntries);
					m_BrushWeights(x, y).reserve(numEntries);

					for (uint32_t i = 0; i < numEntries; i++)
					{
						m_BrushPositions(x, y).push_back(HVector2I(x + xOffset[i], y + yOffset[i]));
						m_BrushWeights(x, y).push_back(weights[i] / weightSum);
					}
				};
				Parallel::ParallelFor<uint32_t>(0, resolution[0], 0, resolution[1], fn);
			}

			/// <summary>
			/// 00 10
			/// 01 11
			/// </summary>
			/// <param name="pos"></param>
			/// <returns></returns>
			HVector2 _CalculateGradient(const HVector2& pos)
			{
				const HVector2I& resolution = m_Resolution;

				const HVector2I ipos = HVector2I(static_cast<int32_t>(pos[0]), static_cast<int32_t>(pos[1]));
				const HVector2 offset = HVector2(pos[0] - ipos[0], pos[1] - ipos[1]);

				HReal h00 = m_ErosionResult(ipos[0], ipos[1]);
				HReal h10 = m_ErosionResult(ipos[0] + 1, ipos[1]);
				HReal h01 = m_ErosionResult(ipos[0], ipos[1] + 1);
				HReal h11 = m_ErosionResult(ipos[0] + 1, ipos[1] + 1);

				HReal gradientX = (h10 - h00) * (1 - offset[1]) + (h11 - h01) * offset[1];
				HReal gradientY = (h01 - h00) * (1 - offset[0]) + (h11 - h10) * offset[0];

				return HVector2(gradientX, gradientY);
			}


		private:
			Random::RandomGenerator* m_RandomGenerator = nullptr;
			Params m_Params;
			HVector2I m_Resolution;
			Array2D<std::vector<HVector2I>> m_BrushPositions;
			Array2D<std::vector<HReal>> m_BrushWeights;

			Array2D<HReal> m_ErosionResult;
		};
	} // namespace Procedural
} // namespace MathLib