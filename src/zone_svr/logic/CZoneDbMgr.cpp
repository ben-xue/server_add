//
// Created by Administrator on 2021/4/27.
//

#include "zone_comm.h"
#include "CZoneDbMgr.h"
#include "CZoneTormHandler.h"

int CZoneDbMgr::Init()
{
    uint32_t uSelfAddr = CSvrConfig::GetSelfBusID();

    int iRet = m_roleTormMgr.Init(GetDbMetaPath(), FUNC_DB_PROXY);
    ASSERT_RET(0 == iRet, -1);

    m_roleTormMgr.AddDbHandler("DbRoleData", new CRoleDbTormHandler);
    
    return 0;
}

int CZoneDbMgr::InsertRoleData(tagDbRoleData *pRoleData, DBOpCbData *pCbData)
{
    return 0;
}

int CZoneDbMgr::UpdateRoleData(tagDbRoleData *pRoleData, DBOpCbData *pCbData)
{
    return 0;
}

int CZoneDbMgr::QueryRoleDataByRoleID(uint64_t uRoleID, DBOpCbData *pCbData)
{
    return 0;
}
