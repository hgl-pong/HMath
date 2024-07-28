#pragma once
#include <gtest/gtest.h>
#include <Math/Visual/ImageUtils.h>

inline void CheckOutputPath()
{
    // create directory for output if not exist
    const wchar_t* filePath = L"output";
    if (_waccess(filePath, 0) == -1)
    {
        _wmkdir(filePath);
        //printf("Directory created.\n");
    }
    else
    {
        //printf("Directory already exists.\n");
    }
}

const std::string testImgPath = "..\\..\\data\\img\\testPng.png";
//const std::string testImgPath = "..\\..\\data\\img\\testPng2.png";
//const std::string testImgPath = "..\\..\\data\\img\\testJpg.jpg";
//const std::string testImgPath = "..\\..\\data\\img\\testBmp.bmp";
//const std::string testImgPath = "..\\..\\data\\img\\testTga.tga";

TEST(TestImageUtils, TestSimpleLoadAndSave)
{
    CheckOutputPath();
    MathLib::Array2D<MathLib::HReal> imageGray;
    EXPECT_TRUE(MathLib::ImageUtils::LoadImage(testImgPath, imageGray));
    EXPECT_TRUE(MathLib::ImageUtils::SaveImage("output\\testPngOutputGray.png", imageGray));
    EXPECT_TRUE(MathLib::ImageUtils::SaveImage("output\\testPngOutputGray.bmp", imageGray));
    EXPECT_TRUE(MathLib::ImageUtils::SaveImage("output\\testPngOutputGray.jpg", imageGray));
    EXPECT_TRUE(MathLib::ImageUtils::SaveImage("output\\testPngOutputGray.tga", imageGray));
}

TEST(TestImageUtils, TestArray2DVector1LoadAndSave)
{
    CheckOutputPath();
    MathLib::Array2D<MathLib::HVector<MathLib::HReal, 1>> imageGray2;
    EXPECT_TRUE(MathLib::ImageUtils::LoadImage(testImgPath, imageGray2));
    EXPECT_TRUE(MathLib::ImageUtils::SaveImage("output\\testPngOutputR.png", imageGray2));
    EXPECT_TRUE(MathLib::ImageUtils::SaveImage("output\\testPngOutputR.bmp", imageGray2));
    EXPECT_TRUE(MathLib::ImageUtils::SaveImage("output\\testPngOutputR.jpg", imageGray2));
    EXPECT_TRUE(MathLib::ImageUtils::SaveImage("output\\testPngOutputR.tga", imageGray2));
}

TEST(TestImageUtils, TestArray2DVector2LoadAndSave)
{
    CheckOutputPath();
    MathLib::Array2D<MathLib::HVector2> imageRG;
    EXPECT_TRUE(MathLib::ImageUtils::LoadImage(testImgPath, imageRG));
    EXPECT_TRUE(MathLib::ImageUtils::SaveImage("output\\testPngOutputRG.png", imageRG));
    EXPECT_TRUE(MathLib::ImageUtils::SaveImage("output\\testPngOutputRG.bmp", imageRG));
    EXPECT_TRUE(MathLib::ImageUtils::SaveImage("output\\testPngOutputRG.jpg", imageRG));
    EXPECT_TRUE(MathLib::ImageUtils::SaveImage("output\\testPngOutputRG.tga", imageRG));
}

TEST(TestImageUtils, TestArray2DVector3LoadAndSave)
{
    CheckOutputPath();
    MathLib::Array2D<MathLib::HVector3> imageRGB;
    EXPECT_TRUE(MathLib::ImageUtils::LoadImage(testImgPath, imageRGB));
    EXPECT_TRUE(MathLib::ImageUtils::SaveImage("output\\testPngOutputRGB.png", imageRGB));
    EXPECT_TRUE(MathLib::ImageUtils::SaveImage("output\\testPngOutputRGB.bmp", imageRGB));
    EXPECT_TRUE(MathLib::ImageUtils::SaveImage("output\\testPngOutputRGB.jpg", imageRGB));
    EXPECT_TRUE(MathLib::ImageUtils::SaveImage("output\\testPngOutputRGB.tga", imageRGB));
}

TEST(TestImageUtils, TestArray2DVector4LoadAndSave)
{
    CheckOutputPath();
    MathLib::Array2D<MathLib::HVector4> imageRGBA;
    EXPECT_TRUE(MathLib::ImageUtils::LoadImage(testImgPath, imageRGBA));
    EXPECT_TRUE(MathLib::ImageUtils::SaveImage("output\\testPngOutputRGBA.png", imageRGBA));
    EXPECT_TRUE(MathLib::ImageUtils::SaveImage("output\\testPngOutputRGBA.bmp", imageRGBA));
    EXPECT_TRUE(MathLib::ImageUtils::SaveImage("output\\testPngOutputRGBA.jpg", imageRGBA));
    EXPECT_TRUE(MathLib::ImageUtils::SaveImage("output\\testPngOutputRGBA.tga", imageRGBA));
}