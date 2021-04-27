/* 
*  共享对象内存池
*/

#ifndef CSHMSORTARRAYPOOL
#define CSHMSORTARRAYPOOL

#include <new>
#include "comm_inc.h"
//#include "comm/tmempool.h"
#include "CShmArray.h"
#include "CShmHash.h"

//排名状态
enum RANK_STATS
{
	EM_RANK_STATS_NULL = 0,	//未变动
	EM_RANK_STATS_UP = 1,	//排名上升
	EM_RANK_STATS_DOWN = 2,	//排名下降
};

template<class KeyT, class T>
class CShmSortArray
{
public:
	CShmSortArray();
	virtual ~CShmSortArray();
	
	/**
	* 创建内存池
	* CanDrop - 当数值满后，是否可以丢弃最小的那个元素，一般排行榜用的都是true
	*/
	int Create(int iShmKey, size_t dwShmSize, int iArraySize);
	T* Get(int iIndex);
	int Size() {return m_pData->iArraySize;}
	int GetMaxSize() {return m_pData->iMaxArraySize;}
	int GetIndexByKey(KeyT key);

	virtual int Update(T* t,bool bRecalcRankChange);

	void Clear()
	{
		m_pData->iArraySize = 0;
		m_hashCheck.Clear();
	}

	void SetSize(int iNewSize)
	{
		m_pData->iArraySize = iNewSize;
	}
	
	const T* GetBuff(){return m_array.GetBuff();}

	int CalcRankChange(T *t,int iIndexBeforn,bool bRecalcRankChange);
	void AdjustOthersRank(int iStartRank,int iEndRank,int iChange);
	int DoInsert(T* t,int iIndexBeforn,bool bRecalcRankChange);
	void DumpInfo(T* t);
	void DoRemove(T* t);

	int *GetArrayByIndex(KeyT key)
	{
		return m_hashCheck.Find(key);
	}
protected:
	virtual int Compare(T*t1, T*t2) = 0;
	virtual KeyT GetKey(T* t) = 0;
	virtual uint64_t GetRankValueByRankType(T* t) = 0;

protected:
	int* GetArrayIndex(T* t)
	{
		return m_hashCheck.Find(GetKey(t));
	}

	struct _ArrayData
	{
		int iArraySize;
		int iMaxArraySize;
	};
	
protected:
	_ArrayData* m_pData;
	CShmArray<T> m_array;
	CShmHashTable<KeyT, int> m_hashCheck;	//用来做是否存在判断
};


template <class KeyT, class T>
CShmSortArray<KeyT, T>::CShmSortArray()
{
	m_pData = NULL;
}

template <class KeyT, class T>
CShmSortArray<KeyT, T>::~CShmSortArray()
{
}

template <class KeyT, class T>
int CShmSortArray<KeyT, T>::Create(int iShmKey, size_t dwShmSize, int iArraySize)
{
	size_t needSize = sizeof(_ArrayData);
	void* pShmMem = CShmemMgr::CreateShmem(iShmKey, dwShmSize, needSize);
	if (NULL == pShmMem)
	{
		LOG_ERR("CreateShmem failed, ShmKey[%d] ShmSize[%u] NeedSize[%u]",
			iShmKey, (uint32_t)dwShmSize, (uint32_t)needSize);
		return -1;
	}
	
	int iRet = 0;
	if (ESSM_Init == CShmemMgr::GetStartFlag(iShmKey))
	{
		memset(pShmMem, 0, needSize);
		m_pData = (_ArrayData*)pShmMem;
		m_pData->iMaxArraySize = iArraySize;
	}
	else
	{
		m_pData = (_ArrayData*)pShmMem;
		if (m_pData->iMaxArraySize != iArraySize)
		{
			LOG_ERR("CShmArray create failed, ArraySize not match[%d-%d]", iArraySize, m_pData->iMaxArraySize);
			return -1;
		}
	}
		
	iRet = m_array.Create(iShmKey, dwShmSize, iArraySize);
	if (iRet != 0)
	{
		LOG_ERR("CShmArray create failed");
		return iRet;
	}
	
	iRet = m_hashCheck.Create(iShmKey, dwShmSize, iArraySize*2);
	return iRet;
}

template <class KeyT, class T>
T* CShmSortArray<KeyT, T>::Get(int iIndex)
{
	return m_array.Get(iIndex);
}


template <class KeyT, class T>
int CShmSortArray<KeyT, T>::GetIndexByKey(KeyT key)
{
	int* pIndex = m_hashCheck.Find(key);
	if (pIndex)
	{
		return *pIndex;
	}

	return -1;
}


template <class KeyT, class T>
int CShmSortArray<KeyT, T>::Update(T* t,bool bRecalcRankChange)
{
	KeyT key = GetKey(t);

	//DumpInfo(t);

	//int iIndexBeforn = GetMaxSize();
	int iIndexBeforn = Size();
	int *tBefornIndex = m_hashCheck.Find(key);
	if (tBefornIndex != NULL)
	{
		//更新
		iIndexBeforn = *tBefornIndex;
		LOG_DBG("tBefornIndex[%d]",iIndexBeforn);
		uint64_t uCurrentValue = GetRankValueByRankType(t);
		T *pBefornFsRankData = m_array.Get(iIndexBeforn);
		uint64_t uBefornValue = GetRankValueByRankType(pBefornFsRankData);
		
		//名次有变动才去重新删除、插入,防止破坏之前排序的稳定性(先入榜的排在前面)
		if(uCurrentValue != uBefornValue)
		{
			DoRemove(t);
			DoInsert(t,*tBefornIndex,bRecalcRankChange);
		}
		else
		{
			*pBefornFsRankData = *t;
		}
	}
	else
	{		
		int iRet = DoInsert(t,iIndexBeforn,bRecalcRankChange);
		ASSERT_RET(0 == iRet, -1);	
	}

	//DumpInfo(t);

	return 0;
}

template<class KeyT, class T>
void CShmSortArray<KeyT, T>::DumpInfo(T *t)
{
	KeyT key = GetKey(t);

	LOG_DBG("key[%llu]",key);
	int iNum = m_pData->iArraySize;
	LOG_DBG("iNum[%d]",iNum);
	for (int i = iNum-1; i >= 0; i--)
	{
		T *pCurr = m_array.Get(i);
		LOG_DBG("index[%d] key[%llu]",i,GetKey(pCurr));
	}
}

template <class KeyT, class T>
int CShmSortArray<KeyT, T>::DoInsert(T* t,int iIndexBeforn,bool bRecalcRankChange)
{
	int iNum = m_pData->iArraySize;

	if (m_pData->iArraySize < m_array.Size())
	{
		m_pData->iArraySize++;
	}
	
	for (int i = iNum-1; i >= 0; i--)
	{
		T* pCurr = m_array.Get(i);
		if (Compare(pCurr,t) > 0)
		{
			//要往上移动,如果超过范围了，则返回抛弃掉
			int iDest = i+1;
			if (iDest >= m_pData->iArraySize)
			{
				m_hashCheck.Remove(GetKey(pCurr));
			}
			else
			{
				T* pDest = m_array.Get(iDest);
				*pDest = *pCurr;
				int* pIndex = GetArrayIndex(pDest);
				*pIndex = iDest;
			}
		}
		else
		{
			int iDest = i+1;

			if (iDest < m_pData->iArraySize)
			{
				T* pDest = m_array.Get(iDest);
				*pDest = *t;
				
				int* pIndex = m_hashCheck.Insert(GetKey(t));
				ASSERT_RET(pIndex, -1);
			
				*pIndex = iDest;
			}

			CalcRankChange(t,iIndexBeforn,bRecalcRankChange);
			return 0;
		}
	}

	//最后肯定是第一个
	*(m_array.Get(0)) = *t;
	int* pIndex = m_hashCheck.Insert(GetKey(t));
	ASSERT_RET(pIndex, -1);
	*pIndex = 0;

	CalcRankChange(t,iIndexBeforn,bRecalcRankChange);
    return 0;
}

template <class KeyT, class T>
void CShmSortArray<KeyT, T>::DoRemove(T* t)
{
	int iNum =  m_pData->iArraySize;

	KeyT rmvKey = GetKey(t);
	int* pIndex = GetArrayIndex(t);
	if (pIndex)
	{
		int i = *pIndex;
		for(; i < iNum-1; i++)
		{	
			T* pCurr = m_array.Get(i);
			T* pNext = m_array.Get(i+1);
			*pCurr = *pNext;

			int* pRepIndex = GetArrayIndex(pCurr);
			*pRepIndex = i;
		}

		m_pData->iArraySize--;
		m_hashCheck.Remove(rmvKey);
	}
	else
	{
		LOG_DBG("[%llu] not found",rmvKey);
	}
}

template<class KeyT, class T>
int CShmSortArray<KeyT, T>::CalcRankChange(T *t,int iIndexBeforn,bool bRecalcRankChange)
{
	//不计算排名变动
	if(!bRecalcRankChange)
	{
		return 0;
	}

	// 下标 ==> 排名
	int iRankBeforn = iIndexBeforn + 1;

	//计算下排行变动信息
	int *pIndexNow = m_hashCheck.Find(GetKey(t));
	if(!pIndexNow)
	{
		//出榜了
		AdjustOthersRank(iRankBeforn,Size(),1);
		return 0;
	}

//	//本来在榜外,进榜的时候位置没人
//	if(iRankBeforn == GetMaxSize() && )
//	{
//
//	}

	int tIndexNow = *pIndexNow;
	T *pCur = Get(tIndexNow);
	if(!pCur)
	{
		LOG_ERR("get Index[%d] element failed",tIndexNow - 1);
		return -1;
	}

	int iRankNow = tIndexNow + 1;
	int iDiv = iIndexBeforn - tIndexNow;
	if(iDiv > 0)
	{
		//上升
		pCur->RankChangeInfo.RankStats = 1;
		pCur->RankChangeInfo.StatsVal = abs(iDiv);
		AdjustOthersRank(iRankNow + 1,iRankBeforn,-1);
	}
	else if(iDiv == 0)
	{
		pCur->RankChangeInfo.RankStats = 0;
	}
	else
	{
		pCur->RankChangeInfo.RankStats = 2;
		pCur->RankChangeInfo.StatsVal = abs(iDiv);
		AdjustOthersRank(iRankBeforn,iRankNow - 1,1);
	}

	return 0;
}

template<class KeyT, class T>
void CShmSortArray<KeyT, T>::AdjustOthersRank(int iStartRank, int iEndRank, int iChange)
{
	LOG_DBG("iStartRank[%d] iEndRank[%d] iChange[%d]",iStartRank,iEndRank,iChange);
	if(iStartRank > iEndRank || iStartRank < 0 || iEndRank < 0 || iStartRank > Size() || iEndRank > Size())
	{
		return ;
	}

	for (int i = iStartRank; i <= iEndRank && i <= Size(); ++i)
	{
		//此处的i是排名,而不是下标
		T *pCur = Get(i-1);
		if(!pCur)
		{
			LOG_ERR("index[i] null");
			continue;
		}
		pCur->RankChangeInfo.RankStats = iChange > 0 ? 1 : 2;
		pCur->RankChangeInfo.StatsVal = abs(iChange);
	}
}

#endif


