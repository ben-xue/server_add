#ifndef CPERFORMSTATSTICMGR_H__
#define CPERFORMSTATSTICMGR_H__

#include "CSingleton.h"
#include "CBArray.h"
#include "log/log.h"
#include "dr/dr_meta.h"

#define STATIC_NAME_LEN 128
#define MAX_STATIC_ITEM_NUM 1024*5

enum ENMLogicStatID
{
	ENM_LOGIC_STAT_ID_PLAYER_TICK = 1,
};

//累计量统计类型
enum ENMCumuStatID
{
	ENM_CUMU_STAT_ACT_LOGIN_SUCC = 1,
	ENM_CUMU_STAT_ACT_LOGIN_FAIL = 2,
};

//标量统计类型
enum ENMScalarStatID
{
	ENM_SCALAR_STAT_ON_LINE_NUM = 1, //在线人数
	ENM_SCALAR_STAT_OFF_LINE_NUM = 2, 
	ENM_SCALAR_STAT_PLAYER_USED = 3, //player pool 已使用数量
	ENM_SCALAR_STAT_PLAYER_CACHE_USED = 4, //player cache pool 已使用数量
	
	ENM_SCALAR_STAT_CO_USED_NUM = 5,  // co 当前已分配总数
	ENM_SCALAR_STAT_CO_FREE_NUM = 6,  // co 空闲没分配总数
	ENM_SCALAR_STAT_CO_IDLE_NUM = 7,  // co IDLE分配空闲总数
	ENM_SCALAR_STAT_CO_YIELD_NUM = 8,  //co YIELD分配挂起总数
	ENM_SCALAR_STAT_CO_ENTITY_NUM = 9,  //co 分配给玩家总数

    ENM_SCALAR_STAT_WORLD_NODE_USED_NUM = 10,      // NODE 当前已分配总数
    ENM_SCALAR_STAT_BATTLE_CACHE_NUM = 11,  // battle cache 当前已分配总数
    ENM_SCALAR_STAT_LEGION_USED_NUM = 12,   // LEGION 当前已分配总数

    ENM_SCALAR_STAT_LIST_USED_NUM = 13,  // list 当前已分配总数

    ENM_SCALAR_STAT_ON_LINE_LEGION_NUM = 14, // 在线军团数
    ENM_SCALAR_STAT_LEGION_USED = 15, // legion pool 已使用数量
    ENM_SCALAR_STAT_BATTLE_FREE_NUM = 16, // battle pool 已空闲数量
    ENM_SCALAR_STAT_BATTLE_USED_NUM = 17, // battle pool 已使用数量

    ENM_SCALAR_STAT_SYS_MAIL_NUM = 18,  // sys mail 数量
    ENM_SCALAR_STAT_WAR_MAIL_NUM = 19,  // war mail 数量
    ENM_SCALAR_STAT_LEGION_MAIL_NUM = 20,  // legion mail 数量

    ENM_SCALAR_STAT_WORLD_FOG_USED_NUM = 21,               // NODE fog 总数
    ENM_SCALAR_STAT_WORLD_NODE_EXPLORE_USED_NUM = 22,      // NODE 当前已分配探索总数
    ENM_SCALAR_STAT_WORLD_NODE_PERIOD_USED_NUM = 23,      // NODE 当前已分配周期建筑总数
    ENM_SCALAR_STAT_WORLD_NODE_RANDOM_USED_NUM = 24,      // NODE 当前已分配随机事件总数
	ENM_SCALAR_STAT_WORLD_TICK_NODE_USED_NUM = 25,        // world tick node 当前已分配总数
	ENM_SCALAR_STAT_WORLD_TICK_PER_NODE_NUM = 26,        // node per tick 数量
    ENM_SCALAR_STAT_MAIL_TICK_POOL_USED_NUM = 27,        // 邮件Tick池使用数量
    ENM_SCALAR_STAT_MAIL_POOL_USED_NUM = 28,             // 邮件数据池使用数量
	ENM_SCALAR_STAT_ROBOT_PLAYER_NUM = 29, // 机器人cache数量
	ENM_SCALAR_STAT_WORLD_MAP_ARMY_USED_NUM = 30,     	        // 当前地图中部队总数
    ENM_SCALAR_STAT_WORLD_MAP_VIEW_USED_NUM = 31,      		    // 当前地图中视野总数
    ENM_SCALAR_STAT_WORLD_MAP_WAY_PATH_VIEW_USED_NUM = 32,      // 当前地图中部队视野总数
    ENM_SCALAR_STAT_WORLD_NPC_ARMY_USED_NUM = 33,       // 大世界野怪数量
    ENM_SCALAR_STAT_WORLD_TICK_PER_ARMY_NUM = 34,        // world ARMY tick node 当前已分配总数
	ENM_SCALAR_STAT_WORLD_ARMY_TICK_POOL_NUM = 35, // 大世界Army Tick pool 数量
	ENM_SCALAR_STAT_OPER_CACHE_QUEUE_NUM = 36, // 操作缓存队列数量
};

//最大标量统计
enum ENMMaxScalarStatID
{
	ENM_MAX_SCALAR_STAT_ON_LINE_NUM = 1,	//周期内最大在线人数	
};

//统计中使用的DB名,主要是为了方便生成唯一Key
enum ENMDBName4Stat
{
	ENM_DB_NAME_ROLE_DATA_4_STAT = 1,
	ENM_DB_NAME_GUILD_DATA_4_STAT = 2,
	ENM_DB_NAME_SYSMAIL_DATA_4_STAT = 3,
	ENM_DB_NAME_MAIL_DATA_4_STAT = 4,
	ENM_DB_NAME_FRIENDMAIL_DATA_4_STAT = 5,
	ENM_DB_NAME_LEGIONMAIL_DATA_4_STAT = 6,
	ENM_DB_NAME_WARMAIL_DATA_4_STAT = 7,
    ENM_DB_NAME_CHANNEL_GIFT_INFO_4_STAT = 8,
    ENM_DB_NAME_UPLOAD_HEAD_INFO_4_STAT = 9,
    ENM_DB_NAME_WAR_DATA_4_STAT = 10,
};

typedef struct _STPerfStatItem
{
	int iID;
	char szItemName[STATIC_NAME_LEN];
	uint64_t iTotalCostTime;
	int32_t iTotalCnt; //周期内统计次数
	uint64_t iTotalBytes; //在统计流量时才有意义
	_STPerfStatItem();
	int Init();
} STPerfStatItem;

typedef struct _STDBStatItem
{
public:
	int iID;	
	//char szDBName[STATIC_NAME_LEN];	
	uint64_t iTotalCostTime;
	int32_t iTotalCnt; //周期内统计次数
	int Init();
} STDBStatItem;

template<typename K, typename T>
class CStatArray : public CUBArray <K, T>
{
	virtual int Compare(K key1, T * p2)
	{
		if (key1 > p2->iID)
		{
			return 1;
		}
		else if (key1 == p2->iID)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
};

typedef struct _STCumuStatItem
{
	int iID;
	char szItemName[STATIC_NAME_LEN];
	int iCnt;

} STCumuStatItem;

typedef struct _STScalarStatItem
{
	int iID;
	char szItemName[STATIC_NAME_LEN];
	int iValue;

} STScalarStatItem;

typedef CStatArray<int32_t, STPerfStatItem> CStaticArray;
typedef CStatArray<int32_t, STCumuStatItem> CLogicCumuStaticArray;
typedef CStatArray<int32_t, STScalarStatItem> CLogicScalarArray;
typedef CStatArray<int32_t, STDBStatItem> CDBStatArray;

class CPerformStatsticMgr : public CSingleton<CPerformStatsticMgr>
{
public:
	int Init();
	int OnTick();
	int InsertProtocolStatiticDsfLib(DR_HANDLE_LIB pLib);

	//增加累积量统计
	void IncCumuLogicStat(ENMCumuStatID iID, const char * pszStatName);
	void UpdateLogicScalarStat(ENMScalarStatID iID, const char * pszStatName, int iValue);
	void UpdateMaxScalarStat(ENMScalarStatID iID, const char * pszStatName, int iValue);

	int GetLogicScalarStat(ENMScalarStatID iID);

	CStaticArray m_oLogicStaticArray;  //用于游戏逻辑的统计, 主要分析耗时
	CLogicCumuStaticArray m_oLogicCumuStaticArray; //游戏逻辑累积量的分析
	CLogicScalarArray m_oLogicScalarStaticArray; //标量统计
	CStaticArray m_oUpStreamArray; //上行流量统计
	CStaticArray m_oDownStreamArray; //下行流量统计
	CDBStatArray m_oDBStatArray; //DB回包统计数组(主要统计时间开销)
	CLogicScalarArray m_oMaxScalarStaticArray;	//统计周期内最大标量统计

	dsf::CLogCategory* m_pstStatLogCat; //性能日志句柄
private:
	int m_iLastTickTime;
	//LPTDRMETALIB m_pCSTDRMetaLib;
};

class CLogicStatHelper
{
public:
	CLogicStatHelper(ENMLogicStatID iID, const char * pszName);
	~CLogicStatHelper();
private:
	uint64_t m_ullRecordTime; //存放微秒数
	int m_iID;
};

class CUpStreamStatHelper
{
public:
	CUpStreamStatHelper(int iCmd, int iPkgLen);
	~CUpStreamStatHelper();
private:
	int m_iID;
	int m_iPkgLen;
	uint64_t m_ullRecordTime; //存放微秒数
};

class CDownStreamStatHelper
{
public:
	CDownStreamStatHelper(int iCmd, int iPkgLen);
	~CDownStreamStatHelper();
private:
	int m_iID;
	int m_iPkgLen;
	uint64_t m_ullRecordTime; //存放微秒数
};

//DB统计
class CDBStatHelper
{
public:
	CDBStatHelper(ENMDBName4Stat enmDBName, int iDBOpID, int iCallBackID);
	~CDBStatHelper();
private:

	int m_iTmpID;
	uint64_t m_ullRecordTime; //本次记录开始的时间
};


//异步的事件单独做统计
//账号登陆统计

//角色登陆统计


#endif
