
#include "comm_inc.h"
#include "proto_macro.h"

uint64_t xg_get_gid(uint32_t iId, uint32_t iTime)
{
	unsigned short unTmp;
	unsigned char cInstance;
	uint64_t ullGid = 0;
	uint16_t unWorldID = 0;
	uint16_t regionID = 0;
	
	static uint16_t usGidSeq = 0;
	
	GET_WORLD_ID(unWorldID, iId);
	GET_REGION_ID(regionID, iId);
	
	GET_INSTANCE_ENTITY(cInstance, iId);
	memcpy(&ullGid, &iTime, sizeof(int));
	unTmp = (regionID << 10) + (unWorldID << 3) + cInstance;
	memcpy( ((char *)(&ullGid)+sizeof(int)), &unTmp, sizeof(unTmp)); 
	memcpy( ((char *)(&ullGid)+sizeof(ullGid)-sizeof(usGidSeq)), &usGidSeq, sizeof(usGidSeq)); 
	usGidSeq++;
	
	return ullGid;
}

uint64_t xg_create_roleid(uint32_t iId, uint32_t iUin, uint32_t iTime)
{
    static uint16_t sSeqNo = 1;
    ++sSeqNo;
    uint64_t ullTmpRoleID = 0;
    uint16_t unWorldID = 0;
    uint16_t unRegionID = 0;
    GET_WORLD_ID(unWorldID, iId);
    GET_REGION_ID(unRegionID, iId);

    uint32_t bSvrID = (unWorldID & ROLEID_SVRID_MASK ); // 10位
    uint32_t bUin = (iUin & ROLEID_UIN_MASK); // 28位
    uint32_t bTime = (iTime & ROLEID_TIME_MASK); // 18位
    uint32_t bSeqNo = (sSeqNo & ROLEID_SEQNO_MASK);  // 8位

    //生成规则: 28位uin, 18位time, 8位随机序列号, 10位svr id
    LOG_DBG("xg_create_roleid unWorldID[%d] bSvrID[%d]", unWorldID, bSvrID);
    //拼装角色在哪个服生成的信息
    ullTmpRoleID = ((uint64_t)bSvrID << 54);
    ullTmpRoleID |= ((uint64_t)bTime << 36);
    ullTmpRoleID |= ((uint64_t)bSeqNo << 28);
    ullTmpRoleID |= bUin;
    return ullTmpRoleID;
}

uint32_t xg_create_roleno(uint32_t iId, uint32_t iUin, uint32_t iTime)
{
    uint32_t ullTmpRoleNo = 0;
    uint16_t unWorldID    = 0;
    uint16_t unRegionID   = 0;
    GET_WORLD_ID(unWorldID, iId);
    GET_REGION_ID(unRegionID, iId);

    uint32_t bSvrID = (unWorldID & 0x3ff); // 10位
    uint32_t bUin   = (iUin & 0x3fffff);  // 22位

    //生成规则: 22位uin, 10位svr id
    LOG_DBG("xg_create_roleno bSvrID[%d] bUin[%u]" , bSvrID, bUin);
    //拼装角色在哪个服生成的信息
    ullTmpRoleNo = (bSvrID << 22);
    ullTmpRoleNo |= bUin;
    return ullTmpRoleNo;
}

uint32_t xg_get_svrid_from_roleid(uint64_t ullRoleID)
{
    if (ullRoleID <= ROBOT_UIN_MAX)
    {
        return ROBOT_WORLD_ID;
    }

    uint32_t uSvrID = ullRoleID >> 54;
    return uSvrID;
}

// 低28位是UIN
uint32_t xg_get_accuin_from_roleid(uint64_t ullRoleID)
{
    uint32_t uUin = (ullRoleID&0xFFFFFFF);
    return uUin;
}

bool IsUinRobot(uint64_t Uin)
{
    return Uin < ROBOT_UIN_MAX;
}

uint64_t ConstructRobotUin(uint64_t uRobotID)
{
    return ROBOT_UIN_MAX - uRobotID;
}

uint64_t GetRobotIDFromUin(uint64_t uUin)
{
    return ROBOT_UIN_MAX - uUin;
}


bool IsUinValid(uint64_t Uin)
{	
    return xg_get_svrid_from_roleid(Uin) > 0;
}

uint32_t GetRedisProxySvrId(uint32_t uZoneSvrId,int iRedisProxyEntityID)
{
    int iRegionID = 0;
    GET_REGION_ID(iRegionID,uZoneSvrId);

    int iWorldID = 0;
    GET_WORLD_ID(iWorldID,uZoneSvrId);

    uint32_t uRedisSvrId = 0;
    RET_BUS_ID(uRedisSvrId,iRegionID,iWorldID,iRedisProxyEntityID,1);

    return uRedisSvrId;
}

