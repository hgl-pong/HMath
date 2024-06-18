#pragma once
#include <Math/HGraphicUtils>
#include <gtest/gtest.h>
class PerlinNoiseTest : public ::testing::Test {
protected:
    void SetUp() override {
        // ��ʼ�� NoiseParams
        params.oxtavesNumbers = 4; 
        params.frequency = 1.0f;   
        params.amplitude = 1.0f;   
        params.seed = 42;          

        // ��ʼ�� PerlinNoise ����
        perlinNoise = new MathLib::NoiseTool::PerlinNoise(params);
    }

    void TearDown() override {
        delete perlinNoise;
    }

    MathLib::NoiseTool::NoiseParams params;
    MathLib::NoiseTool::PerlinNoise* perlinNoise;
};

TEST_F(PerlinNoiseTest, Get2D) {
    MathLib::HVector2 vec2(0.5f, 0.5f); // ʾ������
    MathLib::HReal result = perlinNoise->Get(vec2);
    MathLib::HReal expected_value = 0.0f; // ��������ֵ
    MathLib::HReal tolerance = 1.0f;      // �����ݲ�
    EXPECT_NEAR(result, expected_value, tolerance);
}

// ���� PerlinNoise::Get(const HVector3&)
TEST_F(PerlinNoiseTest, Get3D) {
    MathLib::HVector3 vec3(0.5f, 0.5f, 0.5f); // ʾ������
    MathLib::HReal result = perlinNoise->Get(vec3);
    MathLib::HReal expected_value = 0.0f; // ��������ֵ
    MathLib::HReal tolerance = 1.0f;      // �����ݲ�
    EXPECT_NEAR(result, expected_value, tolerance);
}
