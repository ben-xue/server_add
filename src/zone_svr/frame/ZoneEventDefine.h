#ifndef _ZONE_EVENT_DEFINE_H__
#define _ZONE_EVENT_DEFINE_H__

#include "comm_inc.h"

struct  CPlayer;

enum CommonEnmEventID
{
    EVEID_BEGIN 								= 0,  /* �����¼�ID����ʵֵ����ʵ������ */
    EVEID_ROLE_LOGIN 							= 1,  /* ��ҵ�¼����  @see EvnRoleLogin */
    EVEID_ROLE_LOGOUT 							= 2,  /* ��ҵǳ�����  @see EvnRoleLogout */
    
    EVEID_MAX/* ����ʱ��ID�����ֵ����ʵ������ */
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
* ���е��¼����ݼ���
*/
union EventParam
{
    EvnRoleLogin RoleLogin;
    EvnRoleLogout RoleLogut;
    EveCommonCnt commonCnt;
};


struct EventData
{
    CPlayer* pPlayer;   // ��Ҷ���
    EventParam param;   // �����¼�����

    EventData()
    {
        pPlayer = NULL;
        BZERO(param);
    }
};

#endif

