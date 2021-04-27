/* 
*  共享对象内存池
*/

#include "CShmDataPool.h"

#pragma pack(1)

///内存池的头部
struct ShmDataWrapHead
{
	int iMemId;
	char data[0];
};

#pragma pack()

CShmDataPool::CShmDataPool()
{
	m_pMemPool = NULL;
	m_iFindIndex = -1;
	m_iShmKey = 0;
	m_pMemPtr = NULL;
	m_unitSize = 0;
}

CShmDataPool::~CShmDataPool()
{
	if (m_pMemPool != NULL)
	{
		mempool_destroy(&m_pMemPool);
	}
	
	if (m_pMemPtr != NULL)
	{
		free(m_pMemPtr);
		m_pMemPtr = NULL;
	}
}

int CShmDataPool::Create(int iShmKey, size_t dwShmSize, size_t dwDataUnitSize, size_t dwMaxObjCount)
{
	if (dwMaxObjCount <= 0)
	{
		LOG_ERR("CShmObjPool<T>::Create failed, MaxObjCount[%uz]", dwMaxObjCount);
		return -1;
	}
	
	size_t dwRealUnitSize = dwDataUnitSize + sizeof(ShmDataWrapHead);	
	size_t needSize = MEMPOOL_CALC(dwMaxObjCount, dwRealUnitSize);
	LOG_ERR("shm data pool UnitSize[%d] count[%d] needsize[%.2fM]", dwDataUnitSize, dwMaxObjCount, needSize * 1.0 / 1000000);
	
	void* pShmMem = NULL;
	int iRet = 0;

	if (iShmKey != 0)
	{
		pShmMem = CShmemMgr::CreateShmem(iShmKey, dwShmSize, needSize);

		if (NULL == pShmMem)
		{
			LOG_ERR("CreateShmem failed, ShmKey[%d] ShmSize[%zu] NeedSize[%zu]",			 
				iShmKey, dwShmSize, needSize);
			return -2;
		}	
		
		if (ESSM_Init == CShmemMgr::GetStartFlag(iShmKey))
		{
			iRet = mempool_init(&m_pMemPool, dwMaxObjCount, dwRealUnitSize, pShmMem, needSize);
			ASSERT_RET(0 == iRet, iRet);
		}
		else
		{
			iRet = mempool_attach(&m_pMemPool, dwMaxObjCount, dwRealUnitSize, pShmMem, needSize);
			if (iRet != 0)
			{
				LOG_ERR("tmempool_attach failed, ShmKey[%d] ShmSize[%zu] NeedSize[%zu]", 
					iShmKey, dwShmSize, needSize);
				return -3;
			}			
		}
	}
	else
	{	
		pShmMem = malloc(needSize);
		m_pMemPtr = pShmMem;

		iRet = mempool_init(&m_pMemPool, dwMaxObjCount, dwRealUnitSize, pShmMem, needSize);
		ASSERT_RET(0 == iRet, iRet);
	}
	
	m_unitSize = dwDataUnitSize;
	return 0;
}

void* CShmDataPool::Alloc()
{
	ASSERT(m_pMemPool);
	
	int idx = mempool_alloc(m_pMemPool);
	ASSERT_RET(idx >= 0, NULL);

	void* pUnitMem = mempool_get(m_pMemPool, idx);
	ASSERT(pUnitMem);

	ShmDataWrapHead* pHead = (ShmDataWrapHead*)pUnitMem;
	
	//为了防止objid为0，这儿统一加1
	pHead->iMemId = idx+1;	
	return pHead->data;
}


void * CShmDataPool::GetObj(int iMemId)
{
 	ASSERT(m_pMemPool);
	int idx = iMemId - 1;
	ASSERT_RET(idx >= 0, NULL);
 	void* pUnitMem = mempool_get(m_pMemPool, idx);
 	ASSERT(pUnitMem);
 	ShmDataWrapHead* pHead = (ShmDataWrapHead*)pUnitMem;
 
 	return pHead->data;
}

int CShmDataPool::GetObjId(void * pMem)
{
	ShmDataWrapHead* pHead = (ShmDataWrapHead*)((char*)pMem - sizeof(ShmDataWrapHead));
	ASSERT_RET(pHead, 0);
	return pHead->iMemId;
}


void CShmDataPool::Free(void* pData)
{
	ASSERT(m_pMemPool);

	ShmDataWrapHead* pHead = (ShmDataWrapHead*)((char*)pData - sizeof(ShmDataWrapHead));

	int iPoolId = pHead->iMemId-1;
	void* pUnitMem = mempool_get(m_pMemPool, iPoolId);
	ASSERT_RET(pUnitMem && pUnitMem == (void*)pHead);
	
	mempool_free(m_pMemPool, iPoolId);
}
