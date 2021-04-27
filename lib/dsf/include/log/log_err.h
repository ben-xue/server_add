/**
*
* @file   log_err.h
* @author pls
* @brief  日志错误码
*/


#ifndef _DSF_DR_ERR_H__
#define _DSF_DR_ERR_H__

#ifdef __cplusplus
extern "C"
{
#endif

enum LogErrCode
{
	LOG_ERR_CRIT = -1,
	LOG_ERR_OK = 0,			//正确
	LOG_ERR_OPEN_FAIL_FAIL,	//打开文件失败
	LOG_ERR_FILE_ALREADY_OPEN, //文件已经被打开了
	LOG_ERR_NET_DOWN,		//打开网络连接失败
	LOG_ERR_FORWARD_LOOP,	//forward存在循环
	LOG_ERR_INVALID_FORMAT,	//格式不合法
	LOG_ERR_CREATE_NET_FD_FAIL,	//网络日志socket创建失败
	LOG_ERR_SET_NET_FD_NOBLOCK_FAIL,	//网络日志socket非阻塞失败
	LOG_ERR_CREATE_LOCK_FAIL,	// 日志锁创建失败
};


#ifdef __cplusplus
}
#endif



#endif


