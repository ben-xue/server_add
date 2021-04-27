/**
* 
* @file   dr_meta.h  
* @brief  dr meta info descript api
* @auth 	arrow@dodjoy
* @date 	2018/04/11
*/


#ifndef _DSF_DR_ERR_H__
#define _DSF_DR_ERR_H__

#ifdef __cplusplus
extern "C"
{
#endif


enum DrErrCode
{
	DR_ERR_CRIT = -1,
	DR_ERR_OK = 0,			//正确
	DR_ERR_INPUT_INVALID,	//无效的输入
	DR_ERR_LOAD_FAILD,	//载入DR文件失败
	DR_ERR_INVALID_META,	//无效的meta
	DR_ERR_READ_REF_FAILD,	//读取refer字段失败
	DR_ERR_REF_TOO_LARGE,	//refer字段太大了
	DR_ERR_READ_SELECT_FAILD,//读取select字段失败了
	DR_ERR_UNION_SELECT_INVALID, ///Uion的select字段无效
	DR_ERR_BUFF_INSUFFICE,	//内存大小不够
	DR_ERR_STR_LEN_TOO_LARGE,	///字符串长度太大了
	DR_ERR_STR_LEN_ZERO,		//字符串长度为0
	DR_ERR_STR_FORMAT_ERR,		//字符串不为0结尾
	DR_ERR_INVALID_CUT_VER,		//无效的cutversion
	DR_ERR_WRITE_VER_INDICATOR_FAIL,	//打包写入版本信息失败
	DR_ERR_WRITE_SIZEINFO_FAIL,	//写入包大小失败
	DR_ERR_INVALID_XML_DATA,	//xml文件错误
	DR_ERR_XML_DATA_NOT_EXIST,	//xml数据不存在
	DR_ERR_XML_DATA_TYPE_DISMATCH,	//xml数据类型不匹配
	DR_ERR_READ_XML_FILE_FAILED, //读取XML文件失败
	DR_ERR_ALTER_VERSION_INVALID,	//alter数据版本号错误
	DR_ERR_XML_DATA_BIND_MACRO_NOT_FOUND,	//绑定的宏数值未找到
	DR_ERR_XML_DATA_BIND_MACRO_GROUP_NOT_MATCH,	//绑定的宏组不匹配
	DR_ERR_DB_OP_TYPE_ERR,	//DB操作类型错误
	DR_ERR_DB_WRITE2BUFF_ERR,	//DBSql转换中写入buffer失败
	DR_ERR_BUFF_NO_SPACE,	//buffer空间不足
	DR_ERR_UNPACK_DATA_CONTENT_FAILED,
	DR_ERR_GEN_OPER_SQL_FAILED,
	DR_ERR_PRIM_KEY_COUNT_NOT_MATCH,
	DR_ERR_GEN_SQL_NAME_LIST_FAILED,
	DR_ERR_NAME_VALUE_LIST_SIZE_NOT_MATCH,
	DR_ERR_FIND_ENTRY_NAME_META_ENTRY_FAILED,
	DR_ERR_GEN_PRIM_VALUE_LIST_FAILED,
	DR_ERR_GEN_PRIM_KEY_LIST_FAILED,
	DR_ERR_GEN_SQL_VALUE_LIST_FAILED,
	DR_ERR_UNSUPPORT_UNION_COLUMN,
	DR_ERR_INVALID_OPER_TYPE,			//不合法的操作类型
	DR_ERR_DATA_META_TYPE_INVALID,		//数据meta类型错误
	DR_ERR_CHECK_ENTRY_KEY_PRIM_KEY_NOT_EXIST,	//主键没有放入entrylist
	DR_ERR_CHECK_ENTRY_KEY_NOT_NULL_KEY_NOT_EXIST,	//不能为null的key不存在
	DR_ERR_CHECK_ENTRY_KEY_REFER_ENTEY_NOT_EXIST,	//refer entry 没有一起加入
	DR_ERR_CHECK_ENTRY_KEY_SELECT_ENTEY_NOT_EXIST,	//select entry 没有一起加入
	DR_ERR_HOST_BUFF_SIZE_NOT_SUFFICIENT,	//host buff大小不足
	DR_ERR_ENCODE_ALL_ENTRY_FAILED,	//编码all entry失败
	DR_ERR_FIND_ENTRY_META_FAIELD,		//查找entry name 对应entry meta失败
	DR_ERR_PACK_NAMELIST_ENTRY_FAILED,	//打包名字列表失败
	DR_ERR_PACK_SPLIT_KEY_FAILED,		//打包split key信息失败
	DR_ERR_PACK_PRIM_KEY_FAILED,		//打包prim key信息失败
	DR_ERR_UNPACK_ORM_DATA_CONTENT_FAILED,
	DR_ERR_JSON_NO_ARRAY, // json 对象不是数组类型
	DR_ERR_CHECK_VALUE_INVALID_VAL,	//不合法的数值
};


enum DrDbErrCode
{
	DR_DB_ERR_CRIT = -1,
	DR_DB_ERR_OK = 0, //正确
	DR_DB_ERR_DB_TYPE_ERR,//dropenhandle,dbtype非法
	DR_DB_ERR_FETCH_DB_ROW_FAILED,
	DR_DB_ERR_FETCH_ROW_COL_FAILED,
	DR_DB_ERR_READ_BLOB_DATA_VER_INVALID,
	DR_DB_ERR_READ_BLOB_DATA_LEN_INVALID,
	DR_DB_ERR_DECODE_BLOB_DATA_FAILED,
	DR_DB_ERR_SQL2BUFF_VAL_BUFF_NOT_ENOUGH,
	DR_DB_ERR_READ_BLOB_DATA_FAILED,
	DR_DB_ERR_NO_RESULT_SET,
	DR_DB_ERR_RESULT_FIELD_NOT_FOUND,
	DR_DB_ERR_SET_MYSQL_CHARSET_ERROR,
	DR_DB_ERR_CONNECT_MYSQL_DB_FAILED,
	DR_DB_ERR_MYSQL_QUERY_FAILED,
	DR_DB_ERR_MYSQL_QUERY_GET_RESULT_FAILED,
	DR_ERR 
};


#ifdef __cplusplus
}
#endif



#endif


