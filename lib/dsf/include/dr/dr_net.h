/**
* 
* @file   dr_meta.h  
* @brief  dr meta info descript api
* @auth 	arrow@dodjoy
* @date 	2018/04/11
*/


#ifndef DSF_DR_ENCODE_H__
#define DSF_DR_ENCODE_H__

#include <stdlib.h>
#include "dr_err.h"
#include "dr_meta.h"

#ifndef _WIN32
#include "json/json.h"
#endif


#ifdef __cplusplus
extern "C"
{
#endif


struct DrBuffData
{
	char* pMem;
	int iSize;
};

int dr_encode(const DR_HANDLE_META pMeta, const DrBuffData* pstHost, DrBuffData* pstNet, int iCutVer, DrErrInfo* pErrInfo = NULL);
int dr_decode(const DR_HANDLE_META pMeta, DrBuffData* pstNet, DrBuffData* pstHost, int iCutVer, DrErrInfo* pErrInfo = NULL);

/**
* 格式化输出
*/
int dr_print(const DR_HANDLE_META pMeta, DrBuffData* pstHost, DrBuffData* pstLog, int iCutVer, DrErrInfo* pErrInfo);

/**
 * 序列化json 将dr结构体信息序列化为数数平台需要的json格式
 *
 * {
 *   "#account_id":xxx,
 *   "#distinct_id":xxx,
 *   "#event_name":"xxxxx",
 *    "properties" : {
 *         "BascInfo.level":xxx,
 *         "BascInfo.name":xxx,
 *         具体的结构体内容序列化json值
 *         ......
 *    }
 * }

 */


#ifndef _WIN32

/*
 * @param pMeta 结构体对应drmeta
 * @param pstHost 将要序列化的结构体值
 * @param root 序列化后存储的json value
 * @param logTypeVec 结构体中设置的上报类型tags信息
 * @param iCutVer 版本号
 * @param pErrInfo 错误信息
 * @return 参考 dr_err.h
 */
int dr_encode_shushu_json(const DR_HANDLE_META pMeta, DrBuffData* pstHost, Json::Value & root , std::vector<std::string> & logTypeVec, int iCutVer, DrErrInfo* pErrInfo);


/**
 * 序列化json 将dr结构体信息 序列化成json 格式 序列化的字段会转化为一维json
 * @param pMeta 结构体对应drmeta
 * @param pstHost 将要序列化的结构体值
 * @param root json 序列化后存储的json value
 * @param iCutVer 版本号
 * @param pErrInfo 错误信息
 * @return 返回错误码 参考 dr_err.h
 */
int dr_encode_json(const DR_HANDLE_META pMeta, DrBuffData* pstHost, Json::Value & root , int iCutVer, DrErrInfo* pErrInfo);

/**
 * 反序列化一维json 将序列化一维json内容反序列化为结构体内容
 * @param pMeta 结构体对应drmeta
 * @param pstHost 反序列化后的值
 * @param root json 字符串
 * @param iCutVer 版本号
 * @param pErrInfo 错误信息
 * @return 返回错误码 参考 dr_err.h
 */
int dr_decode_json(const DR_HANDLE_META pMeta, DrBuffData* pstHost, Json::Value & root, int iCutVer, DrErrInfo* pErrInfo);

/**
 * 序列化json 将dr结构体信息 序列化成标准json格式
 * @param pMeta 结构体对应drmeta
 * @param pstHost 将要序列化的结构体值
 * @param root json 序列化后存储的json value
 * @param iCutVer 版本号
 * @param pErrInfo 错误信息
 * @return 返回错误码 参考 dr_err.h
 */
int dr_encode_standard_json(const DR_HANDLE_META pMeta, DrBuffData* pstHost, Json::Value & root , int iCutVer, DrErrInfo* pErrInfo);

/**
 * 反序列化标准格式json 将标准序列化json内容反序列化为结构体内容
 * @param pMeta 结构体对应drmeta
 * @param pstHost 反序列化后的值
 * @param root json 字符串
 * @param iCutVer 版本号
 * @param pErrInfo 错误信息
 * @return 返回错误码 参考 dr_err.h
 */
int dr_decode_standard_json(const DR_HANDLE_META pMeta, DrBuffData* pstHost, Json::Value & root, int iCutVer, DrErrInfo* pErrInfo);

/*
* 从xml数据中加载某个meta 与 将某个meta的xml数据输出到buff中
**/
int dr_input(DR_HANDLE_META meta, DrBuffData* pHost, DrBuffData* pXmlData, int iCutVer, DrErrInfo* pErrInfo = NULL);
int dr_output(DR_HANDLE_META meta, DrBuffData* pHost, DrBuffData* pXmlData, int iCutVer, DrErrInfo* pErrInfo = NULL);

int dr_input_file(DR_HANDLE_META meta, DrBuffData* pHost, const char* pXmlPath, DrErrInfo* pErrInfo = NULL);

int dr_check_value(DR_HANDLE_META meta, DrBuffData* pHost, DrErrInfo* pErrInfo = NULL);

#endif

#ifdef __cplusplus
}
#endif



#endif


