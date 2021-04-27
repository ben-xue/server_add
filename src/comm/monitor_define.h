#ifndef __MONITOR_DEFINE_H__
#define __MONITOR_DEFINE_H__

#include "monitor_client.h"
#include "CSvrConfig.h"

#define MONITOR_MON_FILE_NAME "/msec/agent/monitor/monitor.mmap"

#define MonitorAdd(AttrID) Monitor_Add(CSvrConfig::GetIdentifySvrName(), #AttrID, 1)
#define MonitorGlobalAdd(AttrID) Monitor_Add("global_attr", #AttrID, 1)
#define MonitorAddVal(AttrID, Val) Monitor_Add(CSvrConfig::GetIdentifySvrName(), #AttrID, Val)
#define MonitorSet(AttrID, value) Monitor_Set(CSvrConfig::GetIdentifySvrName(), CSvrConfig::AddInstanceIDSuffix(#AttrID), value)

///设置字符串类型的属性
#define MonitorSetStr(StrAttrID, value) Monitor_Set(CSvrConfig::GetSvrName(), StrAttrID, value)

enum BattleMonitorAttr
{
    BattleFinishQueueCount,
    BattleWaitQueueCount,
    BattleStartCount,
    BattleSucessCount,
    BattleFailCount,
    WarBattleStartCount,
    WarBattleSucessCount,
    WarBattleFailCount,
    PVEBattleStartCount,
    PVEBattleSucessCount,
    PVEBattleFailCount,
    BattleTimeOutCount,
    BattleFrameOutCount,
    BattleNomalCount,
    BattleFreeCount,
    BattleUsedCount,
};


enum MatchSvrMonitorAttr
{
	//请求类监控
	MatchSvrPlayerCount,
	MatchSvrMatchReqCount,
	MatchSvrRecvTunnelMsg,
	HeartBeatRes,
	ZoneToMatchLogoutReq,

	//状态类监控
	MatchPlayerUsed,
	MatchWarUsed,
	SearchListUsed,
	SearchListNodeUsed,

	//协议监控
	SearchEnemy,
	SearchEnemyFailed,
	SearchLegionWar,
	SearchLegionWarFailed,
};

enum TunnelSvrMonitorAttr
{
	SendMsgToTunnel,
	RecvMsgFromTunnel,
	GetAvailableTunnelClientFail, // 获取tunnel client 失败
	SendMsgToTunnelSendFail,	//向tunnel发包失败
	SendMsgToTunnelPackMsgFail,	//
	SendUinMsgToProc,			//按uin转发消息到特定进程
	SendUinMsgToZone,			//按单个uin转发到zone消息
	SendUinMsgToBattle,
	SendGroupMsgToRelation,	
	SendCrossGroupMsgToRelation,
	SendGroupMsgToWorld,
	SendCrossGroupMsgToWorld,
	SendGroupMsgToMatch,
	SendGroupMsgToRank,
	SendCrossGroupMsgToRank,
	SendGroupMsgToBattleTeam,
	SendTeamIdMsgToBattleTeam,
	SendGroupMsgToLbs,
	SendGroupMsgToMisc,
	SendGroupMsgToBi,
	BroadMsgToZone,
	BroadMsgToBattle,
	BroadMsgToRelation,
	BroadMsgToWorld,
	BroadMsgToMatch,
	BroadMsgToRank,
	BroadMsgToBattleTeam,
	BroadMsgToLbs,
	BroadMsgToMisc,
	BroadMsgToBi,
	BroadCastMsg,
	RecvUdpPkg,
	UdpPkgInvalid,				//udp包检查不合法		
	TotalSendUinMsgToZone,
	TotalSendUinMsgToBattle,
	
	//错误类
	UdpRecvPkgFailed,
	UdpSendMsgDstAddrInvalid,
	UdpSendPkgFailed,
	UdpSendPkgSucc,

	TunnelSendMsgToZone,
	TunnelSendMsgToMatch,
	TunnelSendMsgToBattle,


	UdpSendMsgBySockAddrFailed,
	UdpSendMsgBySockAddrSucc,
	UdpSendTunnel2CrossTunnelMsgFailed,
	UdpSendTunnel2CrossTunnelMsgSucc,
};

enum ZoneSvrMonitorAttr
{
	ClientSeqUsedCount,
	AllocClientSeqSucc,
	AllocClientSeqFail,
	DelClientSeqObj,
	UpdateClientSeqSucc,

    // conn msg
    SendConnMsgFail,

	// 发往relation和rank的数据
	ZonePlayerGetGridRank,

	// C->S 协议统计
	CZoneQueryPlayerInfo,
	CZoneQueryPlayerDefenceInfo,

	//player使用个数
	ZonePlayerUsed,
	ZonePlayerCacheUsed,
	ZonePlayerOnlineUsed,
	ZonePlayerOfflineUsed,
	ZoneBattleCacheUsed,
	MailCacheUsed,
	FriendMailCacheUsed,
	ZoneAllocPlayerFailed,

    WorldNodeUsed,
    WorldNodeTickUsed,
	MapArmyUsed,
	MapViewUsed,
	MapWayPathViewUsed,

    // co 个数
    ZoneCoUsedNum,
    ZoneCoFreeNum,
    ZoneCoIdleNum,
    ZoneCoYieldNum,
    ZoneCoEntityNum,
    ZoneCoTimeOutNum,
    ZoneCoListOutNum,

    // mail 个数
    ZoneSysMailUsedNum,
    ZoneWarMailUsedNum,
    ZoneLegionMailUsedNum,

    ZoneCoTimeOut,
    ZoneCoSeqInvalid,
    ZoneCoPkgError,

	ZoneDBQueryRoleDataActLoginReq,
	ZoneDBQueryRoleDataActLoginRes,
	ZoneDBQueryRoleDataRoleLoginReq,
	ZoneDBQueryRoleDataRoleLoginRes,
	ZoneDBInsertRoleData,
	ZoneDBQueryRoleDataByUin,
	ZoneDBQueryRoleDataByName,
	ZoneDBUpdateRoleData,

	ZoneNameDbHandleInsert,
	ZoneRoleDbHandleUpdate,
	ZoneRoleDbHandleSelect,
	ZoneRoleDbHandleInsert,
	ZoneNameLibDbHandleUpdate,
	ZoneNameLibDbHandleSelect,
	ZoneNamesRecordHandleSelect,
	ZoneNamesRecordHandleInsert,
	ZoneSysMailDbHandleSelect,
	ZoneSysMailDbHandleInsert,
	ZoneSysMailDbHandleUpdate,
	ZoneFriendMailDbHandleSelect,
	ZoneFriendMailDbHandleUpdate,
	ZoneFriendMailDbHandleInsert,
	ZoneChannelGiftDbHandleSelect,
	ZoneChannelGiftDbHandleUpdate,


	ZoneInsertNameRecordRes,
	ZoneInsertNameRecordDbError,

	ZoneInsertRoleRes,
	ZoneInsertRoleResDbError,
	ZoneCreateRoleSucc,

	ZoneChargeBalanceAbnormal,
	ZoneChargeSuccProcCount,

	ZoneOSSSendMailFail,

	TotalZoneSSReportResultReq,
	TotalZoneSSQueryHeroReq,
	TotalZoneSSPlayerLogoutReq,

	ZoneMailDataTableQueryByUin,
	ZoneMailDataTableInsert,
	ZoneMailDataTableUpdate,

    RelationDBQueryRelationDataByRoleID,
    RelationDBHandleInsert,
    RelationDBHandleSelect,

    RelationDBQueryRelationDataByUin,
    UploadHeadInsert,

    ZoneMailTickPoolUsedNum,
    ZoneMailPoolUsedNum,
	ZoneRobotPlayerNum,
};

enum RelationSvrMonitorAttr
{   
   // 状态
	CRelationOnlineUsedCount,
	CRelationCacheUsedCount,
	RelationChatRoomCount,
	RelationChatRoomAvgPlayerCount,
	RelationChatRoomMerge,
	ChatRoomMsgQueueFull,
	ZoneChatSpeakReq,

   // 消息

	CRelationFriendApplyReqZone,
	CRelationFriendApplyReqTranfer,
	CRelationFriendApplyReqWorld,

	CRelationFriendGetDataReqZone,
	CRelationBlackGetDataReqReqZone,
	CRelationOppnentGetDataReqZone,

	CRelationFriendApplyGetDataReqZone,
	CRelationCheckIsFriendReqZone,
	
	CRelationPlayerTopReqZone,
	CRelationPlayerTopReqWorld,
	CRelationPlayerTopReqTranfer,
	
	CRelationBlackClearReqZone,
	CRelationCreatePlayerReqZone,
	CRelationPlayerLoginReqZone,
	CRelationPlayerLogoutReqZone,
	CRelationPlayerHeartReqZone,
	CRelationCacheHeartReqZone,
	CRelationCacheQueryStatusRspZone,

	CRelationFriendHandleReqZone,
	CRelationFriendHandleReqWorld,
	CRelationFriendHandleReqTranfer,

    CRelationFriendHandleOppReqZone,
    CRelationFriendHandleOppReqWorld,
	CRelationFriendHandleOppReqTranfer,
	CRelationFriendHandleOppResZone,
	CRelationFriendHandleOppResWorld,
	CRelationFriendHandleOppResTranfer,

	CRelationFriendDeleteReqZone,
	CRelationFriendDeleteReqWorld,
	CRelationFriendDeleteReqTranfer,
	CRelationBlackSetReqZone,
	CRelationBlackSetReqWorld,
	CRelationBlackSetReqTranfer,

	CRelationFriendSendGiftReqZone,
	CRelationFriendSendGiftReqWorld,
	CRelationFriendSendGiftReqTranfer,

	CRelationOpponentSetReqZone,
	CRelationPlayerBaseReqZone,
	CRelationPlayerGridReqZone,
	CRelationPlayerMatchHeroReqZone,
	CRelationPlayerStatusReqZone,
	CRelationFriendUniqueReqZone,
	CRelationCacheStatusResHandlerZone,
	CRelationZoneRestartReqHandlerZone,
};

enum PerformToolMonitorAttr
{
	TaskStartProcessCnt,
	ConnectToSvrFailed,
	ActLoginFailed,
	NewAccountCreateRole,
	NewAccountCreateRoleFailed,
	RoleLoginFailed,
	RoleLoginSucc,
	ExecGmCmdFailed,
	GetZoneSvrAddrFailed
};

enum WorldSvrMonitorAttr
{
	WorldSendUinMsgCount,
	WorldSSGmReq,
	WorldProcChargeReq,
	WorldQueuePoolNum,
	WorldQueueFinishNum,
	WorldTotalPlayerNum,
	WorldTotalPlayerPoolNum,
	WorldTotalPlayerMaxNum,
    WorldCreateOnlineInfoPoolFailed,
	WorldCreateOnlineUinMapFailed,
};


enum CommonMonitorAttr
{
	TormDbSendReq,
	TormMakeReqMsgFailed,
	TormDbSendPkgFailed,
	TormDbSendReqSucc,
	TunnelUdpReadSock,
};


enum MiscMonitorAttr
{
	MiscUdpTextMsgReq,
	MiscUdpTextMsgRsp,
};

enum WsMonitorAttr
{
	WsHandleConnect,
	WsAllocConnFailed,
	WsConnCreateFailed,
	WsHandleConnectSucc,
	WsHandleCloseSync,	//客户端主动断开连接
	WsHandleCloseAck,	//服务器断开连接，收到客户端确认的消息
	WsHandlePing,
	WsGetKcpConnFailed,
	WsConnSeqNotMatch,
	WsConvNotMatch,
	WsHandleKcpNetDataSize,	///收到KCP的网络协议包大小
	WsHandleKcpNetDataUdpTypeSize,	///收到KCP的Udp类型的网络协议
	WsSendNetDataSize,			///KCP发送网络协议包大小
	WsSendCmdPkgCount,			///KCP发送网络命令包的个数
	WsSendCloseSync,			///服务器主动断开连接的请求
	WsPkgSizeExceedMaxBuffLen,	///消息包超过缓冲区的大小
	WsSendUdpPkgFailed,		///网络消息系统发送失败
	WsSendQuickUdpPkgFailed,	///快速发送udp包失败
	WsSystemErr,				///kcp conn为止的异常错误
	WsSendPkgCount,			///KCP发送消息包的个数
	WsSendPkgFailCount,		///KCP发送消息包失败的个数
	WsSendPkgOkCount,			///KCP发送消息包失败的个数
	WsRecvPkgCount,			///上层收到kcp消息包的个数
	WsClientCloseCount,		///kcp连接关闭的个数,汇总
	WsClientIsBusyCount,		///client conn繁忙或者上层缓冲区满了
	WsConnToAppPkgCount,		///kcp conn发送给app的消息包个数
	WsConnToAppDataSize,		///kcp conn发送给app的消息数据大小
	WsConnToAppPkgFail,		///kcp conn发送给app失败的消息包个数
	WsAppToConnPkgCount,		///app 发送给conn的消息包个数
	WsAppToConnDataSize,		///app 发送给Conn的消息数据大小
	WsAppToConnStartOk,		///app 发送给conn连接成功的个数
	WsAppToConnStop,			///app 发送给conn stop连接的个数
	WsAppToConnProcPkgCount,	///app 发送给conn 正常的数据包个数
	
	KcpNetConnUsed,				///Kcp conn占用内存池的大小
	KcpClientConnUsed,			///上层的Client Conn使用的个数
	KcpMemSizeInPool,			///KCP底层协议栈使用的内存的大小
	KcpMemSizeTotal,			///KCP底层协议栈分配的内存的总大小
};

enum BiMonitorAttr
{
	BiChargeReq,
	BiJsonMsgFormatInvalid,			//json
	BiSendEmptyRspOnError,			//bi发送空回包
	BiUdpTextMsgReq,
	BiUdpTextMsgRsp
};

enum LbsSvrMonitorAttr
{
	LbsRedisAlloc,
	LbsRedisFree,
	LbsRedisKeep,
	LbsCBNoFind,
	LbsFreeNoFind,
};

enum BattleTeamSvrMonitorAttr
{
	BattleTeamGroupError,
	BattleTeamAllocCExistInfoObjFail,
	BattleTeamAllocCExistInfoObjSucc,
	BattleTeamCExistInfoObjMgrCount,
	BattleTeamCExistInfoObjMgrDel,
	BattleTeamForceSetSendWeekActiveAward,
	BattleTeamForceSetSendWeekPointsAward,
	BattleTeamForceSetSendSeasonActivesAward,
	BattleTeamForceSetSendSeasonPointssAward,
	BattleTeamSendWeekActiveAward,
	BattleTeamSendWeekActiveAwardOK,
	BattleTeamSendWeePointsAward,
	BattleTeamSendWeekPointsAwardOK,
	BattleTeamSendSeasonActiveAward,
	BattleTeamSendSeasonActiveAwardOK,
	BattleTeamSendSeasonPointsAward,
	BattleTeamSendSeasonPointsAwardOK,
	BattleTeamAllocRankCacheObjFail,
	BattleTeamDeleteObjFail,
	BattleTeamObjSortMgrDeleteObjFail,
	BattleTeamObjSortMgrFindObjFail,
};

enum ChatSvrMonitorAttr
{
	ChatSvrChatRoomUsed,
	ChatSvrChatRoomPlayerUsed,
};

enum WarSvrMonitorAttr
{
	//player使用个数
	WarPlayerUsed,
	WarPlayerOnlineUsed,
	WorldFreePlayerNum,
	WarUsed,
	WarLegionUsed,
    WarBattleCacheUsed,
    WarNpcAttrUsed,

    WarCoUsedNum,
    WarCoFreeNum,
    WarCoIdleNum,
    WarCoYieldNum,
    WarCoEntityNum,
};

enum ZoneSvrMonitorError
{

    CreatePlayerFailed,
    CreatePlayerCacheFailed,
    MaxPlayerCountNoEnough,
    MaxPlayerCacheCountNoEnough,
    OssMailIDRecordFull,

    CreateNodeFailed,
	CreateTickNodeDataFailed,
    MaxNodeCountNoEnough,
	CreateBattleDetailDataFailed,
	CreateSingleBattleDetailDataFailed,

};

enum LegionSvrMonitorError
{

    CreateLegionFailed,

    QueryLegionByIDSyncTimeOut,
    QueryLegionByIDSyncSeqError,
    QueryLegionByIDSyncOrmLenError,
    QueryLegionByIDSyncSelectError,
    QueryLegionByIDSyncResultFailed,
    QueryLegionByIDSyncFetchError,

    InsertLegionSyncSyncTimeOut,
    InsertLegionSyncSyncSeqError,
    InsertLegionSyncSyncOrmLenError,
    InsertLegionSyncSyncInsertError,

    GenLegionIDSyncTimeOut,
    GenLegionIDSyncSeqError,
    GenLegionIDSyncOrmLenError,

};

enum WarSvrMonitorError
{
    WarOnlineNumLimit,
};

enum LegionSvrMonitorAttr
{
    lineLegionUsed,
    FreeLegion,
    LegionUpdateDbNum,

    QueryLegionByIDCnt,
    QueryLegionByIDSuccCnt,

    InsertLegionSyncCnt,
    InsertLegionSyncSuccCnt,

    UpdateDbLegionShortName,
    UpdateDbLegionName,

};

enum BigWorldMonitorAttr
{
    MapFindPathTimeOut,
};

enum TunnelClientMonitorAttr
{
    TunnelClientHeartTimeout,
    TunnelClientClose,
    TunnelClientInitFailed,
    TunnelClientConnectSuccess,
    TunnelClientConnectFailed,
};

#endif
