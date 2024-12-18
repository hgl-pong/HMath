#pragma once
#include <gtest/gtest.h>
#include <../src/FastNoiseLite.h>
#include <Math/Procedural/HydraulicErosion.h>

std::string erosionResultPath = "output\\";
inline void SaveResult(MathLib::Array2DF& heightMap, const char* filename)
{
    MathLib::HReal min, max;
    MathLib::ImageUtils::SaveImage(filename, heightMap, &min, &max);
    imgPerlinCounter++;
}

inline void SaveResultObj(MathLib::Array2DF& heightMap, const char* path)
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
    file << "o HeightMap\n";
    for (const auto& point : points)
    {
        file << "v " << point[0] << " " << point[1] << " " << point[2] << "\n";
    }
    for (uint32_t i = 0; i < indices.size(); i += 3)
    {
        file << "f " << indices[i] + 1 << " " << indices[i + 1] + 1 << " " << indices[i + 2] + 1 << "\n";
    }
    file.close();
}
class TestHydraulicErosion : public testing::Test
{
protected:
    void SetUp() override
    {
        FastNoiseLite noiseGenerator;
        noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        noiseGenerator.SetFractalType(FastNoiseLite::FractalType_FBm);
        noiseGenerator.SetFrequency(0.01f);
        noiseGenerator.SetSeed(1337);
        noiseGenerator.SetFractalLacunarity(2.0f);
        noiseGenerator.SetFractalOctaves(6);
        heightMap.ReSize(size);
        for (int y = 0; y < size[1]; y++)
        {
            for (int x = 0; x < size[0]; x++)
            {
                heightMap(x, y) = noiseGenerator.GetNoise((MathLib::HReal)x, (MathLib::HReal)y) * 0.5;
            }
        }
    }

    void TearDown() override
    {

    }
public:
    const MathLib::HVector2UI size = { 256, 256 };
    MathLib::Array2DF heightMap;
    
};

TEST_F(TestHydraulicErosion, TestHydraulicErosion)
{
    MathLib::Procedural::HydraulicErosion::Params params;
    MathLib::Procedural::HydraulicErosion erosion(heightMap, params);
    erosion.Erode(100000);
    auto result = erosion.Result();
    SaveResult(result, (erosionResultPath + "erosionResult.png").c_str());
    SaveResultObj(result, (erosionResultPath + "erosionResult.obj").c_str());
}