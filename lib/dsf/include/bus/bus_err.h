#pragma once

#include "comm/stdinc.h"

NS_BEGIN(dsf)

#ifdef __cplusplus
extern "C"
{
#endif


enum BusErrCode
{
	BUS_ERR_CRIT = -1,
	BUS_ERR_OK = 0,			//正确
	BUS_ERR_INPUT_FILE_INVALID,	//无效的输入文件
	BUS_ERR_ADDR_TEMPLATE_FORMAT_ERR,	//addr模板格式错误
	BUS_ERR_ADDR_FORMAT_ERR,	//bus地址错误
	BUS_ERR_EXCEED_MAX_CHANNEL, //超过最大channel个数
	BUS_ERR_CREATE_CHANNEL_SHM_FAILED,	//创建channel共享内存错误
	BUS_ERR_ATTACH_CHANNEL_SHM_FAILED,	//加载channel共享内存错误
	BUS_ERR_ATTACH_CHANNEL_MAGIC_NOT_MATCH,	//bus channel magic不匹配
	BUS_ERR_ATTACH_CHANNEL_QUEUE_SIZE_NOT_MATCH,	//共享内存大小不匹配
	BUS_ERR_CHANNEL_QUEUE_FULL,	//bus通道已满
	BUS_ERR_FIND_DST_QUEUE_FAILED,	//寻找对应队列失败
	BUS_ERR_ATTACH_SHM_NOT_EXIST,	//共享内存不存在，attach失败
	BUS_ERR_ATTACH_SHM_MGR_MAGIC_NOT_MATCH,	//加载内存magic不匹配
	BUS_ERR_ATTACH_SHM_MGR_SYS_ERR,	//挂载共享内存系统错误
	BUS_ERR_API_NEED_INIT,		//需要先Init才能使用
	BUS_ERR_DST_CHANNEL_NOT_FOUND,	//目标channel未找到
	BUS_ERR_BUFF_NOT_ENOUGH,	//接收消息缓冲区大小不足
	BUS_ERR_BUSMGR_NEED_INIT,	//需要先Init才能使用
	BUS_ERR_INVALID_CHANNEL_INDEX,	//channel序号不合法
	BUS_ERR_GET_SHMID_INFO_FAILED,	//获取channel shm信息失败
	BUS_ERR_CHANNEL_HAVE_PROCESS_ATTACHED,	//有进程已经attach了该通道
	BUS_ERR_DELETE_CHANNEL_SHM_FAILED,	//删除某个channel的共享内存失败
	BUS_ERR_BUFF_NO_MSG_IN_CHANNEL,	//通道中没有消息
	BUS_ERR_MSG_HEAD_MAGIC_NOT_MATCH,	//消息头magic不匹配
    BUS_ERR_SEND_FUNC_TYPE_INVALID, // 按功能发送功能ID参数非法
};


#ifdef __cplusplus
}
#endif

NS_END()