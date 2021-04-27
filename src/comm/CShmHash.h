/* 
*  共享对象内存池
*/

#ifndef CSHMHASHTABLE
#define CSHMHASHTABLE

#include <new>
#include "comm_inc.h"
#include "comm/shtable.h"
#include "CShmemMgr.h"
#include <typeinfo>
#include "stdint.h"


template<class KeyT, class ValueT>
class CShmHashTable
{
public:
	CShmHashTable()
	{
		m_hash = NULL;
		m_pShmMem = NULL;
		m_iMaxUnitCount = 0;
	}
	
	int Create(int iShmKey, size_t dwShmSize, int iMaxUnitCount);
	ValueT* Find(KeyT key);
	ValueT* Insert(KeyT key);
	ValueT* At(int idx);
	void Remove(KeyT key);
	void Clear();
	int Size() {return m_pShmMem->iUnitCount;}
	int MaxSize() {return m_iMaxUnitCount;}
	
	//表示是热启动的标记位
	bool GetResumeFlag() { return m_bResumeFlag; };
	typedef void(*pFnDump) (KeyT *, ValueT *, void *);
	void DumpAll(pFnDump pFn, void * pUserData);
	void DumpCBFunc(const void * pUserData);
	
	struct DumpCBData
	{
		pFnDump pFn;
		CShmHashTable<KeyT, ValueT> * pHashTable;
		void * pUserData;
	};

	static DumpCBData m_oDumpCbData;



protected:
	static int ShtCmp(const void* pvData1, const void* pvData2);
	static unsigned int ShtHash(const void* pvData);

	
protected:
	struct InnerData
	{
		KeyT key;
		uint32_t dwBucket;
		ValueT value;
	};

	#pragma pack(1)
	struct HashHeadData
	{
		int iUnitCount;
		char hash[0];
	};
	#pragma pack()
	
protected:
	//表示是热启动的标记位
	bool m_bResumeFlag;

	LPSHTABLE m_hash;
	//void* m_pShmMem;
	HashHeadData* m_pShmMem;
	int m_iMaxUnitCount;
};


template<class KeyT, class ValueT>
int CShmHashTable<KeyT, ValueT>::Create(int iShmKey, size_t dwShmSize, int iMaxUnitCount)
{
	int iBucket = iMaxUnitCount;
	int iUnitSize = sizeof(CShmHashTable<KeyT, ValueT>::InnerData);
	size_t hashSize = SHT_SIZE(iBucket, iMaxUnitCount, iUnitSize);
	size_t needSize = hashSize + sizeof(HashHeadData);
	void* pShmMem = CShmemMgr::CreateShmem(iShmKey, dwShmSize, needSize);

	LOG_ERR("shm hash obj type[%s] count[%d] needsize[%.2fM]", typeid(*this).name(), iMaxUnitCount, needSize * 1.0 / 1000000);

	if (NULL == pShmMem)
	{
		LOG_ERR("CreateShmem failed, ShmKey[%d] ShmSize[%u] NeedSize[%u]",
			iShmKey, (uint32_t)dwShmSize, (uint32_t)needSize);
		return -1;
	}
	
	m_pShmMem = (HashHeadData*)pShmMem;
	
	m_iMaxUnitCount = iMaxUnitCount;
	
	int iRet = 0;
	if (ESSM_Init == CShmemMgr::GetStartFlag(iShmKey))
	{
		m_bResumeFlag = false;

		m_hash = sht_init(m_pShmMem->hash, hashSize, iBucket, iMaxUnitCount, iUnitSize);
		m_pShmMem->iUnitCount = 0;

	}
	else
	{
		m_bResumeFlag = true;

		m_hash = sht_attach(m_pShmMem->hash, hashSize, iBucket, iMaxUnitCount, iUnitSize);
	}
	
	if (m_hash == NULL)
	{
		LOG_ERR("sht_init or attach failed, UnitCount[%d]", iMaxUnitCount);
		return -1;
	}
	
	return 0;
}

template<class KeyT, class ValueT>
ValueT* CShmHashTable<KeyT, ValueT>::Find(KeyT key)
{
	InnerData innerData;
	innerData.key = key;
	innerData.dwBucket = m_iMaxUnitCount;
	
	InnerData* pExist = (InnerData*)sht_find(m_hash, &innerData, ShtCmp, ShtHash);
	if (pExist != NULL)
	{
		return &pExist->value;
	}
	
	return NULL;
}

template<class KeyT, class ValueT>
ValueT* CShmHashTable<KeyT, ValueT>::Insert(KeyT key)
{
	InnerData innerData;
	innerData.key = key;
	innerData.dwBucket = m_iMaxUnitCount;
	
	InnerData* pNewData = (InnerData*)sht_insert_unique(m_hash, &innerData, ShtCmp, ShtHash);
	if (NULL == pNewData)
	{
		ASSERT(pNewData != NULL);
		return NULL;
	}

	m_pShmMem->iUnitCount++;
	memcpy(pNewData, &innerData, sizeof(InnerData));
	pNewData->dwBucket = m_iMaxUnitCount;
	
	return &pNewData->value;
}

template<class KeyT, class ValueT>
ValueT* CShmHashTable<KeyT, ValueT>::At(int idx)
{
	ASSERT_RET(idx >= 0 && idx < m_pShmMem->iUnitCount, NULL);
	int iValid = 0;
	InnerData* pExist = (InnerData*)sht_pos(m_hash, idx, &iValid);
	if (pExist == NULL || iValid != 1)
	{
		return NULL;
	}

	return &pExist->value;
}

template<class KeyT, class ValueT>
void CShmHashTable<KeyT, ValueT>::Remove(KeyT key)
{
	InnerData innerData;
	innerData.key = key;
	innerData.dwBucket = m_iMaxUnitCount;
	
	if (sht_remove(m_hash, &innerData, ShtCmp, ShtHash))
	{
		m_pShmMem->iUnitCount--;
	}
}


template<class KeyT, class ValueT>
int CShmHashTable<KeyT, ValueT>::ShtCmp(const void* pvData1, const void* pvData2)
{
	InnerData* pData1 = (InnerData*)pvData1;
	InnerData* pData2 = (InnerData*)pvData2;
	
	KeyT &key1 = pData1->key;
	KeyT &key2 = pData2->key;

	//LOG_ERR("ShtCmp key: %d-%d", (int)key1, (int)key2);

	return key1 == key2 ? 0: ((key1 < key2)?-1:1);
}

template<class KeyT, class ValueT>
unsigned int CShmHashTable<KeyT, ValueT>::ShtHash(const void* pvData)
{
	const InnerData* pData = (const InnerData*)pvData;

	//LOG_ERR("ShtHash key: %d", (int)pData->key);
	return (unsigned int)(pData->key%pData->dwBucket);
}

template<class KeyT, class ValueT>
void CShmHashTable<KeyT, ValueT>::Clear()
{
	if (m_pShmMem)
	{
		int iBucket = m_iMaxUnitCount;
		int iUnitSize = sizeof(CShmHashTable<KeyT, ValueT>::InnerData);
		size_t needSize = SHT_SIZE(iBucket, m_iMaxUnitCount, iUnitSize);
		bzero(m_pShmMem->hash, needSize);
		m_hash = sht_init(m_pShmMem->hash, needSize, iBucket, m_iMaxUnitCount, iUnitSize);	

		m_pShmMem->iUnitCount = 0;
	}
}

template<typename KeyT, typename ValueT>
int DumpHashTable(FILE* fp, const void* pvData)
{
	//CShmHashTable<KeyT, ValueT>::m_oDumpCbData.pFn;
	CShmHashTable<KeyT, ValueT> * pTable = CShmHashTable<KeyT, ValueT>::m_oDumpCbData.pHashTable;
	pTable->DumpCBFunc(pvData);
	//CShmHashTable<KeyT, ValueT>::m_pFnDump;

	//CShmHashTable<KeyT, ValueT> * pTable = (CShmHashTable<KeyT, ValueT> *)fp;
	//pTable->DumpCBFunc(pvData);
	return 0;
}

template<class KeyT, class ValueT>
void CShmHashTable<KeyT, ValueT>::DumpAll(pFnDump pFn, void * pUserData)
{
	m_oDumpCbData.pFn = pFn;
	m_oDumpCbData.pHashTable = this;
	m_oDumpCbData.pUserData = pUserData;
	
	FILE * pFile = fopen("/dev/null", "w+");
	sht_dump_valid(m_hash, (FILE *)pFile, ::DumpHashTable<KeyT, ValueT>);
	fclose(pFile);
}

template<class KeyT, class ValueT>
void CShmHashTable<KeyT, ValueT>::DumpCBFunc(const void * pUserData)
{
 	InnerData * pData = (InnerData *)pUserData;
 	m_oDumpCbData.pFn(&(pData->key), &(pData->value), m_oDumpCbData.pUserData);
};

template<typename KeyT, typename ValueT>
typename CShmHashTable<KeyT, ValueT>::DumpCBData CShmHashTable<KeyT, ValueT>::m_oDumpCbData;

#endif


