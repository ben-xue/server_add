//
// Created by Administrator on 2021/4/27.
//

#include "zone_comm.h"

static ZONE_SVRCONF s_config;

tagzone_svrconf *GetZoneSvrConfig()
{
    return &s_config;
}

const char *GetDbMetaPath()
{
    return "../cfg/db_meta/database.dr";
}
