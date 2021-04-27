#include "CFsDb.h"

/**
* 初始化接口
*/
int CFsDb::Init(const char* szTdrPath, const char* szDataDir)
{
	DrErrInfo err;
	m_pDrLib = dr_lib_load(szTdrPath, &err);
	if (!m_pDrLib)
	{
		LOG_ERR("tdr_load_metalib(%s) failed, Ret[%s]", szTdrPath, err.szErrInfo);
		return -1;
	}
	
	STRNCPY(m_szDataDir, szDataDir, sizeof(m_szDataDir));
	return 0;
}

void CFsDb::OnTick()
{
	CFSTABLEMAP::iterator itr = m_mapTable.begin();
	for(; itr != m_mapTable.end(); itr++)
	{
		itr->second->OnTick();
	}
}


/**
* 获取到数据接口
*/
CFsTable* CFsDb::LoadTable(const char* szFile, const char* szDataName)
{
	return LoadTable(szFile, m_pDrLib, szDataName);
}

CFsTable* CFsDb::LoadTable(const char* szFile, DR_HANDLE_LIB pDrLib, const char* szDataName)
{
	CFSTABLEMAP::iterator itr = m_mapTable.find(szFile);
	if (itr != m_mapTable.end())
	{
		return itr->second;
	}

	CFsTable* pTable = new CFsTable;
	char szFullPath[256];
	snprintf(szFullPath, sizeof(szFullPath), "%s/%s", m_szDataDir, szFile);

	int iRet = pTable->Init(szFullPath, pDrLib, szDataName);
	if (iRet != 0)
	{
		LOG_ERR("Init FsDb Table(%s) fails", szFullPath);
		delete pTable;
		return NULL;
	}

	m_mapTable.insert(CFSTABLEMAP::value_type(szFile, pTable));
	return pTable;
}

/**
* 同步到磁盘里
*/
void CFsDb::ForceSaveAll()
{
	CFSTABLEMAP::iterator itr = m_mapTable.begin();
	for(; itr != m_mapTable.end(); itr++)
	{
		itr->second->ForceSaveAll();
	}
}
