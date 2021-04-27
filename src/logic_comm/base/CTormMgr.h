/*
**  
*  通用的DB逻辑框架
*/

#ifndef CTORMMGR_H__
#define CTORMMGR_H__

#include <map>
#include <string>
#include "comm_inc.h"
#include "CAppFrame.h"
#include "ormapi/COrmApi.h"

using namespace std;

#define TORM_APP_CMD_ID_DB_PROXY 1    /* 用于设置torm head appcmd 字段, 标记是db proxy发起的请求 */
#define TORM_APP_CMD_ID_TORM_MGR 2    /* 用于设置torm head appcmd 字段, 标记是tormmgr发起的请求 */


#define MAX_ORM_TABLE_SUPPORT 50

class CTormMgr;

class CTormHandler
{
public:
    const tagORMPkgHead *GetTormHead()
    {
        return m_pTormHead;
    }

    virtual int HandleUpdate(CTormMgr *pMgr, tagORMUpdateRes *pUpdateRes, void *pCbData, int iCbDataLen) { return 0; }
    virtual int HandleSelect(CTormMgr *pMgr, tagORMSelectRes *pSelectRes, void *pCbData, int iCbDataLen) { return 0; }
    virtual int HandleDelete(CTormMgr *pMgr, tagORMDeleteRes *pDeleteRes, void *pCbData, int iCbDataLen) { return 0; }
    virtual int HandleInsert(CTormMgr *pMgr, tagORMInsertRes *pInsertRes, void *pCbData, int iCbDataLen) { return 0; }
    virtual int HandleReplaceInsert(CTormMgr *pMgr, tagORMReplaceInsertRes *pReplaceInsertRes, void *pCbData, int iCbDataLen) { return 0; }
    virtual int HandleSelectCnt(CTormMgr *pMgr, tagORMSelectCountRes *pSelectCntRes, void *pCbData, int iCbDataLen) { return 0; }

    tagORMPkgHead *m_pTormHead;
};

//对应TORM_APP_CMD_ID_DB_PROXY的处理函数的原形
typedef int(*FnDBProxyHandleMsg)(tagORMPkg *pstTormMsg);

class CTormMgr
{
public:
    /**
    * 初始化DB管理模块
    */
    int Init(const char *szDrPath, int iDstFuncID = FUNC_CACHE_DB_PROXY);

    /**
    * 带where的更新单条数据表
    */
    int Update(const char *szTableName, void *pData, int iDataSize, char *szWhere, void *pCbData, int iCbDataLen, int iSessID);

    /**
    * 更新数据表
    */
    int Update(const char *szTableName, void *pData, int iDataSize,
               void *pCbData, int iCbDataLen, int iSessID = 0);

    int Update(const char *szTableName, void *pData, int iDataSize,
               tagORMEntryNameList *pEntryList, const char *szWhere,
               void *pCbData, int iCbDataLen, int iSessID = 0);

    /**
    * 查询数据表
    */
    int Query(const char *szTableName, void *pData, int iDataSize,
              tagORMEntryNameList *pstEntryList, const char *szWhere, int iMaxLimit,
              void *pCbData, int iCbDataLen, int iSessID = 0);

    /**
    * 插入数据表
    */
    int Insert(const char *szTableName, void *pData, int iDataSize,
               void *pCbData, int iCbDataLen, int iSessID = 0);

    /**
    * 存在则更新，不存在则插入数据表
    */
    int ReplaceInsert(const char *szTableName, void *pData, int iDataSize,
                      void *pCbData, int iCbDataLen, int iSessID = 0);

    /**
    * 删除数据表
    */
    int Delete(const char *szTableName, void *pData, int iDataSize,
               void *pCbData, int iCbDataLen, int iSessID = 0);

    /**
    * 增加数据表异步消息处理函数
    */
    int AddDbHandler(const char *szTableName, CTormHandler *pHandler);

    int HandleTormResInCoContext(char *pPkg, int iPkgLen);

    /**
    *	获得对应表的单行记录大小
    */
    int GetTableMetaSize(const char *szTableName);

    DR_HANDLE_META GetTableMeta(const char *szTableName);

    /**
    *	获得torm回包中的单行记录
    */
    int FetchOneData(char *pData, int iSize);
    int GetTormSeq() { return m_iTormSeq; }
    int AllocSeq();
    
    FnDBProxyHandleMsg m_pFnDBProxyHandleMsg;
    
protected:
    CTormHandler *GetDbHandler(const char *szTableName);

protected:
    dsf::COrmApi m_ormApi;
    DR_HANDLE_LIB m_pDbMetaLib;
    dsf::BUSADDR m_selfAddr;
    dsf::BUSADDR m_tormAddr;
    int m_iTormSeq;
    tagORMPkg m_stPkg;
    char m_szSendBuf[1024 * 1024];

    CTormHandler *m_tormHandler[MAX_ORM_TABLE_SUPPORT];
    char m_aTormTableName[MAX_ORM_TABLE_SUPPORT][128];
    int m_iTormHandleCnt;

    //专门用于torm消息回包后,copy cb数据，然后再回调上层的应用
    //应为消息包中的torm和发送请求是公用一个结构体，容易被回调的函数修改掉
    char m_asynCallBackData[ORM_MAX_ASYNCALLBACK_DATA_LEN];
};


#endif


