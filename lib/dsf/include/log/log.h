#pragma once

#include "log_def.h"
#include "comm/stdinc.h"

NS_BEGIN(dsf)

struct LogLocInfo
{
	const char* loc_file;
	int loc_line;
	const char* loc_function;
	void* loc_data;
};

class CLog;
class CLogCategory;

CLog* log_alloc();

int log_free(CLog** ppLog);

int log_init(CLog* pLog, const tagLogConf* pstConf);

int log_fini(CLog* pLog);

CLogCategory* log_get_category(CLog* pLog, const char* name);

bool is_priority_enable(CLogCategory* cat, int priority);

int write_logv(CLogCategory* cat, LogLocInfo& loc, int priority, const char* fmt, ...);

#define LOG_LOC_INFO_INITIALIZER(user_data) { __FILE__, __LINE__, __FUNCTION__, user_data }

#define log_write(cat, priority, fmt, ...) do{\
if (dsf::is_priority_enable(cat, priority)){\
	dsf::LogLocInfo loc = LOG_LOC_INFO_INITIALIZER(NULL); \
	dsf::write_logv(cat, loc, priority, fmt, ##__VA_ARGS__); \
}}while(0)

CLogCategory* log_get_default_category();

void log_set_category_priority(CLogCategory* pLogCat, int priorityHigh, int priorityLow);

void log_set_default_category(CLogCategory* pLogCategory);

void log_default_category_fini();

#define log_fatal(cat, fmt, ...)		log_write(cat, LOG_PRIORITY_FATAL, fmt, ##__VA_ARGS__)
#define log_alert(cat, fmt, ...)		log_write(cat, LOG_PRIORITY_ALERT, fmt, ##__VA_ARGS__)
#define log_crit(cat,  fmt, ...)		log_write(cat, LOG_PRIORITY_CRIT, fmt, ##__VA_ARGS__)
#define log_error(cat, fmt, ...)		log_write(cat, LOG_PRIORITY_ERROR, fmt, ##__VA_ARGS__)
#define log_warn(cat, fmt, ...)			log_write(cat, LOG_PRIORITY_WARN, fmt, ##__VA_ARGS__)
#define log_notice(cat, fmt, ...)		log_write(cat, LOG_PRIORITY_NOTICE, fmt, ##__VA_ARGS__)
#define log_info(cat, fmt, ...)			log_write(cat, LOG_PRIORITY_INFO, fmt, ##__VA_ARGS__)
#define log_debug(cat, fmt, ...)		log_write(cat, LOG_PRIORITY_DEBUG, fmt, ##__VA_ARGS__)
#define log_trace(cat, fmt, ...)		log_write(cat, LOG_PRIORITY_TRACE, fmt, ##__VA_ARGS__)

#define glog_fatal(fmt, ...)	log_write(log_get_default_category(), LOG_PRIORITY_FATAL, fmt, ##__VA_ARGS__)
#define glog_alert(fmt, ...)	log_write(log_get_default_category(), LOG_PRIORITY_ALERT, fmt, ##__VA_ARGS__)
#define glog_crit(fmt, ...)		log_write(log_get_default_category(), LOG_PRIORITY_CRIT, fmt, ##__VA_ARGS__)
#define glog_error(fmt, ...)	log_write(log_get_default_category(), LOG_PRIORITY_ERROR, fmt, ##__VA_ARGS__)
#define glog_warn(fmt, ...)		log_write(log_get_default_category(), LOG_PRIORITY_WARN, fmt, ##__VA_ARGS__)
#define glog_notice(fmt, ...)	log_write(log_get_default_category(), LOG_PRIORITY_NOTICE, fmt, ##__VA_ARGS__)
#define glog_info(fmt, ...)		log_write(log_get_default_category(), LOG_PRIORITY_INFO, fmt, ##__VA_ARGS__)
#define glog_debug(fmt, ...)	log_write(log_get_default_category(), LOG_PRIORITY_DEBUG, fmt, ##__VA_ARGS__)
#define glog_trace(fmt, ...)	log_write(log_get_default_category(), LOG_PRIORITY_TRACE, fmt, ##__VA_ARGS__)

NS_END()