/* 
*  共享对象内存池
*/

#ifndef CBARRAY_H_
#define CBARRAY_H_

#include <new>
#include "comm_inc.h"


/**
* 定义二分排序数组
* key可以重复
*/
template<class KeyType, class T>
class CBArray
{
public:
	CBArray();
	~CBArray();

	int Init(int iMaxCount);
	virtual int Insert(KeyType key, T* pData);

	/**
	* 寻找和指定Key的数据
	*/
	T* Find(KeyType key);

	/**
	* 寻找大于等于key的最小节点
	*/
	T* FindCeil(KeyType key);
	
	/**
	* 根据Data获取到数据索引
	*/
	int GetDataIndex(T* data);
	
	/**
	* 获取数据，根据索引
	*/
	T* GetDataByIndex(int iIndex) 
	{
		ASSERT_RET(iIndex >= 0 && iIndex < m_iDataCnt, NULL);
		return &m_pData[iIndex];
	}
	
	int GetDataCount() {return m_iDataCnt;}

	void Clear() {m_iDataCnt = 0;}
	
	
protected:
	//比较函数，需要每个模块去具体实现
	virtual int Compare(KeyType key1, T* p2) = 0;

private:
	int FindIndex(KeyType key);
	

private:
	T* m_pData;
	int m_iDataCnt;
	int m_iMaxDataCnt;
};

/**
* key不重复的数组
*/
template<class KeyType, class T>
class CUBArray : public CBArray<KeyType, T>
{
public:
	virtual int Insert(KeyType key, T* pData)
	{
		if (this->Find(key))
		{
			LOG_ERR("Data exist");
			return -1;
		}

		return CBArray<KeyType, T>::Insert(key, pData);
	}
};


template <class KeyType, class	T>
CBArray<KeyType, T>::CBArray()
{
	m_pData = NULL;
	m_iDataCnt = 0;
	m_iMaxDataCnt = 0;
}

template <class KeyType, class	T>
CBArray<KeyType, T>::~CBArray()
{
	if (m_pData )
	{
		delete []m_pData;
	}
}

template <class KeyType, class	T>
int CBArray<KeyType, T>::Init(int iMaxCount)
{
	ASSERT_RET(iMaxCount > 0, -1);
	m_iDataCnt = 0;
	m_iMaxDataCnt = iMaxCount;
	m_pData = new T[m_iMaxDataCnt];

	return 0;
}

template <class KeyType, class	T>
int CBArray<KeyType, T>::Insert(KeyType key, T* pData)
{	
	if (m_iDataCnt >= m_iMaxDataCnt)
	{
		LOG_ERR("Data Count is exceed max[%d][%d]", m_iDataCnt, m_iMaxDataCnt);
		return -2;
	}
	
	//顺序排列
	int i;
	for(i = m_iDataCnt; i > 0; i--)
	{		
		//找到位置了，copy
		if (Compare(key, &m_pData[i-1]) >= 0)
		{
			m_pData[i] = *pData;
			break;
		}
		else
		{
			m_pData[i] = m_pData[i-1];
		}
	}
	
	if (i <= 0)
	{
		m_pData[0] = *pData;
	}
	
	m_iDataCnt++;	
	return 0;
}

template <class KeyType, class	T>
T* CBArray<KeyType, T>::Find(KeyType key)
{
	int iIndex = FindIndex(key);
	if (iIndex < 0 || Compare(key, &m_pData[iIndex]) != 0)
	{
		return NULL;
	}
	
	return &m_pData[iIndex];
}

/**
* 寻找大于等于key的最小节点
*/
template <class KeyType, class	T>
T* CBArray<KeyType, T>::FindCeil(KeyType key)
{
	int iIndex = FindIndex(key);
	if (iIndex < 0)
	{
		return NULL;
	}

	int iCmp = Compare(key, &m_pData[iIndex]);

	//该节点大于等于key,就是要找的节点了
	if (iCmp <= 0)
	{
		return &m_pData[iIndex];
	}
	else //否则，小于key, 则判断是否超过边界了
	{
		int iFindIndex = iIndex + 1;
		if (iFindIndex >= m_iDataCnt)
		{
			return NULL;
		}

		return &m_pData[iFindIndex];
	}
	
	return NULL;
}

/**
* 根据Data获取到数据索引
*/
template <class KeyType, class	T>
int CBArray<KeyType, T>::GetDataIndex(T* data)
{
	ASSERT_RET(data, -1);

	//计算偏移
	intptr_t dptr = (intptr_t)data;
	intptr_t start = (intptr_t)m_pData;
	intptr_t offset = dptr - start;
	if (offset < 0)
	{	
		LOG_ERR("invalid data ptr, offset[%d]", (int)offset);
		return -1;
	}

	if (offset%sizeof(T) != 0)
	{
		LOG_ERR("invalid data ptr, mod size(T) not zero");
		return -1;
	}

	int index = offset/sizeof(T);
	if (index >= m_iDataCnt)
	{
		LOG_ERR("invalid data ptr, exceed data buff, index[%d]", index);
		return -1;
	}

	return index;
}


template <class KeyType, class	T>
int CBArray<KeyType, T>::FindIndex(KeyType key)
{
	ASSERT_RET(m_pData, -1);
	if (m_iDataCnt <= 0)
	{
		return -1;
	}
	
	int a = 0;
	int b = m_iDataCnt-1;

	if (a >= b)
	{
		return a;
	}
	
	int ca = Compare(key, &m_pData[a]);
	int cb = Compare(key, &m_pData[b]);
	
	//相等，包括key小余a的情况	
	if (ca <= 0)
	{
		return a;
	}

	//相等,也包括key大于b的情况
	if (cb >= 0)
	{
		return b;
	}
	
	while(a + 1 < b)
	{		
		int m = (a+b)/2;
		int cm = Compare(key, &m_pData[m]);
		if (0 == cm)
		{
			return m;
		}
		else if (cm < 0)
		{
			b = m;
		}
		else
		{
			a = m;
		}
	}
	
	return a;
}

#endif


