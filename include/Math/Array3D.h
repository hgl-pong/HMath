#pragma once
#include <Math/Math.h>
#include <vector>
namespace MathLib
{
	template <class Type>
	class Array3D
	{
	public:
		Array3D(size_t sizeX, size_t sizeY, size_t sizeZ)
			: m_SizeX(sizeX), m_SizeY(sizeY), m_SizeZ(sizeZ), m_Data(sizeX * sizeY * sizeZ)
		{
			ResetData();
		}
		Array3D(Array3D const& copy_from) = default;
		Array3D(Array3D&& move_from) = default;

		Type& operator()(size_t x, size_t y, size_t z)
		{
			return m_Data[x + y * m_SizeX + z * m_SizeX * m_SizeY];
		}

		const Type& operator()(size_t x, size_t y, size_t z) const
		{
			return m_Data[x + y * m_SizeX + z * m_SizeX * m_SizeY];
		}

		void ReSize(size_t sizeX, size_t sizeY, size_t sizeZ)
		{
			m_SizeX = sizeX;
			m_SizeY = sizeY;
			m_SizeZ = sizeZ;
			m_Data.resize(sizeX * sizeY * sizeZ);
		}

		Array3D& operator=(Array3D const& copy_from) = default;

		Array3D& operator+(Array3D const& rhs) const
		{
			Array3D sum(*this);
			sum += rhs;
			return sum;
		}

		Array3D& operator-(Array3D const& rhs) const
		{
			Array3D diff(*this);
			diff -= rhs;
			return diff;
		}

		Array3D& operator+=(Array3D const& rhs)
		{
			assert(m_SizeX == rhs.m_SizeX && m_SizeY == rhs.m_SizeY && m_SizeZ == rhs.m_SizeZ)
			for (size_t i = 0; i < m_Data.size(); ++i)
			{
				m_Data[i] += rhs.m_Data[i];
			}
			return *this;
		}

		Array3D& operator-=(Array3D const& rhs)
		{
			assert(m_SizeX == rhs.m_SizeX && m_SizeY == rhs.m_SizeY && m_SizeZ == rhs.m_SizeZ)
				for (size_t i = 0; i < m_Data.size(); ++i)
				{
				m_Data[i] -= rhs.m_Data[i];
			}
			return *this;
		}

		Array3D operator*(Type scalar) const
		{
			Array3D result(*this);
			for (size_t i = 0; i < m_Data.size(); ++i)
			{
				result.m_Data[i] *= scalar;
			}
			return result;
		}

		const std::vector<Type>& GetData() const
		{
			return m_Data;
		}

		void SetData(const std::vector<Type>& data)
		{
			assert(m_Data.size() == data.size());
			m_Data = data;
		}

		void ResetData()
		{
			memset(m_Data.data(), 0, m_Data.size() * sizeof(Type));
		}
		
		void ExecuteUpdate(std::function<void(size_t, size_t, size_t)> func)
		{
			Parallel::ParallelFor<size_t>(0,m_SizeX,0, m_SizeY, 0,m_SizeZ, func);
		}

		size_t getSizeX() const
		{
			return m_SizeX;
		}

		size_t getSizeY() const
		{
			return m_SizeY;
		}

		size_t getSizeZ() const
		{
			return m_SizeZ;
		}

	private:
		std::vector<Type> m_Data;
		size_t m_SizeX;
		size_t m_SizeY;
		size_t m_SizeZ;
	};
}