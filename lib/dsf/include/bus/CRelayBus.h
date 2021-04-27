#pragma once

/*
 * @file: bus.h
 * @author: pls
 * @brief: busÍ¨µÀ
 **/

#include "comm/stdinc.h"
#include "log/log.h"
#include "bus/bus_err.h"
#include "bus/bus_def.h"

#include <vector>

NS_BEGIN(dsf)

    class CBusMgr;

    class CRelayBusApi
    {
    public:
        CRelayBusApi();

        static CRelayBusApi* Instance();

        int Init(const char* shmkey, std::vector<std::string> & relaySrcVec, CLogCategory* log_cat = NULL);

        int Send(BUSADDR src, BUSADDR dst, const BusDataBuff& buff);

        int RecvAllWithDst(BUSADDR dst, BUSADDR& src, BusDataBuff& buff);

        int IntAddr(const char* addr, BUSADDR& iaddr);

        const char* StrAddr(BUSADDR iAddr);

    private:
        CBusMgr* m_busmgr;
    };


NS_END()