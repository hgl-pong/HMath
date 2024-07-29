#include <Math/Visual/ImageUtils.h>

using namespace MathLib;
void main()
{
    // create directory for output if not exist
    const wchar_t* filePath = L"output";
    if (_waccess(filePath, 0) == -1)
    {
        _wmkdir(filePath);
        MATHLOG_INFO("Directory created.\n");
    }
    else
    {
        MATHLOG_INFO("Directory already exists.\n");
    }

	Array2D<HReal> imageGray;
	ImageUtils::LoadImage("..\\..\\data\\img\\testPng100.png", imageGray);
	ImageUtils::SaveImage("output\\testPngOutputGray.png", imageGray);
    ImageUtils::SaveImage("output\\testPngOutputGray.bmp", imageGray);
    ImageUtils::SaveImage("output\\testPngOutputGray.jpg", imageGray);
    ImageUtils::SaveImage("output\\testPngOutputGray.tga", imageGray);
    ImageUtils::SaveImage("output\\testPngOutputGray.tiff", imageGray);

	Array2D<HVector<HReal, 1>> imageGray2;
    ImageUtils::LoadImage("..\\..\\data\\img\\testPng2.png", imageGray2);
	ImageUtils::SaveImage("output\\testPngOutputGray2.png", imageGray2);
    ImageUtils::SaveImage("output\\testPngOutputGray2.bmp", imageGray2);
    ImageUtils::SaveImage("output\\testPngOutputGray2.jpg", imageGray2);
    ImageUtils::SaveImage("output\\testPngOutputGray2.tga", imageGray2);

	Array2D<HVector2> imageRG;
	ImageUtils::LoadImage("..\\..\\data\\img\\testPng.png", imageRG);
    ImageUtils::SaveImage("output\\testPngOutputRG.png", imageRG);
    ImageUtils::SaveImage("output\\testPngOutputRG.bmp", imageRG);
    ImageUtils::SaveImage("output\\testPngOutputRG.jpg", imageRG);
    ImageUtils::SaveImage("output\\testPngOutputRG.tga", imageRG);

	Array2D<HVector3> imageRGB;
    ImageUtils::LoadImage("..\\..\\data\\img\\testPng.png", imageRGB);
    ImageUtils::SaveImage("output\\testPngOutputRGB.png", imageRGB);
    ImageUtils::SaveImage("output\\testPngOutputRGB.bmp", imageRGB);
    ImageUtils::SaveImage("output\\testPngOutputRGB.jpg", imageRGB);
    ImageUtils::SaveImage("output\\testPngOutputRGB.tga", imageRGB);

    Array2D<HVector4> imageRGBA;
    ImageUtils::LoadImage("..\\..\\data\\img\\testPng.png", imageRGBA);
    ImageUtils::SaveImage("output\\testPngOutputRGBA.png", imageRGBA);
    ImageUtils::SaveImage("output\\testPngOutputRGBA.bmp", imageRGBA);
    ImageUtils::SaveImage("output\\testPngOutputRGBA.jpg", imageRGBA);
    ImageUtils::SaveImage("output\\testPngOutputRGBA.tga", imageRGBA);
}