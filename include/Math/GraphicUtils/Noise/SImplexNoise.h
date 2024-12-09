#pragma once
#include "Math/Array2D.h"
#include <Math/Math.h>
#include <Math/MathUtils.h>

namespace MathLib
{
    namespace NoiseTool
    {

        class SimplexNoise
        {
        public:
            struct NoiseParams
            {
                uint32_t octavesNumbers;
                HReal frequency;
                HReal amplitude;
                uint32_t seed;
            };

        private:
            static const int X_NOISE_GEN = 1619;
            static const int Y_NOISE_GEN = 31337;
            static const int Z_NOISE_GEN = 6971;
            static const int W_NOISE_GEN = 1999;
            static const int SEED_NOISE_GEN = 1013;
            static const int SHIFT_NOISE_GEN = 8;

        public:
            SimplexNoise() {}

            SimplexNoise(NoiseParams &params) 
                : m_Octaves(params.octavesNumbers), 
                m_Amplitude(params.amplitude), 
                m_Frequency(params.frequency), 
                m_Seed(params.seed) {};

            float Get(HVector2 point)
            {
                return _Sample(point);
            }

            float Get(HVector3 point)
            {
                return _Sample(point);
            }

            void Reset(NoiseParams &params)
            {
                m_Octaves = params.octavesNumbers;
                m_Frequency = params.frequency;
                m_Amplitude = params.amplitude;
                m_Seed = params.seed;
            }

            Array2D<HReal> Get(uint32_t width, uint32_t height)
            {
                Array2D<HReal> outputArray(width, height);

                Parallel::ParallelFor<uint32_t>(0, width, 0, height, [&](uint32_t i, uint32_t j)
                                                { outputArray(i, j) = Get(HVector2(float(i) / width, float(j) / height)); });
                return outputArray;
            }

        private:
            int _FastFloor(float x)
            {
                return (x >= 0) ? (int)x : (int)(x - 1);
            }

            // 2D Simplex噪声的实现
            HVector2 _Grad2D(int hash)
            {
                // 使用查找表来获取2D梯度向量
                static const HVector2 gradients[8] = {
                    HVector2(1, 1), HVector2(-1, 1), HVector2(1, -1), HVector2(-1, -1),
                    HVector2(1, 0), HVector2(-1, 0), HVector2(0, 1), HVector2(0, -1)};
                return gradients[hash & 7];
            }

            float _Eval2D(float x, float y, int seed)
            {
                const float F2 = 0.366025403f; // (sqrt(3) - 1) / 2
                const float G2 = 0.211324865f; // (3 - sqrt(3)) / 6

                // 偏移到单形网格
                float s = (x + y) * F2;
                float xs = x + s;
                float ys = y + s;
                int i = _FastFloor(xs);
                int j = _FastFloor(ys);

                // 未偏移坐标
                float t = (i + j) * G2;
                float X0 = i - t;
                float Y0 = j - t;
                float x0 = x - X0;
                float y0 = y - Y0;

                // 确定我们在哪个三角形中
                int i1, j1;
                if (x0 > y0)
                {
                    i1 = 1;
                    j1 = 0;
                }
                else
                {
                    i1 = 0;
                    j1 = 1;
                }

                float x1 = x0 - i1 + G2;
                float y1 = y0 - j1 + G2;
                float x2 = x0 - 1.0f + 2.0f * G2;
                float y2 = y0 - 1.0f + 2.0f * G2;

                // 计算每个顶点的贡献
                float n0, n1, n2;

                float t0 = 0.5f - x0 * x0 - y0 * y0;
                if (t0 < 0.0f)
                {
                    n0 = 0.0f;
                }
                else
                {
                    t0 *= t0;
                    int gi0 = (int)((X_NOISE_GEN * i + Y_NOISE_GEN * j + SEED_NOISE_GEN * seed) & 0xffffffff);
                    gi0 ^= (gi0 >> SHIFT_NOISE_GEN);
                    HVector2 grad = _Grad2D(gi0);
                    n0 = t0 * t0 * (grad[0] * x0 + grad[1] * y0);
                }

                float t1 = 0.5f - x1 * x1 - y1 * y1;
                if (t1 < 0.0f)
                {
                    n1 = 0.0f;
                }
                else
                {
                    t1 *= t1;
                    int gi1 = (int)((X_NOISE_GEN * (i + i1) + Y_NOISE_GEN * (j + j1) + SEED_NOISE_GEN * seed) & 0xffffffff);
                    gi1 ^= (gi1 >> SHIFT_NOISE_GEN);
                    HVector2 grad = _Grad2D(gi1);
                    n1 = t1 * t1 * (grad[0] * x1 + grad[1] * y1);
                }

                float t2 = 0.5f - x2 * x2 - y2 * y2;
                if (t2 < 0.0f)
                {
                    n2 = 0.0f;
                }
                else
                {
                    t2 *= t2;
                    int gi2 = (int)((X_NOISE_GEN * (i + 1) + Y_NOISE_GEN * (j + 1) + SEED_NOISE_GEN * seed) & 0xffffffff);
                    gi2 ^= (gi2 >> SHIFT_NOISE_GEN);
                    HVector2 grad = _Grad2D(gi2);
                    n2 = t2 * t2 * (grad[0] * x2 + grad[1] * y2);
                }

                // 将结果缩放到[-1,1]范围内
                return 45.23065f * (n0 + n1 + n2);
            }

            HVector4 _Eval4D(float x, float y, float z, float w, int seed)
            {
                // The skewing and unskewing factors are hairy again for the 4D case
                const float F4 = (std::sqrt(5.0f) - 1.0f) / 4.0f;
                const float G4 = (5.0f - std::sqrt(5.0f)) / 20.0f;
                // Skew the (x,y,z,w) space to determine which cell of 24 simplices we're in
                float s = (x + y + z + w) * F4; // Factor for 4D skewing
                int ix = _FastFloor(x + s);
                int iy = _FastFloor(y + s);
                int iz = _FastFloor(z + s);
                int iw = _FastFloor(w + s);
                float tu = (ix + iy + iz + iw) * G4; // Factor for 4D unskewing
                                                     // Unskew the cell origin back to (x,y,z,w) space
                float x0 = x - (ix - tu);            // The x,y,z,w distances from the cell origin
                float y0 = y - (iy - tu);
                float z0 = z - (iz - tu);
                float w0 = w - (iw - tu);

                int c = (x0 > y0) ? (1 << 0) : (1 << 2);
                c += (x0 > z0) ? (1 << 0) : (1 << 4);
                c += (x0 > w0) ? (1 << 0) : (1 << 6);
                c += (y0 > z0) ? (1 << 2) : (1 << 4);
                c += (y0 > w0) ? (1 << 2) : (1 << 6);
                c += (z0 > w0) ? (1 << 4) : (1 << 6);

                HVector4 res;
                res.setZero();

                // Calculate the contribution from the five corners
                for (int p = 4; p >= 0; --p)
                {
                    int ixp = ((c >> 0) & 3) >= p ? 1 : 0;
                    int iyp = ((c >> 2) & 3) >= p ? 1 : 0;
                    int izp = ((c >> 4) & 3) >= p ? 1 : 0;
                    int iwp = ((c >> 6) & 3) >= p ? 1 : 0;

                    float xp = x0 - ixp + (4 - p) * G4;
                    float yp = y0 - iyp + (4 - p) * G4;
                    float zp = z0 - izp + (4 - p) * G4;
                    float wp = w0 - iwp + (4 - p) * G4;

                    float t = 0.6f - xp * xp - yp * yp - zp * zp - wp * wp;
                    if (t > 0)
                    {
                        // get index
                        int gradIndex = int((
                                                X_NOISE_GEN * (ix + ixp) + Y_NOISE_GEN * (iy + iyp) + Z_NOISE_GEN * (iz + izp) + W_NOISE_GEN * (iw + iwp) + SEED_NOISE_GEN * seed) &
                                            0xffffffff);
                        gradIndex ^= (gradIndex >> SHIFT_NOISE_GEN);
                        gradIndex &= 31;

                        HVector4 g;
                        {
                            const int h = gradIndex;
                            const int hs = 2 - (h >> 4);
                            const int h1 = (h >> 3);
                            g[0] = (h1 == 0) ? 0.0f : ((h & 4) ? -1.0f : 1.0f);
                            g[1] = (h1 == 1) ? 0.0f : ((h & (hs << 1)) ? -1.0f : 1.0f);
                            g[2] = (h1 == 2) ? 0.0f : ((h & hs) ? -1.0f : 1.0f);
                            g[3] = (h1 == 3) ? 0.0f : ((h & 1) ? -1.0f : 1.0f);
                        }
                        float gdot = (g[0] * xp + g[1] * yp + g[2] * zp + g[3] * wp);

                        float t2 = t * t;
                        float t3 = t2 * t;
                        float t4 = t3 * t;

                        float dt4gdot = 8 * t3 * gdot;

                        res[0] += t4 * g[0] - dt4gdot * xp;
                        res[1] += t4 * g[1] - dt4gdot * yp;
                        res[2] += t4 * g[2] - dt4gdot * zp;
                        res[3] += t4 * gdot;
                    }
                }
                // scale the result to cover the range [-1,1]
                res *= 27;
                return res;
            }

            float _Sample(HVector2 p)
            {
                p *= m_Frequency;
                float result = 0.0f;
                float alpha = 1.0f;

                for (int32_t i = 1; i <= m_Octaves; ++i)
                {
                    result += _Eval2D(p[0] * i, p[1] * i, m_Seed) * alpha;
                    alpha *= 0.45f;
                }
                return result * m_Amplitude;
            }
            float _Sample(HVector3 p)
            {
                p *= m_Frequency;
                float result = 0.0f;
                float alpha = 1;
                for (int32_t i = 1; i <= m_Octaves; ++i)
                {
                    result += _Eval4D(p[0] * i, p[1] * i, p[2] * i, i * 5.0f, m_Seed)[3] * alpha;
                    alpha *= 0.45f;
                }
                return result * m_Amplitude;
            }

        private:
            uint32_t m_Octaves;
            float m_Amplitude;
            float m_Frequency;
            uint32_t m_Seed;
        };
    }
}