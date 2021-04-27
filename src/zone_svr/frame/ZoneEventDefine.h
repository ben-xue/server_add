#ifndef _ZONE_EVENT_DEFINE_H__
#define _ZONE_EVENT_DEFINE_H__

#include "comm_inc.h"

struct  CPlayer;

enum CommonEnmEventID
{
    EVEID_BEGIN 								= 0,  /* 定义事件ID的其实值，无实际意义 */
    EVEID_ROLE_LOGIN 							= 1,  /* 玩家登录触发  @see EvnRoleLogin */
    EVEID_ROLE_LOGOUT 							= 2,  /* 玩家登出触发  @see EvnRoleLogout */
    
    EVEID_MAX/* 定义时间ID的最大值，无实际意义 */
};

struct EvnRoleLogin
{
    uint32_t uin;
};

struct EvnRoleLogout
{
    uint32_t uin;
};

struct EveCommonCnt
{
    int iCnt;
    int iType;
    int iValue;
    int iValue1;
    int iValue2;
    int iCurrentMaxValue;
};

/**
* 所有的事件数据集合
*/
union EventParam
{
    EvnRoleLogin RoleLogin;
    EvnRoleLogout RoleLogut;
    EveCommonCnt commonCnt;
};


struct EventData
{
    CPlayer* pPlayer;   // 玩家对象
    EventParam param;   // 其它事件数据

    EventData()
    {
        pPlayer = NULL;
        BZERO(param);
    }
};

#endif

