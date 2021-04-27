/*************************************************************
* Copyright: Tencent Computer system Ltd.
* Filename: CEventMgr.h 
* Author: arrowgu
* Version: 1.0
* History: 2011-9-15
* Desc: ��װͨ�õ��¼�֪ͨ����
**************************************************************/


#ifndef _CEVENT_MGR_H__
#define _CEVENT_MGR_H__

#include "ZoneEventDefine.h"


/**
* ����ͳһ���¼�����ӿڣ��ϲ��ҵ���߼�ģ��ͨ���̳�ʵ�ָýӿ�����ȡ�����ĵ��¼�
*/
class CEventListener
{
public:
    /**
    * �¼�����������
    * @param [in] iEventType �¼����ͣ��ο�EnmEventID�Ķ���
    * @param [in] pParam �¼���Ӧ�Ĳ���
    *
    * @return �����¼������Ƿ����쳣
    */
    virtual int OnEvent(int iEventType, EventData *pParam) = 0;
};


/**
* �¼�����ģ�飬����������Ϣ�¼���ע��ͷַ�
*/
class CEventMgr
{
public:
    static CEventMgr* Instance();

public:
    CEventMgr();
    virtual ~CEventMgr();

    /**
    * ע���µ��¼��������
    * @note ��ÿ���ϲ�ģ�����Լ�ע���Լ�
    */
    int RegListener(int iEventType, CEventListener *pListener, const char *szName);

    /**
    * �Ƴ��¼��������������Ҫ��̬ע��ĳ��������������Ƶ����ע����Ƴ�����
    */
    int RmvListener(int iEventType, CEventListener *pListener);

    /**
    * �ַ��¼���Ϣ�������¼������߼�
    * @note ��ÿ���ײ��¼�������������
    */
    int DispathEvent(int iEventType, EventData *pParam);

    // �� Player ������ٷַ����¼�
    int DispathEvent(int iEventType, CPlayer* pPlayer);

private:
    int LogEventInfo(int iEventType);


private:
    struct EventEntry
    {
        CEventListener *pListener;
        char        szListenerName[256];
        EventEntry *pNextEntry;
    };

    EventEntry *m_apEntry[EVEID_MAX];
};



#endif

