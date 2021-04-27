#ifndef CFSSDB_H__
#define CFSSDB_H__

#include <string>
#include <map>
#include <vector>
#include "CShmObjPool.h"
#include "CSingleton.h"
#include "CShmHash.h"
#include "CFsTable.h"

using namespace std;


typedef map<string, CFsTable*> CFSTABLEMAP;

/**
* 本地的文件对象存储接口
*/
class CFsDb : public CSingleton<CFsDb>
{
public:

	/**
	* 初始化接口
	*/
	int Init(const char* szTdrPath, const char* szDataDir);
	void OnTick();

	/**
	* 获取到数据接口
	*/
	CFsTable* LoadTable(const char* szFile, const char* szDataName);
	/*
	 * 指定特殊的metalib的load接口
	 **/
	CFsTable* LoadTable(const char* szFile, DR_HANDLE_LIB pDrLib, const char* szDataName);

	/**
	* 同步到磁盘里
	*/
	void ForceSaveAll();
	
private:
	DR_HANDLE_LIB m_pDrLib;
	char m_szDataDir[256];

	map<string, CFsTable*> m_mapTable;
};

#endif


