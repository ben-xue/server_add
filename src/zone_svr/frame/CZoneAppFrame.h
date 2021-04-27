//
// Created by Administrator on 2021/4/25.
//

#ifndef SERVER_CZONEAPPFRAME_H
#define SERVER_CZONEAPPFRAME_H

#include "comm_inc.h"
#include "CAppFrame.h"
#include "CEventMgr.h"
#include "CZoneAppFrame.h"

class CZoneAppFrame : public CAppFrame
{
public:
    CZoneAppFrame();

protected:
    virtual int OnInit();
    virtual int OnProc();
    virtual void OnProcMsg(dsf::BUSADDR src, dsf::BusDataBuff& busMsg);
    virtual void OnTick();

    //返回是否可以停止服务器
    //返回1表示可以停止
    //0 不能停止，需要继续等待
    virtual int OnStop();
};


#endif //SERVER_CZONEAPPFRAME_H
