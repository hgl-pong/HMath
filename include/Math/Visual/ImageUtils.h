#pragma once
#include <Math/Math.h>
#include <Math/Array2D.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <Math/Visual/stb_image.h>
#include <Math/Visual/stb_image_write.h>

// Use Custom Memory Allocators
// #define STBI_MALLOC(sz)           malloc(sz)
// #define STBI_REALLOC(p,newsz)     realloc(p,newsz)
// #define STBI_FREE(p)              free(p)
//
// #define STBIW_MALLOC(sz)        malloc(sz)
// #define STBIW_REALLOC(p,newsz)  realloc(p,newsz)
// #define STBIW_FREE(p)           free(p)

namespace MathLib
{
    namespace ImageUtils
    {
        enum class ImageFileType
        {
            PNG,
            JPEG,
            BMP,
            TGA,
            GIF,
            HDR,
            PSD,
            PIC,
            PNM,
            UNKNOWN
        };

        enum class ImageFormat : unsigned int
        {
            GRAY = 1,
            RGB = 3,
            RGBA = 4
        };

        inline ImageFileType GetImageFileType(const std::string& filename)
        {
            auto dotPos = filename.find_last_of(".");
            if (dotPos == std::string::npos)
                return ImageFileType::UNKNOWN;

            std::string extension = filename.substr(dotPos + 1);
            if (extension == "png")
                return ImageFileType::PNG;
            else if (extension == "jpg" || extension == "jpeg")
                return ImageFileType::JPEG;
            else if (extension == "bmp")
                return ImageFileType::BMP;
            else if (extension == "tga")
                return ImageFileType::TGA;
            else if (extension == "gif")
                return ImageFileType::GIF;
            else if (extension == "hdr")
                return ImageFileType::HDR;
            else if (extension == "psd")
                return ImageFileType::PSD;
            else if (extension == "pic")
                return ImageFileType::PIC;
            else if (extension == "pnm")
                return ImageFileType::PNM;
            return ImageFileType::UNKNOWN;
        }

        inline ImageFormat GetImageFormat(int channels) { return channels == 1 ? ImageFormat::GRAY : (channels == 3 ? ImageFormat::RGB : ImageFormat::RGBA); }

        template <ImageFormat Format>
        struct Pixel
        {
            uint8_t data[Format];
            uint8_t& operator[](uint8_t index) { return data[index]; }
        };

        typedef Pixel<ImageFormat::GRAY> PixelGray;
        typedef Pixel<ImageFormat::RGB> PixelRGB;
        typedef Pixel<ImageFormat::RGBA> PixelRGBA;

        void Uint8ToReal(const std::vector<uint8_t>& data, std::vector<HReal>& realData, const HReal min = 0, const HReal max = 1)
        {
            realData.resize(data.size());
            const HReal range = max - min;
            for (int i = 0; i < data.size(); i++)
            {
                realData[i] = (HReal)data[i] / 255.0 * range + min;
            }
        }

        void RealToUint8(const std::vector<HReal>& realData, std::vector<uint8_t>& data, HReal* const min = nullptr, HReal* const max = nullptr)
        {
            bool normalize = (min != nullptr && max != nullptr);
            data.resize(realData.size());
            if (normalize)
            {
                HReal& minValue = *min;
                HReal& maxValue = *max;
                minValue = H_REAL_MAX;
                maxValue = H_REAL_MIN;
                for (size_t i = 0; i < realData.size(); i++)
                {
                    minValue = std::min(minValue, realData[i]);
                    maxValue = std::max(maxValue, realData[i]);
                }

                const HReal range = maxValue - minValue;
                for (size_t i = 0; i < realData.size(); i++)
                    data[i] = (uint8_t)((realData[i] - minValue) / range * 255.0);
            }
            else
                for (int i = 0; i < realData.size(); i++)
                    data[i] = (uint8_t)(realData[i] * 255.0);
        }

        bool LoadImage(const std::string& filename, std::vector<uint8_t>& data, int& width, int& height, ImageFormat& format)
        {
            int channels;
            unsigned char* imgData = stbi_load(filename.c_str(), &width, &height, &channels, 0);
            format = static_cast<ImageFormat>(channels);
            if (imgData == nullptr)
                return false;
            data.assign(imgData, imgData + width * height * channels);
            stbi_image_free(imgData);
            return true;
        }

        template <ImageFormat Format>
        bool SaveImage(const std::string& filename, const std::vector<Pixel<Format>>& data, uint32_t width, uint32_t height, const bool compress = true)
        {
            const ImageFileType fileType = GetImageFileType(filename);
            if (fileType == ImageFileType::UNKNOWN)
                return false;
            const uint8_t* dataPtr = reinterpret_cast<const uint8_t*>(data.data());
            switch (fileType)
            {
            case ImageFileType::PNG:
            {
                return stbi_write_png(filename.c_str(), width, height, compress, dataPtr, width * static_cast<int>(Format)) != 0;
                break;
            }
            case ImageFileType::JPEG:
            {
                return stbi_write_jpg(filename.c_str(), width, height, compress, dataPtr, 100) != 0;
                break;
            }
            case ImageFileType::BMP:
            {
                return stbi_write_bmp(filename.c_str(), width, height, compress, dataPtr) != 0;
                break;
            }
            case ImageFileType::TGA:
            {
                return stbi_write_tga(filename.c_str(), width, height, compress, dataPtr) != 0;
                break;
            }
            case ImageFileType::GIF:
            {
                break;
            }
            case ImageFileType::HDR:
            {
                return stbi_write_hdr(filename.c_str(), width, height, compress, reinterpret_cast<const float*>(dataPtr)) != 0;
                break;
            }
            case ImageFileType::PSD:
            {
                break;
            }
            case ImageFileType::PIC:
            {
                break;
            }
            case ImageFileType::PNM:
            {
                break;
            }
            default:
                break;
            }
        }

        bool LoadImage(const std::string& filename, MathLib::Array2D<HReal>& array2D)
        {
            std::vector<uint8_t> data;
            int width, height;
            ImageFormat format;
            if (!LoadImage(filename, data, width, height, format))
                return false;
            std::vector<HReal> realData;
            Uint8ToReal(data, realData);
            array2D.ReSize(width, height);
            if (format == ImageFormat::GRAY)
            {
                array2D.SetData(realData);
            }
            else
            {
                std::vector<HReal> rPath;
                rPath.resize(width * height);
                for (int i = 0; i < width * height; ++i)
                    rPath[i] = realData[i * 3];
                array2D.SetData(rPath);
            }
            return true;
        }

        bool SaveImage(const std::string& filename, const MathLib::Array2D<HReal>& array2D, const bool compress = true)
        {
            std::vector<PixelGray> pixels;
            const uint32_t width = array2D.GetSizeX();
            const uint32_t height = array2D.GetSizeY();
            const std::vector<HReal>& data = array2D.GetData();
            pixels.resize(width * height);
            for (int i = 0; i < width * height; ++i)
                pixels[i][0] = data[i];
            return SaveImage(filename, pixels, width, height, compress);
        }

        template< typename VectorType = HVector2 >
        bool LoadImage(const std::string& filename, MathLib::Array2D<VectorType>& array2D)
        {
            std::vector<uint8_t> data;
            int width, height;
            ImageFormat format;
            if (!LoadImage(filename, data, width, height, format))
                return false;
            array2D.Resize(width , height);
            const uint32_t dim = VectorType().size();
            std::vector<HReal> realData;
            Uint8ToReal(data, realData);

            auto UpdateFun = [&](size_t i, size_t j)
                {

                };
            array2D.ExecuteUpdate(UpdateFun);
            return true;
        }

    } // namespace ImageUtils
} // namespace MathLib