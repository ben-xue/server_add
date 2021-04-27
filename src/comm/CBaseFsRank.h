#ifndef CBASEFSRANK_H__
#define CBASEFSRANK_H__
#include <map>
#include <vector>
#include <limits.h>
#include "CShmObjPool.h"
#include "CSingleton.h"
#include "CBaseRank.h"
#include "CShmSortArray.h"
#include "proto_rank.h"
#include "proto_comm.h"
#include "CFsDb.h"

class CFsTable;

/**
* 所有基于FSTable的排行榜基类
*/
template<class KeyT, class T>
class CBaseFsRank : public CBaseRank
{
public:
	CBaseFsRank(){};
	virtual ~CBaseFsRank(){};

public:
	int Init(int iShmKey, uint64_t iKeySize, int iMaxRank,
			const char* szFileName, const char* szMetaName);
	void OnTick();
	void ForceSave();
	void ForceSaveAll();

	int GetRankSize();
	int GetMaxRankSize();
	int ClearCurrentRank();

	T *GetMinRankData();
	T *GetRankDataByIndex(int iIndex);
	T *GetRankDataByKey(uint64_t uulKey);
	int GetRankByKey(uint64_t uulKey);

	int OnCalRankChanage(T*t1, int iCurRank);

	virtual int Compare(T*t1, T*t2) = 0;
	virtual int IsSameUnit(T*t1, T*t2) = 0;
	virtual KeyT GetKey(T* t) = 0;
	virtual uint64_t GetRankValueByRankType(T* t) = 0;
	
	virtual int SaveasXml() { return m_pFsTable->SaveasXml(); }
	const char *GetSaveFileName();

public:
	void MarkDirty() {m_bDirty = true;}
	void MarkStopUpdate() {m_bAllowUpdate = false;}
	void MarkRestartUpdate() {m_bAllowUpdate = true;}

	void UpdateRank(T* pData,bool bRecalcRankChange = true)
	{
		if (pData && m_bAllowUpdate)
		{
			int iRet = m_rankList.Update(pData,bRecalcRankChange);
			MarkDirty();
			ASSERT_RET(0 == iRet);
		}
	}

	void DeleteRank(T* pData)
	{
		if (pData && m_bAllowUpdate)
		{
			m_rankList.DoRemove(pData);
			MarkDirty();
		}
	}
protected:
	virtual int OnInit(int iShmKey, int iKeySize, int iMaxRank);

	///读取排行榜文件
	int ReadFromFile(const char* szFileName, const char* szMetaName);
	
	class CRankShmSortArray : public CShmSortArray<KeyT, T>
	{
	public:
		void SetRank(CBaseFsRank<KeyT, T>* pRank)
		{
			m_pRank = pRank;
		}
		
		virtual int Compare(T*t1, T*t2)
		{
			return m_pRank->Compare(t1, t2);
		}
		virtual KeyT GetKey(T* t)
		{		
			return m_pRank->GetKey(t);
		}

		virtual uint64_t GetRankValueByRankType(T* t)
		{
			return m_pRank->GetRankValueByRankType(t);
		}
		
	public:
		CBaseFsRank<KeyT, T>* m_pRank;
	};

public:
	//立即刷新排行榜
	bool m_FlushImmediately;

protected:	
	CRankShmSortArray m_rankList;

	///本地存储的接口
	CFsTable* m_pFsTable;
	time_t m_tLastSaveTime;

	int m_iTopShowMaxRank;

	//是否数据有修改
	bool m_bDirty;

	//是否允许更新
	bool m_bAllowUpdate;
};

template<class KeyT, class T>
int CBaseFsRank<KeyT, T>::Init(int iShmKey, uint64_t iKeySize, int iMaxRank, 
		const char* szFileName, const char* szMetaName)
{
	m_FlushImmediately = false;
	m_bDirty = false;
	m_bAllowUpdate = true;
	m_tLastSaveTime = 0;
	m_iTopShowMaxRank = iMaxRank;
	
	int iRet = m_rankList.Create(iShmKey, iKeySize, iMaxRank);
	if (iRet != 0)
	{
		LOG_ERR("CRankShmSortArray create failed: %d, ShmKey[%d] FileName[%s] MetaName[%s], MaxRank[%d]", iRet, 
			iShmKey, szFileName, szMetaName, iMaxRank);
		return iRet;
	}

	m_rankList.SetRank(this);

	LOG_DBG("ReadFromFile [%s] [%s]",szFileName, szMetaName);
	iRet = ReadFromFile(szFileName, szMetaName);
	if (iRet != 0)
	{
		LOG_ERR("read rank file failed: %s, Meta: %s", szFileName, szMetaName);
		return iRet;
	}

	iRet = OnInit(iShmKey, iKeySize, iMaxRank);
	if(iRet != 0)
	{
		LOG_ERR("OnInit failed");
		return iRet;
	}
	return 0;
}

template<class KeyT, class T>
int CBaseFsRank<KeyT, T>::ReadFromFile(const char* szFileName, const char* szMetaName)
{
	ASSERT_RET(!m_pFsTable, -1);
	
	m_pFsTable = CFsDb::Instance()->LoadTable(szFileName, szMetaName);
	if (!m_pFsTable)
	{
		LOG_ERR("Load FSTable today_rank failed");
		return -1;
	}

	//启动之后,共享内存中有数据,说明是热启.  因为内存中的数据比文件中的新,这里额外保存一次
	int iCurrentSize = m_rankList.Size();
	if(iCurrentSize > 0)
	{
		LOG_DBG("ReadFromFile SaveSize [%d]",iCurrentSize);
		ForceSaveAll();
		SaveasXml();
	}

	LOG_DBG("ReadFromFile: m_pFsTable->GetDataNum() [%d]",m_pFsTable->GetDataNum());

	for (int i = 0; i < m_pFsTable->GetDataNum(); i++)
	{
		T* pRankData = (T*)m_pFsTable->GetDataByIndex(i);
		ASSERT(pRankData);

		UpdateRank(pRankData,false);
	}
	
	return 0;
}

template<class KeyT, class T>
void CBaseFsRank<KeyT, T>::OnTick()
{
	time_t tCurTime = CTime::GetTime();
	if (m_tLastSaveTime <= 0)
	{	
		m_tLastSaveTime = tCurTime;
		return;
	}

	if (m_tLastSaveTime + 60 < tCurTime)
	{
		m_tLastSaveTime = tCurTime;

		if (m_bDirty)
		{
			LOG_DBG("Start Save!");
			if(!m_pFsTable->IsSaving())
			{
				m_pFsTable->Save((const char*)m_rankList.GetBuff(), sizeof(T), m_rankList.Size());
				m_bDirty = false;
				SaveasXml();
			}
			else
			{
				LOG_ERR("CBorderBaseRank fstable is writing now~");
			}
		}
	}
}

template<class KeyT, class T>
void CBaseFsRank<KeyT, T>::ForceSave()
{
//	if(m_bDirty)
	{
		m_pFsTable->Save((const char*)m_rankList.GetBuff(), sizeof(T), m_rankList.Size());
		m_bDirty = false;
	}
}

template<class KeyT, class T>
int CBaseFsRank<KeyT, T>::OnInit(int iShmKey, int iKeySize, int iMaxRank)
{
	return 0;
}

//获取排行榜展示的数据
template<class KeyT, class T>
int CBaseFsRank<KeyT, T>::GetRankSize()
{
	int iTopShowRank = m_rankList.Size();
	if (iTopShowRank > m_iTopShowMaxRank)
	{
		return m_iTopShowMaxRank;
	}

	return iTopShowRank;
}

template<class KeyT, class T>
int CBaseFsRank<KeyT, T>::GetMaxRankSize()
{
	return m_iTopShowMaxRank;
}

template<class KeyT, class T>
T* CBaseFsRank<KeyT, T>::GetRankDataByIndex(int iIndex)
{
	ASSERT_ERR_MSG_RET(iIndex >= 0 && iIndex < GetRankSize(), NULL,"iIndex[%d]",iIndex);
	return m_rankList.Get(iIndex);
}

template<class KeyT, class T>
int CBaseFsRank<KeyT, T>::OnCalRankChanage(T *t1, int iCurRank)
{
//	tagRankInfo *pCurrentRankInfo = &t1->RankInfoData;
//	tagRankInfo *pPreviousRankInfo = NULL;
//	uint64_t uulKey = GetKey(t1);
//
//	int iIndex = 0;
//	for (;iIndex < m_previous_rankList.Size(); ++iIndex)
//	{
//		T *pPrevious = m_previous_rankList.Get(iIndex);
//		if(GetKey(pPrevious) == uulKey)
//		{
//			pPreviousRankInfo = &pPrevious->RankInfoData;
//			break;
//		}
//	}
//
//	int iPreviousRank = pPreviousRankInfo ? iIndex+1 : 0;
//
//	if(!pPreviousRankInfo)
//	{
//		//之前不在排行榜
//		pCurrentRankInfo->RankStats = EM_RANK_STATS_UP;
//		pCurrentRankInfo->StatsVal = GetMaxRankSize() - iCurRank;
//
//	}
//	else
//	{
//		if(iPreviousRank > iCurRank)
//		{
//			pCurrentRankInfo->RankStats = EM_RANK_STATS_UP;
//			pCurrentRankInfo->StatsVal = iPreviousRank - iCurRank;
//		}
//		else if(iPreviousRank == iCurRank)
//		{
//			pCurrentRankInfo->RankStats = EM_RANK_STATS_NULL;
//			pCurrentRankInfo->StatsVal = iPreviousRank - iCurRank;
//		}
//		else
//		{
//			pCurrentRankInfo->RankStats = EM_RANK_STATS_DOWN;
//			pCurrentRankInfo->StatsVal = iCurRank - iPreviousRank;
//		}
//	}
//
//	pCurrentRankInfo->AgoRanking = iPreviousRank;

	return 0;
}

template<class KeyT, class T>
T *CBaseFsRank<KeyT, T>::GetRankDataByKey(uint64_t uulKey)
{
	for (int i = 0; i < GetRankSize(); ++i)
	{
		T *pT = GetRankDataByIndex(i);
		if(GetKey(pT) == uulKey)
		{
			return pT;
		}
	}
	return NULL;
}

template<class KeyT, class T>
T *CBaseFsRank<KeyT, T>::GetMinRankData()
{
	int iIndex = m_rankList.Size();
	// 空的话直接返回NULL
	if( iIndex == 0 )
	{
		return NULL;
	}
	ASSERT_RET(iIndex >= 1 && iIndex <= GetMaxRankSize(), NULL);
	return m_rankList.Get(iIndex - 1);
}

template<class KeyT, class T>
void CBaseFsRank<KeyT, T>::ForceSaveAll()
{
	LOG_DBG("m_pFsTable[%p]",m_pFsTable);
	if(m_pFsTable)
	{
		m_pFsTable->Save((const char*)m_rankList.GetBuff(), sizeof(T), m_rankList.Size());
		m_pFsTable->ForceSaveAll();
	}
}

template<class KeyT, class T>
int CBaseFsRank<KeyT, T>::ClearCurrentRank()
{
	m_rankList.Clear();
	return 0;
}

template<class KeyT, class T>
int CBaseFsRank<KeyT, T>::GetRankByKey(uint64_t uulKey)
{
	int *pIndex = m_rankList.GetArrayByIndex(uulKey);
	if(pIndex)
	{
		return *pIndex + 1;
	}

	return 0;
}

template<class KeyT, class T>
const char *CBaseFsRank<KeyT, T>::GetSaveFileName()
{
	return m_pFsTable->GetFileName();
}


#endif


