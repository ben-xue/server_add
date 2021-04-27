
#include "comm_inc.h"
#include "CSingleton.h"
#include "CShareKvData.h"

int CShareKvData::KVWrapClass::Init()
{
	bzero(&m_data, sizeof(m_data));
	return 0;
}

void CShareKvData::KVWrapClass::Resume()
{
}

void CShareKvData::KVWrapClass::Free()
{
}


int CShareKvData::Init(int iShmKey, uint64_t ullKeySize)
{
	int iMax = XG_KEY_MAX * 2;
	int iRet = m_kvPool.Create(iShmKey, ullKeySize, iMax);
	if (iRet != 0)
	{
		LOG_ERR("m_kvPool.Create(%d,%"PRIu64",%d) failed, ret[%d]", iShmKey, ullKeySize, iMax, iRet);
		return -1;
	}
	
	iRet = m_kvMap.Create(iShmKey, ullKeySize, iMax);
	if (iRet != 0)
	{
		LOG_ERR("m_kvMap.Create(%d,%"PRIu64",%d) failed, ret[%d]", iShmKey, ullKeySize, iMax, iRet);
		return -1;
	}

	return 0;
}

XGKvData* CShareKvData::GetData(int iKey)
{
	int* piMemID = m_kvMap.Find(iKey);
	if (piMemID != NULL)
	{
		return &(m_kvPool.FindObj(*piMemID)->m_data);
	}
	else
	{
		return CreateData(iKey);
	}
}

XGKvData* CShareKvData::CreateData(int iKey)
{
	CShareKvData::KVWrapClass* pFree = m_kvPool.AllocObj();
	if (NULL == pFree)
	{
		LOG_ERR("m_kvPool::AllocObj failed, TotalCount[%d]", m_kvPool.GetUsed());
		return NULL;
	}
	
	int* pMemID = m_kvMap.Insert(iKey);
	if (NULL == pMemID)
	{
		ASSERT(pMemID != NULL);
		m_kvPool.FreeObj(pFree);
		return NULL;
	}
	
	*pMemID = pFree->m_iObjID;
	return &pFree->m_data;
}


