//
// Created by Administrator on 2021/4/26.
//

#ifndef SERVER_CPLAYERCMPT_H
#define SERVER_CPLAYERCMPT_H

#include "CPlayerDirty.h"
class CPlayer;

class CPlayerCmpt : public CPlayerDirty
{
public:
    void Resume(CPlayer* pPlayer);
    virtual void OnResume();
    
    void Init(CPlayer* pPlayer);
    virtual void OnInit();
    
    void Free();
    virtual void OnFree();

    virtual void Tick();
    virtual void OnTick();

    virtual void InitOnCreateRole();
    virtual void OnPreRoleLogin();
    virtual void OnAfterRoleLogin();

    virtual void OnZeroTime();
    virtual void OnRoleLogout();
    virtual void OnRoleReconnect();

protected:
    CPlayer *m_pPlayer;
};


#endif //WSERVER_CPLAYERCMPT_H
