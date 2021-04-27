#ifndef KU_LOG_H__
#define KU_LOG_H__

#include "log/log.h"

#define LOG_ERR(fmt, ...) log_error(klog_get_cat(), "%s" fmt, klog_get_prefix(), ##__VA_ARGS__)
#define LOG_DBG(fmt, ...) log_debug(klog_get_cat(), "%s" fmt, klog_get_prefix(), ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) log_info(klog_get_cat(), "%s" fmt, klog_get_prefix(), ##__VA_ARGS__)
#define LOG_TRACE(fmt, ...) log_trace(klog_get_cat(), "%s" fmt, klog_get_prefix(), ##__VA_ARGS__)

#define IS_LOG_DBG_ENABLE dsf::is_priority_enable(klog_get_cat(), LOG_PRIORITY_DEBUG)

#define ASSERT(expr) do{if (!(expr)){LOG_ERR("assert \"%s\" failed", #expr);}}while(0)
#define ASSERT_RET(expr, args...) do{if (!(expr)){LOG_ERR("assert \"%s\" failed", #expr); return	args; }}while(0)


#define ASSERT_ERR_MSG(expr, fmt, args...) \
do \
{ \
	if (!(expr)) \
	{ \
		LOG_ERR("assert \"%s\", msg:" fmt, #expr, ##args); \
	} \
} while (0);

#define ASSERT_ERR_MSG_RET(expr, retval, fmt, args...) \
do \
{ \
	if (!(expr)) \
	{ \
		LOG_ERR("assert \"%s\", msg:" fmt, #expr, ##args); \
		return retval; \
	} \
} while(0);

#define ASSERT_MONITOR(expr, mon_attr) do {if (!(expr)) { LOG_ERR("assert \"%s\" failed", #expr); MonitorAdd(mon_attr); }} while (0);

#define ASSERT_MONITOR_RET(expr, mon_attr, args...) \
do \
{if (!(expr)) {LOG_ERR("assert \"%s\" failed", #expr); \
	MonitorAdd(mon_attr); \
	return	args; \
}} while (0);

#define CAT_TOKEN_1(t1,t2) t1##t2
#define CAT_TOKEN(t1,t2) CAT_TOKEN_1(t1,t2)

// 编译期断言
#define COMPILE_ASSERT(x)  \
        enum {CAT_TOKEN (compile_assert_at_line_, __LINE__) = 1 / !!(x) };

void klog_init(dsf::CLogCategory* pLogCat);

void klog_set_prefix(const char* szPrefix);

dsf::CLogCategory* klog_get_cat();

const char* klog_get_prefix();

#endif

