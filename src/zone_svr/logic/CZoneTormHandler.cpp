//
// Created by Administrator on 2021/4/27.
//

#include "CZoneTormHandler.h"
#include "CZoneDbMgr.h"

int CRoleDbTormHandler::HandleUpdate(CTormMgr *pMgr, tagORMUpdateRes *pUpdateRes, void *pCbData, int iCbDataLen)
{
    return CTormHandler::HandleUpdate(pMgr, pUpdateRes, pCbData, iCbDataLen);
}

int CRoleDbTormHandler::HandleSelect(CTormMgr *pMgr, tagORMSelectRes *pSelectRes, void *pCbData, int iCbDataLen)
{
    return CTormHandler::HandleSelect(pMgr, pSelectRes, pCbData, iCbDataLen);
}

int CRoleDbTormHandler::HandleDelete(CTormMgr *pMgr, tagORMDeleteRes *pDeleteRes, void *pCbData, int iCbDataLen)
{
    return CTormHandler::HandleDelete(pMgr, pDeleteRes, pCbData, iCbDataLen);
}

int CRoleDbTormHandler::HandleInsert(CTormMgr *pMgr, tagORMInsertRes *pInsertRes, void *pAsyncData, int iCbDataLen)
{
    ASSERT_RET(sizeof(DBOpCbData) == iCbDataLen, -1);
    DBOpCbData* pCbData = (DBOpCbData*)pAsyncData;

    if (pInsertRes->ErrNo != 0)
    {
        LOG_ERR("cbuin(%u) insert role db failed, ErrNO[%d]", pCbData->dwUin, pInsertRes->ErrNo);
        return -1;
    }
    else
    {
        LOG_DBG("Insert robot data successed uin[%u]", pCbData->dwUin);
    }

    return 0;
}
