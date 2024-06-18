#pragma once
#include <Math/HGraphicUtils>
#include <gtest/gtest.h>
class PerlinNoiseTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 初始化 NoiseParams
        params.oxtavesNumbers = 4; 
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

// 测试 PerlinNoise::Get(const HVector3&)
TEST_F(PerlinNoiseTest, Get3D) {
    MathLib::HVector3 vec3(0.5f, 0.5f, 0.5f); // 示例输入
    MathLib::HReal result = perlinNoise->Get(vec3);
    MathLib::HReal expected_value = 0.0f; // 假设期望值
    MathLib::HReal tolerance = 1.0f;      // 设置容差
    EXPECT_NEAR(result, expected_value, tolerance);
}
