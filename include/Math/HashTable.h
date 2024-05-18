#pragma once
#include <Math/Math.h>
#include <Math/HasherFunction.h>
#include <unordered_map>
namespace MathLib
{
	template <class Key, class DataType, class HasherFunction = std::hash<Key>, class KeyEqual = std::equal_to<Key>>
	class HashTable
	{
	public:
		HashTable(unsigned int expectedSize=64)
			:m_HashTable(expectedSize)
		{
		}

		void Insert(const Key& key, const DataType& data)
		{
			auto it = m_HashTable.find(key);
			if (it == m_HashTable.end())
			{
				m_HashTable[key] = std::vector<DataType>();
				m_HashTable[key].push_back(data);
			}
			else
			{
				it->second.push_back(data);
			}
		}

		void Erase(const Key& key, const DataType& data)
		{
			auto it = m_HashTable.find(key);
			if (it != m_HashTable.end())
			{
				auto& vec = it->second;
				auto vecIt = std::find(vec.begin(), vec.end(), data);
				if (vecIt != vec.end())
				{
					vec.erase(vecIt);
				}
			}
		}

		unsigned int Size() const
		{
			return m_HashTable.size();
		}

		void Clear()
		{
			m_HashTable.clear();
		}

		void Reserve(unsigned int size)
		{
			m_HashTable.reserve(size);
		}

		bool HasKey(const Key& key) const
		{
			return m_HashTable.find(key) != m_HashTable.end();
		}

		bool GetFirstData(const Key& key, DataType& data) const
		{
			auto it = m_HashTable.find(key);
			if (it != m_HashTable.end())
			{
				if (it->second.size() > 0)
				{
					data = it->second[0];
					return true;
				}
			}
			return false;
		}

		bool GetData(const Key& key, unsigned int index, DataType& data) const
		{
			auto it = m_HashTable.find(key);
			if (it != m_HashTable.end())
			{
				if (it->second.size() > index)
				{
					data = it->second[index];
					return true;
				}
			}
			return false;
		}

		bool GetAllData(const Key& key, std::vector<DataType>& data) const
		{
			auto it = m_HashTable.find(key);
			if (it != m_HashTable.end())
			{
				data = it->second;
				return true;
			}
			return false;
		}

	private:
		std::unordered_map<Key, std::vector<DataType>, HasherFunction, KeyEqual> m_HashTable;
	};
}