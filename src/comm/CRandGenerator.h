#ifndef __XGMTRAND_H__
#define __XGMTRAND_H__

#include <stdint.h>

/**
* 随机数生成算法，封装了random number generator MT19937
*/

#define RAND_PROB_COMMON_BASE 10000

class CRandGenerator
{
public:
	CRandGenerator();

	//设置随机种子
	void SetSeed(uint32_t dwSeedVal);
	uint32_t GetSeed(){return m_dwRandSeed;}

	/**
	* 产生[min,max]闭区间的随机数
	*/
	uint32_t RandUInt(uint32_t min, uint32_t max);

	int RandInt(int min, int max);

	/**
	* 产生[0, 1f]闭区间的随机数
	*/
	float RandFloat();
	
	bool RandHit(uint32_t uRandProb, uint32_t uRandProbBase = RAND_PROB_COMMON_BASE);

private:
	void init_genrand(uint32_t s);	// initialize mt[N] with a seed
	uint32_t genrand_int32(void);	// [0, 0xffffffff]

private:
	static const int N = 624;
	static const int M = 397;
	
	uint64_t	mt[N];	//!< array for the state vector	
	int			mti;	//!< mti==N+1 means mt[N] is not initialized

	uint32_t m_dwRandSeed;
	
};

 
#endif


