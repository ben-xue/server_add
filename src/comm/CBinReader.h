/*
**  
*  资源读取模块
*/

#ifndef CBINREADER_H__
#define CBINREADER_H__

#include "comm_inc.h"
#include "dr/dr_meta.h"
#include "CDrLibCacheMgr.h"

/**
* 封装读取配置bin的接口
*/
class CBinReader
{
public:
	CBinReader();
	~CBinReader();

	/**
	* 读取配置接口
	*/
	int Read(const char* szBinPath, const char* szMetaName, const char* szTdrPath);


	/**
	* 获取数据个数
	*/
	int GetDataCount();

	/**
	* 获取数据
	*/
	void* GetData(int iIndex);

public:
	int LoadDr(const char* szDrPath, const char* szMetaName);

protected:
	int AllocMemory(int iUnitCnt);
	void Free();


///开放给特殊的接口使用，主要configbase
public:
	///新版本的格式
	DR_HANDLE_LIB m_lib;
	DR_HANDLE_META m_meta;


protected:	
	char m_szTableName[256];
	
	int m_iUnitSize;
	int m_iUnitCnt;
	char* m_unitBuff;

	char* m_fileBuff;
	
};

#endif


