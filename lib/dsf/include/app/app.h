#pragma once

/*
 * @file: app.h
 * @author: pls
 * @brief: app运行基础框架（各种初始化，退出，loop tick 等工作)
 **/

#include "log/log.h"
#include "comm/stdinc.h"
#include "dr/dr_meta.h"
#include "dr/dr_net.h"

NS_BEGIN(dsf)

struct CAppImpl;
class CApp;

typedef std::function<int(CApp*, void*)> app_callback_func;

struct AppCreateOption
{
	int argc;
	char** argv;

	void* user_data;
	app_callback_func on_init;
	app_callback_func on_proc;
	app_callback_func on_tick;
	app_callback_func on_idle;
	app_callback_func on_quit;
	app_callback_func on_stop;
	app_callback_func on_fini;

	string config_meta_name;
	const char* config_dr_lib_str;

	AppCreateOption()
	{
		argc = 0;
		argv = NULL;
		user_data = NULL;
		config_dr_lib_str = NULL;
	}
};

class CApp
{
public:
	int Init(const AppCreateOption& create_option);

	int Loop();
	
	void ExitLoop();

	int FinInit();

	CLogCategory* GetLogCategory(const char* name);

    std::string GetBusIdStr();

	std::string GetBusShmkey();

	bool HaveBusShmkey();

    unsigned int GetBusId();

	void Usage();

	CApp();

	~CApp();

	static CApp* Instance();

	CAppImpl* GetAppImpl();

	char* GetAppConfigBuff();

	const char * GetShmKey();

private:

	int InitOsEnv();

	int InitBus();

	int InitLog();

	int InitAppConf();

private:
	CAppImpl* app_impl;
};


NS_END()