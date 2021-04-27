/* 
*  共享对象链表结构内存池
*/

#ifndef CSHMLIST_H__
#define CSHMLIST_H__

#include <new>
#include "comm_inc.h"
#include "CShmemMgr.h"
#include "CShmObjPool.h"
#include "CShmHash.h"


template<class KeyT, class ValueT>
class CShmList
{
public:
	CShmList()
	{
	}
	
	int Create(int iShmKey, size_t dwShmSize, int iMaxKeyCount, int iMaxUnitCount);
	
	ValueT* Push(KeyT key);
	void Remove(KeyT key, ValueT* pValue);
	void Remove(KeyT key, int iObjID);
	void Clear(KeyT key);

	int Size(KeyT key);
	void First(KeyT key);
	ValueT* Next(KeyT key);

	///直接通过数据的内存ID来
	ValueT* GetDataByID(int iObjID);
	
	//表示是热启动的标记位
	bool GetResumeFlag() { return m_listEntry.GetResumeFlag(); };
	

	void Clear() { m_listEntry.Clear(); m_dataPool.FreeAllObject(); }

	int GetUsed() {return m_dataPool.GetUsed();}
	
protected:

	class InnerEntry
	{
	public:
		int iNext;	//第一个节点
		int iLast;	//最后一个节点
		int iCount;

		//用于First,Next
		int iCurrObjID;
	};
	
	class InnerData : public CShmBaseObj
	{
	public:
		KeyT key;	//用于错误校验
		ValueT value;
		int iNextObjID;
		int iPrevObjID;

	public:
		virtual int Init()
		{
			iNextObjID = -1;
			iPrevObjID = -1;
			value.m_iObjID = m_iObjID;
			
			return value.Init();
		}
		
		virtual void Resume()
		{
			return value.Resume();
		}
		virtual void Free()
		{
			value.Free();
		}
	};
	
protected:

	CShmHashTable<KeyT, InnerEntry> m_listEntry;
	CShmObjPool<InnerData> m_dataPool;
};

template<class KeyT, class ValueT>
int CShmList<KeyT, ValueT>::Create(int iShmKey, size_t dwShmSize, int iMaxKeyCount, int iMaxUnitCount)
{
	int iRet = m_dataPool.Create(iShmKey, dwShmSize, iMaxUnitCount);
	if (iRet != 0)
	{
		return iRet;
	}

	iRet = m_listEntry.Create(iShmKey, dwShmSize, iMaxKeyCount);
	if (iRet != 0)
	{
		return iRet;
	}
	
	return 0;
}

template<class KeyT, class ValueT>
ValueT* CShmList<KeyT, ValueT>::Push(KeyT key)
{
	CShmList<KeyT, ValueT>::InnerEntry* pEntry = m_listEntry.Find(key);
	bool bInit = false;
	if (!pEntry)
	{
		bInit = true;
		
		pEntry = m_listEntry.Insert(key);
		if (!pEntry)
		{
			LOG_ERR("List is full, alloc entry failed");
			return NULL;
		}

		//Init list entry
		pEntry->iNext = -1;
		pEntry->iLast = -1;
		pEntry->iCurrObjID = -1;
		pEntry->iCount = 0;
	}
	
	CShmList<KeyT, ValueT>::InnerData* pInnerData = m_dataPool.AllocObj();
	if (!pInnerData)
	{
		LOG_ERR("List is full, alloc data pool failed");

		if (bInit)
		{
			m_listEntry.Remove(key);
		}
		
		return NULL;
	}

	//记录下key,用于错误校验
	pInnerData->key = key;
	
	//链表的头部
	if (pEntry->iNext < 0)
	{
		ASSERT(pEntry->iLast < 0);
		pEntry->iNext = pInnerData->m_iObjID;
		pEntry->iLast = pInnerData->m_iObjID;
		
		pEntry->iCount++;
	}
	else
	{
	/*
		CShmList<KeyT, ValueT>::InnerData* pPrevNode = NULL;
		int iNext = pEntry->iNext;
		while(iNext >= 0)
		{
			pPrevNode = m_dataPool.FindObj(iNext);
			ASSERT(pPrevNode);
			
			iNext = pPrevNode->iNextObjID;
		}
		
		pPrevNode->iNextObjID = pInnerData->m_iObjID;
		pInnerData->iPrevObjID = pPrevNode->m_iObjID;
		pInnerData->iNextObjID = -1;
		*/

		//获取到最后一个节点
		ASSERT(pEntry->iLast > 0);
		CShmList<KeyT, ValueT>::InnerData* pLastNode = m_dataPool.FindObj(pEntry->iLast);
		ASSERT(pLastNode != NULL);
		ASSERT(pLastNode->iNextObjID < 0);
		
		pLastNode->iNextObjID = pInnerData->m_iObjID;
		pInnerData->iPrevObjID = pLastNode->m_iObjID;
		pEntry->iLast = pInnerData->m_iObjID;
		
		pEntry->iCount++;
	}

	if (1 == pEntry->iCount)
	{
		First(key);
	}
	
	return &pInnerData->value;
}


template<class KeyT, class ValueT>
void CShmList<KeyT, ValueT>::Remove(KeyT key, ValueT* pValue)
{
	int iCurrObjID = pValue->m_iObjID;
	Remove(key, iCurrObjID);
}

template<class KeyT, class ValueT>
void CShmList<KeyT, ValueT>::Remove(KeyT key, int iRmvObjID)
{
	int iCurrObjID = iRmvObjID;
	CShmList<KeyT, ValueT>::InnerEntry* pEntry = m_listEntry.Find(key);
	if (!pEntry)
	{
		return;
	}
	
	CShmList<KeyT, ValueT>::InnerData* pCurrNode = m_dataPool.FindObj(iCurrObjID);
	if (!pCurrNode || 
		pCurrNode->key != key)
	{
		LOG_ERR("value has not in shm CurrObjID[%d] CurrNode[%p]", iCurrObjID, pCurrNode);
		return;
	}
	
	//开始删除该节点
	int iPrevObjID = pCurrNode->iPrevObjID;
	int iNextObjID = pCurrNode->iNextObjID;
	CShmList<KeyT, ValueT>::InnerData* pPrevNode = iPrevObjID > 0 ?
												m_dataPool.FindObj(iPrevObjID):NULL;
	CShmList<KeyT, ValueT>::InnerData* pNextNode = iNextObjID > 0 ?
												m_dataPool.FindObj(iNextObjID):NULL;

	//如果是第一个节点，那么肯定也是最后一个节点
	if (pPrevNode)
	{
		pPrevNode->iNextObjID = iNextObjID;
	}
	if (pNextNode)
	{
		pNextNode->iPrevObjID = iPrevObjID;
	}

	//更新索引数据
	if (pEntry->iNext == iCurrObjID)
	{
		pEntry->iNext = iNextObjID;
	}
	if (pEntry->iLast == iCurrObjID)
	{
		pEntry->iLast = iPrevObjID;
	}
	if (pEntry->iCurrObjID == iCurrObjID)
	{
		pEntry->iCurrObjID = iNextObjID;
	}

	m_dataPool.FreeObj(iCurrObjID);
	pEntry->iCount--;

	///如果该key所有节点数据都被释放了，那么key的索引也释放掉
	ASSERT(pEntry->iCount >= 0);
	if (pEntry->iCount <= 0)
	{
		Clear(key);
	}	
}

template<class KeyT, class ValueT>
void CShmList<KeyT, ValueT>::Clear(KeyT key)
{
	CShmList<KeyT, ValueT>::InnerEntry* pEntry = m_listEntry.Find(key);
	if (!pEntry)
	{
		return;
	}
	
	CShmList<KeyT, ValueT>::InnerData* pCurrNode = NULL;
	int iCurr = pEntry->iNext;
	while(iCurr >= 0)
	{
		pCurrNode = m_dataPool.FindObj(iCurr);
		ASSERT(pCurrNode);
		ASSERT(pCurrNode->key == key);
		
		iCurr = pCurrNode->iNextObjID;
		m_dataPool.FreeObj(pCurrNode);
	}

	m_listEntry.Remove(key);
}

template<class KeyT, class ValueT>
int CShmList<KeyT, ValueT>::Size(KeyT key)
{
	CShmList<KeyT, ValueT>::InnerEntry* pEntry = m_listEntry.Find(key);
	if (!pEntry)
	{
		return 0;
	}
	
	return pEntry->iCount;
}

template<class KeyT, class ValueT>
void CShmList<KeyT, ValueT>::First(KeyT key)
{
	CShmList<KeyT, ValueT>::InnerEntry* pEntry = m_listEntry.Find(key);
	if (!pEntry)
	{
		return;
	}

	pEntry->iCurrObjID = pEntry->iNext;
}

template<class KeyT, class ValueT>
ValueT* CShmList<KeyT, ValueT>::Next(KeyT key)
{
	CShmList<KeyT, ValueT>::InnerEntry* pEntry = m_listEntry.Find(key);
	if (!pEntry)
	{
		return NULL;
	}

	if (pEntry->iCurrObjID <= 0)
	{
		return NULL;
	}

	CShmList<KeyT, ValueT>::InnerData* pCurrData =	m_dataPool.FindObj(pEntry->iCurrObjID);
	ASSERT_RET(pCurrData, NULL);

	pEntry->iCurrObjID = pCurrData->iNextObjID;

	return &pCurrData->value;
}

///直接通过数据的内存ID来
template<class KeyT, class ValueT>
ValueT* CShmList<KeyT, ValueT>::GetDataByID(int iObjID)
{
	CShmList<KeyT, ValueT>::InnerData* pInnerData = m_dataPool.FindObj(iObjID);
	if (pInnerData)
	{
		return &pInnerData->value;
	}
	
	return NULL;
}

#endif