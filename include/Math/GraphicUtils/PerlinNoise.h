#pragma once
#include <Math/Math.h>
#include <Math/MathUtils.h>

namespace MathLib
{
	namespace NoiseTool
	{
		struct NoiseParams
		{
			uint32_t octavesNumbers;
			HReal frequency;
			HReal amplitude;
			uint32_t seed;
		};

		class PerlinNoise
		{		
		private:
			static const uint32_t SAMPLE_SIZE = 1024;
			static const uint32_t B = SAMPLE_SIZE;
			static const uint32_t BM = SAMPLE_SIZE - 1;

			static const uint32_t N = 0x1000;
			static const uint32_t NP = 12;  /* 2^N */
			static const uint32_t NM = 0xfff;

		public:
			PerlinNoise(NoiseParams& params)
			{
				m_Octaves = params.octavesNumbers;
				m_Frequency = params.frequency;
				m_Amplitude = params.amplitude;
				m_Seed = params.seed;
				m_Gradient1.resize(SAMPLE_SIZE+SAMPLE_SIZE+2);
				m_Gradient2.resize(SAMPLE_SIZE+SAMPLE_SIZE+2);
				m_Gradient3.resize(SAMPLE_SIZE+SAMPLE_SIZE+2);
				m_Permutation.resize(SAMPLE_SIZE+SAMPLE_SIZE+2);
				m_Start = false;
			}
			~PerlinNoise() = default;

			HReal Get(const HVector2& v) { return _PerlinNoise2D(v); }
			HReal Get(const HVector3& v) { return _PerlinNoise3D(v); }
		private:
			void _Init()
			{
				uint32_t i, j, k;
				for (i = 0; i < SAMPLE_SIZE; i++)
				{
					m_Permutation[i] = i;
					m_Gradient1[i] = _RandomFloat();
					m_Gradient2[i] = HVector2(_RandomFloat(), _RandomFloat()).normalized();
					m_Gradient3[i] = HVector3(_RandomFloat(), _RandomFloat(), _RandomFloat()).normalized();
				}

				while (--i)
				{
					k = m_Permutation[i];
					m_Permutation[i] = m_Permutation[j = rand() % SAMPLE_SIZE];
					m_Permutation[j] = k;
				}

				for (i = 0; i < SAMPLE_SIZE; i++)
				{
					m_Permutation[SAMPLE_SIZE + i] = m_Permutation[i];
					m_Gradient1[SAMPLE_SIZE + i] = m_Gradient1[i];
					m_Gradient2[SAMPLE_SIZE + i] = m_Gradient2[i];
					m_Gradient3[SAMPLE_SIZE + i] = m_Gradient3[i];
				}
			}
			HReal _PerlinNoise3D(const HVector3& v)
			{
				uint32_t terms = m_Octaves;
				HReal freq = m_Frequency;
				HReal result = 0.0f;
				HReal amp = m_Amplitude;

				HVector3 p = v * freq;
				for (uint32_t i = 0; i < terms; i++)
				{
					result += _Noise3(p) * amp;
					p *= 2.0f;
					amp *= 0.5f;
				}
				return result;
			}

			HReal _PerlinNoise2D(const HVector2& v)
			{
				uint32_t terms = m_Octaves;
				HReal freq = m_Frequency;
				HReal result = 0.0f;
				HReal amp = m_Amplitude;

				HVector2 p = v * freq;
				for (uint32_t i = 0; i < terms; i++)
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
				HReal rx0, rx1, sx, t, u, v;

				t = arg + N;
				bx0 = ((int32_t)t) & BM;
				bx1 = (bx0 + 1) & BM;
				rx0 = t - (int32_t)t;
				rx1 = rx0 - 1.0f;

				sx = _Curve(rx0);

				u = rx0 * m_Gradient1[m_Permutation[bx0]];
				v = rx1 * m_Gradient1[m_Permutation[bx1]];

				return 2.0f * Lerp(sx, u, v);
			}

			HReal _Noise2(const HVector2& vec)
			{
				int32_t bx0, bx1, by0, by1, b00, b10, b01, b11;
				HReal rx0, rx1, ry0, ry1, sx, sy, a, b, t, u, v;
				HVector2 q;
				uint32_t i, j;

				if (!m_Start)
				{
					_Init();
					m_Start = true;
				}

				t = vec[0] + N;
				bx0 = ((int32_t)t) & BM;
				bx1 = (bx0 + 1) & BM;
				rx0 = t - (int32_t)t;
				rx1 = rx0 - 1.0f;

				t = vec[1] + N;
				by0 = ((int32_t)t) & BM;
				by1 = (by0 + 1) & BM;
				ry0 = t - (int32_t)t;
				ry1 = ry0 - 1.0f;

				i = m_Permutation[bx0];
				j = m_Permutation[bx1];

				b00 = m_Permutation[i + by0];
				b10 = m_Permutation[j + by0];
				b01 = m_Permutation[i + by1];
				b11 = m_Permutation[j + by1];

				sx = _Curve(rx0);
				sy = _Curve(ry0);

				q = m_Gradient2[b00];
				u = q.dot(HVector2(rx0, ry0));
				q = m_Gradient2[b10];
				v = q.dot(HVector2(rx1, ry0));
				a = Lerp(sx, u, v);

				q = m_Gradient2[b01];
				u = q.dot(HVector2(rx0, ry1));
				q = m_Gradient2[b11];
				v = q.dot(HVector2(rx1, ry1));
				b = Lerp(sx, u, v);

				return 2.0f * Lerp(sy, a, b);
			}
			HReal _Noise3(const HVector3& vec)
			{
				int32_t bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
				HReal rx0, rx1, ry0, ry1, rz0, rz1, sx, sy, sz, a, b, c, d, t, u, v;
				HVector3 q;
				uint32_t i, j;

				if (!m_Start)
				{
					_Init();
					m_Start = true;
				}

				t = vec[0] + N;
				bx0 = ((int32_t)t) & BM;
				bx1 = (bx0 + 1) & BM;
				rx0 = t - (int32_t)t;
				rx1 = rx0 - 1.0f;

				t = vec[1] + N;
				by0 = ((int32_t)t) & BM;
				by1 = (by0 + 1) & BM;
				ry0 = t - (int32_t)t;
				ry1 = ry0 - 1.0f;

				t = vec[2] + N;
				bz0 = ((int32_t)t) & BM;
				bz1 = (bz0 + 1) & BM;
				rz0 = t - (int32_t)t;
				rz1 = rz0 - 1.0f;

				i = m_Permutation[bx0];
				j = m_Permutation[bx1];

				b00 = m_Permutation[i + by0];
				b10 = m_Permutation[j + by0];
				b01 = m_Permutation[i + by1];
				b11 = m_Permutation[j + by1];

				t = _Curve(rx0);
				sx = _Curve(ry0);
				sy = _Curve(rz0);

				q = m_Gradient3[b00 + bz0];
				u = q.dot(HVector3(rx0, ry0, rz0));
				q = m_Gradient3[b10 + bz0];
				v = q.dot(HVector3(rx1, ry0, rz0));
				a = Lerp(t, u, v);

				q = m_Gradient3[b01 + bz0];
				u = q.dot(HVector3(rx0, ry1, rz0));
				q = m_Gradient3[b11 + bz0];
				v = q.dot(HVector3(rx1, ry1, rz0));
				b = Lerp(t, u, v);

				q = m_Gradient3[b00 + bz1];
				u = q.dot(HVector3(rx0, ry0, rz1));
				q = m_Gradient3[b10 + bz1];
				v = q.dot(HVector3(rx1, ry0, rz1));
				c = Lerp(t, u, v);

				q = m_Gradient3[b01 + bz1];
				u = q.dot(HVector3(rx0, ry1, rz1));
				q = m_Gradient3[b11 + bz1];
				v = q.dot(HVector3(rx1, ry1, rz1));
				d = Lerp(t, u, v);

				return 1.5f * Lerp(sy, Lerp(sx, a, b), Lerp(sx, c, d));
			}

			HReal _RandomFloat()
			{
				return (HReal)((rand() % (B + B)) - B) / B;
			}

			HReal _Curve(HReal t)
			{
				return t * t * (3.0f - 2.0f * t);
			}
		private:
			uint32_t m_Octaves;
			HReal m_Frequency;
			HReal m_Amplitude;
			uint32_t m_Seed;

			std::vector<uint32_t> m_Permutation;
			std::vector<HVector3> m_Gradient3;
			std::vector<HVector2> m_Gradient2;
			std::vector<HReal> m_Gradient1;
			bool m_Start;
		};

	}//namespace NoiseTool
} //namespace MathLib