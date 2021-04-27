/*
**  
*  Xml格式的配置读取模块
*/

#ifndef CXMLCONFREADER_H__
#define CXMLCONFREADER_H__

#include "comm_inc.h"


/**
* 封装读取配置bin的接口
*/

template<class ConfigType>
class CXmlConfReader
{
public:

	CXmlConfReader()
	{
		BZERO(m_configData);
	}

	/**
	*  简化接口，默认读取./cfg/metaname.xml配置文件
	*/
	ConfigType* ReadDefault(LPTDRMETALIB pMetaLib, const char* szMetaName)
	{
		LPTDRMETA pMeta = tdr_get_meta_by_name(pMetaLib, szMetaName);
		if (!pMeta)
		{
			LOG_ERR("find meta info failed: %s", szMetaName);
			return NULL;
		}

		char szXmlPath[512];
		snprintf(szXmlPath, sizeof(szXmlPath), "../cfg/%s.xml", szMetaName);
		return Read(pMeta, szXmlPath);
	}
	
	/**
	* 读取配置接口
	*/
	ConfigType* Read(const LPTDRMETA pMetaInfo, const char* szXmlPath)
	{
		TDRDATA stHostData;
		BZERO(stHostData);

		stHostData.pszBuff = (char*)&m_configData;
		stHostData.iBuff = sizeof(m_configData);

		LOG_ERR("xml path: %s", szXmlPath);
		
		int iRet = tdr_input_file(pMetaInfo, &stHostData, szXmlPath, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
		if (iRet != 0)
		{
			LOG_ERR("read xml config failed, xmlPath:%s, error:%s", szXmlPath, tdr_error_string(iRet));
			return NULL;
		}

		return &m_configData;
	}

private:
	ConfigType m_configData;
	
};

#endif


