#pragma once

/*
 * @file: bus.h
 * @author: pls
 * @brief: busͨ��
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

    // �㲥������ ָ����ͨ��func_type, ���ų�����󶨵�ͨ��
    int SendFuncType(uint32_t func_type, const BusDataBuff& buff);

	//����ָ����Դ����Ϣ
	int Recv(BUSADDR src, BusDataBuff& buff);

	//����������Ϣ��Դ����Ϣ
	int RecvAll(BUSADDR& src, BusDataBuff& buff);

	//����ָ��Ŀ�ĵ�ַ����Ϣ
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