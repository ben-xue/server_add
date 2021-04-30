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
    int iRet = m_roleTormMgr.Insert("DbRoleData", pRoleData, sizeof(DBROLEDATA), pCbData, sizeof(DBOpCbData));
    if (iRet != 0)
    {
        LOG_ERR("m_roleTormMgr.Insert DbRoleData failed,Ret[%d]", iRet);
    }

    LOG_ERR("insert role data uin[%llu] ret[%d]", pRoleData->Uin, iRet);

    return iRet;
}

int CZoneDbMgr::UpdateRoleData(tagDbRoleData *pRoleData, DBOpCbData *pCbData)
{
    int iRet = m_roleTormMgr.Update("DbRoleData", pRoleData, sizeof(DBROLEDATA), NULL, NULL, pCbData, sizeof(DBOpCbData));
    if (iRet != 0)
    {
        LOG_ERR("m_roleTormMgr.Update failed,Ret[%d]", iRet);
    }

    return iRet;
}

int CZoneDbMgr::QueryRoleDataByRoleID(uint64_t uRoleID, DBOpCbData *pCbData)
{
    tagDbRoleData stRoleData;
    bzero(&stRoleData, sizeof(stRoleData));
    stRoleData.RoleID = uRoleID;

    char szWhere[128] = { 0 };
    snprintf(szWhere, sizeof(szWhere), "where RoleID=%llu", uRoleID);
    int iRet = m_roleTormMgr.Query("DbRoleData", &stRoleData, sizeof(stRoleData), NULL, szWhere, 1,
                                   pCbData, sizeof(DBOpCbData));

    if (iRet != 0)
    {
        LOG_ERR("m_roleTormMgr.QueryRoleData failed,Ret[%d]", iRet);
    }

    return iRet;
}
