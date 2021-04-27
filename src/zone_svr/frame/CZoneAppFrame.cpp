//
// Created by Administrator on 2021/4/25.
//


#ifndef CZONEAPPFRAME_H__
#define CZONEAPPFRAME_H__

#include "zone_svr_conf_desc.h"
#include "CZoneAppFrame.h"

extern const char *g_szMetaLib_zone_svr;

CZoneAppFrame::CZoneAppFrame() : CAppFrame(g_szMetaLib_zone_svr, "zone_svrconf")
{
}

int CZoneAppFrame::OnInit()
{
    return CAppFrame::OnInit();
}

int CZoneAppFrame::OnProc()
{
    return CAppFrame::OnProc();
}

void CZoneAppFrame::OnProcMsg(dsf::BUSADDR src, dsf::BusDataBuff &busMsg)
{

}

void CZoneAppFrame::OnTick()
{
    CAppFrame::OnTick();
}

int CZoneAppFrame::OnStop()
{
    return CAppFrame::OnStop();
}

#endif
