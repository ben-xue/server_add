/*************************************************************
* Copyright: Tencent Computer system Ltd.
* Filename: CEventMgr.h 
* Author: arrowgu
* Version: 1.0
* History: 2011-9-15
* Desc: 封装通用的事件通知流程
**************************************************************/

#include <string.h>
#include "CEventMgr.h"



CEventMgr* CEventMgr::Instance()
{
    static CEventMgr s_instance;
    return &s_instance;
}
    
CEventMgr::CEventMgr()
{
    bzero(m_apEntry, sizeof(m_apEntry));
}

CEventMgr::~CEventMgr()
{
    for (int i = 0; i < EVEID_MAX; i++)
    {
        EventEntry *pEntry = m_apEntry[i];
        while (pEntry != NULL)
        {
            EventEntry *pNext = pEntry->pNextEntry;
            delete pEntry;
            
            pEntry = pNext;
        }
        
        m_apEntry[i] = NULL;
    }
}

/**
* 注册新的事件处理对象
* @note 再每个上层模块中自己注册自己
*/
int CEventMgr::RegListener(int iEventType, CEventListener *pListener, const char *szName)
{
    if ((iEventType <= EVEID_BEGIN || iEventType >= EVEID_MAX))
        return -1;
    if (pListener == NULL)
        return -1;

    //先循环比较是否已经存在该listener
    EventEntry *pExist = m_apEntry[iEventType];
    while (pExist != NULL)
    {
        if (pExist->pListener == pListener)
        {
            return -1;
        }
        
        pExist = pExist->pNextEntry;
    }
    
    EventEntry *pEntry = new EventEntry;
    bzero(pEntry, sizeof(EventEntry));
    pEntry->pListener = pListener;
    pEntry->pNextEntry = m_apEntry[iEventType];
    if (szName != NULL)
    {
        strncpy(pEntry->szListenerName, szName, sizeof(pEntry->szListenerName)-1);
    }
    
    m_apEntry[iEventType] = pEntry;
    
    return 0;
}


/**
* 移除事件处理对象，用于需要动态注册的场景，不建议调用频繁的注册和移除操作
*/
int CEventMgr::RmvListener(int iEventType, CEventListener *pListener)
{
    if ((iEventType <= EVEID_BEGIN || iEventType >= EVEID_MAX))
        return -1;
    if (pListener == NULL)
        return -1;

    //先循环比较是否已经存在该listener
    EventEntry *pExist = m_apEntry[iEventType];
    EventEntry *pPrev = NULL;
    while (pExist != NULL)
    {
        if (pExist->pListener == pListener)
        {
            if (pPrev != NULL)
            {
                pPrev->pNextEntry = pExist->pNextEntry;
            }
            else
            {
                m_apEntry[iEventType] = pExist->pNextEntry;
            }
            
            delete pExist;
            pExist = NULL;
            
            return 0;
        }
        
        pPrev = pExist;
        pExist = pExist->pNextEntry;
    }

    return -1;
}


int CEventMgr::LogEventInfo(int iEventType)
{
    int i = 0;
    EventEntry *pEntry = m_apEntry[iEventType];
    while (pEntry != NULL)
    {
        LOG_DBG("Index[%d] Event[%d] EventEntry[%p] Event Listener[%p]", 
            i, iEventType, pEntry, pEntry->pNextEntry);
        
        i++;
		pEntry = pEntry->pNextEntry;
    }

    return 0;
}

/**
* 分发事件消息，触发事件处理逻辑
* @note 由每个底层事件触发点来调用
*/
int CEventMgr::DispathEvent(int iEventType, EventData *pParam)
{
    if ((iEventType <= EVEID_BEGIN || iEventType >= EVEID_MAX))
        return -1;

    EventEntry *pEntry = m_apEntry[iEventType];
    while (pEntry != NULL)
    {
        if (pEntry->pListener == NULL)
		{
			pEntry = pEntry->pNextEntry;
			continue;
		}

        int iRet = pEntry->pListener->OnEvent(iEventType, pParam);
        if (iRet != 0)
        {
            //logerror;
        }

		pEntry = pEntry->pNextEntry;
    }

    return 0;
}

int CEventMgr::DispathEvent(int iEventType, CPlayer* pPlayer)
{
	EventData stEveData;
	BZERO(stEveData);
	stEveData.pPlayer = pPlayer;
	DispathEvent(iEventType, &stEveData);

	return 0;
}


