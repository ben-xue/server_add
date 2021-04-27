/**
*   @file   CRandUtil.h
*   @brief  封装的统一随机接口
*   @author arrowgu
*   @date   2013-1-30
*/

#ifndef CRANDUTIL_H__
#define CRANDUTIL_H__

#include <stdint.h>

class CRandUtil
{
public:	
	static int Random(int a, int b);
	static float Random(float a, float b);
	static int RandomArray(int iStart, int iCnt,int outarr[]);

	///随机种子
	static void RandomSeed(uint64_t seed);
};

#endif

