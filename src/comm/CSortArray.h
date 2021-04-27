/* 
*  固定长度的array
*/

#ifndef CSORTARRAY_H__
#define CSORTARRAY_H__

#include <new>
#include "comm_inc.h"

template<class T, int LEN>
class CSortArray
{
public:
	CSortArray();
	virtual ~CSortArray();
	
	T* Get(int iIndex);
	int Size() { return m_aryDataLen; }

	int GetMaxSize() {return LEN;}
	
	virtual int Update(T* t);

	void Clear()
	{
		m_aryDataLen = 0;
	}

	void SetCanDrop(bool bCanDrop){ m_bCanDrop = bCanDrop; }
	
	const T* GetBuff(){ return &m_aryData; }
	
	int GetArrayIndex(T* t);

protected:
	virtual int Compare(T* t1, T* t2) = 0;
	virtual int IsSame(T *t1,T *t2) = 0;
	
public:
	int DoInsert(T* t);
	void DoRemove(T* t);
	
protected:
	int m_aryDataLen;
	T m_aryData[LEN];
	bool m_bCanDrop;
};

template<class T, int LEN>
CSortArray<T, LEN>::CSortArray()
{
	memset(m_aryData, 0, sizeof(m_aryData));
    m_aryDataLen = 0;
	m_bCanDrop = true;
}

template<class T, int LEN>
CSortArray<T, LEN>::~CSortArray()
{
}

template<class T, int LEN>
T* CSortArray<T, LEN>::Get(int iIndex)
{
	ASSERT_RET(iIndex >= 0 && iIndex < m_aryDataLen, NULL);
	return &m_aryData[iIndex];
}

template<class T, int LEN>
int CSortArray<T, LEN>::Update(T* t)
{
	DoRemove(t);
	DoInsert(t);
	return 0;
}

template<class T, int LEN>
int CSortArray<T, LEN>::GetArrayIndex(T* t)
{
	for (int i = 0; i < m_aryDataLen; i++)
	{
		if (IsSame(&m_aryData[i], t))
		{
			return i;
		}
	}
	return -1;
}

template<class T, int LEN>
int CSortArray<T, LEN>::DoInsert(T* t)
{
	if (!m_bCanDrop)
	{
		ASSERT_RET(m_aryDataLen < LEN, -1);
	}	

	int iNum = m_aryDataLen;

	if (m_aryDataLen < LEN)
	{
		m_aryDataLen++;
	}

	for (int i = iNum - 1; i >= 0; i--)
	{
		T* pCurr = &m_aryData[i];
		if (Compare(t,pCurr) > 0)
		{
			//要往上移动,如果超过范围了，则返回抛弃掉
			int iDest = i+1;
			if (iDest < m_aryDataLen)
			{
				m_aryData[iDest] = *pCurr;
			}
		}
		else
		{
			int iDest = i+1;
			if (iDest < m_aryDataLen)
			{
				m_aryData[iDest] = *t;
			}
			return 0;
		}
	}

	//最后肯定是第一个
	m_aryData[0] = *t;
	
	return 0;
}

template<class T, int LEN>
void CSortArray<T, LEN>::DoRemove(T* t)
{
	int i = GetArrayIndex(t);
	if (i >= 0)
	{
		for (; i < m_aryDataLen - 1; i++)
		{	
			m_aryData[i] = m_aryData[i+1];
		}
		m_aryDataLen--;
	}
}

#endif
