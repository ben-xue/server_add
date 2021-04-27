#include <string.h>
#include "klog.h"

using namespace dsf;

static CLogCategory* gs_pLogCat = NULL;
static char gs_szLogPrefix[256] = {0};

void klog_init(CLogCategory* pLogCat)
{
	gs_pLogCat = pLogCat;
}

void klog_set_prefix(const char* szPrefix)
{
	strncpy(gs_szLogPrefix, szPrefix, sizeof(gs_szLogPrefix)-1);
}

CLogCategory* klog_get_cat()
{
	return gs_pLogCat;
}

const char* klog_get_prefix()
{
	return gs_szLogPrefix;
}
