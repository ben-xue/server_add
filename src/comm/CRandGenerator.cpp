#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "CRandGenerator.h"

//!< Period Parameter for Mersenne Twister
#define	MATRIX_A	(0x9908b0dfUL)	//!< constant vector a
#define	UPPER_MASK	(0x80000000UL)	//!< most significant w-r bits
#define	LOWER_MASK	(0x7fffffffUL)	//!< least significant r bits

#define XRAND_MAX (0xffffffff)

CRandGenerator::CRandGenerator()
{
	mti=N+1;
	m_dwRandSeed = 5489U;
}

//设置随机种子
void CRandGenerator::SetSeed(uint32_t dwSeedVal)
{
	init_genrand(dwSeedVal);
	m_dwRandSeed = dwSeedVal;
}

uint32_t CRandGenerator::RandUInt(uint32_t min, uint32_t max)
{
	if (min > max)
	{
		return RandUInt(max, min);
	}
	
	int rank = max-min+1;
	uint32_t dwRandVal = genrand_int32();

	//区间和基数加1，max的概率才和其他的一致，否则max的概率非常低
	uint32_t offset = (uint64_t)dwRandVal*(uint64_t)rank/((uint64_t)(XRAND_MAX)+1);
	return min + offset;
}

float CRandGenerator::RandFloat()
{
	uint32_t dwRandVal = genrand_int32();
	return dwRandVal*(1.0 / 4294967295.0f);	// divided by 2^32-1
}

void CRandGenerator::init_genrand(uint32_t s)
{
	mt[0]= s & 0xffffffffUL;

	for (mti = 1; mti < N; mti++)
	{
		mt[mti] = (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
		/* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
		/* In the previous versions, MSBs of the seed affect   */
		/* only MSBs of the array mt[].                        */
		/* 2002/01/09 modified by Makoto Matsumoto             */
		mt[mti] &= 0xffffffffUL;
		/* for >32 bit machines */
    }
}

uint32_t CRandGenerator::genrand_int32()
{
	unsigned long y;
	static unsigned long mag01[2]={0x0UL, MATRIX_A};
	/* mag01[x] = x * MATRIX_A  for x=0,1 */

	if (mti >= N)
	{
		/* generate N words at one time */
		int kk;

		if (mti == N+1)	/* if init_genrand() has not been called, */
			init_genrand(5489UL);	/* a default initial seed is used */

		for (kk = 0; kk < N-M; kk++)
		{
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}

		for (; kk < N-1; kk++)
		{
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk + (M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}

		y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
		mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

		mti = 0;
	}

	y = mt[mti++];

    /* Tempering */
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= (y >> 18);

	return y;
}


bool CRandGenerator::RandHit(uint32_t uRandProb, uint32_t uRandProbBase /*= RAND_PROB_COMMON_BASE*/)
{
	uint32_t uRand = RandUInt(1, uRandProbBase);
	if (uRand <= uRandProb)
	{
		return true;
	}
	return false;
}

int CRandGenerator::RandInt(int min, int max)
{
	int iCalcMin = min;
	int iCalcMax = max;

	bool bOffSet = false;
	if (min < 0)
	{
		iCalcMin = 0;
		iCalcMax = max - min;
		bOffSet = true;
	}

	int iRand = RandUInt(iCalcMin, iCalcMax);
	if (bOffSet)
	{
		iRand += min;
	}

	return iRand;
}