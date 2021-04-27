//
// Created by Administrator on 2021/4/27.
//

#include "zone_comm.h"
#include "CPlayerPoolMgr.h"

int CPlayerPoolMgr::Init(int iShmKey, uint64_t ullShmSize)
{
    uint32_t uMaxPlayerCount = GetZoneSvrConfig()->MaxPlayerCount;

    int iRet = m_playerPool.Create(iShmKey, ullShmSize, uMaxPlayerCount);
    if (iRet != 0)
    {
        LOG_ERR("m_playerPool.Create(%d,%"PRIu64",%d) failed, ret[%d]", iShmKey, ullShmSize, uMaxPlayerCount, iRet);
        return -1;
    }

    iRet = m_playerUinMap.Create(iShmKey, ullShmSize, uMaxPlayerCount);
    if (iRet != 0)
    {
        LOG_ERR("m_playerMapUinMap.Create(%d,%"PRIu64",%d) failed, ret[%d]", iShmKey, ullShmSize, uMaxPlayerCount, iRet);
        return -1;
    }
    
    return 0;
}

CPlayer *CPlayerPoolMgr::GetPlayer(int iObjID)
{
    return m_playerPool.FindObj(iObjID);
}

CPlayer *CPlayerPoolMgr::CreateCache(uint64_t dwUin)
{
    int *pObjID = m_playerUinMap.Find(dwUin);
    if(!pObjID)
    {
        LOG_ERR("dwUin[%llu] not find ",dwUin);
        return NULL;
    }
    return m_playerPool.FindObj(*pObjID);
}

void CPlayerPoolMgr::FreePlayer(CPlayer *pPlayer)
{
    ASSERT_RET(pPlayer);
    m_playerPool.FreeObj(pPlayer);
}

void CPlayerPoolMgr::RegEvent()
{

}

int CPlayerPoolMgr::OnEvent(int iEventType, EventData *pParam)
{
    return 0;
}
