#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <stdint.h>
#include "pal/pal_os.h"

#define FUNC_DB_PROXY       30
#define FUNC_CACHE_DB_PROXY 31


#define FUNC_ZONE_CONN 	    60
#define FUNC_ZONE_SVR       61
#define FUNC_WORLD_SVR	    62
#define FUNC_RELATION_SVR	63
#define FUNC_MATCH_SVR	    64
#define FUNC_WAR_SVR	    65
#define FUNC_BI_SERVICE	    66
#define FUNC_MISC_SVR	    67
#define FUNC_CHAT_SVR		69
//对战服务器
#define FUNC_BATTLE_SVR	    70
//对战服务器的Conn
#define FUNC_BATTLE_CONN    71
#define FUNC_KCP_CONN 	    72
#define FUNC_LBS_SVR        73
#define FUNC_CACHE_SVR      74
#define FUNC_TUNNNEL_SVR    77
#define FUNC_TUNNEL_TRANSLATE_SVR 78
#define FUNC_REDIS_PROXY    80
#define FUNC_GUILD_SVR      81
#define FUNC_GLOBAL_ORM     89
#define FUNC_PERFORM_TOOL   90
#define FUNC_RANK_SVR       91
#define FUNC_MANOR_SVR      92
#define FUNC_BUSD_SVR       93
#define FUNC_INIT_ROBOT_SVR 94


//busid内存布局是:
//从低地址开始: regionid(6位), worldid(14位), entryid(8位), 实例id(4位)
//修改为 20170509 by bobbbyzhu
//从低地址开始: regionid(6位), worldid(6位), funcid(8位), 实例id(12位)
//修改为 20171225 by bobbyzhu
//从低地址开始: regionid(8位), worldid(5位), funcid(8位), 实例id(11位)

//下面的宏中,传入的iID是busID,整形表示的上述内存的形式,是本机字节序(小序),所以处理的过程中需要先转成网络字节序(大序)
#define REGION_ID_MASK (0xfc000000)
#define REGION_ID_BIT_LEN 6
#define REGION_ID_SHIFT (26)
#define WORLD_ID_MASK (0x3fff000)
#define WORLD_ID_BIT_LEN 14
#define WORLD_ID_SHIFT (12)
#define ENTITY_ID_MASK (0xff0)
#define ENTITY_ID_BIT_LEN 8
#define ENTITY_ID_SHIFT (4)
#define INSTANCE_ID_MASK (0xf)
#define INSTANCE_ID_BIT_LEN 4
#define INSTANCE_ID_SHIFT (0)

#define ROLEID_SVRID_MASK (0x3FF)
#define ROLEID_SVRID_BIT_LEN (10)

#define ROLEID_UIN_MASK (0xFFFFFFF)
#define ROLEID_UIN_BIT_LEN (28)

#define ROLEID_TIME_MASK (0x3FFFF)
#define ROLEID_TIME_BIT_LEN (18)

#define ROLEID_SEQNO_MASK (0xFF)
#define ROLEID_SEQNO_BIT_LEN (8)

#define ROLEID_BIT_LEN (64)

#define ROBOT_WORLD_ID 955
#define ROBOT_UIN_MAX 10000
#define PVP_ARMY_GID_MAX 100000


#define RET_BUS_ID(iRet, iRegionID, iWorldID, iEntityID, iInstID) \
{ \
	iRet = 0; \
	iRet = (iRegionID & ((1<<REGION_ID_BIT_LEN) - 1)); \
	iRet <<= WORLD_ID_BIT_LEN; \
	iRet += (iWorldID & ((1<<WORLD_ID_BIT_LEN) - 1)); \
	iRet <<= ENTITY_ID_BIT_LEN; \
	iRet += (iEntityID & ((1<<ENTITY_ID_BIT_LEN) - 1)); \
	iRet <<= INSTANCE_ID_BIT_LEN; \
	iRet += (iInstID & ((1<<INSTANCE_ID_BIT_LEN) - 1));\
	iRet = ntohl(iRet); \
}

#define GET_WORLD_INST_ENTRY_ID(iRet, iID, ucEntity, instID) \
{ \
	unsigned int uInnerID = (unsigned int)htonl(iID); \
	unsigned int uRegionID = (uInnerID & REGION_ID_MASK) >> REGION_ID_SHIFT; \
	unsigned int uWorldID = (uInnerID & WORLD_ID_MASK) >> WORLD_ID_SHIFT; \
	RET_BUS_ID(iRet, uRegionID, uWorldID, ucEntity, instID); \
}

#define GET_WORLD_SVR_INST_ENTRY_ID(iRet, iID, uWorldID, instID) \
{ \
	unsigned int uInnerID = (unsigned int)htonl(iID); \
	unsigned int uRegionID = (uInnerID & REGION_ID_MASK) >> REGION_ID_SHIFT; \
	unsigned int uEntityID = (uInnerID & ENTITY_ID_MASK) >> ENTITY_ID_SHIFT; \
	RET_BUS_ID(iRet, uRegionID, uWorldID, ucEntity, instID); \
}

#define GET_WORLD_SVR_ID(iRet, iID, uWorldID) \
{\
	GET_WORLD_SVR_INST_ENTRY_ID(iRet, iID, uWorldID, 1);\
}

#define GET_WORLD_ENTITY_ID(iRet, iID, ucEntity) \
{\
	GET_WORLD_INST_ENTRY_ID(iRet, iID, ucEntity, 1);\
}

#define GET_WORLD_ENTITY_ID(iRet, iID, ucEntity) \
{\
	GET_WORLD_INST_ENTRY_ID(iRet, iID, ucEntity, 1);\
}

#define GET_REGION_ENTITY_ID(iRet, iID, ucEntity) \
{ \
	unsigned int uInnerID = (unsigned int)htonl(iID); \
	unsigned int uRegionID = (uInnerID & REGION_ID_MASK) >> REGION_ID_SHIFT; \
	RET_BUS_ID(iRet, uRegionID, 0, ucEntity, 1); \
}

#define GET_WORLD_ID(iRet, iID) \
{ \
	unsigned int uInnerID = (unsigned int)htonl(iID); \
	iRet = (uInnerID & WORLD_ID_MASK) >> WORLD_ID_SHIFT; \
}

#define GET_REGION_ID(iRet, iID) \
{\
	unsigned int uInnerID = (unsigned int)htonl(iID); \
	iRet = (uInnerID & REGION_ID_MASK) >> REGION_ID_SHIFT; \
}

#define GET_FUNC_ENTITY(ucEntity, iID) \
{\
	unsigned int uInnerID = (unsigned int)htonl(iID); \
	ucEntity = (uInnerID & ENTITY_ID_MASK) >> ENTITY_ID_SHIFT; \
}

#define GET_INSTANCE_ENTITY(ucInstance, iID) \
{ \
	unsigned int uInnerID = (unsigned int)htonl(iID); \
	ucInstance = (uInnerID & INSTANCE_ID_MASK) >> INSTANCE_ID_SHIFT; \
}

#define GET_BUS_ID_STR(pStr, iLen, iID) \
{ \
	unsigned int uInnerID = (unsigned int)htonl(iID); \
	unsigned int uRegionID = (uInnerID & REGION_ID_MASK) >> REGION_ID_SHIFT; \
	unsigned int uWorldID = (uInnerID & WORLD_ID_MASK) >> WORLD_ID_SHIFT; \
	unsigned int uEntityID = (uInnerID & ENTITY_ID_MASK) >> ENTITY_ID_SHIFT; \
	unsigned int uInstanceID = (uInnerID & INSTANCE_ID_MASK) >> INSTANCE_ID_SHIFT; \
	snprintf(pStr, iLen, "%u.%u.%u.%u", uRegionID, uWorldID, uEntityID, uInstanceID); \
}

#define STR_TO_BUS_ID(iID, pStr) \
{ \
	do \
	{ \
		char const * pBegin = pStr; \
		char const * pEnd = strchr(pBegin, '.'); \
		unsigned int uRegionID = strtol(pBegin, (char**)&pEnd, 10); \
		pBegin = pEnd + 1; \
		pEnd = strchr(pBegin, '.'); \
		unsigned int uWorldID = strtol(pBegin, (char**)&pEnd, 10); \
		pBegin = pEnd + 1; \
		pEnd = strchr(pBegin, '.'); \
		unsigned int uEntityID = strtol(pBegin, (char**)&pEnd, 10); \
		pBegin = pEnd + 1; \
		unsigned int uInstanceID = atoi(pBegin); \
		RET_BUS_ID(iID, uRegionID, uWorldID, uEntityID, uInstanceID); \
	} while (0); \
}

#define GET_TUNNEL_INST_ID(iRet, iID, instID) \
{ \
	unsigned int uInnerID = (unsigned int)htonl(iID); \
	unsigned int uRegionID = (uInnerID & REGION_ID_MASK) >> REGION_ID_SHIFT; \
	unsigned int uWorldID = (uInnerID & WORLD_ID_MASK) >> WORLD_ID_SHIFT; \
	RET_BUS_ID(iRet, uRegionID, uWorldID, FUNC_TUNNNEL_SVR, instID); \
}

#define GET_TUNNEL_REG_INST_ID(iRet, iID, iRegion, instID) \
{ \
	unsigned int uInnerID = (unsigned int)htonl(iID); \
	unsigned int uWorldID = (uInnerID & WORLD_ID_MASK) >> WORLD_ID_SHIFT; \
	RET_BUS_ID(iRet, iRegion, uWorldID, FUNC_TUNNNEL_SVR, instID); \
}

uint64_t xg_get_gid(uint32_t iId, uint32_t iTime);

uint64_t xg_create_roleid(uint32_t iId, uint32_t iUin, uint32_t iTime);

uint32_t xg_create_roleno(uint32_t iId, uint32_t iUin, uint32_t iTime);

uint32_t xg_get_svrid_from_roleid(uint64_t ullRoleID);

uint32_t xg_get_accuin_from_roleid(uint64_t ullRoleID);

bool IsUinRobot(uint64_t Uin);

bool IsUinValid(uint64_t Uin);

uint64_t ConstructRobotUin(uint64_t uRobotID);

uint64_t GetRobotIDFromUin(uint64_t uUin);

uint32_t GetRedisProxySvrId(uint32_t uZoneSvrId,int iRedisProxyEntityID);

#endif
