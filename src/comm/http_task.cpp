#include <curl/curl.h>
#include <curl/easy.h>
#include <stdint.h>
#include <string.h>

#include "klog.h"
#include "http_task.h"

struct tagHttpTask
{
	CURL * pstCurl;
	curl_slist *pHeaderList;
		
	int    iUsed;
	char   szUrl[1024];
	time_t tStartTime;
	int    iTimeoutMs;
	
	char szBuff[1024*10];	//10k的缓冲区
	
	FnHttpTaskCallBack fnCB;
	int iUserDataSize;
	uint8_t aucUserData[0];
};

static void* gs_pHttpTasks = NULL;
static int gs_iMaxTaskCount = 0;
static int gs_iUserDataSize = 0;

CURLM* gs_pMultiHandle = NULL;

static int get_task_unit_size()
{
	return sizeof(HttpTask)+gs_iUserDataSize;
}

static HttpTask* get_task_by_index(int iIndex)
{
	return (HttpTask*)((char*)gs_pHttpTasks + get_task_unit_size()*iIndex);
}

int http_task_running_count()
{
	int iRunning = 0;
	int i;
	for (i = 0; i < gs_iMaxTaskCount; i++)
	{
		HttpTask* pTask = get_task_by_index(i);
		if (!pTask->iUsed)
		{
			continue;
		}
		
		iRunning++;
	}

	return iRunning;
}

static size_t http_write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	HttpTask* pTask = (HttpTask*)stream;
	strncpy(pTask->szBuff, (char *)ptr, sizeof(pTask->szBuff)-1);	
	return size*nmemb;
}

static int start_http_request(HttpTask* pTask)
{
	ASSERT_RET(NULL == pTask->pstCurl, -1);

	pTask->pstCurl = curl_easy_init();
	ASSERT_RET(pTask->pstCurl != NULL, -1);

	curl_easy_setopt(pTask->pstCurl, CURLOPT_URL, pTask->szUrl);
	curl_easy_setopt(pTask->pstCurl, CURLOPT_WRITEFUNCTION, http_write_data);
	curl_easy_setopt(pTask->pstCurl, CURLOPT_WRITEDATA, pTask);
	curl_easy_setopt(pTask->pstCurl, CURLOPT_PRIVATE, pTask);
	curl_easy_setopt(pTask->pstCurl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(pTask->pstCurl, CURLOPT_TIMEOUT_MS, pTask->iTimeoutMs);
	curl_easy_setopt(pTask->pstCurl, CURLOPT_CONNECTTIMEOUT_MS, pTask->iTimeoutMs);

	//增加头部字段
	if (pTask->pHeaderList != NULL)
	{
		curl_easy_setopt(pTask->pstCurl, CURLOPT_HTTPHEADER, pTask->pHeaderList);
	}
	
	int iErrCode = curl_multi_add_handle(gs_pMultiHandle, pTask->pstCurl);
	if (iErrCode != 0)
	{
		LOG_ERR("curl_multi_add_handle failed, ret[%d]", iErrCode);
		return -1;
	}
	
	return 0;
}

/**
* 初始化http多任务管理模块
* @param iMaxTaskCnt 最大的任务个数
* @param iUserDataSize 每个任务绑定的用户数据大小
*
* @return 返回初始化结果
*		0 - 初始化成功
*		其他 - 失败
*/
int http_task_init(int iMaxTaskCnt, int iUserDataSize)
{
	ASSERT_RET(iMaxTaskCnt > 0 && iUserDataSize > 0, -1);

	gs_iMaxTaskCount = iMaxTaskCnt;
	gs_iUserDataSize = iUserDataSize;

	int iMaxSize = get_task_unit_size() * iMaxTaskCnt;
	gs_pHttpTasks = malloc(iMaxSize);

	bzero(gs_pHttpTasks, iMaxSize);

	gs_pMultiHandle = curl_multi_init();
	ASSERT_RET(gs_pMultiHandle != NULL, -1);
	
	return 0;
}

void http_task_destroy()
{
	if (gs_pHttpTasks != NULL)
	{
		free(gs_pHttpTasks);
		gs_pHttpTasks = NULL;
	}

	if (gs_pMultiHandle != NULL)
	{
		curl_multi_cleanup(gs_pMultiHandle);
	}
}

static void notify_cb(HttpTask* pTask, int iResult)
{
	//HttpTask* pTask, int iResult, const char* szText
	
	pTask->fnCB(pTask, iResult, pTask->szBuff);
}

/**
* 循环调度模块，用于驱动多任务的处理逻辑
*/
int http_task_proc()
{	
	static time_t s_tLastLogTime = 0;

	
	if (s_tLastLogTime + 1 < time(NULL))
	{
		s_tLastLogTime = time(NULL);
		//LOG_ERR("http task static, running count[%d]", http_task_running_count());
	}
	
	
	CURLMsg *pUrlMgs = NULL;
	int iStillRunning = 0;
	int i;

	curl_multi_perform(gs_pMultiHandle, &iStillRunning);
	
	for (i = 0; i < gs_iMaxTaskCount; i++)
	{
		HttpTask* pTask = get_task_by_index(i);
		if (!pTask->iUsed)
		{
			continue;
		}

		//循环获取到所有的对象，判断是否有待启动的任务
		if (pTask->tStartTime > 0 && NULL == pTask->pstCurl)
		{
			int iRet = start_http_request(pTask);
			ASSERT(0 == iRet);
		}
	}
	
	curl_multi_perform(gs_pMultiHandle, &iStillRunning);
	
	//获取到所有完成的请求
	while ((pUrlMgs = curl_multi_info_read(gs_pMultiHandle, &iStillRunning)))
    {
    	if (pUrlMgs->msg == CURLMSG_DONE)
    	{
    		//get the HttpTask
    		HttpTask* pTask = NULL;
    		curl_easy_getinfo(pUrlMgs->easy_handle, CURLINFO_PRIVATE, &pTask);

			if (pTask != NULL && pTask->pstCurl == pUrlMgs->easy_handle)
			{
				notify_cb(pTask, pUrlMgs->data.result);
				
				//free函数内部会free easy handle 接口
				http_task_free(pTask);
			}
			else
			{
				LOG_ERR("cant find private date, pTask: %p, pTask->Curl: %p, pUrlMgs->easy_handle: %p", 
					pTask, pTask!=NULL?pTask->pstCurl:NULL, pUrlMgs->easy_handle);
				
				curl_easy_cleanup(pUrlMgs->easy_handle);
			}
    	}
	}
	
	return iStillRunning;
}


/**
* 分配新的http请求任务
*/
HttpTask* http_task_alloc()
{
	int i;
	for (i = 0; i < gs_iMaxTaskCount; i++)
	{
		HttpTask* pTask = get_task_by_index(i);
		if (!pTask->iUsed)
		{
			bzero(pTask, sizeof(HttpTask));
			pTask->iUsed = 1;
			return pTask;
		}		
	}
	
	return NULL;
}


/**
* 释放http任务
*/
void http_task_free(HttpTask* pTask)
{
	int iTaskUnitSize = get_task_unit_size();
	int iOffset = (intptr_t)pTask - (intptr_t)gs_pHttpTasks;
	int iIndex = iOffset/iTaskUnitSize;
	ASSERT_RET( (iOffset%iTaskUnitSize) == 0 && 
		iIndex >= 0 && iIndex < gs_iMaxTaskCount);

	if (!pTask->iUsed)
	{
		return;
	}

	if (pTask->pstCurl != NULL)
	{	
		curl_easy_cleanup(pTask->pstCurl);	
		pTask->pstCurl = NULL;
	}

	if (pTask->pHeaderList != NULL)
	{
		curl_slist_free_all(pTask->pHeaderList);
		pTask->pHeaderList = NULL;
	}
	
	bzero(pTask, iTaskUnitSize);
}

/**
* 设置http的请求URL
* @note 如果是get类型，直接将参数带到url里
*/
int http_task_set_url(HttpTask* pTask, const char* szUrl)
{
	strncpy(pTask->szUrl, szUrl, sizeof(pTask->szUrl)-1);
	return 0;
}

/**
* 设置用户绑定数据
* 内部将会做copy操作，内部内存管理
*/
int http_task_set_userdata(HttpTask* pTask, void *pUserData, int iLen)
{
	ASSERT_RET(iLen <= gs_iUserDataSize, -1);
	
	memcpy((void*)pTask->aucUserData, pUserData, (size_t)iLen);
	return 0;
}

/**
* 获取用户数据
*/
void* http_task_get_userdata(HttpTask* pTask)
{
	return pTask->aucUserData;
}

const char* http_task_get_url(HttpTask* pTask)
{
	return pTask->szUrl;
}

/**
* 设置回调函数
*/
void http_task_set_callback(HttpTask* pTask, FnHttpTaskCallBack fnCB)
{
	pTask->fnCB = fnCB;
}


/**
* 开始http请求
*/
int http_task_start(HttpTask* pTask, int iTimeoutMs, map<string, string>* httpHeader)
{
	pTask->tStartTime = time(NULL);
	pTask->iTimeoutMs = iTimeoutMs;

	if (httpHeader != NULL)
	{
		curl_slist *headerlist=NULL;
		map<string, string>::iterator itr = httpHeader->begin();
		for(; itr != httpHeader->end(); itr++)
		{
			string key = itr->first;
			string val = itr->second;
			char szBuff[512];
			snprintf(szBuff, sizeof(szBuff), "%s: %s", key.c_str(), val.c_str());
			headerlist = curl_slist_append(headerlist, szBuff);
		}

		if (headerlist != NULL)
		{
			pTask->pHeaderList = headerlist;
		}
	}
	
	return 0;
}



