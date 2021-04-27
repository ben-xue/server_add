/* 
*  共享内存的结构体
*/

#ifndef CSHMSTRUCT_H_
#define CSHMSTRUCT_H_

#include <new>
#include "comm_inc.h"
#include "comm/mempool.h"
#include "CShmemMgr.h"


/**
* 共享内存的结构体
*/
template<class T>
class CShmStruct
{
public:
	CShmStruct();
	~CShmStruct();
	
	/**
	* 创建内存池
	*/
	int Create(int iShmKey, size_t dwShmSize, bool& bNeedInit);	
	T* operator->() {return m_pData->data;}
	T* Get(){return m_pData->data;}

private:
	struct _DataInfo
	{
		int iDataSize;
		T data[0];
	};
	
	_DataInfo* m_pData;
};


template <class	T>
CShmStruct<T>::CShmStruct()
{
	m_pData = NULL;
}

template <class	T>
CShmStruct<T>::~CShmStruct()
{
}

template <class	T>
int CShmStruct<T>::Create(int iShmKey, size_t dwShmSize, bool& bNeedInit)
{
	size_t needSize = sizeof(T) + sizeof(CShmStruct<T>::_DataInfo);
	void* pShmMem = NULL;
	
	bNeedInit = false;
	if (iShmKey > 0)
	{
		pShmMem = CShmemMgr::CreateShmem(iShmKey, dwShmSize, needSize);
		if (NULL == pShmMem)
		{
			LOG_ERR("CreateShmem failed, ShmKey[%d] ShmSize[%u] NeedSize[%u]",
				iShmKey, (uint32_t)dwShmSize, (uint32_t)needSize);
			return -1;
		}

		bNeedInit = (ESSM_Init == CShmemMgr::GetStartFlag(iShmKey));
	}
	else
	{
		pShmMem = malloc(needSize);
		if (NULL == pShmMem)
		{
			LOG_ERR("memory malloc failed, NeedSize[%u]", (uint32_t)needSize);
			return -1;
		}
		
		bNeedInit = true;
	}
	
	if (bNeedInit)
	{
		memset(pShmMem, 0, needSize);
		m_pData = (_DataInfo*)pShmMem;
		m_pData->iDataSize = needSize;		
	}
	else
	{
		m_pData = (_DataInfo*)pShmMem;
		if (m_pData->iDataSize != needSize)
		{
			LOG_ERR("CShmStruct create failed, DataSize not match[%d-%d]", needSize, m_pData->iDataSize);
			return -1;
		}
	}
	
	return 0;
}

#endif


