#pragma once
#include <Math/HGraphicUtils>
#include <gtest/gtest.h>
class PerlinNoiseTest : public ::testing::Test {
protected:
    void SetUp() override {
        // ��ʼ�� NoiseParams
        params.octavesNumbers = 4;
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

TEST_F(PerlinNoiseTest, Get3D) {
    MathLib::HVector3 vec3(0.5f, 0.5f, 0.5f); // ʾ������
    MathLib::HReal result = perlinNoise->Get(vec3);
    MathLib::HReal expected_value = 0.0f; // ��������ֵ
    MathLib::HReal tolerance = 1.0f;      // �����ݲ�
    EXPECT_NEAR(result, expected_value, tolerance);
}

// �����������������Բ�ͬƵ���µ�2D����ֵ
TEST_F(PerlinNoiseTest, Get2DWithDifferentFrequency) {
    params.frequency = 2.0f;
    delete perlinNoise;
    perlinNoise = new MathLib::NoiseTool::PerlinNoise(params);

    MathLib::HVector2 vec2(0.5f, 0.5f);
    MathLib::HReal result = perlinNoise->Get(vec2);
    MathLib::HReal expected_value = 0.0f; // ��������ֵ
    MathLib::HReal tolerance = 1.0f;      // �����ݲ�
    EXPECT_NEAR(result, expected_value, tolerance);
}

// �����������������Բ�ͬ����µ�2D����ֵ
TEST_F(PerlinNoiseTest, Get2DWithDifferentAmplitude) {
    params.amplitude = 2.0f;
    delete perlinNoise;
    perlinNoise = new MathLib::NoiseTool::PerlinNoise(params);

    MathLib::HVector2 vec2(0.5f, 0.5f);
    MathLib::HReal result = perlinNoise->Get(vec2);
    MathLib::HReal expected_value = 1.0f; // ��������ֵ
    MathLib::HReal tolerance = 1.0f;      // �����ݲ�
    EXPECT_NEAR(result, expected_value, tolerance);
}

// �����������������Բ�ͬ�����µ�2D����ֵ
TEST_F(PerlinNoiseTest, Get2DWithDifferentSeed) {
    params.seed = 12345;
    delete perlinNoise;
    perlinNoise = new MathLib::NoiseTool::PerlinNoise(params);

    MathLib::HVector2 vec2(0.5f, 0.5f);
    MathLib::HReal result = perlinNoise->Get(vec2);
    MathLib::HReal expected_value = 0.0f; // ��������ֵ
    MathLib::HReal tolerance = 1.0f;      // �����ݲ�
    EXPECT_NEAR(result, expected_value, tolerance);
}

// �����������������Բ�ͬƵ���µ�3D����ֵ
TEST_F(PerlinNoiseTest, Get3DWithDifferentFrequency) {
    params.frequency = 2.0f;
    delete perlinNoise;
    perlinNoise = new MathLib::NoiseTool::PerlinNoise(params);

    MathLib::HVector3 vec3(0.5f, 0.5f, 0.5f);
    MathLib::HReal result = perlinNoise->Get(vec3);
    MathLib::HReal expected_value = 0.0f; // ��������ֵ
    MathLib::HReal tolerance = 1.0f;      // �����ݲ�
    EXPECT_NEAR(result, expected_value, tolerance);
}

// �����������������Բ�ͬ����µ�3D����ֵ
TEST_F(PerlinNoiseTest, Get3DWithDifferentAmplitude) {
    params.amplitude = 2.0f;
    delete perlinNoise;
    perlinNoise = new MathLib::NoiseTool::PerlinNoise(params);

    MathLib::HVector3 vec3(0.5f, 0.5f, 0.5f);
    MathLib::HReal result = perlinNoise->Get(vec3);
    MathLib::HReal expected_value = 1.0f; // ��������ֵ
    MathLib::HReal tolerance = 1.0f;      // �����ݲ�
    EXPECT_NEAR(result, expected_value, tolerance);
}

// �����������������Բ�ͬ�����µ�3D����ֵ
TEST_F(PerlinNoiseTest, Get3DWithDifferentSeed) {
    params.seed = 12345;
    delete perlinNoise;
    perlinNoise = new MathLib::NoiseTool::PerlinNoise(params);

    MathLib::HVector3 vec3(0.5f, 0.5f, 0.5f);
    MathLib::HReal result = perlinNoise->Get(vec3);
    MathLib::HReal expected_value = 0.0f; // ��������ֵ
    MathLib::HReal tolerance = 1.0f;      // �����ݲ�
    EXPECT_NEAR(result, expected_value, tolerance);
}
