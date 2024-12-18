#pragma once
#include <Math/MathUtils.h>
#include <Math/Parallel.h>
#include <functional>
namespace MathLib
{
    template <class Type, class Alloc = std::allocator<Type>>
    class Array2D
    {
    public:
        typedef std::function<Type(uint32_t, uint32_t)> ArrayUpdateFn;

    public:
        Array2D(size_t sizeX = 0, size_t sizeY = 0) : m_Data(sizeX * sizeY),
                                                      m_Size(sizeX, sizeY)
        {
            ResetData();
        };

        Array2D(HVector2I &size) : m_Data(size[0] * size[1]), m_Size(size)
        {
            ResetData();
        }
        Array2D(Array2D const &copy_from) = default;
        Array2D(Array2D &&move_from) = default;

        Type &operator()(size_t x, size_t y)
        {
            x = Clamp(x, 0, m_Size[0] - 1);
            y = Clamp(y, 0, m_Size[1] - 1);
            assert((x < m_Size[0]) && (y < m_Size[1]));
            return m_Data[x * m_Size[0] + y];
        }
        Type operator()(size_t x, size_t y) const
        {
            x = Clamp(x, 0, m_Size[0] - 1);
            y = Clamp(y, 0, m_Size[1] - 1);
            assert((x < m_Size[0]) && (y < m_Size[1]));
            return m_Data[x * m_Size[0] + y];
        }

        Type &operator()(const HVector2I &pos)
        {
            return (*this)(pos[0], pos[1]);
        }

        Type operator()(const HVector2I &pos) const
        {
            return (*this)(pos[0], pos[1]);
        }

        void ReSize(size_t sizeX, size_t sizeY)
        {
            m_Size[0] = sizeX;
            m_Size[1] = sizeY;

            m_Data.resize(sizeX * sizeY);
            memset(&m_Data[0], 0, sizeX * sizeY * sizeof(Type));
        }

        void ReSize(const HVector2UI &size)
        {
            ReSize(size[0], size[1]);
        }

        Array2D &operator=(Array2D const &copy_from) = default;

        Array2D operator+(Array2D const &field) const
        {

            Array2D sum(*this);
            sum += field;

            return sum;
        }
        Array2D operator-(Array2D const &field) const
        {

            Array2D diff(*this);
            diff -= field;

            return diff;
        }
        Array2D &operator+=(Array2D const &field)
        {
            assert(m_Size[0] == field.m_Size[0] && m_Size[1] == field.m_Size[1]);

            size_t size = field.m_Data.size();
            for (size_t i = 0; i < size; ++i)
            {
                m_Data[i] += field.m_Data[i];
            }

            return *this;
        }
        Array2D &operator-=(Array2D const &field)
        {
            assert(m_Size[0] == field.m_Size[0] && m_Size[1] == field.m_Size[1]);

            size_t size = field.m_Data.size();
            for (size_t i = 0; i < size; ++i)
            {
                m_Data[i] -= field.m_Data[i];
            }

            return *this;
        }
        Array2D operator*(double multiplier)
        {

            Array2D scaled(*this);
            for (double &element : scaled.m_Data)
            {
                element *= multiplier;
            }

            return scaled;
        };

        const std::vector<Type> &GetData() const
        {
            return m_Data;
        }

        void SetData(std::vector<Type> &data)
        {
            assert(m_Data.size() == data.size());
            m_Data = data;
        }

        void ResetData()
        {
            memset(m_Data.data(), 0, m_Data.size() * sizeof(Type));
        }

        void ExecuteUpdate(ArrayUpdateFn updateFn)
        {
            const Parallel::ParallelFunction2<uint32_t> fn = [&](uint32_t x, uint32_t y)
            {
                (*this)(x, y) = updateFn(x, y);
            };
            Parallel::ParallelFor<uint32_t>(0, m_Size[0], 0, m_Size[1], fn);
        }

        uint32_t GetSizeX() const
        {
            return m_Size[0];
        }

        uint32_t GetSizeY() const
        {
            return m_Size[1];
        }

        HVector2UI GetDimension() const
        {
            return m_Size;
        }

        Type Sample (const HVector2 &pos) const
        {
            HVector2I posI = HVector2I(pos[0], pos[1]);
            HVector2 offset = HVector2(pos[0] - posI[0], pos[1] - posI[1]);
            HVector2I leftPos = HVector2I(posI[0], posI[1]);
            HVector2I rightPos = HVector2I(posI[0] + 1, posI[1]);
            HVector2I downPos = HVector2I(posI[0], posI[1] + 1);
            HVector2I rightDownPos = HVector2I(posI[0] + 1, posI[1] + 1);

            Type left = (*this)(leftPos);
            Type right = (*this)(rightPos);
            Type down = (*this)(downPos);
            Type rightDown = (*this)(rightDownPos);
            return BiLerp(left, right, down, rightDown, offset[0], offset[1]);
        }

    private:
        std::vector<Type, Alloc> m_Data;
        HVector2UI m_Size;
    };

    template<typename Type>
    using AlignedArray2D = Array2D<Type, HAlignedAllocator<Type>>;

#ifdef ALIGNED_ARRAY2D
    typedef AlignedArray2D<HReal> Array2DF;
    typedef AlignedArray2D<HVector2> Array2D2F;
    typedef AlignedArray2D<HVector3> Array2D3F;
    typedef AlignedArray2D<HVector4> Array2D4F;
#else
    typedef Array2D<HReal> Array2DF;
    typedef Array2D<HVector2> Array2D2F;
    typedef Array2D<HVector3> Array2D3F;
    typedef Array2D<HVector4> Array2D4F;
#endif

} // namespace Utility
