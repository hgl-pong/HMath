#pragma once
#include <Parallel.h>
namespace MathLib
{
    template <class Type>
    class Array2D
    {
    public:
        Array2D(size_t sizeX = 0, size_t sizeY = 0) : m_Data(sizeX * sizeY),
                                                      m_SizeX(sizeX),
                                                      m_SizeY(sizeY)
        {
            ResetData();
        };
        Array2D(Array2D const &copy_from) = default;
        Array2D(Array2D &&move_from) = default;

        Type &operator()(size_t x, size_t y)
        {

            assert((x < m_SizeX) && (y < m_SizeY));
            return m_Data[x * m_SizeY + y];
        }
        Type operator()(size_t x, size_t y) const
        {

            assert((x < m_SizeX) && (y < m_SizeY));
            return m_Data[x * m_SizeY + y];
        }

        void ReSize(size_t m_SizeX, size_t m_SizeY)
        {

            this->m_SizeX = m_SizeX;
            this->m_SizeY = m_SizeY;

            m_Data.resize(m_SizeX * m_SizeY);
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
            assert(m_SizeX == field.m_SizeX && m_SizeY == field.m_SizeY);

            size_t size = field.m_Data.size();
            for (size_t i = 0; i < size; ++i)
            {
                m_Data[i] += field.m_Data[i];
            }

            return *this;
        }
        Array2D &operator-=(Array2D const &field)
        {
            assert(m_SizeX == field.m_SizeX && m_SizeY == field.m_SizeY);

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

        void ExecuteUpdate(std::function<void(size_t, size_t)> update)
        {
            Parallel::ParallelFor<size_t>(0, m_SizeX, 0, m_SizeY, update);
        }

        size_t GetSizeX() const
        {
			return m_SizeX;
		}

        size_t GetSizeY() const
        {
            return m_SizeY;
        }

    private:
        std::vector<Type> m_Data;

        size_t m_SizeX;
        size_t m_SizeY;
    };

} // namespace Utility
