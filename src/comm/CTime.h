/*
**  
*  通用的业务逻辑框架
*/

#ifndef CTIME_H__
#define CTIME_H__

#include <sys/time.h>
#include <stdint.h>

struct CWeekTime
{
	int iDiffWeek;      // 周间隔
	int iWeekDiffTime;	// 和周开始的时间
};

class CTime
{
public:
	static const int SECDAILY = 3600*24;
	static const int SECHOUR = 3600;
	static const int SECMINUTE = 60;
	static const int DAYSINONEWEEK = 7;
public:
	static void OnTick();

	static int CalcUTCAndLocalZone();

	//设置调试时间
	static void GMSetTime(time_t tNewTime);
	static void GMNextHour(int iHour = 1);

	//将yy-mm-dd hh-mm-ss 格式的字符串转为utc格式
	static time_t ConvDateTime(const char* szTime);

	//时间戳转成yy-mm-dd hh-mm-ss字符串
	static int GetTimeStr(time_t tTime,char *strOutput);

	static const char* GetTimeStr(time_t tTime, const char * format="yyyy-MM-dd HH:mm:ss.SSS");
	
	/**
	* 将类似 18:20:00 时间格式转为0点为基准经过的秒数
	*/
	static int ConvDayTimeToSec(const char* szTime);

	/**
	* 获取当前时间
	*/
	static time_t GetTime();
	static timeval GetTimeVal();
	/**
	* 获取当前时间，毫秒为单位，虽然会溢出，但还是降低精度为32位，主要给KCP用
	*/
	static uint32_t GetTimeMS32();

	//更新时间Cache
	static void UpdateTimeCache();

	/**
	* 比较两个时间
	*/
	static int64_t DiffTimeMs(const timeval& tv1, const timeval& tv2);
	static void AddTimeValDiff(timeval& tv, int64_t llDiffMs);
	
	static bool IsSameDay(time_t time1, time_t time2);
	static bool CalcDiffMonth(time_t tTime1, time_t tTime2, uint32_t iSecOffsetZero = 0);
	static int CalcDiffDay(time_t tTime1, time_t tTime2, int iSecOffsetZero = 0);
	static int CalcDiffWeek(time_t tTime1, time_t tTime2, int wday = 0, int iSecOffsetZero = 0);
	static bool IsSameWeek(time_t tTime1, time_t tTime2);
	static bool IsSameMonth(time_t tTime1, time_t tTime2);
	static time_t ZeroTimeOfDay(time_t tNow, int iAfterDay = 0);
	static bool IsSameHour(time_t tTime1, time_t tTime2);
	
	static time_t GetDayTime();

	 /**
     * 计算某一整时的时间戳 
     *
     * @param[in] tNow 当前时间
     * @param[in] iAfterHour 当前时间之后（之前）的 N 个小时
     * @retval 返回时间戳 
     */
    static time_t CalcTheHour(time_t tNow, int iAfterHour);
	static int DiffHour(time_t t1, time_t t2);
	static int GetHour(time_t tTime);		
	static int GetMin(time_t tTime);
	static int GetSec(time_t tTime);
	static int GetYear(time_t tTime);
	static int GetMonth(time_t tTime);	
	static int GetMDay(time_t tTime);
	static int GetWDay(time_t tTime);
	//从1到7的周几
	static int GetWHDay(time_t tTime);

	/**
	* 和客户端的浮点时间进行转换
	*/
	static void TimeAdd(timeval& tv1, float sec);

	/**
	* 获得下一个周时间点
	*/
	static time_t GetNextWeekPoint(int wday, int hour, int  min, int sec);
	static time_t GetNextWeekPoint(int wday, int clock);

	/**
	 *  获取下个月时间点
	 */
    static time_t GetNextMonthPoint(int mday, int hour, int min, int sec);


	/**
	 *  获取上个月时间点
	 */
	static time_t GetPreviousMonthPoint(int mday, int hour, int min, int sec);

	/**
	* 获得下一个天时间点
	*/
	static time_t GetNextDayPoint(int clock);


	/**
	*	获得当前周的起点, (周一)0点0分0秒
	*/
	static time_t GetCurWeekBeginTime();

	static time_t GetWeekBeginTime(uint32_t iTime);

	/* 获取月初时间 */
	static time_t GetCurMonthBeginTime();

	static uint64_t GetCurMicroSec();

	static CWeekTime GetWeekDiffTime(uint32_t uCompareTime);

	/**
	* 获得下一个重置时间点
	*/
	static time_t GetNextDayResetTime(int iSecOffsetZero);
};


#endif


