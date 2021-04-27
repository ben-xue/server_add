/*
 *  ��DR lib ���л��� ����ÿ�ν���lib��������������
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