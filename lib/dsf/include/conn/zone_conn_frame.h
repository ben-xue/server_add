/*** 
* file: zone_conn_frame.h 
* auto generate by dodjoy
*/
#ifndef ZONE_CONN_FRAME_H__
#define ZONE_CONN_FRAME_H__


#include <stdint.h>
#include "pal/pal_os.h"


#ifdef __cplusplus
extern "C" {
#endif


#define FRAMEHEAD_MAX_BATCH_IDENTIFY                 512    /*一次最大conn数量*/
#define FRAME_HEAD_MAGIC                             914
/*命令号*/
enum tagFrameHeadCmd
{
    FRAME_HEAD_CMD_START = 1,    /*开始*/
    FRAME_HEAD_CMD_STOP = 2,    /*断开*/
    FRAME_HEAD_CMD_INPROC = 3,    /*Proc*/
    FRAME_HEAD_CMD_QUEUE = 4    /*排队*/
};

/*断开连接reason*/
enum tagConnStopReason
{
    CONN_STOP_REASON_NONE = 0,
    CONN_STOP_REASON_SHUTDOWN = 1,
    CONN_STOP_REASON_IDLE = 2,
    CONN_STOP_REASON_APP_CLOSE = 3,
    CONN_STOP_REASON_NETWORK = 4,
    CONN_STOP_REASON_BADPKG = 5,
    CONN_STOP_REASON_KICKOFF = 7,
    CONN_STOP_REASON_LOGOUT = 8,
    CONN_STOP_REASON_LOST = 9,
    CONN_STOP_REASON_STATUS_FAILED = 10,
    CONN_STOP_REASON_BUS_BUSY = 11
};

/*广播flag*/
enum tagBroadcastFlag
{
    BROADCAST_FLAG_NO = 0,
    BROADCAST_FLAG_GROUP = 1,
    BROADCAST_FLAG_ALL = 2
};

#pragma pack(1)


struct tagAddrInfo;
typedef struct tagAddrInfo                                  ADDRINFO;
typedef struct tagAddrInfo                                  *LPADDRINFO;

struct tagFrameCmdStart;
typedef struct tagFrameCmdStart                             FRAMECMDSTART;
typedef struct tagFrameCmdStart                             *LPFRAMECMDSTART;

struct tagFrameCmdStop;
typedef struct tagFrameCmdStop                              FRAMECMDSTOP;
typedef struct tagFrameCmdStop                              *LPFRAMECMDSTOP;

struct tagConnIdentify;
typedef struct tagConnIdentify                              CONNIDENTIFY;
typedef struct tagConnIdentify                              *LPCONNIDENTIFY;

struct tagFrameCmdInProc;
typedef struct tagFrameCmdInProc                            FRAMECMDINPROC;
typedef struct tagFrameCmdInProc                            *LPFRAMECMDINPROC;

struct tagFrameCmdQueue;
typedef struct tagFrameCmdQueue                             FRAMECMDQUEUE;
typedef struct tagFrameCmdQueue                             *LPFRAMECMDQUEUE;

union tagFrameCmdData;
typedef union tagFrameCmdData                              FRAMECMDDATA;
typedef union tagFrameCmdData                              *LPFRAMECMDDATA;

struct tagFrameExtenData;
typedef struct tagFrameExtenData                            FRAMEEXTENDATA;
typedef struct tagFrameExtenData                            *LPFRAMEEXTENDATA;

struct tagFrameHead;
typedef struct tagFrameHead                                 FRAMEHEAD;
typedef struct tagFrameHead                                 *LPFRAMEHEAD;

struct tagAddrInfo
{
    uint32_t Ip;
    uint16_t Port;
};

struct tagFrameCmdStart
{
    /*预留*/
    uint8_t Reserve;
};

struct tagFrameCmdStop
{
    /*原因码*/
    int32_t Reason;
};

/*连接Id*/
struct tagConnIdentify
{
    /*conn id*/
    int32_t AppConnId;
    /*conn idx*/
    uint32_t ConnId;
};

struct tagFrameCmdInProc
{
    /*BroadcastFlag*/
    int32_t Flag;
    int32_t Count;
    tagConnIdentify ConnList[FRAMEHEAD_MAX_BATCH_IDENTIFY];
};

struct tagFrameCmdQueue
{
    /*队列位置*/
    int32_t QueueIdx;
};

union tagFrameCmdData
{
    tagFrameCmdStart Start;
    tagFrameCmdStop Stop;
    tagFrameCmdInProc InProc;
    tagFrameCmdQueue Queue;
};

struct tagFrameExtenData
{
    /*ip端口信息*/
    tagAddrInfo Addr;
};

struct tagFrameHead
{
    /*magic*/
    uint16_t Magic;
    /*命令号*/
    char Cmd;
    /*连接ID*/
    tagConnIdentify ConnId;
    /*usec*/
    uint64_t timestamp;
    /*命令详细信息*/
    tagFrameCmdData CmdData;
    /*扩展数据*/
    tagFrameExtenData ExtenData;
};

#pragma pack()


#ifdef __cplusplus
}
#endif

#endif
