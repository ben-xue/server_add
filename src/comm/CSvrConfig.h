/*
**  
*  通用的业务逻辑框架
*/

#ifndef CSVRCONFIG_H_
#define CSVRCONFIG_H_

#include "comm_inc.h"

class CSvrConfig
{
public:	
	static dsf::BUSADDR GetSelfBusID();
	static dsf::BUSADDR GetZoneSvrBusID(int iZoneInstID);
	
	static dsf::BUSADDR GetDbProxyBusID(int iDbProxyIndex);
	static dsf::BUSADDR GetGlobalDbProxyBusID(int iDbProxyIndex);
	static dsf::BUSADDR GetCacheDBProxyBusID(int iDbProxyIndex);
	
	static void SetSelfBusID(dsf::BUSADDR selfBusID);


	/**
	* 获取资源配置tdr路径
	*/
	static const char* GetResTdrPath();
	static const char* GetResBinPath(const char* szFileName);

	static int GetWorldID();
	static int GetInstanceID();
	
	static int GetRegionID();

	static const char* AddInstanceIDSuffix(const char* str);

	static const char* GetSvrName();

	static const char* GetIdentifySvrName();

	static dsf::BUSADDR GetConnBusID();

	static dsf::BUSADDR GetTunnelTranslateBusID();
};

#endif


