
#include "CSvrConfig.h"
#include "CBinReader.h"

using namespace dsf;

static BUSADDR s_selfBusAddr = 0;
static int s_iBusHandle = 0;

BUSADDR CSvrConfig::GetSelfBusID()
{
    return s_selfBusAddr;
}

BUSADDR CSvrConfig::GetZoneSvrBusID(int iZoneInstID)
{
    BUSADDR ret = 0;
    GET_WORLD_INST_ENTRY_ID(ret, s_selfBusAddr, FUNC_ZONE_SVR, iZoneInstID);
    return ret;
}

BUSADDR CSvrConfig::GetGlobalDbProxyBusID(int iDbProxyIndex)
{
	BUSADDR ret = 0;
	GET_WORLD_INST_ENTRY_ID(ret, s_selfBusAddr, FUNC_GLOBAL_ORM, iDbProxyIndex);
    return ret;
}

BUSADDR CSvrConfig::GetCacheDBProxyBusID(int iDbProxyIndex)
{
	BUSADDR ret = 0;
	GET_WORLD_INST_ENTRY_ID(ret, s_selfBusAddr, FUNC_CACHE_DB_PROXY, iDbProxyIndex);
	return ret;
}

BUSADDR CSvrConfig::GetDbProxyBusID(int iDbProxyIndex)
{
	BUSADDR ret = 0;
	GET_WORLD_INST_ENTRY_ID(ret, s_selfBusAddr, FUNC_DB_PROXY, iDbProxyIndex);
    return ret;
}


void CSvrConfig::SetSelfBusID(BUSADDR selfBusID)
{
    s_selfBusAddr = selfBusID;
}

const char* CSvrConfig::GetResTdrPath()
{
	return "../cfg/proto/ResMeta.dr";
}

const char* CSvrConfig::GetResBinPath(const char* szFileName)
{
	static char szFullPath[256] = {0};
	snprintf(szFullPath, sizeof(szFullPath), "../cfg/res/%s", szFileName);
	return szFullPath;
}

int CSvrConfig::GetWorldID()
{
	int iWorldID = 0;
	GET_WORLD_ID(iWorldID, GetSelfBusID());
	return iWorldID;
}

// identify
const char* CSvrConfig::GetIdentifySvrName()
{
    static char identifySvrName[64] = { 0};

	int iID = GetSelfBusID();
	int iSvrID = GetWorldID();
	int iFuncID;
	GET_FUNC_ENTITY(iFuncID, iID);
	switch (iFuncID)
	{
		case FUNC_DB_PROXY:
		    snprintf(identifySvrName, sizeof(identifySvrName)-1, "kgame_{%d}.role_db", iSvrID);
			return identifySvrName;
		case FUNC_ZONE_CONN:
            snprintf(identifySvrName, sizeof(identifySvrName)-1, "kgame_{%d}.zone_conn", iSvrID);
            return identifySvrName;
		case FUNC_ZONE_SVR:
            snprintf(identifySvrName, sizeof(identifySvrName)-1, "kgame_{%d}.zone_svr", iSvrID);
            return identifySvrName;
		case FUNC_PERFORM_TOOL:
            snprintf(identifySvrName, sizeof(identifySvrName)-1, "kgame_{%d}.perform_tool", iSvrID);
            return identifySvrName;
		case FUNC_WORLD_SVR:
            snprintf(identifySvrName, sizeof(identifySvrName)-1, "kgame_{%d}.world_svr", iSvrID);
            return identifySvrName;
		case FUNC_RELATION_SVR:
            snprintf(identifySvrName, sizeof(identifySvrName)-1, "kgame_{%d}.relation_svr", iSvrID);
            return identifySvrName;
		case FUNC_CHAT_SVR:
            snprintf(identifySvrName, sizeof(identifySvrName)-1, "kgame_{%d}.chat_svr", iSvrID);
            return identifySvrName;
		case FUNC_TUNNNEL_SVR:
            snprintf(identifySvrName, sizeof(identifySvrName)-1, "kgame_{%d}.tunnel_svr", iSvrID);
            return identifySvrName;
		case FUNC_REDIS_PROXY:
            snprintf(identifySvrName, sizeof(identifySvrName)-1, "kgame_{%d}.redis_proxy_svr", iSvrID);
            return identifySvrName;
		case FUNC_MANOR_SVR:
            snprintf(identifySvrName, sizeof(identifySvrName)-1, "kgame_{%d}.manor_svr", iSvrID);
            return identifySvrName;
		case FUNC_MATCH_SVR:
            snprintf(identifySvrName, sizeof(identifySvrName)-1, "kgame_{%d}.match_svr", iSvrID);
            return identifySvrName;
		default:
			break;
	}
    snprintf(identifySvrName, sizeof(identifySvrName)-1, "kgame_{%d}.unknown", iSvrID);
	return identifySvrName;
}

const char* CSvrConfig::GetSvrName()
{
	int iID = GetSelfBusID();
	int iFuncID;
	GET_FUNC_ENTITY(iFuncID, iID);
	switch (iFuncID)
	{
	case FUNC_DB_PROXY:
		return "kgame.role_db";
	case FUNC_ZONE_CONN:
		return "kgame.zone_conn";
	case FUNC_ZONE_SVR:
		return "kgame.zone_svr";
	case FUNC_PERFORM_TOOL:
		return "kgame.perform_tool";
    case FUNC_WORLD_SVR:
        return "kgame.world_svr";
    case FUNC_RELATION_SVR:
        return "kgame.relation_svr";
    case FUNC_CHAT_SVR:
        return "kgame.chat_svr";
    case FUNC_TUNNNEL_SVR:
        return "kgame.tunnel_svr";
    case FUNC_REDIS_PROXY:
        return "kgame.redis_proxy_svr";
    case FUNC_MANOR_SVR:
        return "kgame.manor_svr";
    case FUNC_MATCH_SVR:
        return "kgame.match_svr";
	default:
		break;
	}
	return "kgame.unknown svr";
}

int CSvrConfig::GetInstanceID()
{
	int iInstanceID = 0;
	GET_INSTANCE_ENTITY(iInstanceID, GetSelfBusID());
	return iInstanceID;
}

const char* CSvrConfig::AddInstanceIDSuffix(const char* str)
{
	static char szInstanceBuff[1024];
	snprintf(szInstanceBuff, sizeof(szInstanceBuff), "%s_%d", str, GetInstanceID());

	return szInstanceBuff;
}

int CSvrConfig::GetRegionID()
{
	int iRegionID = 0;
	GET_REGION_ID(iRegionID, GetSelfBusID());
	return iRegionID;
}

BUSADDR CSvrConfig::GetConnBusID()
{
	BUSADDR ret = 0;
	GET_WORLD_INST_ENTRY_ID(ret, s_selfBusAddr, FUNC_ZONE_CONN,GetInstanceID());
	return ret;
}

BUSADDR CSvrConfig::GetTunnelTranslateBusID()
{
	BUSADDR ret = 0;
	GET_WORLD_INST_ENTRY_ID(ret, s_selfBusAddr, FUNC_TUNNEL_TRANSLATE_SVR,GetInstanceID());
	return ret;
}