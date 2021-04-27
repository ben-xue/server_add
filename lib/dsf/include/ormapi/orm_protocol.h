/*** 
* file: orm_protocol.h 
* auto generate by dodjoy
*/
#ifndef ORM_PROTOCOL_H__
#define ORM_PROTOCOL_H__


#include <stdint.h>
#include "pal/pal_os.h"

#ifdef __cplusplus
extern "C" {
#endif


#define ORM_MAGIC                                    0x0927
#define ORM_MAX_PKG_LEN                              3147152
#define ORM_MAX_BODY_LEN                             3145728
#define ORM_MAX_DATA_BUFF_LEN                        1038336
#define ORM_MAX_META_NAME                            32
#define ORM_MAX_ENTRYLIST                            256
#define ORM_MAX_WHERE_DEF                            1024
#define ORM_MAX_INTER_CHANNEL                        256
#define ORM_MAX_BUS_CHANNEL                          1000
#define ORM_MAX_ASYNCALLBACK_DATA_LEN                1024    /*请求方能保留的回调数据最大字节数*/
enum tagORM_CMDS
{
    ORM_EXGVER_REQ = 1,    /*进程间开始通信时的版本交换协议*/
    ORM_EXGVER_RES = 2,
    ORM_INSERT_REQ = 3,
    ORM_INSERT_RES = 4,
    ORM_UPDATE_REQ = 5,
    ORM_UPDATE_RES = 6,
    ORM_DELETE_REQ = 7,
    ORM_DELETE_RES = 8,
    ORM_SELECT_REQ = 9,
    ORM_SELECT_RES = 10,
    ORM_SELECT_COUNT_REQ = 11,
    ORM_SELECT_COUNT_RES = 12,
    ORM_REPLACE_INSERT_REQ = 13,
    ORM_REPLACE_INSERT_RES = 14,
    ORM_BODY_MAX_SIZE = 15
};

#pragma pack(1)


struct tagORMMetaNameVer;
typedef struct tagORMMetaNameVer                            ORMMETANAMEVER;
typedef struct tagORMMetaNameVer                            *LPORMMETANAMEVER;

struct tagORMPkgHeadComm;
typedef struct tagORMPkgHeadComm                            ORMPKGHEADCOMM;
typedef struct tagORMPkgHeadComm                            *LPORMPKGHEADCOMM;

struct tagORMPkgHeadCmd;
typedef struct tagORMPkgHeadCmd                             ORMPKGHEADCMD;
typedef struct tagORMPkgHeadCmd                             *LPORMPKGHEADCMD;

struct tagORMPkgHeadApp;
typedef struct tagORMPkgHeadApp                             ORMPKGHEADAPP;
typedef struct tagORMPkgHeadApp                             *LPORMPKGHEADAPP;

struct tagORMPkgHead;
typedef struct tagORMPkgHead                                ORMPKGHEAD;
typedef struct tagORMPkgHead                                *LPORMPKGHEAD;

struct tagORMExgVerReq;
typedef struct tagORMExgVerReq                              ORMEXGVERREQ;
typedef struct tagORMExgVerReq                              *LPORMEXGVERREQ;

struct tagORMExgVerRes;
typedef struct tagORMExgVerRes                              ORMEXGVERRES;
typedef struct tagORMExgVerRes                              *LPORMEXGVERRES;

struct tagORMDataBuff;
typedef struct tagORMDataBuff                               ORMDATABUFF;
typedef struct tagORMDataBuff                               *LPORMDATABUFF;

struct tagORMEntryNameList;
typedef struct tagORMEntryNameList                          ORMENTRYNAMELIST;
typedef struct tagORMEntryNameList                          *LPORMENTRYNAMELIST;

struct tagORMOperInfo;
typedef struct tagORMOperInfo                               ORMOPERINFO;
typedef struct tagORMOperInfo                               *LPORMOPERINFO;

struct tagORMInsertReq;
typedef struct tagORMInsertReq                              ORMINSERTREQ;
typedef struct tagORMInsertReq                              *LPORMINSERTREQ;

struct tagORMInsertRes;
typedef struct tagORMInsertRes                              ORMINSERTRES;
typedef struct tagORMInsertRes                              *LPORMINSERTRES;

struct tagORMUpdateReq;
typedef struct tagORMUpdateReq                              ORMUPDATEREQ;
typedef struct tagORMUpdateReq                              *LPORMUPDATEREQ;

struct tagORMUpdateRes;
typedef struct tagORMUpdateRes                              ORMUPDATERES;
typedef struct tagORMUpdateRes                              *LPORMUPDATERES;

struct tagORMDeleteReq;
typedef struct tagORMDeleteReq                              ORMDELETEREQ;
typedef struct tagORMDeleteReq                              *LPORMDELETEREQ;

struct tagORMDeleteRes;
typedef struct tagORMDeleteRes                              ORMDELETERES;
typedef struct tagORMDeleteRes                              *LPORMDELETERES;

struct tagORMSelectReq;
typedef struct tagORMSelectReq                              ORMSELECTREQ;
typedef struct tagORMSelectReq                              *LPORMSELECTREQ;

struct tagORMSelectResult;
typedef struct tagORMSelectResult                           ORMSELECTRESULT;
typedef struct tagORMSelectResult                           *LPORMSELECTRESULT;

struct tagORMSelectRes;
typedef struct tagORMSelectRes                              ORMSELECTRES;
typedef struct tagORMSelectRes                              *LPORMSELECTRES;

struct tagORMSelectCountReq;
typedef struct tagORMSelectCountReq                         ORMSELECTCOUNTREQ;
typedef struct tagORMSelectCountReq                         *LPORMSELECTCOUNTREQ;

struct tagORMSelectCountRes;
typedef struct tagORMSelectCountRes                         ORMSELECTCOUNTRES;
typedef struct tagORMSelectCountRes                         *LPORMSELECTCOUNTRES;

struct tagORMBodyMaxSize;
typedef struct tagORMBodyMaxSize                            ORMBODYMAXSIZE;
typedef struct tagORMBodyMaxSize                            *LPORMBODYMAXSIZE;

struct tagORMReplaceInsertReq;
typedef struct tagORMReplaceInsertReq                       ORMREPLACEINSERTREQ;
typedef struct tagORMReplaceInsertReq                       *LPORMREPLACEINSERTREQ;

struct tagORMReplaceInsertRes;
typedef struct tagORMReplaceInsertRes                       ORMREPLACEINSERTRES;
typedef struct tagORMReplaceInsertRes                       *LPORMREPLACEINSERTRES;

union tagORMPkgBody;
typedef union tagORMPkgBody                                ORMPKGBODY;
typedef union tagORMPkgBody                                *LPORMPKGBODY;

struct tagORMPkg;
typedef struct tagORMPkg                                    ORMPKG;
typedef struct tagORMPkg                                    *LPORMPKG;

struct tagORMMetaNameVer
{
    char MetaName[ORM_MAX_META_NAME];
    uint32_t MetaVer;
};

struct tagORMPkgHeadComm
{
    uint16_t Magic;
    int32_t Seq;
    /*消息bus源地址*/
    uint32_t Src;
};

struct tagORMPkgHeadCmd
{
    uint16_t Cmd;
    /*本次操作相关MetaName和版本信息*/
    tagORMMetaNameVer MetaNameVer;
    /*tormapi内部使用，应用层不需为此字段赋值*/
    uint32_t SplitFactor;
};

struct tagORMPkgHeadApp
{
    /*会话ID*/
    int32_t SessID;
    /*回话seq*/
    int32_t SessSeq;
    /*cmd*/
    int32_t SessCmd;
    int32_t AsynCallBackDataLen;
    /*ORMSVR会将请求消息 中此字段中的信息原封不动地放到响应消息中，这样请求方可使用此字段保留回调数据 */
    uint8_t AsynCallBackData[ORM_MAX_ASYNCALLBACK_DATA_LEN];
};

struct tagORMPkgHead
{
    /*公共字段*/
    tagORMPkgHeadComm HeadComm;
    /*请求命令字相关*/
    tagORMPkgHeadCmd HeadCmd;
    /*用户带过来的*/
    tagORMPkgHeadApp HeadApp;
};

struct tagORMExgVerReq
{
    uint32_t Version;
};

struct tagORMExgVerRes
{
    uint32_t Version;
};

struct tagORMDataBuff
{
    uint32_t Len;
    uint8_t Buff[ORM_MAX_DATA_BUFF_LEN];
};

struct tagORMEntryNameList
{
    uint32_t EntryNameNum;
    char EntryName[ORM_MAX_ENTRYLIST][ORM_MAX_META_NAME];
};

struct tagORMOperInfo
{
    /*若EntryNameNum为0,则操作针对头部指定的Meta*/
    tagORMEntryNameList EntryNameList;
    /*entry list的pack后数据*/
    tagORMDataBuff EntryContent;
    /*分表因子*/
    int32_t SplitFactor;
    /*主键个数*/
    int32_t PrimKeyCount;
    /*主键信息*/
    tagORMDataBuff PrimKeyContent;
    /*没有自定操作约束(where etc.),必须设置为空串;如果此字段不是空串，ORMAPI不会自动生成主键信息*/
    char WhereDef[ORM_MAX_WHERE_DEF];
};

struct tagORMInsertReq
{
    tagORMOperInfo OperInfo;
};

struct tagORMInsertRes
{
    /*错误码参考orm_err.h*/
    int32_t ErrNo;
    /*数据库返回的错误码*/
    int32_t DBMSErrNo;
    /*返回本次insert操作所生成的AUTO_INCREMENT的值，此字段只有当操作数据中定义了AUTO_INCREMENT成员才有意义*/
    uint64_t LastInsertID;
};

struct tagORMUpdateReq
{
    tagORMOperInfo OperInfo;
};

struct tagORMUpdateRes
{
    /*错误码参考torm_error.h*/
    int32_t ErrNo;
    /*数据库返回的错误码*/
    int32_t DBMSErrNo;
    /*受影响的行数*/
    int32_t AffectRows;
};

struct tagORMDeleteReq
{
    tagORMOperInfo OperInfo;
};

struct tagORMDeleteRes
{
    /*错误码参考orm_err.h*/
    int32_t ErrNo;
    /*数据库返回的错误码*/
    int32_t DBMSErrNo;
    /*受影响的行数*/
    int32_t AffectRows;
};

struct tagORMSelectReq
{
    tagORMOperInfo OperInfo;
    /*如其值大于0则设置从数据库中检索出的最大记录行数,如果其值为0,则对记录行数没有限制*/
    uint32_t Limit;
    /*则设置返回的第一行记录在整个结果集中的偏移量(初始行的偏移量为0),如果其值为0，则从结果集的开头返回记录行*/
    uint32_t OffSet;
};

struct tagORMSelectResult
{
    uint32_t ResultTotal;
    uint32_t ThisResultStart;
    int32_t ResultNum;
    int32_t ResultLen;
    uint8_t ResultList[ORM_MAX_BODY_LEN];
    /*若EntryNameNum为0,则操作针对头部指定的Meta*/
    tagORMEntryNameList EntryNameList;
};

struct tagORMSelectRes
{
    /*错误码参考torm_error.h*/
    int32_t ErrNo;
    /*数据库返回的错误码*/
    int32_t DBMSErrNo;
    tagORMSelectResult SelectResult;
};

/*指定查询条件，请求tormsvr执行select count(*)操作，即请求检索出符合查询条件的记录行总数*/
struct tagORMSelectCountReq
{
    tagORMOperInfo OperInfo;
};

struct tagORMSelectCountRes
{
    /*错误码参考torm_error.h*/
    int32_t ErrNo;
    /*返回数据库表中匹配查询条件的记录行总数*/
    uint32_t RowsCount;
};

struct tagORMBodyMaxSize
{
    uint8_t MaxBody[ORM_MAX_BODY_LEN];
};

struct tagORMReplaceInsertReq
{
    tagORMOperInfo OperInfo;
};

struct tagORMReplaceInsertRes
{
    /*错误码参考orm_err.h*/
    int32_t ErrNo;
    /*数据库返回的错误码*/
    int32_t DBMSErrNo;
    /*返回本次insert操作所生成的AUTO_INCREMENT的值，此字段只有当操作数据中定义了AUTO_INCREMENT成员才有意义*/
    uint64_t LastInsertID;
};

union tagORMPkgBody
{
    tagORMExgVerReq ExgVerReq;
    tagORMExgVerRes ExgVerRes;
    tagORMInsertReq InsertReq;
    tagORMInsertRes InsertRes;
    tagORMUpdateReq UpdateReq;
    tagORMUpdateRes UpdateRes;
    tagORMDeleteReq DeleteReq;
    tagORMDeleteRes DeleteRes;
    tagORMSelectReq SelectReq;
    tagORMSelectRes SelectRes;
    tagORMSelectCountReq SelectCountReq;
    tagORMSelectCountRes SelectCountRes;
    tagORMReplaceInsertReq ReplaceInsertReq;
    tagORMReplaceInsertRes ReplaceInsertRes;
};

struct tagORMPkg
{
    tagORMPkgHead Head;
    tagORMPkgBody Body;
};

#pragma pack()


#ifdef __cplusplus
}
#endif

#endif
