/*************************************************************
* Copyright: Tencent Computer system Ltd.
* Filename: CEventMgr.h 
* Author: arrowgu
* Version: 1.0
* History: 2011-9-15
* Desc: 封装通用的事件通知流程
**************************************************************/


#ifndef _CEVENT_MGR_H__
#define _CEVENT_MGR_H__

#include "ZoneEventDefine.h"


/**
* 定义统一的事件处理接口，上层的业务逻辑模块通过继承实现该接口来获取到关心的事件
*/
class CEventListener
{
public:
    /**
    * 事件触发处理函数
    * @param [in] iEventType 事件类型，参考EnmEventID的定义
    * @param [in] pParam 事件对应的参数
    *
    * @return 返回事件处理是否有异常
    */
    virtual int OnEvent(int iEventType, EventData *pParam) = 0;
};


/**
* 事件管理模块，负责所有消息事件的注册和分发
*/
class CEventMgr
{
public:
    static CEventMgr* Instance();

public:
    CEventMgr();
    virtual ~CEventMgr();

    /**
    * 注册新的事件处理对象
    * @note 再每个上层模块中自己注册自己
    */
    int RegListener(int iEventType, CEventListener *pListener, const char *szName);

    /**
    * 移除事件处理对象，用于需要动态注册的场景，不建议调用频繁的注册和移除操作
    */
    int RmvListener(int iEventType, CEventListener *pListener);

    /**
    * 分发事件消息，触发事件处理逻辑
    * @note 由每个底层事件触发点来调用
    */
    int DispathEvent(int iEventType, EventData *pParam);

    // 由 Player 对象快速分发简单事件
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

