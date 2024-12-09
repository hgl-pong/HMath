#pragma once
#include "Math/Array2D.h"
#include <Math/Math.h>
#include <Math/MathUtils.h>

namespace MathLib
{
	namespace NoiseTool
	{
		class PerlinNoise
		{
		public:
			struct NoiseParams
			{
				uint32_t octavesNumbers;
				HReal frequency;
				HReal amplitude;
				uint32_t seed;
			};

		public:
			PerlinNoise(NoiseParams &params, const uint32_t sampleSize = 512)
			{
				m_Octaves = params.octavesNumbers;
				m_Frequency = params.frequency;
				m_Amplitude = params.amplitude;
				m_Seed = params.seed;
				m_SampleSize = sampleSize;

				m_Gradient1.resize(m_SampleSize + m_SampleSize + 2);
				m_Gradient2.resize(m_SampleSize + m_SampleSize + 2);
				m_Gradient3.resize(m_SampleSize + m_SampleSize + 2);
				m_Permutation.resize(m_SampleSize + m_SampleSize + 2);
				m_Start = false;
			}
			PerlinNoise(const uint32_t sampleSize = 512)
			{
				m_Octaves = 1;
				m_Frequency = 1.0f;
				m_Amplitude = 1.0f;
				m_Seed = 0;
				m_SampleSize = sampleSize;

				m_Gradient1.resize(m_SampleSize + m_SampleSize + 2);
				m_Gradient2.resize(m_SampleSize + m_SampleSize + 2);
				m_Gradient3.resize(m_SampleSize + m_SampleSize + 2);
				m_Permutation.resize(m_SampleSize + m_SampleSize + 2);
				m_Start = false;
			}
			~PerlinNoise() = default;

			void Reset(NoiseParams &params)
			{
				m_Octaves = params.octavesNumbers;
				m_Frequency = params.frequency;
				m_Amplitude = params.amplitude;
				m_Seed = params.seed;
				_Init();
				m_Start = true;
			}

			HReal Get(const HVector2 &v) { return _PerlinNoise2D(v); }
			HReal Get(const HVector3 &v) { return _PerlinNoise3D(v); }

			Array2D<HReal> Get(uint32_t width, uint32_t height)
			{
				Array2D<HReal> outputArray(width, height);

				Parallel::ParallelFor<uint32_t>(0, width, 0, height, [&](uint32_t i, uint32_t j)
												{ outputArray(i, j) = Get(HVector2(float(i) / width, float(j) / height)); });
				return outputArray;
			}

		private:
			void _Init()
			{
				uint32_t i = 0, j = 0;
				int k;
				Parallel::ParallelFor<uint32_t>(0, m_SampleSize, [&](uint32_t index)
												{
						m_Permutation[i] = index;
						m_Gradient1[i] = _RandomFloat();
						m_Gradient2[i] = HVector2(_RandomFloat(), _RandomFloat()).normalized();
						m_Gradient3[i] = HVector3(_RandomFloat(), _RandomFloat(), _RandomFloat()).normalized();
						i++; });

				while (--i)
				{
					k = m_Permutation[i];
					j = _RandomFloat() * m_SampleSize;
					m_Permutation[i] = m_Permutation[j];
					m_Permutation[j] = k;
				}

				Parallel::ParallelFor<uint32_t>(0, m_SampleSize + 2, [&](uint32_t index)
												{
						m_Permutation[m_SampleSize + index] = m_Permutation[index];
						m_Gradient1[m_SampleSize + index] = m_Gradient1[index];
						m_Gradient2[m_SampleSize + index] = m_Gradient2[index];
						m_Gradient3[m_SampleSize + index] = m_Gradient3[index]; });
			}

			HReal _PerlinNoise3D(const HVector3 &v)
			{
				if (!m_Start)
				{
					_Init();
					m_Start = true;
				}
				uint32_t octaves = m_Octaves;
				HReal freq = m_Frequency;
				HReal result = 0.0f;
				HReal amp = m_Amplitude;

				HVector3 p = v * freq;
				for (uint32_t i = 0; i < octaves; i++)
				{
					result += _Noise3(p) * amp;
					p *= 2.0f;
					amp *= 0.5f;
				}
				return result;
			}

			HReal _PerlinNoise2D(const HVector2 &v)
			{
				if (!m_Start)
				{
					_Init();
					m_Start = true;
				}
				uint32_t octaves = m_Octaves;
				HReal freq = m_Frequency;
				HReal result = 0.0f;
				HReal amp = m_Amplitude;

				HVector2 p = v * freq;
				for (uint32_t i = 0; i < octaves; i++)
				{
					result += _Noise2(p) * amp;
					p *= 2.0f;
					amp *= 0.5f;
				}
				return result;
			}

			HReal _Noise1(HReal arg)
			{
				int32_t bx0, bx1;
				HReal rx0, rx1, t, u, v;
				uint32_t i, j;

				_Setup(arg, t, bx0, bx1, rx0, rx1);
				t = _Fade(rx0);

				i = m_Permutation[bx0];
				j = m_Permutation[bx1];

				u = rx0 * m_Gradient1[i];
				v = rx1 * m_Gradient1[j];
				return Lerp(u, v, t);
			}

			HReal _Noise2(const HVector2 &vec)
			{
				int32_t bx0, bx1, by0, by1, b00, b10, b01, b11;
				HReal rx0, rx1, ry0, ry1, sy, a, b, t, u, v;
				HVector2 q;
				uint32_t i, j;

				_Setup(vec[0], t, bx0, bx1, rx0, rx1);
				_Setup(vec[1], t, by0, by1, ry0, ry1);

				i = m_Permutation[bx0];
				j = m_Permutation[bx1];

				b00 = m_Permutation[i + by0];
				b10 = m_Permutation[j + by0];
				b01 = m_Permutation[i + by1];
				b11 = m_Permutation[j + by1];

				t = _Fade(rx0);
				sy = _Fade(ry0);

				q = m_Gradient2[b00];
				u = _At(rx0, ry0, q);
				q = m_Gradient2[b10];
				v = _At(rx1, ry0, q);
				a = Lerp(u, v, t);

				q = m_Gradient2[b01];
				u = _At(rx0, ry1, q);
				q = m_Gradient2[b11];
				v = _At(rx1, ry1, q);
				b = Lerp(u, v, t);

				return Lerp(a, b, sy);
			}

			HReal _Noise3(const HVector3 &vec)
			{
				int32_t bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
				HReal rx0, rx1, ry0, ry1, rz0, rz1, sy, sz, a, b, c, d, t, u, v;
				HVector3 q;
				uint32_t i, j;

				_Setup(vec[0], t, bx0, bx1, rx0, rx1);
				_Setup(vec[1], t, by0, by1, ry0, ry1);
				_Setup(vec[2], t, bz0, bz1, rz0, rz1);

				i = m_Permutation[bx0];
				j = m_Permutation[bx1];

				b00 = m_Permutation[i + by0];
				b10 = m_Permutation[j + by0];
				b01 = m_Permutation[i + by1];
				b11 = m_Permutation[j + by1];

				t = _Fade(rx0);
				sy = _Fade(ry0);
				sz = _Fade(rz0);

				q = m_Gradient3[b00 + bz0];
				u = _At(rx0, ry0, rz0, q);
				q = m_Gradient3[b10 + bz0];
				v = _At(rx1, ry0, rz0, q);
				a = Lerp(u, v, t);

				q = m_Gradient3[b01 + bz0];
				u = _At(rx0, ry1, rz0, q);
				q = m_Gradient3[b11 + bz0];
				v = _At(rx1, ry1, rz0, q);
				b = Lerp(u, v, t);

				c = Lerp(a, b, sy);

				q = m_Gradient3[b00 + bz1];
				u = _At(rx0, ry0, rz1, q);
				q = m_Gradient3[b10 + bz1];
				v = _At(rx1, ry0, rz1, q);
				a = Lerp(u, v, t);

				q = m_Gradient3[b01 + bz1];
				u = _At(rx0, ry1, rz1, q);
				q = m_Gradient3[b11 + bz1];
				v = _At(rx1, ry1, rz1, q);
				b = Lerp(u, v, t);

				d = Lerp(a, b, sy);

				return Lerp(c, d, sz);
			}

			/// @brief generate random float in range [0, 1]
			/// @return
			HReal _RandomFloat()
			{
				return static_cast<HReal>(rand()) / static_cast<HReal>(RAND_MAX);
			}

			HReal _Fade(HReal t)
			{
				return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
			}

			float _At(const float &x, const float &y, const float &z, const HVector3 &vec)
			{
				return x * vec[0] + y * vec[1] + z * vec[2];
			}

			float _At(const float &x, const float &y, const HVector2 &vec)
			{
				return x * vec[0] + y * vec[1];
			}

			void _Setup(float v, float &t, int &b0, int &b1, float &r0, float &r1)
			{
				t = v + (0x1000);
				b0 = ((int)t) & (m_SampleSize - 1);
				b1 = (b0 + 1) & (m_SampleSize - 1);
				r0 = t - (int)t;
				r1 = r0 - 1.0f;
			}

		private:
			uint32_t m_Octaves;
			HReal m_Frequency;
			HReal m_Amplitude;
			uint32_t m_Seed;
			uint32_t m_SampleSize;

			std::vector<uint32_t> m_Permutation;
			std::vector<HVector3> m_Gradient3;
			std::vector<HVector2> m_Gradient2;
			std::vector<HReal> m_Gradient1;
			bool m_Start;
		};

	} // namespace NoiseTool
} // namespace MathLib