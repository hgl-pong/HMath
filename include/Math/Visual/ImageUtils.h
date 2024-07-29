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
            UNKNOWN
        };

        enum ImageFormat : int
        {
            GRAY = 1,
            RG = 2,
            RGB = 3,
            RGBA = 4
        };

        inline ImageFileType GetImageFileType(const std::string &filename)
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
            return ImageFileType::UNKNOWN;
        }

        inline ImageFormat GetImageFormat(const uint32_t channels)
        {
            return channels > 4 ? ImageFormat::RGBA : static_cast<ImageFormat>(channels);
        }

        template <int N>
        using HPixel = HVector<uint8_t, N>;
        typedef HPixel<ImageFormat::GRAY> HPixelGray;
        typedef HPixel<ImageFormat::RGB> HPixelRG;
        typedef HPixel<ImageFormat::RGB> HPixelRGB;
        typedef HPixel<ImageFormat::RGBA> HPixelRGBA;

        void Uint8ToReal(const std::vector<uint8_t> &data, std::vector<HReal> &realData, const HReal min = 0, const HReal max = 1)
        {
            realData.resize(data.size());
            const HReal range = max - min;
            for (int i = 0; i < data.size(); i++)
            {
                realData[i] = (HReal)data[i] / 255.0 * range + min;
            }
        }

        void RealToUint8(const std::vector<HReal> &realData, std::vector<uint8_t> &data, HReal *const min = nullptr, HReal *const max = nullptr)
        {
            bool normalize = (min != nullptr && max != nullptr);
            data.resize(realData.size());
            Parallel::ParallelFunction<int> pFn;
            if (normalize)
            {
                HReal &minValue = *min;
                HReal &maxValue = *max;
                minValue = H_REAL_MAX;
                maxValue = H_REAL_MIN;

                for (size_t i = 0; i < realData.size(); i++)
                {
                    minValue = std::min(minValue, realData[i]);
                    maxValue = std::max(maxValue, realData[i]);
                }

                const HReal range = maxValue - minValue;
                pFn = [&](int i)
                { data[i] = static_cast<uint8_t>((realData[i] - minValue) / range * 255.0); };
            }
            else
                pFn = [&](int i)
                { data[i] = static_cast<uint8_t>(realData[i] * 255.0); };
            Parallel::ParallelFor<int>(0, realData.size(), pFn);
        }

        bool LoadImage(const std::string &filename, std::vector<uint8_t> &data, int &width, int &height, ImageFormat &format)
        {
            int channels;
            unsigned char *imgData = stbi_load(filename.c_str(), &width, &height, &channels, 0);
            format = static_cast<ImageFormat>(channels);
            if (imgData == nullptr)
                return false;
            data.assign(imgData, imgData + width * height * channels);
            stbi_image_free(imgData);
            return true;
        }

        bool SaveImage(const std::string &filename, void *pData, uint32_t width, uint32_t height, ImageFormat format, const bool compress = true)
        {
            const ImageFileType fileType = GetImageFileType(filename);
            if (fileType == ImageFileType::UNKNOWN)
                return false;
            switch (fileType)
            {
            case ImageFileType::PNG:
            {
                return stbi_write_png(filename.c_str(), width, height, static_cast<int>(format), pData, width * static_cast<int>(format)) != 0;
                break;
            }
            case ImageFileType::JPEG:
            {
                return stbi_write_jpg(filename.c_str(), width, height, static_cast<int>(format), pData, 100) != 0;
                break;
            }
            case ImageFileType::BMP:
            {
                return stbi_write_bmp(filename.c_str(), width, height, static_cast<int>(format), pData) != 0;
                break;
            }
            case ImageFileType::TGA:
            {
                return stbi_write_tga(filename.c_str(), width, height, static_cast<int>(format), pData) != 0;
                break;
            }
            default:
                break;
            }
        }

        bool LoadImage(const std::string &filename, Array2D<HReal> &array2D, const HReal min = 0.f, const HReal max = 1.f)
        {
            std::vector<uint8_t> data;
            int width, height;
            ImageFormat format;
            if (!LoadImage(filename, data, width, height, format))
            {
                MATHLOG_ERROR("Failed to load image: %s\n", filename.c_str());
                return false;
            }
            std::vector<HReal> realData;
            Uint8ToReal(data, realData, min, max);
            array2D.ReSize(width, height);
            std::vector<HReal> rPath;
            rPath.resize(width * height);
            Parallel::ParallelFor<int>(0, width * height, [&](int i)
                                       { rPath[i] = realData[i * format]; });
            array2D.SetData(rPath);
            return true;
        }

        bool SaveImage(const std::string &filename, const Array2D<HReal> &array2D, HReal *max = nullptr, HReal *min = nullptr, const bool compress = true)
        {
            std::vector<HPixelGray> pixels;
            const uint32_t width = array2D.GetSizeX();
            const uint32_t height = array2D.GetSizeY();
            const std::vector<HReal> &data = array2D.GetData();
            pixels.resize(width * height);
            std::vector<uint8_t> data8;
            RealToUint8(data, data8, min, max);
            Parallel::ParallelFor<int>(0, width * height, [&](int i)
                                       { pixels[i][0] = data8[i]; });
            return SaveImage(filename, pixels.data(), width, height, ImageFormat::GRAY, compress);
        }

        template <typename Type, int N>
        bool LoadImage(const std::string &filename, Array2D<HVector<Type, N>> &array2D, const HReal min = 0.f, const HReal max = 1.f)
        {
            std::vector<uint8_t> data;
            int width, height;
            ImageFormat format;
            if (!LoadImage(filename, data, width, height, format))
                return false;
            array2D.ReSize(width, height);

            std::vector<HReal> realData;
            Uint8ToReal(data, realData, min, max);

            Array2D<HVector<Type, N>>::ArrayUpdateFn updateFn;

            uint32_t channels = std::min(static_cast<int>(format), N);

            updateFn = [&](std::vector<HVector<Type, N>> &localData, size_t i, size_t j)
            {
                for (uint32_t k = 0; k < channels; ++k)
                    localData[i * width + j][k] = realData[(i * width + j) * format + k];
            };
            array2D.ExecuteUpdate(updateFn);
            return true;
        }

        template <typename Type, int N>
        bool SaveImage(const std::string &filename, const Array2D<HVector<Type, N>> &array2D, HReal *max = nullptr, HReal *min = nullptr, const bool compress = true)
        {
            const std::vector<HVector<Type, N>> &data = array2D.GetData();
            std::vector<HReal> realData;
            std::vector<uint8_t> u8Data;
            const uint32_t width = array2D.GetSizeX();
            const uint32_t height = array2D.GetSizeY();
            realData.resize(width * height * N);
            Parallel::ParallelFor<int>(0, width * height, [&](int i)
                                       { for (int j = 0; j < N; j++) 
                realData[i * N + j] = data[i][j]; });

            RealToUint8(realData, u8Data, min, max);
            return SaveImage(filename, u8Data.data(), width, height, static_cast<ImageFormat>(N), compress);
        }

    } // namespace ImageUtils
} // namespace MathLib