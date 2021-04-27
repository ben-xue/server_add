
#include <stdio.h>
#include "CTormMgr.h"
#include "CPerformStatsticMgr.h"
#include "db_proxy_desc.h"
#include "CSvrConfig.h"
#include "ormapi/COrmApi.h"
#include "CSvrRuntimeContext.h"

using namespace dsf;
extern const char* g_szMetaLib_db_proxy;

int CTormMgr::Init(const char* szDrPath, int iDstFuncID)
{
	m_iTormSeq = 1;
	bzero(&m_stPkg, sizeof(m_stPkg));

	m_iTormHandleCnt = 0;

	DrErrInfo err;
	m_pDbMetaLib = dr_lib_load(szDrPath, &err);
	if (!m_pDbMetaLib)
	{
		LOG_ERR("load [%s] dr failed err[%s]", szDrPath, err.szErrInfo);
		return -1;
	}

	CLogCategory* pLogCat = CSvrRuntimeContext::Instance()->GetRunningAppFrame()->GetLogCategory("app.ormapi");

	//初始化访问db的index
	DrBuffData stHost;

	tagDbProxyConf stConf;
	BZERO(stConf);

	stHost.iSize = sizeof(stConf);
	stHost.pMem = (char*)&stConf;

	DR_HANDLE_LIB pDbProxyMetaLib = dr_lib_load_from_b64str(g_szMetaLib_db_proxy, &err);
	if (!pDbProxyMetaLib)
	{
		LOG_ERR("load db_proxy meta lib failed errInfo[%d]", err.szErrInfo);
		return -1;
	}

	DR_HANDLE_META pConfMeta = dr_lib_find_meta_by_name(pDbProxyMetaLib, "DbProxyConf");
	if (NULL == pConfMeta)
	{
		LOG_ERR("tdr_get_meta_by_name failed name[%s]", "DbProxyConf");
		return -1;
	}

	int iRet = dr_input_file(pConfMeta, &stHost, "../cfg/db_proxy.xml", &err);
	if (0 != iRet)
	{
		LOG_ERR("dr_input_file(%s) ret(%d) invalid, error[%s]", "../cfg/db_proxy.xml", iRet, err.szErrInfo);
		return -1;
	}

	int iDbProxyIndex = 0;
	m_selfAddr = CSvrConfig::GetSelfBusID();

	if(FUNC_GLOBAL_ORM == iDstFuncID)
    {
		iDbProxyIndex = stConf.DbGlobalProxyIndex;
	    m_selfAddr = CSvrConfig::GetSelfBusID();
		m_tormAddr = CSvrConfig::GetGlobalDbProxyBusID(iDbProxyIndex);
    }
	else if (FUNC_CACHE_DB_PROXY == iDstFuncID)
	{
		iDbProxyIndex = stConf.DbCacheProxyIndex;
		m_tormAddr = CSvrConfig::GetCacheDBProxyBusID(iDbProxyIndex);
	}
    else
    {
    	iDbProxyIndex = stConf.DbProxyIndex;
		m_tormAddr = CSvrConfig::GetDbProxyBusID(iDbProxyIndex);
    }
	
	iRet = m_ormApi.Init(m_pDbMetaLib, m_tormAddr, pLogCat);
	if (iRet != 0)
	{
		LOG_ERR("init COrmApi failed ret[%d]", iRet);
		return -1;
	}

	LOG_DBG("db_proxy read succ db_proxy_index[%d] bGlobal[%d]", iDbProxyIndex, iDstFuncID);

	return 0;
}

int CTormMgr::Update(const char* szTableName, void* pData, int iDataSize, char * szWhere, void* pCbData, int iCbDataLen, int iSessID)
{
	tagORMEntryNameList stEntryList;
	bzero(&stEntryList, sizeof(stEntryList));

	return Update(szTableName, pData, iDataSize, &stEntryList, szWhere, pCbData, iCbDataLen, iSessID);
}

int CTormMgr::Update(const char* szTableName, void* pData, int iDataSize, void* pCbData, int iCbDataLen, int iSessID)
{
	tagORMEntryNameList stEntryList;
	bzero(&stEntryList, sizeof(stEntryList));
	
	return Update(szTableName, pData, iDataSize, &stEntryList, "", pCbData, iCbDataLen, iSessID);
}

int CTormMgr::Update(const char* szTableName, void* pData, int iDataSize, tagORMEntryNameList* pEntryList, const char* szWhere,
	void* pCbData, int iCbDataLen, int iSessID)
{
	int iSeq = AllocSeq();
	return m_ormApi.Update(szTableName, pData, iDataSize, pEntryList, szWhere, iSessID, iSeq, pCbData, iCbDataLen);
}

int CTormMgr::Query(const char* szTableName, void* pData, int iDataSize, 
		tagORMEntryNameList* pstEntryList, const char* szWhere,  int iMaxLimit, 
		void* pCbData, int iCbDataLen, int iSessID)
{
	int iSeq = AllocSeq();
	return m_ormApi.Query(szTableName, pData, iDataSize, pstEntryList, szWhere, iMaxLimit, iSessID, iSeq, pCbData, iCbDataLen);
}

int CTormMgr::Insert(const char* szTableName, void* pData, int iDataSize,
		void* pCbData, int iCbDataLen, int iSessID)
{
	int iSeq = AllocSeq();
	return m_ormApi.Insert(szTableName, pData, iDataSize, iSessID, iSeq, pCbData, iCbDataLen);
}

int CTormMgr::ReplaceInsert(const char* szTableName, void* pData, int iDataSize,
				  void* pCbData, int iCbDataLen, int iSessID)
{
	int iSeq = AllocSeq();
	return m_ormApi.ReplaceInsert(szTableName, pData, iDataSize, iSessID, iSeq, pCbData, iCbDataLen);
}

int CTormMgr::Delete(const char* szTableName, void* pData, int iDataSize,
	void* pCbData, int iCbDataLen, int iSessID)
{
	int iSeq = AllocSeq();
	return m_ormApi.Delete(szTableName, pData, iDataSize, NULL,  iSessID, iSeq, pCbData, iCbDataLen);
}

int CTormMgr::AddDbHandler(const char* szTableName, CTormHandler* pHandler)
{
	if (m_iTormHandleCnt < MAX_ORM_TABLE_SUPPORT)
	{
		bzero(m_aTormTableName[m_iTormHandleCnt], sizeof(m_aTormTableName[m_iTormHandleCnt]));
		strncpy(m_aTormTableName[m_iTormHandleCnt], szTableName, sizeof(m_aTormTableName[m_iTormHandleCnt])-1);
		m_tormHandler[m_iTormHandleCnt] = pHandler;
		m_iTormHandleCnt++;
	}
	return 0;
}

CTormHandler* CTormMgr::GetDbHandler(const char* szTableName)
{
	for (int i = 0; i < m_iTormHandleCnt; i++)
	{
		if (strcmp(m_aTormTableName[i], szTableName) == 0)
		{
			return m_tormHandler[i];
		}
	}

	return NULL;
}

bool IsDbOpCmd(int iDbCmd)
{
	switch(iDbCmd)
	{
	case ORM_SELECT_RES:
	case ORM_UPDATE_RES:
	case ORM_INSERT_RES:
	case ORM_DELETE_RES:
	case ORM_SELECT_COUNT_RES:
	case ORM_REPLACE_INSERT_REQ:
		return true;

	default:
		break;
	}

	return false;
}

int CTormMgr::HandleTormResInCoContext(char * pPkg, int iPkgLen)
{
	LOG_DBG("enter HandleTormResInCoContext");
	tagORMPkg * pstPkg = (tagORMPkg *)pPkg;

	if (!IsDbOpCmd(pstPkg->Head.HeadCmd.Cmd))
	{
		return 0;
	}

	CTormHandler* pTormHander = GetDbHandler(pstPkg->Head.HeadCmd.MetaNameVer.MetaName);
	if (NULL == pTormHander)
	{
		LOG_ERR("find no torm handler, table name: %s", pstPkg->Head.HeadCmd.MetaNameVer.MetaName);
		return -1;
	}

	pTormHander->m_pTormHead = &pstPkg->Head;

	LOG_DBG("Handle torm response, MetaName[%s] TormCmd[%d]", pstPkg->Head.HeadCmd.MetaNameVer.MetaName, (int)pstPkg->Head.HeadCmd.Cmd);

	ASSERT_RET(pstPkg->Head.HeadApp.AsynCallBackDataLen < ORM_MAX_ASYNCALLBACK_DATA_LEN, -1);
	void *pCbData = NULL;
	int iCbLen = 0;
	if (pstPkg->Head.HeadApp.AsynCallBackDataLen > 0)
	{
		pCbData = m_asynCallBackData;
		iCbLen = pstPkg->Head.HeadApp.AsynCallBackDataLen;
		memcpy(m_asynCallBackData, pstPkg->Head.HeadApp.AsynCallBackData, pstPkg->Head.HeadApp.AsynCallBackDataLen);
	}

	switch (pstPkg->Head.HeadCmd.Cmd)
	{
	case ORM_SELECT_RES:
		pTormHander->HandleSelect(this, &pstPkg->Body.SelectRes, pCbData, iCbLen);
		break;

	case ORM_UPDATE_RES:
		pTormHander->HandleUpdate(this, &pstPkg->Body.UpdateRes, pCbData, iCbLen);
		break;

	case ORM_INSERT_RES:
		pTormHander->HandleInsert(this, &pstPkg->Body.InsertRes, pCbData, iCbLen);
		break;

	case ORM_DELETE_RES:
		pTormHander->HandleDelete(this, &pstPkg->Body.DeleteRes, pCbData, iCbLen);
		break;

	case ORM_SELECT_COUNT_RES:
		pTormHander->HandleSelectCnt(this, &pstPkg->Body.SelectCountRes, pCbData, iCbLen);
		break;
    case ORM_REPLACE_INSERT_REQ:
        pTormHander->HandleReplaceInsert(this, &pstPkg->Body.ReplaceInsertRes, pCbData, iCbLen);
        break;

	default:
		break;
	}

	return 0;
}

DR_HANDLE_META CTormMgr::GetTableMeta(const char * szTableName)
{
	return dr_lib_find_meta_by_name(m_pDbMetaLib, szTableName);
}

int CTormMgr::GetTableMetaSize(const char * szTableName)
{
	DR_HANDLE_META pMeta = GetTableMeta(szTableName);
	if (!pMeta)
	{
		LOG_ERR("GetTableMeta tableName[%s] failed", szTableName);
		return -1;
	}
	return dr_meta_get_mem_size(pMeta);
}

int CTormMgr::FetchOneData(char * pData, int iSize)
{
	return m_ormApi.FetchOneSelectData(pData, iSize);
}

int CTormMgr::AllocSeq()
{
	return m_iTormSeq++;
}
