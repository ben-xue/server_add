/* 
*  快速版本的内存池
*/

#ifndef CSHMFASTPOOL_H__
#define CSHMFASTPOOL_H__

#include <new>
#include "comm_inc.h"
#include "CShmemMgr.h"
#include "CShmObjPool.h"
#include "CShmHash.h"
#include "CShmStruct.h"


template<class ValueT>
class CShmFastPool
{
public:
	CShmFastPool()
	{
	}
	
	int Create(int iShmKey, size_t dwShmSize, size_t iMaxUnitCount);

    /**
	* 分布tick需要设置此值
	*/
	void SetSlowCnt(int iCnt){m_iSlowCnt=iCnt;}
	
	ValueT* AllocObj();
	
	/**
	* 释放内存池里的对象
	*/
	void FreeObj(ValueT* pObj);
	void FreeObj(int iObjID);	

	/**
	* 查找对象
	*/
	ValueT* FindObj(int iObjID);

	int GetUsed(){return m_dataPool.GetUsed();}

	/**
	* 用于轮询内存池里的所有对象
	*/
	void FirstObj();
	ValueT* NextObj();

    void    GetSlowObj();
	void    StartGetSlowObj();
	ValueT* NextSlowObj();
	
	void FreeAllObject()
	{
		FirstObj();
		for (ValueT* curr = NextObj(); curr != NULL; curr = NextObj())
		{
			FreeObj(curr);
		}
	}
	
	bool IsFirstObject(ValueT * pValue);


protected:

	class InnerEntry
	{
	public:
		int iNext;	//第一个节点
		int iLast;	//最后一个节点
		int iCount;

		//用于First,Next
		int iCurrObjID;
		int iSlowCurrObjID;
	};
	
	class InnerData : public CShmBaseObj
	{
	public:
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
	InnerEntry* GetEntry()
	{
		return m_entry.Get();
	}
	
protected:
	CShmStruct<InnerEntry> m_entry;
	CShmObjPool<InnerData> m_dataPool;
	int m_iSlowCnt;
	int m_iFindSlowCnt;
};

template<class ValueT>
int CShmFastPool<ValueT>::Create(int iShmKey, size_t dwShmSize, size_t dwMaxObjCount)
{	
	///只有一个入口
	bool bNeedInit;
	int iRet = m_entry.Create(iShmKey, dwShmSize, bNeedInit);
	if (iRet != 0)
	{
		return iRet;
	}
	if (bNeedInit)
	{
		///判断是否第一次创建
		InnerEntry* pEntry = GetEntry();
		pEntry->iNext = -1;
		pEntry->iLast = -1;
		pEntry->iCurrObjID = -1;
		pEntry->iSlowCurrObjID = -1;
	}	
	
	iRet = m_dataPool.Create(iShmKey, dwShmSize, dwMaxObjCount);
	if (iRet != 0)
	{
		return iRet;
	}
	
	return 0;
}

template<class ValueT>	
ValueT* CShmFastPool<ValueT>::AllocObj()
{
	CShmFastPool<ValueT>::InnerEntry* pEntry = GetEntry();		
	CShmFastPool<ValueT>::InnerData* pInnerData = m_dataPool.AllocObj();
	if (!pInnerData)
	{
		LOG_ERR("List is full, alloc data pool failed");		
		return NULL;
	}
		
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
		CShmFastPool<ValueT>::InnerData* pPrevNode = NULL;
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
		CShmFastPool<ValueT>::InnerData* pLastNode = m_dataPool.FindObj(pEntry->iLast);
		ASSERT(pLastNode != NULL);
		ASSERT(pLastNode->iNextObjID < 0);
		
		pLastNode->iNextObjID = pInnerData->m_iObjID;
		pInnerData->iPrevObjID = pLastNode->m_iObjID;
		pEntry->iLast = pInnerData->m_iObjID;
		
		pEntry->iCount++;
	}

	if (1 == pEntry->iCount)
	{
		FirstObj();
		GetSlowObj();
	}
	
	return &pInnerData->value;
}


template<class ValueT>
void CShmFastPool<ValueT>::FreeObj(ValueT* pValue)
{
	int iCurrObjID = pValue->m_iObjID;
	FreeObj(iCurrObjID);
}

template<class ValueT>
void CShmFastPool<ValueT>::FreeObj(int iRmvObjID)
{
	int iCurrObjID = iRmvObjID;
	CShmFastPool<ValueT>::InnerEntry* pEntry = GetEntry();
	CShmFastPool<ValueT>::InnerData* pCurrNode = m_dataPool.FindObj(iCurrObjID);
	if (!pCurrNode)
	{
		LOG_ERR("value has not in shm CurrObjID[%d] CurrNode[%p]", iCurrObjID, pCurrNode);
		return;
	}
	
	//开始删除该节点
	int iPrevObjID = pCurrNode->iPrevObjID;
	int iNextObjID = pCurrNode->iNextObjID;
	CShmFastPool<ValueT>::InnerData* pPrevNode = iPrevObjID > 0 ?												
		m_dataPool.FindObj(iPrevObjID):NULL;
	CShmFastPool<ValueT>::InnerData* pNextNode = iNextObjID > 0 ?
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
	// add
	if (pEntry->iSlowCurrObjID == iCurrObjID)
	{
		pEntry->iSlowCurrObjID = iNextObjID;
	}

	m_dataPool.FreeObj(iCurrObjID);
	pEntry->iCount--;

	///如果该key所有节点数据都被释放了，那么key的索引也释放掉
	ASSERT(pEntry->iCount >= 0);
}

template<class ValueT>
void CShmFastPool<ValueT>::FirstObj()
{
	CShmFastPool<ValueT>::InnerEntry* pEntry = GetEntry();
	pEntry->iCurrObjID = pEntry->iNext;
}

template<class ValueT>
ValueT* CShmFastPool<ValueT>::NextObj()
{
	CShmFastPool<ValueT>::InnerEntry* pEntry = GetEntry();
	if (pEntry->iCurrObjID <= 0)
	{
		return NULL;
	}
	
	CShmFastPool<ValueT>::InnerData* pCurrData = m_dataPool.FindObj(pEntry->iCurrObjID);
	ASSERT_RET(pCurrData, NULL);

	pEntry->iCurrObjID = pCurrData->iNextObjID;

	return &pCurrData->value;
}

template <class	ValueT>
void CShmFastPool<ValueT>::StartGetSlowObj()
{
	CShmFastPool<ValueT>::InnerEntry* pEntry = GetEntry();
	if( pEntry->iSlowCurrObjID <= 0 )
	{
	    pEntry->iSlowCurrObjID = pEntry->iNext;
	}
	m_iFindSlowCnt = 0;
}

template <class	ValueT>
void CShmFastPool<ValueT>::GetSlowObj()
{
	StartGetSlowObj();
}

template <class	ValueT>
ValueT* CShmFastPool<ValueT>::NextSlowObj()
{	
    CShmFastPool<ValueT>::InnerEntry* pEntry = GetEntry();
    if( m_iFindSlowCnt >= m_iSlowCnt || pEntry->iSlowCurrObjID <= 0 )
    {   
        return NULL;
    }
    
	CShmFastPool<ValueT>::InnerData* pCurrData = m_dataPool.FindObj(pEntry->iSlowCurrObjID);
	ASSERT_RET(pCurrData, NULL);

    pEntry->iSlowCurrObjID = pCurrData->iNextObjID;

	m_iFindSlowCnt++;
	return &pCurrData->value;
}

///直接通过数据的内存ID来
template<class ValueT>
ValueT* CShmFastPool<ValueT>::FindObj(int iObjID)
{
	CShmFastPool<ValueT>::InnerData* pInnerData = m_dataPool.FindObj(iObjID);
	if (pInnerData)
	{
		return &pInnerData->value;
	}
	
	return NULL;
}

template<class ValueT>
bool CShmFastPool<ValueT>::IsFirstObject(ValueT * pValue)
{
	bool bRet = false;
	CShmFastPool<ValueT>::InnerEntry* pEntry = GetEntry();
	if (pEntry->iCount <= 0)
	{
		return bRet;
	}
	if (pEntry->iNext == pValue->m_iObjID)
	{
		bRet = true;
	}
	
	return bRet;
}



#endif


