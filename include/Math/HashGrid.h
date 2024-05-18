#pragma once
#include <Math/Math.h>
#include <Math/HashTable.h>
#include <Math/MathUtils.h>
namespace MathLib {
	template <typename DataType>
	class HashGrid2D
	{
	public:
		HashGrid2D(HReal dx = 1.f, unsigned int expectedSize = 512)
			:m_Dx(dx)
			, m_OverDx(1.f / dx)
			, m_Grid(expectedSize)
		{
		}

		void SetGridSize(HReal dx)
		{
			assert(Size() == 0);
			m_Dx = dx;
			m_OverDx = 1.f / dx;
		}

		void AddPoint(const HVector2& position, const DataType& data)
		{
			m_Grid.Insert(VectorRound2(position * m_OverDx), data);
		}

		void DeletePoint(const HVector2& position, const DataType& data)
		{
			m_Grid.Erase(VectorRound2(position * m_OverDx), data);
		}

		unsigned int Size() const
		{
			return m_Grid.Size();
		}

		void Clear()
		{
			m_Grid.Clear();
		}

		void Reserve(unsigned int expectedSize)
		{
			m_Grid.Reserve(expectedSize);
		}

		bool FindFirstPoint(const HVector2& position, DataType& data) const
		{
			return m_Grid.GetFirstData(VectorRound2(position * m_OverDx), data);
		}

		bool FindPoint(const HVector2& position, std::vector<DataType>& data) const
		{
			data.resize(0);
			return m_Grid.GetAllData(VectorRound2(position * m_OverDx), data) && data.size() > 0;
		}
		void AddBox(const HVector2& min, const HVector2& max, const DataType& data)
		{
			HVector2UI minIndex = VectorRound2(min * m_OverDx);
			HVector2UI maxIndex = VectorRound2(max * m_OverDx);

			for (unsigned int j = minIndex[1]; j <= maxIndex[1]; j++)
				for (unsigned int i = minIndex[0]; i <= maxIndex[0]; i++)
				{
					m_Grid.Insert(HVector2UI(i, j), data);
				}
		}
		void DeleteBox(const HVector2& min, const HVector2& max, const DataType& data)
		{
			HVector2UI minIndex = VectorRound2(min * m_OverDx);
			HVector2UI maxIndex = VectorRound2(max * m_OverDx);

			for (unsigned int j = minIndex[1]; j <= maxIndex[1]; j++)
				for (unsigned int i = minIndex[0]; i <= maxIndex[0]; i++)
				{
					m_Grid.Erase(HVector2UI(i, j), data);
				}
		}
		bool FindBox(const HVector2& min, const HVector2& max, std::vector<DataType>& data)const
		{
			data.resize(0);
			HVector2UI minIndex = VectorRound2(min * m_OverDx);
			HVector2UI maxIndex = VectorRound2(max * m_OverDx);
			for (unsigned int j = minIndex[1]; j <= maxIndex[1]; j++)
				for (unsigned int i = minIndex[0]; i <= maxIndex[0]; i++)
				{
					std::vector<DataType> temp;
					if (m_Grid.GetAllData(HVector2UI(i, j), temp) && (!temp.empty()))
					{
						data.insert(data.end(), temp.begin(), temp.end());
					}
				}
			return data.size() > 0;
		}
	private:
		HReal m_Dx;
		HReal m_OverDx;
		HashTable<HVector2UI, DataType> m_Grid;
	};

	template <typename DataType>
	class HashGrid3D
	{
	public:
		HashGrid3D(HReal dx = 1.f, unsigned int expectedSize = 512)
			:m_Dx(dx)
			, m_OverDx(1.f / dx)
			, m_Grid(expectedSize)
		{
		}

		void SetGridSize(HReal dx)
		{
			assert(Size() == 0);
			m_Dx = dx;
			m_OverDx = 1.f / dx;
		}

		void AddPoint(const HVector3& position, const DataType& data)
		{
			m_Grid.Insert(VectorRound3(position * m_OverDx), data);
		}

		void DeletePoint(const HVector3& position, const DataType& data)
		{
			m_Grid.Erase(VectorRound3(position * m_OverDx), data);
		}

		unsigned int Size() const
		{
			return m_Grid.Size();
		}

		void Clear()
		{
			m_Grid.Clear();
		}

		void Reserve(unsigned int expectedSize)
		{
			m_Grid.Reserve(expectedSize);
		}

		bool FindFirstPoint(const HVector3& position, DataType& data) const
		{
			return m_Grid.GetFirstData(VectorRound3(position * m_OverDx), data);
		}

		bool FindPoint(const HVector3& position, std::vector<DataType>& data) const
		{
			data.resize(0);
			return m_Grid.GetAllData(VectorRound3(position * m_OverDx), data) && data.size() > 0;
		}
		void AddBox(const HVector3& min, const HVector3& max, const DataType& data)
		{
			HVector3UI minIndex = VectorRound3(min * m_OverDx);
			HVector3UI maxIndex = VectorRound3(max * m_OverDx);

			for (unsigned int k = minIndex[2]; k <= maxIndex[2]; k++)
				for (unsigned int j = minIndex[1]; j <= maxIndex[1]; j++)
					for (unsigned int i = minIndex[0]; i <= maxIndex[0]; i++)
					{
						m_Grid.Insert(HVector3UI(i, j, k), data);
					}
		}
		void DeleteBox(const HVector3& min, const HVector3& max, const DataType& data)
		{
			HVector3UI minIndex = VectorRound3(min * m_OverDx);
			HVector3UI maxIndex = VectorRound3(max * m_OverDx);

			for (unsigned int k = minIndex[2]; k <= maxIndex[2]; k++)
				for (unsigned int j = minIndex[1]; j <= maxIndex[1]; j++)
					for (unsigned int i = minIndex[0]; i <= maxIndex[0]; i++)
					{
						m_Grid.Erase(HVector3UI(i, j, k), data);
					}
		}

		bool FindBox(const HVector3& min, const HVector3& max, std::vector<DataType>& data)const
		{
			data.resize(0);
			HVector3UI minIndex = VectorRound3(min * m_OverDx);
			HVector3UI maxIndex = VectorRound3(max * m_OverDx);
			for (unsigned int k = minIndex[2]; k <= maxIndex[2]; k++)
				for (unsigned int j = minIndex[1]; j <= maxIndex[1]; j++)
					for (unsigned int i = minIndex[0]; i <= maxIndex[0]; i++)
					{
						std::vector<DataType> temp;
						if (m_Grid.GetAllData(HVector3UI(i, j, k), temp) && (!temp.empty()))
						{
							data.insert(data.end(), temp.begin(), temp.end());
						}
					}
			return data.size() > 0;
		}

	private:
		HReal m_Dx;
		HReal m_OverDx;
		HashTable<HVector3UI, DataType> m_Grid;
	};

	typedef HashGrid2D<unsigned int> HashGrid2DUI;
	typedef HashGrid2D<char> HashGrid2DC;
	typedef HashGrid3D<unsigned int> HashGrid3DUI;
	typedef HashGrid3D<char> HashGrid3DC;

};// namespace MathLib