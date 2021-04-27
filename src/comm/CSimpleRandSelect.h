/**
*   @file   CSimpleRand.h
*   @brief  封装简单的随机算法，用于等概率从一个集合中选出N个数出来
*   @author arrowgu
*   @date   2014-8-18
*/

#ifndef CSIMPLERANDSELECT_H_
#define CSIMPLERANDSELECT_H_

#include "CRandGenerator.h"


class CSimpleRandSelect
{
public:
    CSimpleRandSelect();

	/**
	* 随机集合的数据个数
	*/
	void Init(int iMaxCnt);

	/**
	* 随机选取ResultCnt个数据
	* pRanGen 指定随机算法，用于和客户端同步重现
	*		如果为null，则用默认随机数
	* @return 返回实际随机的个数
	*/
	int SelectSet(int aiResultIndex[], int iResultCnt, CRandGenerator* pRanGen = NULL);

private:
	int Random(int a, int b);


private:
	int m_iMaxCnt;
};

#endif

