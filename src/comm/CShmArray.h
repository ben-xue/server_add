/* 
*  共享对象内存池
*/

#ifndef CSHMARRAY_H_
#define CSHMARRAY_H_

#include <new>
#include "comm_inc.h"
#include "comm/mempool.h"
#include "CShmemMgr.h"


/**
* 单纯的共享内存的数组
*/
template<class T>
class CShmArray
{
public:
	CShmArray();
	~CShmArray();
	
	/**
	* 创建内存池
	*/
	int Create(int iShmKey, size_t dwShmSize, int iArraySize);
	T* Get(int iIndex);
	int Size() {return m_pArray->iArraySize;}
	void SetSize(int iNewSize) { /*ASSERT_RET(iNewSize <= , -1); */ m_pArray->iArraySize = iNewSize;}

	T* GetBuff() {return &m_pArray->data[0];}

private:
	struct _ArrayInfo
	{
		int iArraySize;
		T data[0];
	};
	
	_ArrayInfo* m_pArray;
};


template <class	T>
CShmArray<T>::CShmArray()
{
	m_pArray = NULL;
}

template <class	T>
CShmArray<T>::~CShmArray()
{
}

template <class	T>
int CShmArray<T>::Create(int iShmKey, size_t dwShmSize, int iArraySize)
{
	if (iArraySize <= 0)
	{
		LOG_ERR("CShmArray<T>::Create failed, ArraySize[%d]", (int)iArraySize);
		return -1;
	}

	size_t needSize = sizeof(T) * iArraySize + sizeof(CShmArray<T>::_ArrayInfo);
	void* pShmMem = CShmemMgr::CreateShmem(iShmKey, dwShmSize, needSize);
	if (NULL == pShmMem)
	{
		LOG_ERR("CreateShmem failed, ShmKey[%d] ShmSize[%u] NeedSize[%u]",
			iShmKey, (uint32_t)dwShmSize, (uint32_t)needSize);
		return -1;
	}

	int iOffset = CShmemMgr::GetOffset(iShmKey, pShmMem);
	LOG_ERR("ShmArray Size[%d] Offset[%d]", iArraySize, iOffset);
	
	int iRet = 0;
	if (ESSM_Init == CShmemMgr::GetStartFlag(iShmKey))
	{
		memset(pShmMem, 0, needSize);
		m_pArray = (_ArrayInfo*)pShmMem;
		m_pArray->iArraySize = iArraySize;		
	}
	else
	{
		m_pArray = (_ArrayInfo*)pShmMem;
		if (m_pArray->iArraySize != iArraySize)
		{
			LOG_ERR("CShmArray create failed, ArraySize not match[%d-%d]", iArraySize, m_pArray->iArraySize);
			return -1;
		}
	}
	
	return 0;
}

template <class	T>
T* CShmArray<T>::Get(int iIndex)
{
	ASSERT_RET(iIndex >= 0 && iIndex < Size(), NULL);
	return &m_pArray->data[iIndex];
}

#endif


