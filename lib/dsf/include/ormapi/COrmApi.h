#pragma once

/*
 * @file:	COrmApi.h
 * @author: pls
 * @brief:  ORM操作api
 **/

#include "bus/bus.h"
#include "dr/dr_meta.h"
#include "orm_protocol.h"
#include "dr/dr_net.h"
#include "log/log.h"

NS_BEGIN(dsf)

class COrmApi
{
public:
	COrmApi();

	int Init(DR_HANDLE_LIB pMetaLib, BUSADDR ormSvrAddr, CLogCategory* pLogCat);

	int Update(const char* szTableName, void* pData, int iDataSize, tagORMEntryNameList* pEntryList,
		const char* szWhere, int iSessID, int iSessSeq, void* pCbData, int iCbDataLen);

	int Insert(const char* szTableName, void* pData, int iDataSize,
		int iSessID, int iSessSeq, void* pCbData, int iCbDataLen);

	int Query(const char* szTableName, void* pData, int iDataSize,
		tagORMEntryNameList* pstEntryList, const char* szWhere, int iMaxLimit,
		int iSessID, int iSessSeq, void* pCbData, int iCbDataLen);

	int QueryCnt(const char* szTableName, void* pData, int iDataSize, const char* szWhere,
		int iSessID, int iSessSeq, void* pCbData, int iCbDataLen);

	int Delete(const char* szTableName, void* pData, int iDataSize, const char* szWhere,
		int iSessID, int iSessSeq, void* pCbData, int iCbDataLen);

	int ReplaceInsert(const char* szTableName, void* pData, int iDataSize,
			   int iSessID, int iSessSeq, void* pCbData, int iCbDataLen);

	DR_HANDLE_META GetOrmPkgMeta();

	int AttachResMsg(char* pMsg, int iMsgLen);

	tagORMPkgHead* GetOrmResMsgHead();
	
	tagORMPkg* GetOrmResPkg();

	int FetchOneSelectData(char* pMsg, int iMsgLen);

private:

	int InitOrmPkgHead(tagORMPkgHead* pHead, int iOrmCmd, const char* szMetaName, int iSessID, int iSessSeq, void* pCbData, int iCbDataLen);

	int SendReqToOrmSvr(tagORMPkg* pPkg, const void* pvData, int iDataLen);

	int BuildReqPkg(DrBuffData& stNet, tagORMPkg* pPkg, DrBuffData& stMetaData);

	//根据请求类型自动填充一些字段
	int BuildOperInfo(tagORMPkgBody* pPkgBody, DrBuffData& stMetaData, tagORMPkgHeadCmd* pHeadCmd);

	int check_orm_entry_list_valid(DR_HANDLE_META pMeta, tagORMEntryNameList* pEntryNameList, int iCheckFlag);

	bool check_exist_in_entry_list(tagORMEntryNameList* pEntryNameList, const char* pName);

	int pack_data_content(DR_HANDLE_META pMeta, DrBuffData* pHost, tagORMEntryNameList* pEntryNameList, tagORMDataBuff* pOrmNetBuff);

	int pack_split_table_info(DR_HANDLE_META pMeta, DrBuffData* pHost, int32_t& iSplitFactor);
	
	int pack_prim_key_content(DR_HANDLE_META pMeta, DrBuffData* pHost, tagORMDataBuff* pOrmNetBuff);

    void OrmCalcBinCode(unsigned char *pData, IN unsigned int iLen, OUT unsigned int *piFactor);

private:

	DR_HANDLE_LIB m_pMetalib;
	BUSADDR	m_ormSvrAddr;
	
	int m_iOrmSeq;

	tagORMPkg m_stPkg;
	char m_szSendBuff[1024 * 1024 * 4];

	CLogCategory* m_pLogCat;

	DR_HANDLE_LIB m_pOrmProtoMetaLib;
	DR_HANDLE_META m_pOrmPkgMeta;

	tagORMPkg m_stOrmResPkg;
	int m_iFetchCount;
	int m_iFetchResultOffset;
	DR_HANDLE_META m_pOrmResMeta;
	int m_iResMetaVer;

};

NS_END()