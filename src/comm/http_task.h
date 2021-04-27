#ifndef HTTP_TASK_H__
#define HTTP_TASK_H__

#include <string>
#include <map>

using std::map;
using std::string;

typedef struct tagHttpTask HttpTask;

/**
* 初始化http多任务管理模块
* @param iMaxTaskCnt 最大的任务个数
* @param iUserDataSize 每个任务绑定的用户数据大小
*
* @return 返回初始化结果
*		0 - 初始化成功
*		其他 - 失败
*/
int http_task_init(int iMaxTaskCnt, int iUserDataSize);

/**
* 循环调度模块，用于驱动多任务的处理逻辑
*/
int http_task_proc();

/**
* 分配新的http请求任务
*/
HttpTask* http_task_alloc();

/**
* 释放http任务
*/
void http_task_free(HttpTask* pTask);

/**
* 设置http的请求URL
* @note 如果是get类型，直接将参数带到url里
*/
int http_task_set_url(HttpTask* pTask, const char* szUrl);

/**
* 设置用户绑定数据
* 内部将会做copy操作，内部内存管理
*/
int http_task_set_userdata(HttpTask* pTask, void *pUserData, int iLen);

/**
* 获取用户数据
*/
void* http_task_get_userdata(HttpTask* pTask);

const char* http_task_get_url(HttpTask* pTask);

/**
* Http请求回调函数
* iResult - 返回错误，参考CURLcode的定义
*/
typedef void (*FnHttpTaskCallBack)(HttpTask* pTask, int iResult, const char* szText);

/**
* 设置回调函数
*/
void http_task_set_callback(HttpTask* pTask, FnHttpTaskCallBack fnCB);


/**
* 开始http请求
*/
int http_task_start(HttpTask* pTask, int iTimeout, map<string, string>* pHttpHeader);


/**
* 获取统计数据
*/
int http_task_running_count();






#endif

