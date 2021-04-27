#include "CDrLibCacheMgr.h"

DR_HANDLE_LIB CDrLibCacheMgr::LoadDrLib(const char* szDrPath)
{
	auto it = m_cache.find(string(szDrPath));
	if (it != m_cache.end())
	{
		return it->second;
	}
	DrErrInfo err;

	DR_HANDLE_LIB lib = dr_lib_load(szDrPath, &err);
	if (!lib)
	{
		LOG_ERR("dr_lib_load %s failed errinfo[%s]", szDrPath, err.szErrInfo);
		return NULL;
	}

	m_cache.insert(make_pair(string(szDrPath), lib));
	return lib;
}

CDrLibCacheMgr::~CDrLibCacheMgr()
{
	for (auto it = m_cache.begin(); it != m_cache.end(); it++)
	{
		dr_lib_free(it->second);
	}
}

