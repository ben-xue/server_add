/*** 
* file: log_def.h 
* auto generate by dodjoy
*/
#ifndef LOG_DEF_H__
#define LOG_DEF_H__


#include <stdint.h>
#include "pal/pal_os.h"


#ifdef __cplusplus
extern "C" {
#endif


#define LOG_FILE_PATTERN_LEN                         256    /*日志文件格式最大长度*/
#define LOG_MAX_NET_IP_LEN                           64    /*IP最大长度*/
#define LOG_CATEGORY_NAME_MAX_LEN                    256    /*日志最大长度*/
#define LOG_CATEGORY_FORMAT_MAX_LEN                  256    /*日志最大长度*/
#define MAX_LOG_CATEGORY_COUNT                       32    /*最大日志类型*/
/*日志级别定义*/
enum tagLogPriority
{
    LOG_PRIORITY_FATAL = 0,
    LOG_PRIORITY_ALERT = 100,
    LOG_PRIORITY_CRIT = 200,
    LOG_PRIORITY_ERROR = 300,
    LOG_PRIORITY_WARN = 400,
    LOG_PRIORITY_NOTICE = 500,
    LOG_PRIORITY_INFO = 600,
    LOG_PRIORITY_DEBUG = 700,
    LOG_PRIORITY_TRACE = 800,
    LOG_PRIORITY_MAX = 1000
};

/*网络设备类型定义*/
enum tagLogDeviceType
{
    LOG_DEVICE_TYPE_FILE = 0,    /*文件*/
    LOG_DEVICE_TYPE_NET = 1,    /*网络*/
    LOG_DEVICE_TYPE_STD = 2    /*标准输出*/
};

#pragma pack(1)


struct tagLogDeviceFileConf;
typedef struct tagLogDeviceFileConf                         LOGDEVICEFILECONF;
typedef struct tagLogDeviceFileConf                         *LPLOGDEVICEFILECONF;

struct tagLogDeviceNetConf;
typedef struct tagLogDeviceNetConf                          LOGDEVICENETCONF;
typedef struct tagLogDeviceNetConf                          *LPLOGDEVICENETCONF;

struct tagLogDeviceStdConf;
typedef struct tagLogDeviceStdConf                          LOGDEVICESTDCONF;
typedef struct tagLogDeviceStdConf                          *LPLOGDEVICESTDCONF;

union tagLogDeviceSelectorConf;
typedef union tagLogDeviceSelectorConf                     LOGDEVICESELECTORCONF;
typedef union tagLogDeviceSelectorConf                     *LPLOGDEVICESELECTORCONF;

struct tagLogDeviceConf;
typedef struct tagLogDeviceConf                             LOGDEVICECONF;
typedef struct tagLogDeviceConf                             *LPLOGDEVICECONF;

struct tagLogCategoryConf;
typedef struct tagLogCategoryConf                           LOGCATEGORYCONF;
typedef struct tagLogCategoryConf                           *LPLOGCATEGORYCONF;

struct tagLogConf;
typedef struct tagLogConf                                   LOGCONF;
typedef struct tagLogConf                                   *LPLOGCONF;

/*文件日志*/
struct tagLogDeviceFileConf
{
    /*文件名格式*/
    char Pattern[LOG_FILE_PATTERN_LEN];
    /*Buff大小*/
    int32_t BuffSize;
    /*文件大小*/
    int32_t SizeLimit;
    /*同步间隔*/
    int32_t SyncTime;
};

/*网络日志*/
struct tagLogDeviceNetConf
{
    /*Ip最大长度*/
    char Ip[LOG_MAX_NET_IP_LEN];
    /*端口*/
    int32_t Port;
    /*发送缓冲区大小*/
    int32_t SendBuff;
    /*超时时长*/
    int32_t Timeout;
};

/*标准输出*/
struct tagLogDeviceStdConf
{
    /*预留*/
    uint8_t reserve;
};

/*日志设备*/
union tagLogDeviceSelectorConf
{
    /*文件日志*/
    tagLogDeviceFileConf File;
    /*网络日志*/
    tagLogDeviceNetConf Net;
    /*标准输出*/
    tagLogDeviceStdConf Std;
};

/*日志输出设备*/
struct tagLogDeviceConf
{
    /*设备类型*/
    int32_t Type;
    /*设备*/
    tagLogDeviceSelectorConf Device;
};

/*日志类型*/
struct tagLogCategoryConf
{
    /*名称*/
    char Name[LOG_CATEGORY_NAME_MAX_LEN];
    /*日志级别区间*/
    int32_t PriorityHigh;
    /*日志级别区间*/
    int32_t PriorityLow;
    /*消息最大长度*/
    int32_t MaxMsgSize;
    /*日志行格式*/
    char Format[LOG_CATEGORY_FORMAT_MAX_LEN];
    /*后置category*/
    char ForwardCat[LOG_CATEGORY_NAME_MAX_LEN];
    /*日志输出设备*/
    tagLogDeviceConf Device;
};

/*日志配置*/
struct tagLogConf
{
    int32_t PriorityHigh;
    int32_t PriorityLow;
    /*Category个数*/
    int32_t Count;
    /*日志类型列表*/
    tagLogCategoryConf CategoryList[MAX_LOG_CATEGORY_COUNT];
};

#pragma pack()


#ifdef __cplusplus
}
#endif

#endif
