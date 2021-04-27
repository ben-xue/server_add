/* 
*  共享对象内存池
*/

#ifndef CSHMARRAYLIST_H__
#define CSHMARRAYLIST_H__

#include <new>
#include <string.h>
#include "comm_inc.h"
#include "comm/mempool.h"
#include "CShmemMgr.h"


/**
* 单纯的共享内存的数组
* 内部自带长度数据
*/
template<class T>
class CShmArrayList
{
public:
	CShmArrayList();
	~CShmArrayList();
	
	/**
	* 创建内存池
	*/
	int Create(int iShmKey, size_t dwShmSize, int iArrayMaxSize);
	T* Get(int iIndex);	
	int Insert(int iIndex, T& data);
	int Insert(T& data);

	void Remove(int iIndex);
	void Clear();
	
	void Set(int iIndex, T& data);
	
	bool IsEmpty() {return m_pArray->iArraySize <= 0;}
	bool IsFull() {return m_pArray->iArraySize >= m_pArray->iArrayMaxSize;}
	
	int Size() {return m_pArray->iArraySize;}
	int MaxSize() {return m_pArray->iArrayMaxSize;}

	const T* GetBuff(){return m_pArray->data;}
	
	//表示是热启动的标记位
	bool GetResumeFlag() { return m_bResumeFlag; };
private:
	struct _ArrayInfo
	{
		int iArrayMaxSize;
		int iArraySize;
		T data[0];
	};
	
	bool m_bResumeFlag;

	_ArrayInfo* m_pArray;
};


template <class	T>
CShmArrayList<T>::CShmArrayList()
{
	m_pArray = NULL;
}

template <class	T>
CShmArrayList<T>::~CShmArrayList()
{
}

template <class	T>
int CShmArrayList<T>::Create(int iShmKey, size_t dwShmSize, int iArrayMaxSize)
{
	if (iArrayMaxSize <= 0)
	{
		LOG_ERR("CShmArray<T>::Create failed, ArrayMaxSize[%d]", (int)iArrayMaxSize);
		return -1;
	}

	size_t needSize = sizeof(T) * iArrayMaxSize + sizeof(CShmArrayList<T>::_ArrayInfo);
	void* pShmMem = CShmemMgr::CreateShmem(iShmKey, dwShmSize, needSize);
	if (NULL == pShmMem)
	{
		LOG_ERR("CreateShmem failed, ShmKey[%d] ShmSize[%u] NeedSize[%u]",
			iShmKey, (uint32_t)dwShmSize, (uint32_t)needSize);
		return -1;
	}

	int iOffset = CShmemMgr::GetOffset(iShmKey, pShmMem);
	LOG_ERR("ShmArray Size[%d] Offset[%d]", iArrayMaxSize, iOffset);
	
	int iRet = 0;
	if (ESSM_Init == CShmemMgr::GetStartFlag(iShmKey))
	{
		memset(pShmMem, 0, needSize);
		m_pArray = (_ArrayInfo*)pShmMem;
		m_pArray->iArrayMaxSize = iArrayMaxSize;		
		m_bResumeFlag = false;
	}
	else
	{
		m_bResumeFlag = true;
		m_pArray = (_ArrayInfo*)pShmMem;
		if (m_pArray->iArrayMaxSize != iArrayMaxSize)
		{
			LOG_ERR("CShmArray create failed, ArraySize not match[%d-%d]", iArrayMaxSize, m_pArray->iArrayMaxSize);
			return -1;
		}
	}
	
	return 0;
}

template <class	T>
T* CShmArrayList<T>::Get(int iIndex)
{
	ASSERT_RET(iIndex >= 0 && iIndex < Size(), NULL);
	return &m_pArray->data[iIndex];
}

template <class	T>
int CShmArrayList<T>::Insert(int iIndex, T& data)
{
	if (IsFull())
	{
		LOG_ERR("array list is full");
		return -1;
	}
		
	ASSERT_RET(iIndex >= 0 && iIndex <= Size(), NULL);
	
	if (iIndex < Size())
	{
		memmove(&m_pArray->data[iIndex+1], &m_pArray->data[iIndex], (Size()-iIndex)*sizeof(T));
	}

	m_pArray->data[iIndex] = data;
	m_pArray->iArraySize++;
	return 0;
}

template <class	T>
int CShmArrayList<T>::Insert(T& data)
{
	if (IsFull())
	{
		LOG_ERR("array list is full");
		return -1;
	}
	
	return Insert(Size(), data);
}

template <class	T>
void CShmArrayList<T>::Set(int iIndex, T& data)
{
	ASSERT_RET(iIndex >= 0 && iIndex < Size());
	m_pArray->data[iIndex] = data;
}

template <class	T>
void CShmArrayList<T>::Remove(int iIndex)
{
	ASSERT_RET(iIndex >= 0 && iIndex < Size());

	if (iIndex+1 < Size())
	{
		memmove(&m_pArray->data[iIndex], &m_pArray->data[iIndex+1], (Size()-iIndex-1)*sizeof(T));
	}

	m_pArray->iArraySize--;
}

template <class	T>
void CShmArrayList<T>::Clear()
{
	m_pArray->iArraySize = 0;
}


#endif


