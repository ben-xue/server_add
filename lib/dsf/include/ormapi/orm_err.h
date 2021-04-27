#pragma once

#include "stdinc.h"

NS_BEGIN(dsf)

#ifdef __cplusplus
extern "C"
{
#endif

	enum OrmErrCode
	{
		ORM_ERR_CRIT = -1,
		ORM_ERR_OK = 0,			//正确
		ORM_ERR_ASYNC_CALLBACK_LEN_EXCEED,	//回调消息过长
		ORM_ERR_SEND_PKG_TO_ORM_SVR_FAILED,	//向orm服务器发送消息失败	
		ORM_ERR_INIT_PROTO_METALIB_FAILED,	//初始化proto metalib failed
		ORM_ERR_LOAD_ORM_PKG_META_FAILED,	//加在orm pkg meta失败
		ORM_ERR_FIND_DATA_META_FAILED,		//寻找数据的meta失败
		ORM_ERR_DATA_META_TYPE_INVALID,		//数据meta类型错误
		ORM_ERR_CHECK_ENTRY_KEY_PRIM_KEY_NOT_EXIST,	//主键没有放入entrylist
		ORM_ERR_CHECK_ENTRY_KEY_NOT_NULL_KEY_NOT_EXIST,	//不能为null的key不存在
		ORM_ERR_CHECK_ENTRY_KEY_REFER_ENTEY_NOT_EXIST,	//refer entry 没有一起加入
		ORM_ERR_CHECK_ENTRY_KEY_SELECT_ENTEY_NOT_EXIST,	//select entry 没有一起加入
		ORM_ERR_HOST_BUFF_SIZE_NOT_SUFFICIENT,	//host buff大小不足
		ORM_ERR_ENCODE_ALL_ENTRY_FAILED,	//编码all entry失败
		ORM_ERR_FIND_ENTRY_META_FAIELD,		//查找entry name 对应entry meta失败
		ORM_ERR_PACK_NAMELIST_ENTRY_FAILED,	//打包名字列表失败
		ORM_ERR_INVALID_OPER_TYPE,			//不合法的操作类型
		ORM_ERR_PACK_SPLIT_KEY_FAILED,		//打包split key信息失败
		ORM_ERR_PACK_PRIM_KEY_FAILED,		//打包prim key信息失败
	};


#ifdef __cplusplus
}
#endif

NS_END()