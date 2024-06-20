#pragma once
#include <Math/HGraphicUtils>
#include <gtest/gtest.h>
class PerlinNoiseTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 初始化 NoiseParams
        params.octavesNumbers = 4;
        params.frequency = 1.0f;
        params.amplitude = 1.0f;
        params.seed = 42;

        // 初始化 PerlinNoise 对象
        perlinNoise = new MathLib::NoiseTool::PerlinNoise(params);
    }

    void TearDown() override {
        delete perlinNoise;
    }

    MathLib::NoiseTool::NoiseParams params;
    MathLib::NoiseTool::PerlinNoise* perlinNoise;
};

TEST_F(PerlinNoiseTest, Get2D) {
    MathLib::HVector2 vec2(0.5f, 0.5f); // 示例输入
    MathLib::HReal result = perlinNoise->Get(vec2);
    MathLib::HReal expected_value = 0.0f; // 假设期望值
    MathLib::HReal tolerance = 1.0f;      // 设置容差
    EXPECT_NEAR(result, expected_value, tolerance);
}

TEST_F(PerlinNoiseTest, Get3D) {
    MathLib::HVector3 vec3(0.5f, 0.5f, 0.5f); // 示例输入
    MathLib::HReal result = perlinNoise->Get(vec3);
    MathLib::HReal expected_value = 0.0f; // 假设期望值
    MathLib::HReal tolerance = 1.0f;      // 设置容差
    EXPECT_NEAR(result, expected_value, tolerance);
}

// 新增测试用例：测试不同频率下的2D噪声值
TEST_F(PerlinNoiseTest, Get2DWithDifferentFrequency) {
    params.frequency = 2.0f;
    delete perlinNoise;
    perlinNoise = new MathLib::NoiseTool::PerlinNoise(params);

    MathLib::HVector2 vec2(0.5f, 0.5f);
    MathLib::HReal result = perlinNoise->Get(vec2);
    MathLib::HReal expected_value = 0.0f; // 假设期望值
    MathLib::HReal tolerance = 1.0f;      // 设置容差
    EXPECT_NEAR(result, expected_value, tolerance);
}

// 新增测试用例：测试不同振幅下的2D噪声值
TEST_F(PerlinNoiseTest, Get2DWithDifferentAmplitude) {
    params.amplitude = 2.0f;
    delete perlinNoise;
    perlinNoise = new MathLib::NoiseTool::PerlinNoise(params);

    MathLib::HVector2 vec2(0.5f, 0.5f);
    MathLib::HReal result = perlinNoise->Get(vec2);
    MathLib::HReal expected_value = 1.0f; // 假设期望值
    MathLib::HReal tolerance = 1.0f;      // 设置容差
    EXPECT_NEAR(result, expected_value, tolerance);
}

// 新增测试用例：测试不同种子下的2D噪声值
TEST_F(PerlinNoiseTest, Get2DWithDifferentSeed) {
    params.seed = 12345;
    delete perlinNoise;
    perlinNoise = new MathLib::NoiseTool::PerlinNoise(params);

    MathLib::HVector2 vec2(0.5f, 0.5f);
    MathLib::HReal result = perlinNoise->Get(vec2);
    MathLib::HReal expected_value = 0.0f; // 假设期望值
    MathLib::HReal tolerance = 1.0f;      // 设置容差
    EXPECT_NEAR(result, expected_value, tolerance);
}

// 新增测试用例：测试不同频率下的3D噪声值
TEST_F(PerlinNoiseTest, Get3DWithDifferentFrequency) {
    params.frequency = 2.0f;
    delete perlinNoise;
    perlinNoise = new MathLib::NoiseTool::PerlinNoise(params);

    MathLib::HVector3 vec3(0.5f, 0.5f, 0.5f);
    MathLib::HReal result = perlinNoise->Get(vec3);
    MathLib::HReal expected_value = 0.0f; // 假设期望值
    MathLib::HReal tolerance = 1.0f;      // 设置容差
    EXPECT_NEAR(result, expected_value, tolerance);
}

// 新增测试用例：测试不同振幅下的3D噪声值
TEST_F(PerlinNoiseTest, Get3DWithDifferentAmplitude) {
    params.amplitude = 2.0f;
    delete perlinNoise;
    perlinNoise = new MathLib::NoiseTool::PerlinNoise(params);

    MathLib::HVector3 vec3(0.5f, 0.5f, 0.5f);
    MathLib::HReal result = perlinNoise->Get(vec3);
    MathLib::HReal expected_value = 1.0f; // 假设期望值
    MathLib::HReal tolerance = 1.0f;      // 设置容差
    EXPECT_NEAR(result, expected_value, tolerance);
}

// 新增测试用例：测试不同种子下的3D噪声值
TEST_F(PerlinNoiseTest, Get3DWithDifferentSeed) {
    params.seed = 12345;
    delete perlinNoise;
    perlinNoise = new MathLib::NoiseTool::PerlinNoise(params);

    MathLib::HVector3 vec3(0.5f, 0.5f, 0.5f);
    MathLib::HReal result = perlinNoise->Get(vec3);
    MathLib::HReal expected_value = 0.0f; // 假设期望值
    MathLib::HReal tolerance = 1.0f;      // 设置容差
    EXPECT_NEAR(result, expected_value, tolerance);
}
