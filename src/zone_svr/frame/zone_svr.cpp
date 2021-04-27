//
// Created by Administrator on 2021/4/25.
//

#include "CZoneAppFrame.h"

CZoneAppFrame g_appFrame;

int main(int argc, char* argv[])
{
    SetAppName(argv[0]);
    if (g_appFrame.Init(argc, argv) != 0)
    {
        printf("zone_svr app frame init failed\n");
        return -1;
    }

    printf("zone_sv begin loop");
    g_appFrame.Loop();

    printf("zone_svr finish\n");
    return 0;
}
