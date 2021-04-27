#include "CPerformStatsticMgr.h"
#include "CSvrRuntimeContext.h"
#include "CAppFrame.h"

const char * GetDBOpName(int iDBOp)
{
	switch (iDBOp)
	{
		case ORM_SELECT_RES:
		{
			return "ORM_SELECT_RES";
			break;
		}
		case ORM_UPDATE_RES:
		{
			return "ORM_UPDATE_RES";
			break;

		}
		case ORM_INSERT_RES:
		{
			return "ORM_INSERT_RES";
			break;
		}
		case ORM_DELETE_RES:
		{
			return "ORM_DELETE_RES";
			break;
		}
		default:
		{
			return "invalid";
		}
	}
}

const char * GetDBName(ENMDBName4Stat enmDbName)
{
	switch(enmDbName)
	{
		case ENM_DB_NAME_ROLE_DATA_4_STAT:
		{
			return "DbRoleData";
			break;
		}
		case ENM_DB_NAME_GUILD_DATA_4_STAT:
		{
			return "DbCorpData";
			break;
		}
		case ENM_DB_NAME_SYSMAIL_DATA_4_STAT:
		{
			return "DbSysMailData";
			break;
		}
		default:
		{
			return "DBInvalid";
		}
	}
}

inline _STPerfStatItem::_STPerfStatItem()
{
	
}

inline int _STPerfStatItem::Init()
{
	iID = 0;
	BZERO(szItemName);
	iTotalCostTime = 0;
	iTotalCnt = 0;
	iTotalBytes = 0;
	return 0;
}

int _STDBStatItem::Init()
{
	iID = 0;
	iTotalCostTime = 0;
	iTotalCnt = 0;
//	BZERO(szDBName);
	return 0;
}

int CPerformStatsticMgr::Init()
{
	m_iLastTickTime = CTime::GetTime();
	m_oLogicStaticArray.Clear();
	m_oLogicStaticArray.Init(MAX_STATIC_ITEM_NUM);
	m_oLogicCumuStaticArray.Clear();
	m_oLogicCumuStaticArray.Init(MAX_STATIC_ITEM_NUM);
	m_oLogicScalarStaticArray.Clear();
	m_oLogicScalarStaticArray.Init(MAX_STATIC_ITEM_NUM);
	m_oUpStreamArray.Clear();
	m_oUpStreamArray.Init(MAX_STATIC_ITEM_NUM);
	m_oDownStreamArray.Clear();
	m_oDownStreamArray.Init(MAX_STATIC_ITEM_NUM);
	m_oDBStatArray.Clear();
	m_oDBStatArray.Init(MAX_STATIC_ITEM_NUM);

	//初始化性能日志句柄
	m_pstStatLogCat = CSvrRuntimeContext::Instance()->GetRunningAppFrame()->GetLogCategory("app.stat");

	if (NULL == m_pstStatLogCat)
	{
		printf("m_pstStatLogCat init fails\n");
		return 0;
	}
	else
	{
		printf("stat_log init ok\n");
	}

	//_LOG_STAT("stat_log inited");
	return 0;
}

int CPerformStatsticMgr::OnTick()
{
	//输出性能调试日志
	//清理统计内存
	int iCnt = 0;
	if (CTime::GetTime() >= m_iLastTickTime)
	{
		//该tick中不需要处理的情况
		if (60 > (CTime::GetTime() - m_iLastTickTime))
		{
			return 0;
		}
		else
		{
			//需要输出的情况,继续执行
		}

	}
	else //CTime::GetTime() < m_iLastTickTime
	{
		//修正时间
		m_iLastTickTime = CTime::GetTime();
		return 0;
	}
	log_error(m_pstStatLogCat, "TICK|%u", CTime::GetTime());
	iCnt = m_oLogicStaticArray.GetDataCount();
	for (int i = 0; i < iCnt && i<MAX_STATIC_ITEM_NUM; i++)
	{
		STPerfStatItem * pItem = m_oLogicStaticArray.GetDataByIndex(i);
		//逻辑统计全部输出
		log_error(m_pstStatLogCat, "%s|%d|%s|%"PRIu64"|%d", "LogicStat", pItem->iID, pItem->szItemName, pItem->iTotalCostTime, pItem->iTotalCnt);
		pItem->iTotalCostTime = 0;
		pItem->iTotalBytes = 0;
		pItem->iTotalCnt = 0;
	}

	//输出累计量的统计
	iCnt = m_oLogicCumuStaticArray.GetDataCount();
	for (int i = 0; i < iCnt && i < MAX_STATIC_ITEM_NUM; i++)
	{
		STCumuStatItem * pItem = m_oLogicCumuStaticArray.GetDataByIndex(i);
		//累计量的统计全部输出
		log_error(m_pstStatLogCat, "%s|%d|%s|%d", "CumuStat", pItem->iID, pItem->szItemName, pItem->iCnt);
		pItem->iCnt = 0;

	}

	//输出标量统计
	iCnt = m_oLogicScalarStaticArray.GetDataCount();
	for (int i = 0; i < iCnt && i < MAX_STATIC_ITEM_NUM; i++)
	{
		STScalarStatItem * pItem = m_oLogicScalarStaticArray.GetDataByIndex(i);
		log_error(m_pstStatLogCat, "%s|%d|%s|%d", "ScalarStat", pItem->iID, pItem->szItemName, pItem->iValue);
		pItem->iValue = 0;
	}


	iCnt = m_oUpStreamArray.GetDataCount();
	for (int i = 0; i < iCnt && i < MAX_STATIC_ITEM_NUM; i++)
	{
		STPerfStatItem * pItem = m_oUpStreamArray.GetDataByIndex(i);
		if (pItem->iTotalCnt)
		{
			log_error(m_pstStatLogCat, "%s|%d|%s|%"PRIu64"|%d|%"PRIu64"", "UpStreamStat", pItem->iID, pItem->szItemName, pItem->iTotalCostTime, pItem->iTotalCnt, pItem->iTotalBytes);
		}
		pItem->iTotalCostTime = 0;
		pItem->iTotalBytes = 0;
		pItem->iTotalCnt = 0;
	}

	iCnt = m_oDownStreamArray.GetDataCount();
	for (int i = 0; i < iCnt && i < MAX_STATIC_ITEM_NUM; i++)
	{
		STPerfStatItem * pItem = m_oDownStreamArray.GetDataByIndex(i);
		if (pItem->iTotalCnt)
		{
			log_error(m_pstStatLogCat, "%s|%d|%s|%d|%"PRIu64"", "DownStreamStat", pItem->iID, pItem->szItemName, pItem->iTotalCnt, pItem->iTotalBytes);
		}
		pItem->iTotalCostTime = 0;
		pItem->iTotalBytes = 0;
		pItem->iTotalCnt = 0;
	}

	//输出DB的统计
	iCnt = m_oDBStatArray.GetDataCount();
	for (int i = 0; i < iCnt && i < MAX_STATIC_ITEM_NUM; i++)
	{
		STDBStatItem * pItem = m_oDBStatArray.GetDataByIndex(i);
		if (pItem->iTotalCnt)
		{
			int iCallbackID = pItem->iID / 10000;
			ENMDBName4Stat enmDBName = (ENMDBName4Stat) ((pItem->iID%10000)/100);
			int iDBOpID = pItem->iID % 100;

			//输出DB回包的统计
			log_error(m_pstStatLogCat, "%s|%s|%d|%s|%d|%"PRIu64"", "DBResStat", GetDBName(enmDBName), iCallbackID, GetDBOpName(iDBOpID), pItem->iTotalCnt, pItem->iTotalCostTime);
		}
		pItem->iTotalCnt = 0;
		pItem->iTotalCostTime = 0;
	}
	m_iLastTickTime = CTime::GetTime();

	//输出最大标量统计数据
	iCnt = m_oMaxScalarStaticArray.GetDataCount();
	for (int i = 0; i < iCnt && i < MAX_STATIC_ITEM_NUM; i++)
	{
		STScalarStatItem * pItem = m_oMaxScalarStaticArray.GetDataByIndex(i);
		log_error(m_pstStatLogCat, "%s|%d|%s|%d", "MaxScalarStat", pItem->iID, pItem->szItemName, pItem->iValue);
		pItem->iValue = 0;
	}

	return 0;
}

int CPerformStatsticMgr::InsertProtocolStatiticDsfLib(DR_HANDLE_LIB pLib)
{
	//遍历所有的CS协议
	DR_HANDLE_META pPkgBodyMeta = dr_lib_find_meta_by_name(pLib, "CSPkgBody");
	if (!pPkgBodyMeta)
	{
		LOG_ERR("pPkgBodyMeta NULL for CSPkgBody");
		return -1;
	}
	
	int iEntryNum = dr_meta_get_entry_count(pPkgBodyMeta);
	for (int i = 0; i<iEntryNum; i++)
	{
		DR_HANDLE_ENTRY entry = dr_meta_get_entry_by_index(pPkgBodyMeta, i);
		DR_HANDLE_META entryTypeMeta = dr_entry_get_type_meta(entry);
		if (entryTypeMeta)
		{
			int iID = dr_entry_get_id(entry);
			
			STPerfStatItem stPerfStatItem;
			stPerfStatItem.Init();
			stPerfStatItem.iID = iID;
			STRNCPY(stPerfStatItem.szItemName, dr_meta_get_name(entryTypeMeta), sizeof(stPerfStatItem.szItemName));
			m_oUpStreamArray.Insert(iID, &stPerfStatItem);
			m_oDownStreamArray.Insert(iID, &stPerfStatItem);
		}

	}
	
	return 0;
}


void CPerformStatsticMgr::IncCumuLogicStat(ENMCumuStatID iID, const char * pszStatName)
{
	STCumuStatItem * pItem = m_oLogicCumuStaticArray.Find((int)iID);
	if (!pItem)
	{
		STCumuStatItem stTmpItem;
		BZERO(stTmpItem);
		stTmpItem.iID = (int)iID;
		STRNCPY(stTmpItem.szItemName, pszStatName, sizeof(stTmpItem.szItemName));
		m_oLogicCumuStaticArray.Insert(iID, &stTmpItem);
		pItem = m_oLogicCumuStaticArray.Find((int)iID);
	}

	if (!pItem)
	{
		return;
	}

	pItem->iCnt++;
	return;
}

void CPerformStatsticMgr::UpdateLogicScalarStat(ENMScalarStatID iID, const char * pszStatName, int iValue)
{
	STScalarStatItem * pItem = m_oLogicScalarStaticArray.Find((int)iID);
	if (!pItem)
	{
		STScalarStatItem stTmpItem;
		BZERO(stTmpItem);
		stTmpItem.iID = (int)iID;
		STRNCPY(stTmpItem.szItemName, pszStatName, sizeof(stTmpItem.szItemName));
		m_oLogicScalarStaticArray.Insert(iID, &stTmpItem);
		pItem = m_oLogicScalarStaticArray.Find((int)iID);
	}

	if (!pItem)
	{
		return;
	}

	pItem->iValue = iValue;
}

void CPerformStatsticMgr::UpdateMaxScalarStat(ENMScalarStatID iID, const char * pszStatName, int iValue)
{
	STScalarStatItem * pItem = m_oMaxScalarStaticArray.Find((int)iID);
	if (!pItem)
	{
		STScalarStatItem stTmpItem;
		BZERO(stTmpItem);
		stTmpItem.iID = (int)iID;
		STRNCPY(stTmpItem.szItemName, pszStatName, sizeof(stTmpItem.szItemName));
		m_oMaxScalarStaticArray.Insert(iID, &stTmpItem);
		pItem = m_oMaxScalarStaticArray.Find((int)iID);
	}

	if (!pItem)
	{
		return;
	}

	if (iValue > pItem->iValue)
	{
		pItem->iValue = iValue;
	}
}

int CPerformStatsticMgr::GetLogicScalarStat(ENMScalarStatID iID)
{
	STScalarStatItem * pItem = m_oLogicScalarStaticArray.Find((int)iID);
	if (!pItem)
	{
		return 0;
	}
	return pItem->iValue;
}

CLogicStatHelper::CLogicStatHelper(ENMLogicStatID iID, const char * pszName)
{
	m_iID = (int)iID;
	STPerfStatItem * pstPerfStatItem = CPerformStatsticMgr::Instance()->m_oLogicStaticArray.Find(iID);
	if (!pstPerfStatItem)
	{
		STPerfStatItem stPerfStatItem;
		stPerfStatItem.Init();
		stPerfStatItem.iID = m_iID;
		STRNCPY(stPerfStatItem.szItemName, pszName, sizeof(stPerfStatItem.szItemName));
		CPerformStatsticMgr::Instance()->m_oLogicStaticArray.Insert(iID, &stPerfStatItem);
		pstPerfStatItem = CPerformStatsticMgr::Instance()->m_oLogicStaticArray.Find(iID);
	}

	struct timeval tv;
	gettimeofday(&tv, NULL);
	m_ullRecordTime = tv.tv_sec * 1000 * 1000 + tv.tv_usec;

}

CLogicStatHelper::~CLogicStatHelper()
{
	STPerfStatItem * pstPerfStatItem = CPerformStatsticMgr::Instance()->m_oLogicStaticArray.Find(m_iID);
	if (!pstPerfStatItem)
	{
		return;
	}
	struct timeval tv;
	gettimeofday(&tv, NULL);
	uint64_t ullEndTime = tv.tv_sec * 1000 * 1000 + tv.tv_usec;
	pstPerfStatItem->iTotalCostTime += (ullEndTime - m_ullRecordTime);
	pstPerfStatItem->iTotalCnt++;

}

CUpStreamStatHelper::CUpStreamStatHelper(int iCmd, int iPkgLen)
{
	m_iID = iCmd;
	m_iPkgLen = iPkgLen;
	STPerfStatItem * pstPerfStatItem = CPerformStatsticMgr::Instance()->m_oUpStreamArray.Find(m_iID);
	if (!pstPerfStatItem)
	{
		return;
	}
	struct timeval tv;
	gettimeofday(&tv, NULL);
	m_ullRecordTime = tv.tv_sec * 1000 * 1000 + tv.tv_usec;
	
}

CUpStreamStatHelper::~CUpStreamStatHelper()
{
	STPerfStatItem * pstPerfStatItem = CPerformStatsticMgr::Instance()->m_oUpStreamArray.Find(m_iID);
	if (!pstPerfStatItem)
	{
		return;
	}
	struct timeval tv;
	gettimeofday(&tv, NULL);
	uint64_t ullEndTime = tv.tv_sec * 1000 * 1000 + tv.tv_usec;
	pstPerfStatItem->iTotalCostTime += (ullEndTime - m_ullRecordTime);
	pstPerfStatItem->iTotalBytes += m_iPkgLen;
	pstPerfStatItem->iTotalCnt++;
}

CDownStreamStatHelper::CDownStreamStatHelper(int iCmd, int iPkgLen)
{
	m_iID = iCmd;
	m_iPkgLen = iPkgLen;
	STPerfStatItem * pstPerfStatItem = CPerformStatsticMgr::Instance()->m_oDownStreamArray.Find(m_iID);
	if (!pstPerfStatItem)
	{
		return;
	}
	//下行不用关注时间开销

}

CDownStreamStatHelper::~CDownStreamStatHelper()
{
	STPerfStatItem * pstPerfStatItem = CPerformStatsticMgr::Instance()->m_oDownStreamArray.Find(m_iID);
	if (!pstPerfStatItem)
	{
		return;
	}

	pstPerfStatItem->iTotalBytes += m_iPkgLen;
	pstPerfStatItem->iTotalCnt++;
}

CDBStatHelper::CDBStatHelper(ENMDBName4Stat enmDBName, int iDBOpID, int iCallBackID)
{

	
	m_iTmpID = iCallBackID * 10000 + enmDBName * 100 + iDBOpID;
	STDBStatItem * pstDbStatItem = CPerformStatsticMgr::Instance()->m_oDBStatArray.Find(m_iTmpID);
	if (!pstDbStatItem)
	{
		STDBStatItem stDbStatItem;
		stDbStatItem.Init();
		stDbStatItem.iID = m_iTmpID;
		//STRNCPY(stDbStatItem.szDBName, pszDbName, sizeof(stDbStatItem.szDBName));
		CPerformStatsticMgr::Instance()->m_oDBStatArray.Insert(m_iTmpID, &stDbStatItem);
		
	}

	struct timeval tv;
	gettimeofday(&tv, NULL);
	m_ullRecordTime = tv.tv_sec * 1000 * 1000 + tv.tv_usec;
}

CDBStatHelper::~CDBStatHelper()
{
	STDBStatItem * pstDbStatItem = CPerformStatsticMgr::Instance()->m_oDBStatArray.Find(m_iTmpID);
	if (!pstDbStatItem)
	{
		return;
	}
	struct timeval tv;
	gettimeofday(&tv, NULL);
	uint64_t ullEndTime = tv.tv_sec * 1000 * 1000 + tv.tv_usec;
	pstDbStatItem->iTotalCostTime += (ullEndTime - m_ullRecordTime);
	pstDbStatItem->iTotalCnt++;

}

