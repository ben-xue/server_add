#pragma once

/*
 * @file: bus.h
 * @author: pls
 * @brief: bus通道
 **/

#include "comm/stdinc.h"
#include "log/log.h"
#include "bus/bus_err.h"
#include "bus/bus_def.h"

NS_BEGIN(dsf)

class CBusMgr;

struct BusChannelInfo
{
	BUSADDR src;
	BUSADDR dst;
	unsigned int size;
};

class CBusApi
{
public:
	CBusApi();

	static CBusApi* Instance();

	bool IsInit();

	int Init(const char* shmkey, const char* src, CLogCategory* log_cat = NULL);

	int Send(BUSADDR dst, const BusDataBuff& buff);

	int Send(BUSADDR src, BUSADDR dst, const BusDataBuff& buff);

    // 广播给所有 指定的通道func_type, 会排除自身绑定的通道
    int SendFuncType(uint32_t func_type, const BusDataBuff& buff);

	//接收指定来源的消息
	int Recv(BUSADDR src, BusDataBuff& buff);

	//接收所有消息来源的消息
	int RecvAll(BUSADDR& src, BusDataBuff& buff);

	//接收指定目的地址的消息
	int RecvWithDst(BUSADDR dst, BUSADDR src, BusDataBuff& buff);

	int RecvAllWithDst(BUSADDR dst, BUSADDR& src, BusDataBuff& buff);

	int IntAddr(const char* addr, BUSADDR& iaddr);

	const char* StrAddr(BUSADDR iAddr);
	
	BUSADDR GetSelfAddr();

    int GetAllSrcAddrsWithDst(BUSADDR dst, std::vector<BUSADDR> & srcAddrVec);

	int GetChannelCount();

	BusChannelInfo GetChannelInfoByIdx(int iIdx);

	int BroadcastMsg(const BusDataBuff& buff);

private:
	CBusMgr* m_busmgr;
};


NS_END()