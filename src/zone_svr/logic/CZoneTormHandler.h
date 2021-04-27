//
// Created by Administrator on 2021/4/27.
//

#ifndef SERVER_ADD_CZONETORMHANDLER_H
#define SERVER_ADD_CZONETORMHANDLER_H

#include "CTormMgr.h"

class CRoleDbTormHandler : public CTormHandler
{
public:
    virtual int HandleUpdate(CTormMgr* pMgr, tagORMUpdateRes* pUpdateRes, void* pCbData, int iCbDataLen);
    virtual int HandleSelect(CTormMgr* pMgr, tagORMSelectRes *pSelectRes, void* pCbData, int iCbDataLen);
    virtual int HandleDelete(CTormMgr* pMgr, tagORMDeleteRes* pDeleteRes, void* pCbData, int iCbDataLen);
    virtual int HandleInsert(CTormMgr* pMgr, tagORMInsertRes* pInsertRes, void* pCbData, int iCbDataLen);
};


#endif //SERVER_ADD_CZONETORMHANDLER_H
