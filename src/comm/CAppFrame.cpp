/*
**  
*  通用的业务逻辑框架
*/
#include "CAppFrame.h"
#include "klog.h"
#include "event2/event.h"
#include "entity.h"

#define MAJOR 1
#define MINOR 0
#define REV 0
#define BUILD 20180305

char* g_szAppName;
char* g_szAppStack;
using namespace dsf;

void SetAppName(char* szName)
{
	g_szAppName = szName;
}

CAppFrame::CAppFrame(const char* pConfMetaLibStr, const char* pConfMetaName)
{
	m_pConfMetaLibStr = pConfMetaLibStr;
	m_pConfMetaName = pConfMetaName;
}

void SetAppStack(char* szAddr)
{
	g_szAppStack = szAddr;
}


/**
* 初始化框架
*/
int CAppFrame::Init(int argc, char* argv[])
{
	AppCreateOption create_option;
	create_option.argc = argc;
	create_option.argv = argv;
	create_option.user_data = (void*)this;

	create_option.on_init = std::bind(&CAppFrame::OnAppInit, this, std::placeholders::_1, std::placeholders::_2);
	create_option.on_proc = std::bind(&CAppFrame::OnAppProc, this, std::placeholders::_1, std::placeholders::_2);
	create_option.on_tick = std::bind(&CAppFrame::OnAppTick, this, std::placeholders::_1, std::placeholders::_2);
	create_option.on_idle = std::bind(&CAppFrame::OnAppIdle, this, std::placeholders::_1, std::placeholders::_2);
	create_option.on_quit = std::bind(&CAppFrame::OnAppQuit, this, std::placeholders::_1, std::placeholders::_2);
	create_option.on_stop = std::bind(&CAppFrame::OnAppStop, this, std::placeholders::_1, std::placeholders::_2);
	create_option.on_fini = std::bind(&CAppFrame::OnAppFini, this, std::placeholders::_1, std::placeholders::_2);

	create_option.config_meta_name = m_pConfMetaName;
	create_option.config_dr_lib_str = m_pConfMetaLibStr;
	return m_app.Init(create_option);
}

/**
* 进入游戏逻辑循环
*/
void CAppFrame::Loop()
{
	m_app.Loop();
}

int CAppFrame::OnProc()
{
	return 0;
}

bool CAppFrame::GetLibEventLoopBusyFlag()
{
	return m_bEventLibBusyFlag;
}

int CAppFrame::OnAppInit(CApp* pApp, void* user_data)
{
	m_pLibEventBase = NULL;
	m_bEventLibBusyFlag = false;

	klog_init(m_app.GetLogCategory("app.debug"));

	printf("%s begin start now\n", g_szAppName);
	LOG_ERR("%s begin start now", g_szAppName);
    LOG_ERR("start stack addr %p", g_szAppStack);

	int iRet = OnInit();
	
	if (iRet != 0)
	{
		printf("\033[;31m  %s OnInit failed: %d \033[0m\n", g_szAppName, iRet);
		LOG_ERR("%s OnInit failed: %d", g_szAppName, iRet);
		return -1;
	}

	printf("\033[;32m %s begin init succ now\033[0m\n", g_szAppName);

	return 0;
}

bool CAppFrame::IsAllSysReady()
{
    return true;
}

void CAppFrame::OnProcMsg(BUSADDR src, BusDataBuff& busMsg)
{
	LOG_DBG("src bus msg from [%u] len[%d]", src, busMsg.size);
	return;
}

void CAppFrame::OnTick()
{
	return;
}

int CAppFrame::OnStop()
{
	return 0;
}

char* CAppFrame::GetConfig()
{
	return m_app.GetAppConfigBuff();
}

void CAppFrame::SetLibEventBase(event_base * pEventBase)
{
	m_pLibEventBase = pEventBase;
}

int CAppFrame::NewLibEventBase()
{
	if (m_pLibEventBase)
	{
		printf("m_pLibEventBase: %p not NULL\n", m_pLibEventBase);
		return -1;
	}
	m_pLibEventBase = event_base_new();
	if (!m_pLibEventBase)
	{
		printf("event_base_new ret NULL\n");
		return -1;
	}
	return 0;
}

event_base* CAppFrame::GetFrameLibEventBase()
{
	return m_pLibEventBase;
}

void CAppFrame::SetLibEventLoopBusy()
{
	m_bEventLibBusyFlag = true;
}

void CAppFrame::ClearLibEventLoopBusy()
{
	m_bEventLibBusyFlag = false;
}

int CAppFrame::OnInit()
{
	return 0;
}

int CAppFrame::OnAppProc(CApp* pApp, void* user_data)
{
	int iProcCnt = OnProc();
	int iProcBusMsgCount = 0;
    int iProcEventCount = 0;

    if (CBusApi::Instance()->IsInit())
    {
        BusDataBuff busMsg;
        for (int iLoopNum = 0; iLoopNum < 100; iLoopNum++)
        {
            busMsg.buff = m_szRecvBuff;
            busMsg.size = sizeof(m_szRecvBuff);
            unsigned int src = 0;
            int ret = CBusApi::Instance()->RecvAll(src, busMsg);
            if (ret != 0)
            {
                break;
            }

            OnProcMsg(src, busMsg);
            iProcBusMsgCount++;
        }
    }

	bool bLibEventBusy = false;
	if (m_pLibEventBase)
	{
		for (int iLoopNum = 0; iLoopNum < 100; iLoopNum++)
		{
			ClearLibEventLoopBusy();
			event_base_loop(m_pLibEventBase, EVLOOP_ONCE | EVLOOP_NONBLOCK);
			bool bLibEventBusFlag = GetLibEventLoopBusyFlag();
			if (!bLibEventBusFlag)
			{
				break;
			}
			else
			{
				bLibEventBusy = true;
				iProcEventCount++;
			}
		}
	}
	//co_eventloop_once(co_get_epoll_ct(), 1);
	return iProcEventCount + iProcBusMsgCount + iProcCnt;
}

int CAppFrame::OnAppFini(dsf::CApp* pApp, void* user_data)
{
	return OnFini();
}

int CAppFrame::OnFini()
{
	return 0;
}

CLogCategory* CAppFrame::GetLogCategory(const char* name)
{
	return m_app.GetLogCategory(name);
}

int CAppFrame::OnAppTick(dsf::CApp* pApp, void* user_data)
{
	OnTick();
	return 0;
}

int CAppFrame::OnAppIdle(dsf::CApp* pApp, void* user_data)
{
	return 0;
}

void CAppFrame::OnIdle()
{
	
}

int CAppFrame::OnQuit()
{
	return 0;
}

int CAppFrame::OnAppStop(dsf::CApp* pApp, void* user_data)
{
	return OnStop();
}

int CAppFrame::OnAppQuit(dsf::CApp* pApp, void* user_data)
{
	return OnQuit();
}

