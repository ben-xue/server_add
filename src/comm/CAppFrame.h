/*
**  
*  通用的业务逻辑框架
*/

#ifndef CAPPFRAME_H__
#define CAPPFRAME_H__
  
#include "comm_inc.h"

extern char* g_szAppName;

extern char* g_szAppStack;


extern void SetAppName(char * szName);

extern void SetAppStack(char* szAddr);


class CAppFrame
{
public:
	CAppFrame(const char* pConfMetaLibStr, const char* pConfMetaName);

	int Init(int argc, char* argv[]);

	struct event_base* GetFrameLibEventBase();

    virtual bool IsAllSysReady();

    virtual void OnProcMsg(dsf::BUSADDR src, dsf::BusDataBuff& busMsg);

	void SetLibEventLoopBusy();

	void Loop();

	dsf::CLogCategory* GetLogCategory(const char* name);

protected:

	int OnAppInit(dsf::CApp* pApp, void* user_data);
	int OnAppProc(dsf::CApp* pApp, void* user_data);
	int OnAppTick(dsf::CApp* pApp, void* user_data);
	int OnAppIdle(dsf::CApp* pApp, void* user_data);
	int OnAppQuit(dsf::CApp* pApp, void* user_data);
	int OnAppStop(dsf::CApp* pApp, void* user_data);
	int OnAppFini(dsf::CApp* pApp, void* user_data);
	
	virtual int OnInit();

	virtual int OnStop();

	virtual int OnProc();
	
	virtual void OnTick();

	virtual int OnFini();

	virtual void OnIdle();

	virtual int OnQuit();

protected:

	char* GetConfig();
	
	void SetLibEventBase(struct event_base* pEventBase);
	
	int NewLibEventBase();
	
	void ClearLibEventLoopBusy();

	bool GetLibEventLoopBusyFlag();

protected:
	dsf::CApp m_app;

	const char* m_pConfMetaName;
	const char* m_pConfMetaLibStr;

	char m_szRecvBuff[1024 * 1024 * 4];
	struct event_base * m_pLibEventBase;
	bool m_bEventLibBusyFlag;
};

#endif
