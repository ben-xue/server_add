#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "CTime.h"
#include "CStrUtil.h"
#include "klog.h"
#include "comm/util.h"

const static int csiSecOfDay   = 86400;
const static int csiSecOfHour  = 3600;
const static int csiSecOfMin   = 60;

// 1970-01-01 00:00:00 为 周四 
// const static int XG_TIME_CONVERT_WEEK_BEGIN = 4;
// spb mod 以周日0点作为判断周开始的时间
const static int XG_TIME_CONVERT_WEEK_BEGIN = 0;



static int CalcUTCAndLocalDiffSec()
{
    struct tm *pTempTm = NULL;
    time_t tSecondDay = csiSecOfDay;

    pTempTm = localtime(&tSecondDay);
    if( !pTempTm )
    {
        return 0;
    }

    int iDiffSeconds = (pTempTm->tm_mday - 2) * csiSecOfDay
        + pTempTm->tm_hour * csiSecOfHour;

    return iDiffSeconds;
}


static int siUTCAndLocalDiffSec = CalcUTCAndLocalDiffSec();

static int s_iGmTimeOffset = 0;

static time_t s_tTimeCache = 0;
static timeval s_tTimevalCache;

// 获取服务器时区
int CTime::CalcUTCAndLocalZone()
{
    struct tm *pTempTm = NULL;
	time_t tSecondDay = csiSecOfDay;
    pTempTm = localtime(&tSecondDay);
    if( !pTempTm )
    {
        return 0;
    }

    int iZone = (pTempTm->tm_mday - 2) + pTempTm->tm_hour ;
    return iZone;
}

void CTime::OnTick()
{
	UpdateTimeCache();
}

void CTime::GMSetTime(time_t tNewTime)
{
	if(tNewTime > 0)
	{
		s_iGmTimeOffset = tNewTime - time(NULL);
	}
	else
	{
		s_iGmTimeOffset = 0;
	}
}

void CTime::GMNextHour(int iHour)
{
	s_iGmTimeOffset += iHour * csiSecOfHour;
}

//将yy-mm-dd hh-mm-ss 格式的字符串转为utc格式
time_t CTime::ConvDateTime(const char* szTime)
{
    struct tm tm;
    memset(&tm, 0, sizeof(tm));

    sscanf(szTime, "%d-%d-%d %d:%d:%d",
           &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
           &tm.tm_hour, &tm.tm_min, &tm.tm_sec);

    tm.tm_year -= 1900;
    tm.tm_mon--;

    return mktime(&tm);
}

const char* CTime::GetTimeStr(time_t tTime, const char * format)
{
    static char szTimeStrBuf[1024];
    struct tm stTime;
    localtime_r(&tTime, &stTime);
    ::strftime(szTimeStrBuf, sizeof(szTimeStrBuf), format, &stTime);
    return szTimeStrBuf;
}

/**
* 将 18:20:00 时间格式转为0点为基准经过的秒数
*/
int CTime::ConvDayTimeToSec(const char* szTime)
{
    vector<string> listSubStr;
    CStrUtil::SplitStr(szTime, ":", listSubStr);
    if (listSubStr.size() < 3)
    {
        return 0;
    }
    
    int iHour = atoi(listSubStr[0].c_str());
    int iMin = atoi(listSubStr[1].c_str());
    int iSec = atoi(listSubStr[2].c_str());
    
    return iHour*3600 + iMin*60 + iSec;
}


time_t CTime::GetTime()
{
	if (s_tTimeCache > 0)
	{
		return s_tTimeCache + s_iGmTimeOffset;
	}
	return time(NULL) + s_iGmTimeOffset;
}

/**
* 获取当前时间，毫秒为单位，虽然会溢出，但还是降低精度为32位，主要给KCP用
*/
uint32_t CTime::GetTimeMS32()
{
	timeval tv = GetTimeVal();
	uint64_t ullMSec = (uint64_t)tv.tv_sec * 1000 + (uint64_t)tv.tv_usec / 1000;
	return ullMSec& 0xfffffffful;	
}

timeval CTime::GetTimeVal()
{
	if (s_tTimevalCache.tv_sec > 0)
	{
		timeval tv = s_tTimevalCache;
		tv.tv_sec += s_iGmTimeOffset;
		return tv;
	}
	timeval tv;
	bzero(&tv, sizeof(tv));
	
	gettimeofday(&tv, NULL);

	tv.tv_sec += s_iGmTimeOffset;	//这里也需要加上GM调时间的偏移值
	return tv;
}

int64_t CTime::DiffTimeMs(const timeval& tv1, const  timeval& tv2)
{
	return ((int64_t)tv1.tv_sec-(int64_t)tv2.tv_sec)*1000 +
		((int64_t)tv1.tv_usec-(int64_t)tv2.tv_usec)/1000;
}

void CTime::AddTimeValDiff(timeval& tv, int64_t llDiffMs)
{
	timeval tvOld = tv;
	int64_t llTotalMSec = tv.tv_sec*1000 + llDiffMs;
	tv.tv_sec = llTotalMSec/1000;
	
	int64_t leftMsec = llTotalMSec%1000;
	int64_t llTotalUSec = leftMsec*1000 + tv.tv_usec;
	int64_t llLeftSec = llTotalUSec/1000000;
	int64_t llLeftUSec = llTotalUSec%1000000;
	tv.tv_sec += llLeftSec;
	tv.tv_usec = llLeftUSec;

	int64_t llDiffCheck = DiffTimeMs(tv, tvOld);
	if (llDiffCheck != llDiffMs)
	{
		ASSERT(llDiffCheck == llDiffMs);
		LOG_ERR("AddTimeValDiff failed, tv[%d,%d] Diff[%lld,%lld]", tv.tv_sec, tv.tv_usec, 
			llDiffCheck, llDiffMs);
	}
	
}

int CTime::CalcDiffDay(time_t tTime1, time_t tTime2, int iSecOffsetZero)
{
	if(tTime1 == tTime2)
	{
		return 0;
	}

    int iMin = (tTime1 < tTime2)? tTime1: tTime2;
    int iMax = (tTime1 < tTime2)? tTime2: tTime1;

    iMin += siUTCAndLocalDiffSec;
    iMax += siUTCAndLocalDiffSec;

	iMin -= iSecOffsetZero;
	iMax -= iSecOffsetZero;
	
    iMin -= (iMin % csiSecOfDay);

    return ((iMax - iMin) / csiSecOfDay);
}

//int CTime::CalcDiffWeek(time_t tTime1, time_t tTime2, int wday, int iSecOffsetZero)
//{
//	if(tTime1 == tTime2)
//	{
//		return 0;
//	}
//
//	int iMin = (tTime1 < tTime2) ? tTime1 : tTime2;
//	int iMax = (tTime1 < tTime2) ? tTime2 : tTime1;
//
//	// spb mod
//	iMin += (XG_TIME_CONVERT_WEEK_BEGIN * csiSecOfDay + siUTCAndLocalDiffSec);
//	iMax += (XG_TIME_CONVERT_WEEK_BEGIN * csiSecOfDay + siUTCAndLocalDiffSec);
//
//	iMin = iMin - wday * csiSecOfDay - iSecOffsetZero;
//	iMax = iMax - wday * csiSecOfDay - iSecOffsetZero;
//
//	iMin = iMin - (iMin % (7 * csiSecOfDay));
//
//	return ((iMax - iMin) / (7 * csiSecOfDay));
//}

// spb mod
int CTime::CalcDiffWeek(time_t tTime1, time_t tTime2, int wday, int iSecOffsetZero)
{
	if(tTime1 == tTime2)
	{
		return 0;
	}

	int iMin = (tTime1 < tTime2) ? tTime1 : tTime2;
	int iMax = (tTime1 < tTime2) ? tTime2 : tTime1;

	iMin = iMin - wday * csiSecOfDay - iSecOffsetZero;
	iMax = iMax - wday * csiSecOfDay - iSecOffsetZero;

	// 减去周开始时间
	iMin = GetWeekBeginTime(iMin);

	return ((iMax - iMin) / (7 * csiSecOfDay));
}

bool CTime::IsSameDay(time_t time1, time_t time2)
{
	return 0 == CalcDiffDay(time1, time2);
}

bool CTime::IsSameWeek(time_t tTime1, time_t tTime2)
{
    int iDay1 = (tTime1 + siUTCAndLocalDiffSec) / csiSecOfDay + XG_TIME_CONVERT_WEEK_BEGIN - 1;
    int iDay2 = (tTime2 + siUTCAndLocalDiffSec) / csiSecOfDay + XG_TIME_CONVERT_WEEK_BEGIN - 1;

    return ((iDay1 / 7) == (iDay2 / 7));
}

bool CTime::IsSameMonth(time_t tTime1, time_t tTime2)
{
	bool bSameYear = CTime::GetYear(tTime1) == CTime::GetYear(tTime2);
	bool bSameMonth = CTime::GetMonth(tTime1) == CTime::GetMonth(tTime2);
	return bSameYear && bSameMonth;
}

bool CTime::IsSameHour(time_t tTime1, time_t tTime2)
{
	bool bSameDay = CTime::IsSameDay(tTime1, tTime2);
	bool bSameHour = CTime::GetHour(tTime1) == CTime::GetHour(tTime2);
	return bSameDay && bSameHour;
}

time_t CTime::ZeroTimeOfDay(time_t tNow, int iAfterDay)
{
	if (tNow == 0)
	{
		return (time_t)0;
	}

    return (tNow - (tNow + siUTCAndLocalDiffSec) % csiSecOfDay) + iAfterDay * csiSecOfDay;
}

int CTime::DiffHour(time_t t1, time_t t2)
{
	int iHour1 = t1/3600;
	int iHour2 = t2/3600;
	return iHour1 - iHour2;
}

/**
* 计算某一整时的时间戳 
*
* @param[in] tNow 当前时间
* @param[in] iAfterHour 当前时间之后（之前）的 N 个小时
* @retval 返回时间戳 
*/
time_t CTime::CalcTheHour(time_t tNow, int iAfterHour)
{
	return (tNow - (tNow + siUTCAndLocalDiffSec) % csiSecOfHour) + iAfterHour * csiSecOfHour;
}

int CTime::GetHour(time_t tTime)
{
	time_t zeroTime = ZeroTimeOfDay(tTime, 0);
	time_t hourTime = CalcTheHour(tTime, 0);
	return (hourTime - zeroTime)/csiSecOfHour;
}

int CTime::GetMin(time_t tTime)
{
    time_t zeroTime = ZeroTimeOfDay(tTime, 0);
    int sec = tTime - zeroTime;
    return (sec%csiSecOfHour)/csiSecOfMin;
}

int CTime::GetSec(time_t tTime)
{
	time_t zeroTime = ZeroTimeOfDay(tTime, 0);
	int sec = tTime - zeroTime;
	return sec%csiSecOfMin;
}

int CTime::GetYear(time_t tTime)
{
    tm* pTm = localtime(&tTime);
    return 1900 + pTm->tm_year;
}

int CTime::GetMonth(time_t tTime)
{
    tm* pTm = localtime(&tTime);
    return pTm->tm_mon + 1;
}

int CTime::GetMDay(time_t tTime)
{
    tm* pTm = localtime(&tTime);
    return pTm->tm_mday;
}

int CTime::GetWDay(time_t tTime)
{
	tm* pTm = localtime(&tTime);
	return pTm->tm_wday;
}

void CTime::TimeAdd(timeval& tv, float sec)
{
	//float fVal = tv.tv_sec + (float)tv.tv_usec/1000000.0f;
	///fVal += sec;
	
	tv.tv_sec += ((int)sec-1);
	tv.tv_usec +=  (1 + sec - (int)sec)*1000000.0f;
	if (tv.tv_usec > 1000000)
	{
		tv.tv_sec++;
		tv.tv_usec -= 1000000;
	}
}

time_t CTime::GetNextMonthPoint(int mday, int hour, int min, int sec)
{
	time_t tCurTime = GetTime();
	tm* pTm = localtime(&tCurTime);

	if (pTm->tm_mon >= 11)
	{
		pTm->tm_year += 1;
		pTm->tm_mon = 0;
	}
	else
	{
		pTm->tm_mon += 1;
	}

	pTm->tm_mday = 1;
	pTm->tm_hour = hour;
	pTm->tm_min = min;
	pTm->tm_sec = sec;

	return mktime(pTm);
}

time_t CTime::GetNextWeekPoint(int wday, int hour, int  min, int sec)
{
	time_t tCurTime = GetTime();
	tm* pTm = localtime(&tCurTime);
	time_t tOffSet = (wday - pTm->tm_wday) * 24 * 3600 + hour * 3600 + min * 60 + sec;
	time_t tCurWeekPoint = ZeroTimeOfDay(tCurTime) + tOffSet;
	if (tCurTime < tCurWeekPoint)
	{
		return tCurWeekPoint;
	}
	else
	{
		return tCurWeekPoint + 7 * 24 * 3600;
	}
}

time_t CTime::GetNextWeekPoint(int wday, int clock)
{
	time_t tCurTime = GetTime();
	tm* pTm = localtime(&tCurTime);
	time_t tOffSet = (wday - pTm->tm_wday) * 24 * 3600 + clock;
	time_t tCurWeekPoint = ZeroTimeOfDay(tCurTime) + tOffSet;
	//LOG_ERR("=======>ZeroTimeOfDay(%d), tOffSet(%d), tCurTime(%d), tCurWeekPoint(%d), "
	//	, ZeroTimeOfDay(tCurTime)
	//	, tOffSet
	//	, tCurTime
	//	, tCurWeekPoint);
	if (tCurTime < tCurWeekPoint)
	{
		return tCurWeekPoint;
	}
	else
	{
		return tCurWeekPoint + 7 * 24 * 3600;
	}
}

time_t CTime::GetNextDayPoint(int clock)
{
	time_t tCurTime = CTime::GetTime();
	time_t tOffSet = clock;
	time_t tGenTime = CTime::ZeroTimeOfDay(tCurTime) + tOffSet;
	if (tCurTime < tGenTime)
	{
		return tGenTime;
	}
	else
	{
		return tGenTime + 24 * 3600;
	}
}

time_t CTime::GetDayTime()
{
	time_t tCurTime = GetTime();
	time_t tZeroTime = ZeroTimeOfDay(tCurTime);

	return tCurTime - tZeroTime;
}

int CTime::GetWHDay(time_t tTime)
{
	int iDay = GetWDay(tTime);
	if (iDay == 0)
	{
		iDay = 7;
	}
	return iDay;
}

void CTime::UpdateTimeCache()
{
	s_tTimeCache = time(NULL);

	bzero(&s_tTimevalCache, sizeof(s_tTimevalCache));
	gettimeofday(&s_tTimevalCache, NULL);
}



//获得当前周的起点, (周一)0点0分0秒
time_t CTime::GetCurWeekBeginTime()
{
	int iCurTime = CTime::GetTime();

	time_t tZeroTime = CTime::ZeroTimeOfDay(iCurTime);

	//获得今天是周几
	int iWeekNo = CTime::GetWHDay(iCurTime);

	int iWeekBeginTime = tZeroTime - ((iWeekNo-1) * csiSecOfDay);

	return iWeekBeginTime;
}

//获得当前周的起点, (周一)0点0分0秒
time_t CTime::GetWeekBeginTime(uint32_t iTime)
{
	time_t tZeroTime = CTime::ZeroTimeOfDay(iTime);
	//获得今天是周几
	int iWeekNo = CTime::GetWHDay(iTime);
	int iWeekBeginTime = tZeroTime - ((iWeekNo-1) * csiSecOfDay);

	return iWeekBeginTime;
}


/* 获取月初时间 */
time_t CTime::GetCurMonthBeginTime()
{
	/* 获取今日零点时间 */
	int iCurTime = CTime::GetTime();
	time_t tZeroTime = CTime::ZeroTimeOfDay(iCurTime);
	/* 获取今日是几号 */
	int iDay = GetMDay(iCurTime);
	/* 推算月初时间 */
	int iMonthBeginTime = tZeroTime - ((iDay - 1) * csiSecOfDay);
	return iMonthBeginTime;
}

uint64_t CTime::GetCurMicroSec()
{
	timeval tv;
	bzero(&tv, sizeof(tv));

	gettimeofday(&tv, NULL);

	uint64_t  uRet = tv.tv_sec * 1000000 + tv.tv_usec;


	return uRet;
}

time_t CTime::GetNextDayResetTime(int iSecOffsetZero)
{
	time_t  tCurTime = CTime::GetTime();

	tCurTime -= CTime::GetDayTime();

	tCurTime += csiSecOfDay + iSecOffsetZero;

	return tCurTime;
}

bool CTime::CalcDiffMonth(time_t tTime1, time_t tTime2, uint32_t iSecOffsetZero)
{
	if(tTime1 == tTime2)
	{
		return 0;
	}

	bool bSameYear = CTime::GetYear(tTime1-iSecOffsetZero) == CTime::GetYear(tTime2-iSecOffsetZero);
	bool bSameMonth = CTime::GetMonth(tTime1-iSecOffsetZero) == CTime::GetMonth(tTime2-iSecOffsetZero);
	return !bSameYear || !bSameMonth;
}

int CTime::GetTimeStr(time_t tTime,char *strOutput)
{
	struct tm tm;
	memset(&tm, 0, sizeof(tm));
	localtime_r(&tTime, &tm);

	sprintf(strOutput, "%d-%02d-%02d %02d:%02d:%02d",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
		   tm.tm_hour, tm.tm_min, tm.tm_sec);

	return 0;
}

time_t CTime::GetPreviousMonthPoint(int mday, int hour, int min, int sec)
{
	time_t tCurTime = GetTime();
	tm* pTm = localtime(&tCurTime);

	if (pTm->tm_mon == 0)
	{
		pTm->tm_year -= 1;
		pTm->tm_mon = 11;
	}
	else
	{
		pTm->tm_mon -= 1;
	}

	pTm->tm_mday = 1;
	pTm->tm_hour = hour;
	pTm->tm_min = min;
	pTm->tm_sec = sec;

	return mktime(pTm);
}

CWeekTime CTime::GetWeekDiffTime(uint32_t uCompareTime)
{	
	CWeekTime stWeekTime ;
	uint32_t uTime = CTime::GetTime();
    stWeekTime.iDiffWeek = CTime::CalcDiffWeek(uTime , uCompareTime);
	stWeekTime.iWeekDiffTime = uTime - CTime::GetCurWeekBeginTime();
	return stWeekTime;
}

