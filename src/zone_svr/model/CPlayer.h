//
// Created by Administrator on 2021/4/26.
//

#ifndef SERVER_CPLAYER_H
#define SERVER_CPLAYER_H

#include "CShmObjPool.h"
#include "CPlayerCmpt.h"
#include "CPlayerBag.h"

#define MAX_PLAYER_CMPT_CNT 128

class CPlayer : public CShmBaseObj
{
public:
    CPlayer();
    bool AddCmpt(CPlayerCmpt* pCmpt);

    virtual int Init() ;
    virtual void Resume() ;
    virtual void Free() ;

public:
    //功能模块集合
    CPlayerCmpt* m_aCmpt[MAX_PLAYER_CMPT_CNT];
    int m_iCmptCnt;

    CPlayerBag m_bag;
};


#endif //WSERVER_CPLAYER_H
