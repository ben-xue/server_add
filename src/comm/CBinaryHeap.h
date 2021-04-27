#ifndef BINARYHEAP_H
#define BINARYHEAP_H

#include <iostream>
#include <vector>
#include <assert.h>
#include <map>


//带索引的二叉堆，可以查找某个key的元素是否在这个堆里面 最小堆
//也可以根据key单独调整某个节点的 优先级

template <typename KeyT, typename ValueT>
class CIndexedBinaryHeap
{
public:

	CIndexedBinaryHeap<KeyT, ValueT>()
	{

	}
  
	int Push(ValueT& elem)
	{
		m_value.push_back(elem);
		m_keyIndex[elem.key] = m_value.size() - 1;
		return FixUp(elem.key, m_value.size() - 1);
	}

	bool Empty()
	{
		return m_value.size() == 0;
	}

	bool Exist(const KeyT& key)
	{
		return m_keyIndex.find(key) != m_keyIndex.end();
	}

	ValueT& Get(const KeyT& key)
	{
		if (Exist(key))
		{
			int idx = m_keyIndex[key];
			return m_value[idx];
		}
		
		throw "key not found exception";
	}

	ValueT Pop()
	{
		if (Empty())
		{
			throw "empty heap can not pop";
		}

		ValueT v = m_value[0];
		std::swap(m_value[0], m_value[m_value.size() - 1]);
		m_value.pop_back();
		m_keyIndex.erase(v.key);

		if (!Empty())
		{
			m_keyIndex[m_value[0].key] = 0;	//value下标变化，索引到跟着调整
			FixDown(m_value[0].key, 0);
		}

		return v;
	}
	
	int Update(const KeyT& key)
	{
		if (m_keyIndex.find(key) == m_keyIndex.end())
		{
			return -1;
		}
		
		int idx = m_keyIndex[key];
		
		int f = (idx + 1) / 2 - 1;
        // 有父节点并且父节点大
		if (f >= 0 && m_value[idx] < m_value[f])	
		{
			return FixUp(key, idx);
		}

        return FixDown(key, idx);
	}

private:
	std::vector<ValueT> m_value;
	std::map<KeyT, int> m_keyIndex;

	int FixDown(const KeyT& key, int idx)
	{
		int f = idx;
		int k = 0;
		while ((k = (f * 2) + 1) < m_value.size())
		{ 
			if (k < m_value.size() - 1)	//存在右子节点
			{
				if (m_value[k + 1] < m_value[k])
				{
					k++;
				}
			}
            
			if (m_value[f] <= m_value[k]) 				//father已经最小了
			{
				break;
			}

			KeyT fk = m_value[f].key;
			KeyT kk = m_value[k].key;

			std::swap(m_value[f], m_value[k]);
			std::swap(m_keyIndex[fk], m_keyIndex[kk]);
			f = k;
		}
		return k;
	}

	int FixUp(const KeyT& key, int idx)
	{
		int c = idx;
		int f = 0;
		while ((f =  (c + 1) / 2 - 1 )>= 0)
		{
			if (m_value[f] <= m_value[c])
			{
				break;
			}

			KeyT fk = m_value[f].key;
			KeyT ck = m_value[c].key;

			std::swap(m_value[f], m_value[c]);
			std::swap(m_keyIndex[fk], m_keyIndex[ck]);

			c = f;
		}

        return f;
	}

    
};

#endif


		