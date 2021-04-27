/**
* 
* @file   dr_meta.h  
* @brief  dr meta info descript api
* @auth 	arrow@dodjoy
* @date 	2018/04/11
*/


#ifndef _DSF_DR_META_H__
#define _DSF_DR_META_H__

#include <stdlib.h>
#include <stdint.h>
#include "dr_err.h"

#ifdef __cplusplus
extern "C"
{
#endif


struct DrMetaLibData;
struct DrMetaData;
struct DrMetaEntryData;
struct DrMetaMacroData;

typedef void* DR_HANDLE;
typedef DrMetaLibData* DR_HANDLE_LIB;
typedef DrMetaData* DR_HANDLE_META;
typedef DrMetaEntryData* DR_HANDLE_ENTRY;
typedef DrMetaMacroData* DR_HANDLE_MACRO;



#define DR_MAX_NAME_LEN 256


enum DrMetaType
{
	DR_META_NONE,
	DR_META_STRUCT,
	DR_META_UNION,
	DR_META_MACROGROUP,
};

enum DrEntryType
{
	DR_ENTRY_TYPE_NONE = 0,
	DR_ENTRY_TYPE_STRUCT,
	DR_ENTRY_TYPE_UNION,
	DR_ENTRY_TYPE_STRING,
	DR_ENTRY_TYPE_DATE,
	DR_ENTRY_TYPE_TIME,
	DR_ENTRY_TYPE_DATETIME,
	DR_ENTRY_TYPE_INT8,
	DR_ENTRY_TYPE_UINT8,
	DR_ENTRY_TYPE_INT16,
	DR_ENTRY_TYPE_UINT16,
	DR_ENTRY_TYPE_INT32,
	DR_ENTRY_TYPE_UINT32,
	DR_ENTRY_TYPE_INT64,
	DR_ENTRY_TYPE_UINT64,
	DR_ENTRY_TYPE_FLOAT,
	DR_ENTRY_TYPE_DOUBLE,
	DR_ENTRY_TYPE_FIX_POINT_FLOAT,
	DR_ENTRY_TYPE_FIX_POINT_FLOAT_H5,
};



#define DR_ENTRY_TYPE_NUMBER_START DR_ENTRY_TYPE_INT8
#define DR_ENTRY_TYPE_NUMBER_END DR_ENTRY_TYPE_UINT32

struct DrErrInfo
{
	DrErrCode errCode;
	char szErrInfo[512];
	char szErrStack[512];	///������Ķ�ջ��Ϣ
};



/**
* �����Ķ�ȡMeta��
*/
DR_HANDLE_LIB dr_lib_load(const char* szPath, DrErrInfo* pErr, bool bFixPoint = false, bool bFixPointH5Format = false);
DR_HANDLE_LIB dr_lib_load_from_str(const char* szDrLib, DrErrInfo* pErr, bool bFixPoint = false);
DR_HANDLE_LIB dr_lib_load_from_b64str(const char* szb64DrLib, DrErrInfo* pErr, bool bFixPoint = false);

bool is_lib_fix_point(DR_HANDLE_LIB lib);

void dr_lib_free(DR_HANDLE_LIB lib);

int dr_get_mete_size(DR_HANDLE_LIB lib);

DR_HANDLE_META dr_get_mete_of_index(DR_HANDLE_LIB lib,int iIndex);


/**
* ��ȡ�ṹ��
*/
DR_HANDLE_META dr_lib_find_meta_by_name(DR_HANDLE_LIB lib, const char* szName);

DR_HANDLE_META dr_lib_find_meta_by_id(DR_HANDLE_LIB lib, int iId);

DR_HANDLE_MACRO dr_lib_find_macro_by_name(DR_HANDLE_LIB lib, const char* szName);

int dr_macro_get_int_val(DR_HANDLE_MACRO macro);

int64_t dr_macro_get_int64_val(DR_HANDLE_MACRO macro);

/**
* ��ȡmeta������
*/
const char* dr_meta_get_name(DR_HANDLE_META meta);

/**
 * ��ȡmeta cname������
 * @param meta
 * @return
 */
const char* dr_meta_get_cname(DR_HANDLE_META meta);

/**
* ��ȡmeta��������
*/
DrMetaType dr_meta_get_type(DR_HANDLE_META meta); 

/**
* ��ȡmeta�İ汾
*/
int dr_meta_get_create_version(DR_HANDLE_META meta);
int dr_meta_get_curr_version(DR_HANDLE_META meta);

/**
* ��ȡmeta��Ԫ�ظ���
*/
int dr_meta_get_entry_count(DR_HANDLE_META meta);
DR_HANDLE_ENTRY dr_meta_get_entry_by_index(DR_HANDLE_META meta, int iIndex);
DR_HANDLE_ENTRY dr_meta_get_entry_by_name(DR_HANDLE_META meta, const char* szName);

int dr_meta_get_macro_count(DR_HANDLE_META meta);

DR_HANDLE_MACRO dr_meta_get_macro_by_index(int iIndex);

/**
* ��ȡMetaռ�õ��ڴ��С
*/
int dr_meta_get_mem_size(DR_HANDLE_META meta);


/**
* ��ȡentry����
*/
DrEntryType dr_entry_get_type(DR_HANDLE_ENTRY entry);
/**
* ��ȡentry������Meta�������struct,union����
*/
DR_HANDLE_META dr_entry_get_type_meta(DR_HANDLE_ENTRY entry);

/**
* ��ȡ�����ڴ�ռ�ô�С
*/
int  dr_entry_get_mem_size(DR_HANDLE_ENTRY entry);
/**
* ��ȡ��Ŀ�ﵱ����Ԫ�Ĵ�С��DrEntryGetMemSize = DrEntryGetUnitMemSize * DrEntryGetArrayCount
*/
int dr_entry_get_unit_mem_size(DR_HANDLE_ENTRY entry);

/**
* ��ȡ�ֶ��������ṹ���е�ƫ��
*/
int dr_entry_get_offset(DR_HANDLE_ENTRY entry);

/**
* ��ȡ�������,�������1����Ϊ����Ԫ��
*/
int dr_entry_get_array_count(DR_HANDLE_ENTRY entry);

/**
* ��ֵ��С������entry
*/
DR_HANDLE_ENTRY dr_entry_get_array_ref_entry(DR_HANDLE_ENTRY entry);

/**
* ��ȡentry������meta
*/
DR_HANDLE_META dr_entry_get_own_meta(DR_HANDLE_ENTRY entry);

/**
* ��ȡentry��id
*/
int dr_entry_get_id(DR_HANDLE_ENTRY entry);

/*
 * ��ȡentry��name
 **/
const char* dr_entry_get_name(DR_HANDLE_ENTRY entry);

/*
 * ��ȡentry��cname
 **/
const char* dr_entry_get_cname(DR_HANDLE_ENTRY entry);

/*
 * ��ȡentry�Ƿ������ר��
 **/
bool dr_entry_res_only_svr(DR_HANDLE_ENTRY entry);

bool dr_entry_has_tag(DR_HANDLE_ENTRY entry, const char* tg);

bool dr_meta_has_tag(DR_HANDLE_META meta, const char* tg);

#ifdef __cplusplus
}
#endif



#endif


