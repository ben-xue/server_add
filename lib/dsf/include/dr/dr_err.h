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
	DR_ERR_OK = 0,			//��ȷ
	DR_ERR_INPUT_INVALID,	//��Ч������
	DR_ERR_LOAD_FAILD,	//����DR�ļ�ʧ��
	DR_ERR_INVALID_META,	//��Ч��meta
	DR_ERR_READ_REF_FAILD,	//��ȡrefer�ֶ�ʧ��
	DR_ERR_REF_TOO_LARGE,	//refer�ֶ�̫����
	DR_ERR_READ_SELECT_FAILD,//��ȡselect�ֶ�ʧ����
	DR_ERR_UNION_SELECT_INVALID, ///Uion��select�ֶ���Ч
	DR_ERR_BUFF_INSUFFICE,	//�ڴ��С����
	DR_ERR_STR_LEN_TOO_LARGE,	///�ַ�������̫����
	DR_ERR_STR_LEN_ZERO,		//�ַ�������Ϊ0
	DR_ERR_STR_FORMAT_ERR,		//�ַ�����Ϊ0��β
	DR_ERR_INVALID_CUT_VER,		//��Ч��cutversion
	DR_ERR_WRITE_VER_INDICATOR_FAIL,	//���д��汾��Ϣʧ��
	DR_ERR_WRITE_SIZEINFO_FAIL,	//д�����Сʧ��
	DR_ERR_INVALID_XML_DATA,	//xml�ļ�����
	DR_ERR_XML_DATA_NOT_EXIST,	//xml���ݲ�����
	DR_ERR_XML_DATA_TYPE_DISMATCH,	//xml�������Ͳ�ƥ��
	DR_ERR_READ_XML_FILE_FAILED, //��ȡXML�ļ�ʧ��
	DR_ERR_ALTER_VERSION_INVALID,	//alter���ݰ汾�Ŵ���
	DR_ERR_XML_DATA_BIND_MACRO_NOT_FOUND,	//�󶨵ĺ���ֵδ�ҵ�
	DR_ERR_XML_DATA_BIND_MACRO_GROUP_NOT_MATCH,	//�󶨵ĺ��鲻ƥ��
	DR_ERR_DB_OP_TYPE_ERR,	//DB�������ʹ���
	DR_ERR_DB_WRITE2BUFF_ERR,	//DBSqlת����д��bufferʧ��
	DR_ERR_BUFF_NO_SPACE,	//buffer�ռ䲻��
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
	DR_ERR_INVALID_OPER_TYPE,			//���Ϸ��Ĳ�������
	DR_ERR_DATA_META_TYPE_INVALID,		//����meta���ʹ���
	DR_ERR_CHECK_ENTRY_KEY_PRIM_KEY_NOT_EXIST,	//����û�з���entrylist
	DR_ERR_CHECK_ENTRY_KEY_NOT_NULL_KEY_NOT_EXIST,	//����Ϊnull��key������
	DR_ERR_CHECK_ENTRY_KEY_REFER_ENTEY_NOT_EXIST,	//refer entry û��һ�����
	DR_ERR_CHECK_ENTRY_KEY_SELECT_ENTEY_NOT_EXIST,	//select entry û��һ�����
	DR_ERR_HOST_BUFF_SIZE_NOT_SUFFICIENT,	//host buff��С����
	DR_ERR_ENCODE_ALL_ENTRY_FAILED,	//����all entryʧ��
	DR_ERR_FIND_ENTRY_META_FAIELD,		//����entry name ��Ӧentry metaʧ��
	DR_ERR_PACK_NAMELIST_ENTRY_FAILED,	//��������б�ʧ��
	DR_ERR_PACK_SPLIT_KEY_FAILED,		//���split key��Ϣʧ��
	DR_ERR_PACK_PRIM_KEY_FAILED,		//���prim key��Ϣʧ��
	DR_ERR_UNPACK_ORM_DATA_CONTENT_FAILED,
	DR_ERR_JSON_NO_ARRAY, // json ��������������
	DR_ERR_CHECK_VALUE_INVALID_VAL,	//���Ϸ�����ֵ
};


enum DrDbErrCode
{
	DR_DB_ERR_CRIT = -1,
	DR_DB_ERR_OK = 0, //��ȷ
	DR_DB_ERR_DB_TYPE_ERR,//dropenhandle,dbtype�Ƿ�
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


