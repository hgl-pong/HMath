#pragma once
#include <Math/Math.h>
#include <Math/Core/Random.h>
#include <Math/Array2D.h>
#include <Math/MathUtils.h>

namespace MathLib
{
	namespace Procedural
	{
		class Erosion
		{
		public:
			Erosion() = delete;
			Erosion(const Array2D<HReal>& heightMap, const int32_t erosionRadius, Random::RandomGenerator* randomGenerator = nullptr)
			{
				m_ErosionResult = heightMap;
				m_RandomGenerator = randomGenerator;
				if (m_RandomGenerator == nullptr)
					m_RandomGenerator = &Random::g_DefaultRandomGenerator;
				m_Resolution = HVector2I((int32_t)heightMap.GetSizeX(), (int32_t)heightMap.GetSizeY());
				_InitBrush(erosionRadius);
			}

			virtual void Erode(const uint32_t numIterations)
			{
			}

			Array2D<HReal> Result() const
			{
				return m_ErosionResult;
			}

		protected:
			void _InitBrush(const int32_t erosionRadius)
			{
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
						m_BrushPositions(x, y).resize(numEntries);
						m_BrushWeights(x, y).resize(numEntries);

						for (uint32_t i = 0; i < numEntries; i++)
						{
							m_BrushPositions(x, y)[i] = HVector2I(x + xOffset[i], y + yOffset[i]);
							m_BrushWeights(x, y)[i] = weights[i] / weightSum;
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


		protected:
			Random::RandomGenerator* m_RandomGenerator = nullptr;
			HVector2I m_Resolution;
			Array2D<std::vector<HVector2I>> m_BrushPositions;
			Array2D<std::vector<HReal>> m_BrushWeights;

			Array2D<HReal> m_ErosionResult;
		};
	} // namespace Procedural
} // namespace MathLib