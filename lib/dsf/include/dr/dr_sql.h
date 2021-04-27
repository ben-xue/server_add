/*
* file:   dr_meta_export_to_sql.h
* author: pls@dodjoy.com
* brief:  根据meta导出sql语句
**/

#pragma once


#include "dr_err.h"
#include "dr_meta.h"
#include "dr_net.h"
#include <string>

using namespace std;

#ifdef __cplusplus
extern "C"
{
#endif
	int dr_create_table(DR_HANDLE_META pMeta, DrBuffData* pstSql, const char* charset, const char* engine, int iVersion);

	int dr_alter_table(DR_HANDLE_META pMeta, DrBuffData* pstSql, int iAlterVersion, int iVersion);

	typedef enum
	{
		DR_DBOPTYPE_NONE = 0,
		DR_DBOPTYPE_INSERT,
		DR_DBOPTYPE_UPDATE,
		DR_DBOPTYPE_DELETE,
		DR_DBOPTYPE_SELECT,
		DR_DBOPTYPE_SELECT_COUNT,
	}DRDBOPTYPE;


#define DB_HOST_MAX_LEN                              64    /*host最大长度*/
#define DB_USER_MAX_LEN                              64    /*user最大长度*/
#define DB_PWD_MAX_LEN                               64    /*pwd最大长度*/
#define DB_DATABASE_MAX_LEN                          64    /*database最大长度*/
#define DB_CHARSET_MAX_LEN                           32    /*charset最大长度*/

	/*连接db信息*/
	struct tagConnDBInfo
	{
		/*类型*/
		char type[32];
		/*host最大长度*/
		char host[DB_HOST_MAX_LEN];
		/*db的端口*/
		int32_t port;
		/*用户名*/
		char user[DB_USER_MAX_LEN];
		/*密码*/
		char pwd[DB_PWD_MAX_LEN];
		/*db*/
		char database[DB_DATABASE_MAX_LEN];
		/*字符集*/
		char charset[DB_CHARSET_MAX_LEN];
	};

	struct DRDBObject
	{
		DR_HANDLE_META pMeta;
		int iSize;
		char *pData;
	};
	typedef DRDBObject* DR_DB_OBJECT;



	int dr_open_dbhanlde(DR_HANDLE * pHandle, const tagConnDBInfo& stConnDbInfo);

	void dr_close_dbhanlde(DR_HANDLE pHandle);

	int dr_obj2sql(DR_HANDLE pHandle, int iOpType, DR_DB_OBJECT stObj, const char *pSqlWhere, string &strSql, std::vector<DR_HANDLE_ENTRY> & vEntryList);

	int dr_query(DR_HANDLE pHandle, int iOpType, const char *szSql);

	int dr_num_rows(DR_HANDLE pHandle);

	int dr_affected_rows(DR_HANDLE pHandle);

	int dr_fetch_row(DR_HANDLE pHandle, std::vector<DR_HANDLE_ENTRY> vEntryList, DR_DB_OBJECT pDbObj);

	uint64_t dr_dbms_insert_id(DR_HANDLE pHandle);

#ifdef __cplusplus
}
#endif