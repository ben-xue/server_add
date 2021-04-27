#ifndef CFSSTABLE_H__
#define CFSSTABLE_H__

#include <map>
#include <vector>
#include "CShmObjPool.h"
#include "CSingleton.h"
#include "CShmHash.h"

/**
* 封装对每个文件的访问
*/
class CFsTable
{
public:
	CFsTable();	
	~CFsTable();
	
	/**
	* 初始化
	*/
	int Init(const char* szFileName, DR_HANDLE_LIB pDrLib, const char* szDataName);

	int OnTick();

public:
	int GetDataNum();
	char * GetFileName();

	char* GetDataByIndex(int iIndex);

	/**
	* 上层一次性将数据全部传进来，然后由table内部驱动增量写入
	* 通过调用IsSaving来判断是否已经全部写入了
	*/
	int Save(const char* pData, int iUnitSize, int iDataCnt);

	/**
	* 判断写入操作是否已经结束了
	*/
	bool IsSaving();

	/**
	* 强制存档
	*/
	void ForceSaveAll();
	
private:

	int AllocMemory(int iUnitCnt);

	int LoadFile(const char* szFileName);

	void Free();

	int StartSave();	
	
	const char* GetTempSaveFile();

	void StopSave();	
	
	int WriteToFs(int iIndex);

private:

	DR_HANDLE_META m_pTableMeta;
	int m_iVersion;

	char m_szDataName[256];
	char m_szFileName[256];
	char m_szTmpFilePath[257];

	int m_iUnitSize;

	int m_iMaxUnitCnt;
	int m_iUnitCnt;
	char* m_unitBuff;
	char* m_writeBuff;
	
	char* m_fileBuff;

	int m_iStartSaveIndex;
	FILE* m_fpSave;

	///每次tick最高写入的数据，避免数据太多造成CPU峰值
	int m_iMaxSaveSizePerTick;

	bool m_bDirty;

public:
	// 保存为 xml 文件方便查看
	int SaveasXml();
};

#endif

