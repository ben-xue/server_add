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
* ��ʽ�����
*/
int dr_print(const DR_HANDLE_META pMeta, DrBuffData* pstHost, DrBuffData* pstLog, int iCutVer, DrErrInfo* pErrInfo);

/**
 * ���л�json ��dr�ṹ����Ϣ���л�Ϊ����ƽ̨��Ҫ��json��ʽ
 *
 * {
 *   "#account_id":xxx,
 *   "#distinct_id":xxx,
 *   "#event_name":"xxxxx",
 *    "properties" : {
 *         "BascInfo.level":xxx,
 *         "BascInfo.name":xxx,
 *         ����Ľṹ���������л�jsonֵ
 *         ......
 *    }
 * }

 */


#ifndef _WIN32

/*
 * @param pMeta �ṹ���Ӧdrmeta
 * @param pstHost ��Ҫ���л��Ľṹ��ֵ
 * @param root ���л���洢��json value
 * @param logTypeVec �ṹ�������õ��ϱ�����tags��Ϣ
 * @param iCutVer �汾��
 * @param pErrInfo ������Ϣ
 * @return �ο� dr_err.h
 */
int dr_encode_shushu_json(const DR_HANDLE_META pMeta, DrBuffData* pstHost, Json::Value & root , std::vector<std::string> & logTypeVec, int iCutVer, DrErrInfo* pErrInfo);


/**
 * ���л�json ��dr�ṹ����Ϣ ���л���json ��ʽ ���л����ֶλ�ת��Ϊһάjson
 * @param pMeta �ṹ���Ӧdrmeta
 * @param pstHost ��Ҫ���л��Ľṹ��ֵ
 * @param root json ���л���洢��json value
 * @param iCutVer �汾��
 * @param pErrInfo ������Ϣ
 * @return ���ش����� �ο� dr_err.h
 */
int dr_encode_json(const DR_HANDLE_META pMeta, DrBuffData* pstHost, Json::Value & root , int iCutVer, DrErrInfo* pErrInfo);

/**
 * �����л�һάjson �����л�һάjson���ݷ����л�Ϊ�ṹ������
 * @param pMeta �ṹ���Ӧdrmeta
 * @param pstHost �����л����ֵ
 * @param root json �ַ���
 * @param iCutVer �汾��
 * @param pErrInfo ������Ϣ
 * @return ���ش����� �ο� dr_err.h
 */
int dr_decode_json(const DR_HANDLE_META pMeta, DrBuffData* pstHost, Json::Value & root, int iCutVer, DrErrInfo* pErrInfo);

/**
 * ���л�json ��dr�ṹ����Ϣ ���л��ɱ�׼json��ʽ
 * @param pMeta �ṹ���Ӧdrmeta
 * @param pstHost ��Ҫ���л��Ľṹ��ֵ
 * @param root json ���л���洢��json value
 * @param iCutVer �汾��
 * @param pErrInfo ������Ϣ
 * @return ���ش����� �ο� dr_err.h
 */
int dr_encode_standard_json(const DR_HANDLE_META pMeta, DrBuffData* pstHost, Json::Value & root , int iCutVer, DrErrInfo* pErrInfo);

/**
 * �����л���׼��ʽjson ����׼���л�json���ݷ����л�Ϊ�ṹ������
 * @param pMeta �ṹ���Ӧdrmeta
 * @param pstHost �����л����ֵ
 * @param root json �ַ���
 * @param iCutVer �汾��
 * @param pErrInfo ������Ϣ
 * @return ���ش����� �ο� dr_err.h
 */
int dr_decode_standard_json(const DR_HANDLE_META pMeta, DrBuffData* pstHost, Json::Value & root, int iCutVer, DrErrInfo* pErrInfo);

/*
* ��xml�����м���ĳ��meta �� ��ĳ��meta��xml���������buff��
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


