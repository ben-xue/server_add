//
// Created by Administrator on 2021/4/27.
//

#ifndef SERVER_ADD_CPLAYERPOOLMGR_H
#define SERVER_ADD_CPLAYERPOOLMGR_H

#include "comm_inc.h"
#include "CSingleton.h"
#include "CPlayer.h"
#include "CShmFastPool.h"
#include "CEventMgr.h"
#include <set>

//�ع��������̼��ʱ��(s)
const static int GAME_POOL_MGR_SAVE_MAX_TIME    = 30;

//��Ҵ��̼��ʱ��(s)
const static int GAME_POOL_PLAYER_SAVE_MAX_TIME = 300;

//ÿ�δ洢����
const static int GAME_POOL_SAVE_MAX_NUM	        = 1000;

class CPlayerPoolMgr: public CSingleton<CPlayerPoolMgr>, public CEventListener
{
public:
    int Init(int iShmKey, uint64_t ullShmSize);
    CPlayer* GetPlayer(int iObjID);
    CPlayer* CreateCache(uint64_t dwUin);
    void	 FreePlayer(CPlayer* pPlayer);

protected:
    void RegEvent();
    virtual int OnEvent(int iEventType, EventData *pParam);

protected:
    CShmFastPool<CPlayer> m_playerPool;
    CShmHashTable<uint64_t, int> m_playerUinMap;
    
    //�ϴ���������ʱ��
    unsigned int m_iLastSaveTime;
};


#endif //SERVER_ADD_CPLAYERPOOLMGR_H
