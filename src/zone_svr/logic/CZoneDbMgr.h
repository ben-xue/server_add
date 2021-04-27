//
// Created by Administrator on 2021/4/27.
//

#ifndef SERVER_ADD_CZONEDBMGR_H
#define SERVER_ADD_CZONEDBMGR_H

#include "zone_comm.h"
#include "CTormMgr.h"
#include "CSingleton.h"
#include "proto_macro.h"
#include "database.h"

struct DbOpCbGlobalQuery
{
    int iStartIndex;
    int iQueryMaxCnt;
};

struct DbOpCbGlobalUpdate
{
    int iDataKey;
    uint32_t dwUpdateTime;
};

union DbOpCbDataDetail
{
    DbOpCbGlobalQuery globalQuery;
    DbOpCbGlobalUpdate globalUpdate;
};

struct DBOpCbData
{
    int 		iCmdID;				//异步命令ID
    uint32_t	dwUin;				//Uin信息
    uint32_t 	dwSeqID;			//客户端的回带字段
    uint64_t 	ullRoleID;			//角色ID
    DbOpCbDataDetail detailData;	//具体的数据
};


class CZoneDbMgr: public CSingleton<CZoneDbMgr>
{
public:
    int Init();
    int InsertRoleData(tagDbRoleData* pRoleData, DBOpCbData* pCbData);
    int UpdateRoleData(tagDbRoleData* pRoleData, DBOpCbData* pCbData);
    int QueryRoleDataByRoleID(uint64_t uRoleID,DBOpCbData* pCbData);

    DR_HANDLE_META GetTableMeta(const char * szTableName) { return  m_roleTormMgr.GetTableMeta(szTableName); }

protected:
    CTormMgr m_roleTormMgr;
};


#endif //SERVER_ADD_CZONEDBMGR_H
