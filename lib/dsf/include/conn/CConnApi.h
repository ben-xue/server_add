#pragma once

#include "conn/zone_conn_frame.h"
#include "dr/dr_meta.h"
#include "dr/dr_net.h"
#include "comm/stdinc.h"

NS_BEGIN(dsf)

class CConnApi
{
public:
	static CConnApi* Instance();

	int Init();

	int Decode(char* pMsg, int iMsgLen, tagFrameHead* pFrameHead, int& iFrameHeadLen);

	int Encode(char* pMsg, int& iMsgLen, tagFrameHead* pFrameHead);

	DR_HANDLE_META GetFrameHeadMeta();

private:

	CConnApi(){}
	~CConnApi(){}
	CConnApi(const CConnApi&){}

	DR_HANDLE_META m_frameHeadMeta;
};

NS_END()