#include <stdlib.h>
#include "CSimpleRandSelect.h"

CSimpleRandSelect::CSimpleRandSelect()
{
	m_iMaxCnt = 0;
}


/**
* 随机集合的数据个数
*/
void CSimpleRandSelect::Init(int iMaxCnt)
{
	m_iMaxCnt = iMaxCnt;
}


/**
* 随机选取ResultCnt个数据
* @return 返回实际随机的个数
*/
int CSimpleRandSelect::SelectSet(int aiResultIndex[], int iResultCnt, CRandGenerator* pRanGen)
{
	int iHitResultCnt = 0;
	for (int i = 0; i < m_iMaxCnt && iHitResultCnt < iResultCnt; i++)
	{
		int iRand = pRanGen != NULL?pRanGen->RandUInt(0, m_iMaxCnt-i):Random(0, m_iMaxCnt-i);
		if (iRand <= (iResultCnt-iHitResultCnt))
		{
			//hit
			aiResultIndex[iHitResultCnt] = i;
			iHitResultCnt++;
		}
	}
	
	return iHitResultCnt;
}

int CSimpleRandSelect::Random(int a, int b)
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




