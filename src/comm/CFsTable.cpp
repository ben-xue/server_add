
#include <stdio.h>
#include "CFsTable.h"

#define FSTABLEHEADTAG 0x15

#pragma pack(1)

struct FSTableHead
{
	uint32_t tag;
	uint32_t len;
	uint32_t version;
	uint32_t num;

	void Hton()
	{
		tag = htonl(tag);
		len = htonl(len);
		version = htonl(version);
		num = htonl(num);
	}
};

#pragma pack()

//10k
#define DEF_MAX_SAVE_SIZE_PER_TICK 10240

CFsTable::CFsTable()
{
	m_pTableMeta = NULL;
	m_iUnitSize = 0;

	BZERO(m_szDataName);
	BZERO(m_szFileName);
	
	m_iUnitSize = 0;
	m_iUnitCnt = 0;
	m_iMaxUnitCnt = 0;
	
	m_unitBuff = NULL;
	m_fileBuff = NULL;
	m_writeBuff = NULL;
	
	m_iStartSaveIndex = -1;
	m_fpSave = NULL;

	m_bDirty = false;
	
	m_iMaxSaveSizePerTick = DEF_MAX_SAVE_SIZE_PER_TICK;
}

CFsTable::~CFsTable()
{
	Free();
	if (m_writeBuff)
	{
		delete[] m_writeBuff;
		m_writeBuff = NULL;
	}
}

int CFsTable::Init(const char* szFileName, DR_HANDLE_LIB pDrLib, const char* szDataName)
{
	m_pTableMeta = dr_lib_find_meta_by_name(pDrLib, szDataName);
	if (NULL == m_pTableMeta)
	{
		LOG_ERR("get meta %s from %s failed ", szDataName, "fs_data.tdr");
		return -1;
	}
	
	m_iVersion = dr_meta_get_curr_version(m_pTableMeta);
	m_iUnitSize = dr_meta_get_mem_size(m_pTableMeta);

	STRNCPY(m_szDataName, szDataName, sizeof(m_szDataName));
	STRNCPY(m_szFileName, szFileName, sizeof(m_szFileName));
	snprintf(m_szTmpFilePath, sizeof(m_szTmpFilePath), "%s_tmp", m_szFileName);

	ASSERT_RET(m_iUnitSize > 0, -1);
	
	m_writeBuff = new char[m_iUnitSize];
	
	//读取文件
	int iRet =  LoadFile(szFileName);
	if (iRet  != 0)
	{
		LOG_ERR("LoadFile failed: %s", szFileName);
		delete[]m_writeBuff;
		m_writeBuff = NULL;
		
		return -1;
	}

	return 0;
}

int CFsTable::Save(const char* pData, int iUnitSize, int iDataCnt)
{
	if (IsSaving())
	{
		LOG_ERR("stop saving opertion, resave");
		StopSave();
	}
	
	if (m_iUnitSize != iUnitSize)
	{
		LOG_ERR("UnitSize[%d-%d] not match", m_iUnitSize, iUnitSize);
		return -1;
	}
	
	ASSERT_RET(m_iUnitSize == iUnitSize, -1);	
	if (iDataCnt > m_iMaxUnitCnt)
	{
		if (m_unitBuff)
		{
			delete[] m_unitBuff;
		}
		
		int iRet = AllocMemory(iDataCnt);
		ASSERT_RET(0 == iRet, -1);
	}

	ASSERT_RET(m_unitBuff, -1);
	memcpy(m_unitBuff, pData, m_iUnitSize*iDataCnt);
	m_iUnitCnt = iDataCnt;

	m_bDirty = true;
	return StartSave();
}

/**
* 强制存档
*/
void CFsTable::ForceSaveAll()
{
	if (IsSaving())
	{
		StopSave();
	}
	
	StartSave();
	ASSERT_RET(IsSaving());
	
	for (m_iStartSaveIndex = 0; m_iStartSaveIndex < m_iUnitCnt; m_iStartSaveIndex++)
	{
		int iRet = WriteToFs(m_iStartSaveIndex);
		if (iRet <= 0)
		{
			return;
		}
	}
	
	StopSave();
	
	//移动文件
	rename(GetTempSaveFile(), m_szFileName);
	LOG_ERR("%s save finish, UnitNum[%d]", m_szFileName, m_iUnitCnt);
}

bool CFsTable::IsSaving()
{
	LOG_ERR("in CFsTable::IsSaving() m_fpSave[%p]",m_fpSave);
	return m_fpSave != NULL;
}

void CFsTable::Free()
{
	if (m_unitBuff)
	{
		delete[]m_unitBuff;
		m_unitBuff = NULL;
	}

	if (m_fileBuff)
	{
		delete[]m_fileBuff;
		m_fileBuff = NULL;
	}

	m_iStartSaveIndex = -1;
	if (m_fpSave)
	{
		fclose(m_fpSave);
	}
	
	m_fpSave = NULL;	
	m_iUnitCnt = 0;
	m_iMaxUnitCnt = 0;
}

int CFsTable::LoadFile(const char* szFileName)
{
	FILE *fp = fopen(szFileName, "a+");
	if (!fp)
	{
		LOG_ERR("Failed to open file:%s, error: %s", szFileName, strerror(errno));
		return -1;
	}

	//get the file size
	if (fseek(fp, 0, SEEK_END) != 0)
	{
		LOG_ERR("Failed to get file size:%s, error:%s", szFileName, strerror(errno));
		return -1;
	}

	int iFileSize = ftell(fp);
	if (iFileSize <= 0)
	{
		LOG_ERR("init fs table: %s", szFileName);
		int iRet = AllocMemory(0);
		ASSERT(0 == iRet);
		
		return 0;
	}
	
	m_fileBuff = new char[iFileSize];
	
	fseek(fp, 0, SEEK_SET);
	clearerr(fp);
	
	char* pCurr = m_fileBuff;
	int iLeftSize = iFileSize;
	while(!feof(fp) && !ferror(fp) && iLeftSize > 0)
	{
		int iReadLen = fread(pCurr, 1, iLeftSize, fp);
		if (iReadLen > 0)
		{
			iLeftSize -= iReadLen;
			pCurr += iReadLen;
		}
		else
		{
			break;
		}
	}
	
	if(iLeftSize > 0)
	{		
		LOG_ERR("Failed to read file:%s, error:%s, filesize:%d, readlen:%s", szFileName, strerror(errno),
			iFileSize, iFileSize-iLeftSize);
		return -1;
	}
	
	//读取配置
	FSTableHead* pBinHead = (FSTableHead*)m_fileBuff;
	pBinHead->Hton();

	if (pBinHead->tag != FSTABLEHEADTAG)
	{
		LOG_ERR("%s tag check failed", m_szFileName);
		return -1;
	}
		
	if (AllocMemory(pBinHead->num) != 0)
	{
		LOG_ERR("Failed to read file:%s, invalid head, resnum: %u, UnitSize: %d", szFileName, 
			pBinHead->num, m_iUnitSize);
		Free();
		return -1;
	}
	
	char* pDataCur = m_fileBuff + sizeof(FSTableHead);
	int iDataCurLeft = iFileSize - sizeof(FSTableHead);

	int iReadCnt = 0;
	while (iDataCurLeft > 0 && iReadCnt < m_iUnitCnt)
	{
		DrBuffData stHost;
		DrBuffData stNet;

		stHost.pMem = m_unitBuff + m_iUnitSize * iReadCnt;
		stHost.iSize = m_iUnitSize;

		stNet.pMem = pDataCur;
		stNet.iSize = iDataCurLeft;
		
		int iRet = dr_decode(m_pTableMeta, &stNet, &stHost, pBinHead->version);
		if (iRet != 0)
		{
			LOG_ERR("Read file unit data failed:%s,dr_decode failed:%d, ReadCnt[%d], File[%s]", "fs_data.tdr", iRet, iReadCnt, szFileName);
			Free();
			return -1;
		}

		pDataCur += stNet.iSize;
		iDataCurLeft -= stNet.iSize;
		iReadCnt++;
	}

	if (iReadCnt != pBinHead->num|| iDataCurLeft != 0)
	{
		LOG_ERR("Read file failed,last read count[%d] not match head[%d] or data left[%d] File[%s] error", 
				iReadCnt, pBinHead->num, iDataCurLeft, szFileName);
		Free();
		return -1;
	}

	delete[]m_fileBuff;
	m_fileBuff = NULL;	

	LOG_ERR("read %s ok, num: %d", szFileName, iReadCnt);
	return 0;
}

int CFsTable::AllocMemory(int iUnitCnt)
{
	ASSERT_RET(m_iUnitSize > 0, -1);
	
	m_iMaxUnitCnt = iUnitCnt+1;
	m_iUnitCnt = iUnitCnt;
	m_unitBuff = new char[m_iMaxUnitCnt*m_iUnitSize];
	return m_unitBuff != NULL?0:-1;
}

const char* CFsTable::GetTempSaveFile()
{
	return m_szTmpFilePath;
}

void CFsTable::StopSave()
{
	if (m_fpSave)
	{
		fclose(m_fpSave);
		m_fpSave = NULL;
		m_iStartSaveIndex = -1;
	}
}

int CFsTable::StartSave()
{
	if (m_fpSave)
	{
		fclose(m_fpSave);
		m_fpSave = NULL;
		m_iStartSaveIndex = -1;
	}
	
	m_fpSave = fopen(GetTempSaveFile(), "w");
	if (m_fpSave)
	{
		m_iStartSaveIndex = 0;

		FSTableHead stHead;
		BZERO(stHead);

		stHead.version = m_iVersion;
		stHead.num = m_iUnitCnt;
		stHead.tag = FSTABLEHEADTAG;
		stHead.Hton();

		int iRet = fwrite(&stHead, 1, sizeof(stHead), m_fpSave);
		ASSERT_RET(iRet == sizeof(stHead), -1);

		LOG_DBG("start saving %s....", m_szTmpFilePath);
		
		return 0;
	}
	else
	{
		LOG_ERR("fopen %s failed, %s", GetTempSaveFile(), strerror(errno));
	}
	
	return -1;
}

int CFsTable::WriteToFs(int iIndex)
{
	DrBuffData stHost;
	DrBuffData stNet;

	stHost.pMem = m_unitBuff + m_iUnitSize*iIndex;
	stHost.iSize = m_iUnitSize;
	stNet.pMem = m_writeBuff;
	stNet.iSize = m_iUnitSize;
	
	int iRet = dr_encode(m_pTableMeta, &stHost, &stNet, m_iVersion);
	if (iRet != 0)
	{
		LOG_ERR("fstable %s,dr_encode failed:%d", m_szFileName, iRet);
		
		StopSave();
		return -1;
	}

	iRet = fwrite(m_writeBuff, 1, stNet.iSize, m_fpSave);
	if (iRet != stNet.iSize)
	{
		LOG_ERR("fstable %s,fwrite failed:%s", m_szFileName, strerror(errno));
		
		StopSave();
		return -1;
	}

	return stNet.iSize;
}

int CFsTable::OnTick()
{
	if (m_fpSave != NULL && m_iStartSaveIndex >= 0)
	{
		int iWriteSize = 0;
		for (; m_iStartSaveIndex < m_iUnitCnt && iWriteSize < m_iMaxSaveSizePerTick; m_iStartSaveIndex++)
		{
			int iRet = WriteToFs(m_iStartSaveIndex);
			if (iRet <= 0)
			{
				LOG_ERR(" CFsTable::OnTick() iRet[%d]",iRet);
				StopSave();
				return 0;
			}

			iWriteSize += iRet;
		}

		if (m_iStartSaveIndex >= m_iUnitCnt)
		{
			StopSave();

			//移动文件
			rename(GetTempSaveFile(), m_szFileName);
			LOG_DBG("%s save finish", m_szFileName);

			m_bDirty = false;
		}
	}
	
	return 0;
}

int CFsTable::GetDataNum()
{
	return m_iUnitCnt;
}

char* CFsTable::GetDataByIndex(int iIndex)
{
	ASSERT_RET(iIndex >= 0 && iIndex < m_iUnitCnt, NULL);
	return m_unitBuff + iIndex*m_iUnitSize;
}

int CFsTable::SaveasXml()
{
	char szXmlFileName[256];
	snprintf(szXmlFileName, 256, "%s.xml", m_szFileName);
	LOG_ERR("// FsTable[%s] will be saveas xml in [%s]", m_szDataName, szXmlFileName);

	FILE* fpXml = fopen(szXmlFileName, "w");
	if (!fpXml)
	{
		LOG_ERR("Failed to open file:%s, error: %s", szXmlFileName, strerror(errno));
		return -1;
	}

	int iRet = 0;

	char szXmlDataBuff[2048];

	for (int i = 0; i < m_iUnitCnt; ++i)
	{
		DrBuffData stHost;
		stHost.pMem = m_unitBuff + m_iUnitSize * i;
		stHost.iSize = m_iUnitSize;

		DrBuffData stXml;
		stXml.pMem = szXmlDataBuff;
		stXml.iSize = sizeof(szXmlDataBuff);

		iRet = dr_output(m_pTableMeta, &stHost, &stXml, 0);
		if (iRet != 0)
		{
			LOG_ERR("fail to save xml for FsTable[%s] in unit[%d]", m_szDataName, i);
			break;
		}

		fwrite(stXml.pMem, 1, stXml.iSize, fpXml);
	}

	fclose(fpXml);

	return iRet;
}

char *CFsTable::GetFileName()
{
	return m_szFileName;
}

