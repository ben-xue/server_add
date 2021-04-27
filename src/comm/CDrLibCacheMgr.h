/*
 *  对DR lib 进行缓存 避免每次解析lib带来的新能消耗
 **/


#include "dr/dr_meta.h"
#include <map>
#include <string>
#include "CSingleton.h"
#include "comm_inc.h"

class CDrLibCacheMgr : public CSingleton<CDrLibCacheMgr>
{
public:
	~CDrLibCacheMgr();

	DR_HANDLE_LIB LoadDrLib(const char* szDrPath);

private:
	std::map<string, DR_HANDLE_LIB> m_cache;
};