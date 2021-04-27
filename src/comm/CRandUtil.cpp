/**
*   @file   CRandUtil.h
*   @brief  封装的统一随机接口
*   @author arrowgu
*   @date   2013-1-30
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "CRandUtil.h"
#include "klog.h"



int CRandUtil::Random(int a, int b)
{	
    if (a == b)
    {
        return a;
    }
    else if (a > b)
    {
        return b;
    }
    else
    {
        if (a < 0)
        {
            return a + Random(0, b-a);
        }
        else
        {
            return a + (int) ((b-a+1) * ((float)rand() / (RAND_MAX + 1.0)));
        }
    }
    
    return a;
}

float CRandUtil::Random(float a, float b)
{
	return a + (b-a)* (float)rand() / (float)RAND_MAX;
}

int CRandUtil::RandomArray(int iStart, int iCnt,int outarr[])
{	
	int iIndex = 0;
    int iEnd = iStart+iCnt;
    for (int i = iStart; i < iEnd; i++) 
    {
        outarr[iIndex++] = i;
    }

    for (int i = iIndex - 1; i > 0; i--) 
    {
        int x = Random(0, i);
        int t = outarr[i];
        outarr[i] = outarr[x];
        outarr[x] = t;
    }
    
    return iIndex;
}


static int32_t JumpConsistentHash(uint64_t key, int32_t num_buckets)
{
    int64_t b = -1, j = 0;
    while (j < num_buckets) {
        b = j;
        key = key * 2862933555777941757ULL + 1;
        j = (b + 1) * (double(1LL << 31)/double((key >> 33) + 1));
    }
	
    return b;
}

void CRandUtil::RandomSeed(uint64_t seed)
{
	int randHash = JumpConsistentHash(seed, 10);
	LOG_ERR("srand seed: %d", randHash);
	srand(randHash);
}

