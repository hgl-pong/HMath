#pragma once
#include "Math/Math.h"
#include <Math/HGraphicUtils>
#include <gtest/gtest.h>
#include <Math/Visual/ImageUtils.h>
#include <fstream>

uint32_t imgPerlinCounter = 0;
uint32_t imgSimplexCounter = 0;
class NoiseTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // 初始化 PerlinNoiseParams
        perlinParams.octavesNumbers = 4;
        perlinParams.frequency = 1.0f;
        perlinParams.amplitude = 1.0f;
        perlinParams.seed = 42;

        // 初始化 SimplexNoiseParams
        simplexParams.octavesNumbers = 4;
        simplexParams.frequency = 1.0f;
        simplexParams.amplitude = 1.0f;
        simplexParams.seed = 42;

        // 初始化 PerlinNoise 对象
        perlinNoise.Reset(perlinParams);
        simplexNoise.Reset(simplexParams);
    }

    void SavePerlinNoise2D()
    {
        MathLib::Array2D<MathLib::HReal> imageR = perlinNoise.Get(100, 100);

        std::string imgPath = "output\\testPerlinNoise" + std::to_string(imgPerlinCounter) + ".png";
        MathLib::HReal min, max;
        MathLib::ImageUtils::SaveImage(imgPath, imageR, &min, &max);
        imgPerlinCounter++;
    }

    void SaveSimplexNoise2D()
    {
        MathLib::Array2D<MathLib::HReal> imageR = perlinNoise.Get(100, 100);

        std::string imgPath = "output\\testSimplexNoise" + std::to_string(imgSimplexCounter) + ".png";
        MathLib::HReal min, max;
        MathLib::ImageUtils::SaveImage(imgPath, imageR, &min, &max);
        imgSimplexCounter++;
    }

    void TearDown() override
    {
    }

    MathLib::NoiseTool::PerlinNoise::NoiseParams perlinParams;
    MathLib::NoiseTool::SimplexNoise::NoiseParams simplexParams;
    MathLib::NoiseTool::PerlinNoise perlinNoise;
    MathLib::NoiseTool::SimplexNoise simplexNoise;
};

TEST_F(NoiseTest, Get2D)
{
    MathLib::HVector2 vec2(0.5f, 0.5f); // 示例输入
    MathLib::HReal result = perlinNoise.Get(vec2);
    MathLib::HReal expected_value = 0.0f; // 假设期望值
    MathLib::HReal tolerance = 1.0f;      // 设置容差
    SavePerlinNoise2D();
    // EXPECT_NEAR(result, expected_value, tolerance);
}

TEST_F(NoiseTest, Get3D)
{
    MathLib::HVector3 vec3(0.5f, 0.5f, 0.5f); // 示例输入
    MathLib::HReal result = perlinNoise.Get(vec3);
    MathLib::HReal expected_value = 0.0f; // 假设期望值
    MathLib::HReal tolerance = 1.0f;      // 设置容差
    SavePerlinNoise2D();
    // EXPECT_NEAR(result, expected_value, tolerance);
}

// 新增测试用例：测试不同频率下的2D噪声值
TEST_F(NoiseTest, Get2DWithDifferentFrequency)
{
    perlinParams.frequency = 2.0f;
    perlinNoise.Reset(perlinParams);

    MathLib::HVector2 vec2(0.5f, 0.5f);
    MathLib::HReal result = perlinNoise.Get(vec2);
    MathLib::HReal expected_value = 0.0f; // 假设期望值
    MathLib::HReal tolerance = 1.0f;      // 设置容差
    SavePerlinNoise2D();
    // EXPECT_NEAR(result, expected_value, tolerance);
}

// 新增测试用例：测试不同振幅下的2D噪声值
TEST_F(NoiseTest, Get2DWithDifferentAmplitude)
{
    perlinParams.amplitude = 2.0f;
    perlinNoise.Reset(perlinParams);

    MathLib::HVector2 vec2(0.5f, 0.5f);
    MathLib::HReal result = perlinNoise.Get(vec2);
    MathLib::HReal expected_value = 1.0f; // 假设期望值
    MathLib::HReal tolerance = 1.0f;      // 设置容差
    SavePerlinNoise2D();
    // EXPECT_NEAR(result, expected_value, tolerance);
}

// 新增测试用例：测试不同种子下的2D噪声值
TEST_F(NoiseTest, Get2DWithDifferentSeed)
{
    perlinParams.seed = 12345;
    perlinNoise.Reset(perlinParams);

    MathLib::HVector2 vec2(0.5f, 0.5f);
    MathLib::HReal result = perlinNoise.Get(vec2);
    MathLib::HReal expected_value = 0.0f; // 假设期望值
    MathLib::HReal tolerance = 1.0f;      // 设置容差
    SavePerlinNoise2D();
    // EXPECT_NEAR(result, expected_value, tolerance);
}

// 新增测试用例：测试不同频率下的3D噪声值
TEST_F(NoiseTest, Get3DWithDifferentFrequency)
{
    perlinParams.frequency = 2.0f;
    perlinNoise.Reset(perlinParams);

    MathLib::HVector3 vec3(0.5f, 0.5f, 0.5f);
    MathLib::HReal result = perlinNoise.Get(vec3);
    MathLib::HReal expected_value = 0.0f; // 假设期望值
    MathLib::HReal tolerance = 1.0f;      // 设置容差
    SavePerlinNoise2D();
    // EXPECT_NEAR(result, expected_value, tolerance);
}

// 新增测试用例：测试不同振幅下的3D噪声值
TEST_F(NoiseTest, Get3DWithDifferentAmplitude)
{
    perlinParams.amplitude = 2.0f;
    perlinNoise.Reset(perlinParams);

    MathLib::HVector3 vec3(0.5f, 0.5f, 0.5f);
    MathLib::HReal result = perlinNoise.Get(vec3);
    MathLib::HReal expected_value = 1.0f; // 假设期望值
    MathLib::HReal tolerance = 2.0f;      // 设置容差
    SavePerlinNoise2D();
    // EXPECT_NEAR(result, expected_value, tolerance);
}

// 新增测试用例：测试不同种子下的3D噪声值
TEST_F(NoiseTest, Get3DWithDifferentSeed)
{
    perlinParams.seed = 12345;
    perlinNoise.Reset(perlinParams);

    MathLib::HVector3 vec3(0.5f, 0.5f, 0.5f);
    MathLib::HReal result = perlinNoise.Get(vec3);
    MathLib::HReal expected_value = 0.0f; // 假设期望值
    MathLib::HReal tolerance = 1.0f;      // 设置容差
    SavePerlinNoise2D();
    // EXPECT_NEAR(result, expected_value, tolerance);
}

TEST_F(NoiseTest, Get2DWithDifferentFrequencyAndAmplitude)
{
    perlinParams.frequency = 1.0f;
    perlinParams.amplitude = 1.0f;
    perlinParams.octavesNumbers = 4;
    perlinNoise.Reset(perlinParams);

    MathLib::Array2D<float> noiseMap = perlinNoise.Get(100, 100);

    std::vector<MathLib::HVector3> points;
    std::vector<uint32_t> indices;
    for (int x = 0; x < 100; ++x)
    {
        for (int y = 0; y < 100; ++y)
        {
            MathLib::HVector3 point(x * 0.01f, noiseMap(x, y), y * 0.01f);
            points.push_back(point);
        }
    }
    for (int x = 0; x < 99; ++x)
    {
        for (int y = 0; y < 99; ++y)
        {
            indices.push_back(x * 100 + y);
            indices.push_back((x + 1) * 100 + y);
            indices.push_back(x * 100 + (y + 1));
            indices.push_back((x + 1) * 100 + y);
            indices.push_back((x + 1) * 100 + (y + 1));
            indices.push_back(x * 100 + (y + 1));
        }
    }

    std::ofstream file("output\\noise_map.obj");
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

TEST_F(NoiseTest, SimplexGet2DWithDifferentFrequencyAndAmplitude)
{
    simplexParams.frequency = 1.0f;
    simplexParams.amplitude = 0.5f;
    simplexParams.octavesNumbers = 8;
    simplexNoise.Reset(simplexParams);

    MathLib::Array2D<float> noiseMap = simplexNoise.Get(100, 100);

    std::vector<MathLib::HVector3> points;
    std::vector<uint32_t> indices;
    for (int x = 0; x < 100; ++x)
    {
        for (int y = 0; y < 100; ++y)
        {
            MathLib::HVector3 point(x * 0.01f, noiseMap(x, y), y * 0.01f);
            points.push_back(point);
        }
    }
    for (int x = 0; x < 99; ++x)
    {
        for (int y = 0; y < 99; ++y)
        {
            indices.push_back(x * 100 + y);
            indices.push_back((x + 1) * 100 + y);
            indices.push_back(x * 100 + (y + 1));
            indices.push_back((x + 1) * 100 + y);
            indices.push_back((x + 1) * 100 + (y + 1));
            indices.push_back(x * 100 + (y + 1));
        }
    }

    std::ofstream file("output\\simplex_noise_map.obj");
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

TEST_F(NoiseTest, SimplexGet3DWithDifferentSeed)
{
    simplexParams.seed = 12345;
    simplexNoise.Reset(simplexParams);

    MathLib::HVector3 vec3(0.5f, 0.5f, 0.5f);
    MathLib::HReal result = perlinNoise.Get(vec3);
    MathLib::HReal expected_value = 0.0f; // 假设期望值
    MathLib::HReal tolerance = 1.0f;      // 设置容差
    SaveSimplexNoise2D();
    // EXPECT_NEAR(result, expected_value, tolerance);
}