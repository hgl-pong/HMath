#include "FastNoiseLite.h"
#include <Math/Math.h>
#include <Math/Array2D.h>
#include <Math/Visual/ImageUtils.h>
#include <Math/Parallel.h>
#include <fstream>
#include <Math/Procedural/HydraulicErosion.h>

#define SAVE_IMAGE 1
#define SAVE_MESH 1
#define PARTICLE_BASE 1

using namespace MathLib;

const HReal cellSize = 1.f;
const HVector2I size(256, 256);
int SEED = 10;
std::chrono::milliseconds tickLength = std::chrono::milliseconds(1000);
bool updated = false;

const HReal scale = 6.0;

bool active = false;
int remaining = 50000;
uint32_t erosionstep = 10000;

const HReal dt = 1.2;
const HReal density = 1.0;
const HReal evapRate = 0.001;
const HReal depositionRate = 0.1;
const HReal minVol = 0.01;
const HReal friction = 0.05;

const HReal Kc = 0.05;
const HReal Kd = 0.036;
const HReal Ks = 0.006;

const HReal rainingRate = 0.1f;
const HReal rainingAmount = 0.1f;

#if PARTICLE_BASE

struct Particle
{
    Particle(HVector2 pos) { m_Pos = pos; }

    HVector2 m_Pos;
    HVector2 m_Speed = HVector2(0.f, 0.f);

    HReal m_Volume = 1.0;
    HReal m_Sediment = 0.0;
};

HVector3 GetNormal(const Array2D<HReal> &heightMap, int x, int y)
{
    HReal heightL = heightMap(x - 1, y);
    HReal heightR = heightMap(x + 1, y);
    HReal heightU = heightMap(x, y - 1);
    HReal heightD = heightMap(x, y + 1);
    HReal normalX = heightR - heightL;
    HReal normalY = heightD - heightU;
    HVector3 normal(normalX, 0.5f * cellSize, normalY);
    normal.norm();
    return normal;
}

void Erode(Array2D<HReal> &heightmap, int cycles)
{
    for (int i = 0; i < cycles; i++)
    {
        HVector2 newpos = HVector2(rand() % (int)size[0], rand() % (int)size[1]);
        Particle drop(newpos);

        while (drop.m_Volume > minVol)
        {

            HVector2I ipos = HVector2I(drop.m_Pos[0], drop.m_Pos[1]); // Floored Droplet Initial Position
            HVector3 n = GetNormal(heightmap, ipos[0], ipos[1]);                   // Surface Normal at Position
            drop.m_Speed += dt * HVector2(n[0], n[2]) * scale / (drop.m_Volume * density);
            drop.m_Pos += dt * drop.m_Speed;
            drop.m_Speed *= (1.0 - dt * friction); // Friction Factor
            if (drop.m_Pos[0] < 0 || drop.m_Pos[0] >= size[0] || drop.m_Pos[1] < 0 || drop.m_Pos[1] >= size[1])
                break;
            HReal maxsediment = drop.m_Volume * (drop.m_Speed.norm()) * (heightmap(ipos[0], ipos[1]) - heightmap((int)drop.m_Pos[0], (int)drop.m_Pos[1]));
            if (maxsediment < 0.0)
                maxsediment = 0.0;
            HReal sdiff = maxsediment - drop.m_Sediment;
            drop.m_Sediment += dt * depositionRate * sdiff;
            heightmap(ipos[0], ipos[1]) -= dt * drop.m_Volume * depositionRate * sdiff;
            drop.m_Volume *= (1.0 - dt * evapRate);
        }
    }
}

#else
//HVector3 GetNormal(const Array2D<HReal> &heightMap, int x, int y)
//{
//    HReal heightL = heightMap(x - 1, y);
//    HReal heightR = heightMap(x + 1, y);
//    HReal heightU = heightMap(x, y - 1);
//    HReal heightD = heightMap(x, y + 1);
//    HReal normalX = heightR - heightL;
//    HReal normalY = heightD - heightU;
//    HVector3 normal(normalX, 0.5f * cellSize, normalY);
//    normal.norm();
//    return normal;
//}

HReal RandomHReal(const HReal min = 0.f, const HReal max = 1.f)
{
    return static_cast<HReal>(rand()) / RAND_MAX * (max - min) + min;
}

void Raining(Array2D<HReal> &water)
{
    Array2D<HReal>::ArrayUpdateFn fn = [&](uint32_t x, uint32_t y) -> HReal
        {
            HReal drop = 0.f;
            if (RandomHReal() < rainingRate)
                drop += rainingAmount;
            return water(x, y) + drop;
        };

    water.ExecuteUpdate(fn);
}

void FluidSim(const Array2DF& height, const Array2DF& preWater, Array2DF& water)
{
    
}

void Erosion(const Array2D<HReal>& preHeight, const Array2D<HReal>& preSediment, const Array2D2F& velocity,const Array2D<HVector3>& normal,
             const uint32_t &width, const uint32_t &height,
             Array2D<HReal> &heightMap, Array2D<HReal> &sediment)
{
    Array2DF::ArrayUpdateFn heightUpdateFn = [&](uint32_t x, uint32_t y) -> HReal
        {
            const HVector3& n = normal(x, y);
            const HReal cosAlpha = n[1];
            const HReal sinAlpha = std::sqrt(1 - cosAlpha * cosAlpha);
            const HVector2& v = velocity(x, y);
            const HReal vLenth = v.norm();
            const HReal sedimentCap = Kc * sinAlpha * vLenth;
            const HReal& preS = preSediment(x, y);
            HReal deltaSediment = std::fabsf(preS - sedimentCap);

            if( preS>sedimentCap)
            {
                return preHeight(x, y) + deltaSediment * Kd;
            }
            else
            {
                return preHeight(x, y) - deltaSediment * Ks;
            }
        };

    Array2DF::ArrayUpdateFn sedimentUpdateFn = [&](uint32_t x, uint32_t y) -> HReal
        {
            const HVector3& n = normal(x, y);
            const HReal cosAlpha = n[1];
            const HReal sinAlpha = std::sqrt(1 - cosAlpha * cosAlpha);
            const HVector2& v = velocity(x, y);
            const HReal vLenth = v.norm();
            const HReal sedimentCap = Kc * sinAlpha * vLenth;
            const HReal& preS = preSediment(x, y);
            HReal deltaSediment = std::fabsf(preS - sedimentCap);

            if (preS > sedimentCap)
            {
                return preS - deltaSediment * Kd;
            }
            else
            {
                return preS + deltaSediment * Ks;
            }
        };

    heightMap.ExecuteUpdate(heightUpdateFn);
    sediment.ExecuteUpdate(sedimentUpdateFn);
}



void Advect(const Array2D2F& velocity, const Array2DF& preSediment, Array2DF& sediment)
{
    Array2DF::ArrayUpdateFn updateFn = [&](size_t x, size_t y)-> HReal
        {
            const HVector2& v = velocity(x, y);
            const HVector2 pos((HReal)x - v[0] * dt, (HReal)y - v[1] * dt);
            const HVector2I posI((int)pos[0], (int)pos[1]);
            const HVector2I rightPosI((int)pos[0] + 1, (int)pos[1]);
            const HVector2I bottomPosI((int)pos[0], (int)pos[1] + 1);
            const HVector2I rightBottomPosI((int)pos[0] + 1, (int)pos[1] + 1);
            const HVector2 uv(pos[0] - posI[0], pos[1] - posI[1]);
            const HReal curSediment = posI[0] >= size[0] || posI[1] >= size[1] ? 0 : preSediment(posI);
            const HReal rightSediment = rightPosI[0] >= size[0] || rightPosI[1] >= size[1] ? 0 : preSediment(rightPosI);
            const HReal bottomSediment = bottomPosI[0] >= size[0] || bottomPosI[1] >= size[1] ? 0 : preSediment(bottomPosI);
            const HReal rightBottomSediment = rightBottomPosI[0] >= size[0] || rightBottomPosI[1] >= size[1] ? 0 : preSediment(rightBottomPosI);
            return BiLerp(curSediment, rightSediment, bottomSediment, rightSediment, uv[0], uv[1]);
        };
    sediment.ExecuteUpdate(updateFn);
}

void Evaporate(Array2D<HReal> &water)
{
    Array2DF::ArrayUpdateFn fn = [&](size_t x, size_t y)-> HReal
        {
            return std::max(0.f, water(x, y) - evapRate * dt);
        };
    water.ExecuteUpdate(fn);
}
#endif

Array2D<HVector3> ComputeNormals(const Array2D<HReal> &heightMap)
{
    const uint32_t &width = heightMap.GetSizeX();
    const uint32_t &height = heightMap.GetSizeY();
    Array2D<HVector3> normals(width, height);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            HReal heightL = heightMap(x - 1, y);
            HReal heightR = heightMap(x + 1, y);
            HReal heightU = heightMap(x, y - 1);
            HReal heightD = heightMap(x, y + 1);
            HReal normalX = heightR - heightL;
            HReal normalY = heightD - heightU;
            HVector3 normal(normalX, 0.5f * cellSize, normalY);
            normal.normalized();
            normals(x, y) = normal;
        }
    }
    return normals;
}

#if SAVE_IMAGE
void SaveHeightMap(const char *path, Array2D<HReal> &heightMap)
{
    HReal min, max;
    ImageUtils::SaveImage(path, heightMap, &min, &max);
}
#endif

#if SAVE_MESH

void SaveHeightMesh(const char *path, Array2D<HReal> &heightMap)
{
    std::vector<MathLib::HVector3> points;
    std::vector<uint32_t> indices;
    const uint32_t width = heightMap.GetSizeX();
    const uint32_t height = heightMap.GetSizeY();
    for (uint32_t x = 0; x < width; ++x)
    {
        for (uint32_t y = 0; y < height; ++y)
        {
            MathLib::HVector3 point(x * 0.01f, heightMap(x, y), y * 0.01f);
            points.push_back(point);
        }
    }
    for (uint32_t x = 0; x < width - 1; ++x)
    {
        for (int y = 0; y < height - 1; ++y)
        {
            indices.push_back(x * width + y);
            indices.push_back((x + 1) * width + y);
            indices.push_back(x * width + (y + 1));
            indices.push_back((x + 1) * width + y);
            indices.push_back((x + 1) * width + (y + 1));
            indices.push_back(x * width + (y + 1));
        }
    }

    std::ofstream file(path);
    file << "o NoiseMap\n";
    for (const auto &point : points)
    {
        file << "v " << point[0] << " " << point[1] << " " << point[2] << "\n";
    }
    for (uint32_t i = 0; i < indices.size(); i += 3)
    {
        file << "f " << indices[i] + 1 << " " << indices[i + 1] + 1 << " " << indices[i + 2] + 1 << "\n";
    }
    file.close();
}
#endif

void main()
{
    FastNoiseLite noiseGenerator;
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noiseGenerator.SetFractalType(FastNoiseLite::FractalType_FBm);
    noiseGenerator.SetFrequency(0.01f);
    noiseGenerator.SetSeed(1337);
    noiseGenerator.SetFractalLacunarity(2.0f);
    noiseGenerator.SetFractalOctaves(6);
    Array2D<HReal> heightMap(size[0], size[1]);
    for (int y = 0; y < size[1]; y++)
    {
        for (int x = 0; x < size[0]; x++)
        {
            heightMap(x, y) = noiseGenerator.GetNoise((HReal)x, (HReal)y) * 0.25f  + 1;
        }
    }
#if SAVE_IMAGE
    SaveHeightMap("pre.png", heightMap);
#endif

#if SAVE_MESH
    SaveHeightMesh("pre.obj", heightMap);
#endif
    //Array2D<HVector3> normals = ComputeNormals(heightMap);

#if PARTICLE_BASE
#if 0
    while (remaining > 0)
    {
        Erode(heightMap, erosionstep);
        remaining -= erosionstep;
        printf("Remaining: %d\n", remaining);
#if SAVE_IMAGE
        SaveHeightMap("eroding.png", heightMap);
#endif
    }
#else
    MathLib::Procedural::HydraulicErosion::Params params;

    MathLib::Procedural::HydraulicErosion erosion(heightMap, params);
    std::vector<MathLib::HReal> heights = heightMap.GetData();

    while (remaining > 0)
    {
        erosion.Erode(erosionstep);
        heightMap = erosion.Result();

        remaining -= erosionstep;
        printf("Remaining: %d\n", remaining);
#if SAVE_IMAGE
        SaveHeightMap("eroding.png", heightMap);
#endif
    }
#endif
#else

    Array2D<HReal> preSediment;
    Array2D<HReal> curSediment;

    Array2D<HVector2> preVelocity;
    Array2D<HVector2> curVelocity;

    Array2D<HReal> slipage;
    Array2D<HVector4> flux;

    Array2D<HReal> preWater;
    Array2D<HReal> curWater;
    
    Raining(preWater);
    

#endif

#if SAVE_IMAGE
    SaveHeightMap("eroded.png", heightMap);
#endif

#if SAVE_MESH
    SaveHeightMesh("erodedMesh.obj", heightMap);
#endif
}