/**
*   @file   RandSelect.cpp
*   @brief  封装的统一随机选择模块，比如类似于100个物品，随机选择几个类似的需求
*   @author arrowgu
*   @date   2013-1-30
*/

#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>

#include "CRandGenerator.h"
#include "RandSelect.h"


CRandSelect::CRandSelect()
{
    m_iRandType = RAND_SELECT_NONE;
    m_iMaxAbsoluteRandBase = 0;
    m_iRandSetCnt = 0;
    m_iDistinctRandSetIdxCnt = 0;
    bzero(m_aiRandSet, sizeof(m_aiRandSet));
    bzero(m_aiDistinctRandSetIdx, sizeof(m_aiDistinctRandSetIdx));
	m_pRandGen = NULL;
}

void CRandSelect::SetRandGenerator(CRandGenerator* pRandGen)
{
	m_pRandGen = pRandGen;
}

/**
* 初始化函数
* @parma [in] iRandType 随机模式，参考ERandSelectType定义
* @param [in] iMaxAbsoluteRandBase 绝对概率情况下，计算随机数的最大计数，默认为DEF_MAX_ABSOLUTE_RANDOM_BASE
* 
* @return 返回初始化是否成功
*	0 - 成功
*	其他-失败
*/
int CRandSelect::Init(int iRandType, int iMaxAbsoluteRandBase)
{
    m_iRandType = RAND_SELECT_NONE;
    m_iRandSetCnt = 0;
    m_iDistinctRandSetIdxCnt = 0;
    
    if (iRandType <= 0 || iRandType >= RAND_SELECT_TYPE_CNT)
    {
        return -1;
    }

    m_iRandType = iRandType;
    if (iMaxAbsoluteRandBase <= 0)
    {
        m_iMaxAbsoluteRandBase = DEF_MAX_ABSOLUTE_RANDOM_BASE;
    }
    else
    {
        m_iMaxAbsoluteRandBase = iMaxAbsoluteRandBase;
    }
    
    return 0;
}

/**
* 设置需要随机选取的数据集合
* @param [in] aiRandSet 所有待选取项的概率值
* @param [in] iRandItemCnt 待选取项集合的个数
* @return 返回设置是否成功
*	0 - 成功
* 	其他 - 失败
*/
int CRandSelect::SetRandSet(int aiRandSet[], int iRandItemCnt)
{
    if (!IsInit())
    {
        return -1;
    }
    
    if (NULL == aiRandSet  || iRandItemCnt <= 0 || iRandItemCnt > MAX_RAND_SET_ITEM_COUNT)
    {
        return -2;
    }

    memcpy(m_aiRandSet, aiRandSet, iRandItemCnt*sizeof(aiRandSet[0]));
    m_iRandSetCnt = iRandItemCnt;
    
    return 0;
}

void CRandSelect::ResetRandSet()
{
	m_iRandSetCnt = 0;
}


//增量增加数据集合
int CRandSelect::AddRandSet(int iRandParam)
{
	if (!IsInit())
    {
        return -1;
    }

	if (m_iRandSetCnt >= MAX_RAND_SET_ITEM_COUNT)
	{
		return -2;
	}
	
	m_aiRandSet[m_iRandSetCnt++] = iRandParam;
	return 0;
}

int CRandSelect::SelectAbsoluteSet(int aiRandSetIndex[], int iNeedSelectCnt)
{
	int iResultCnt = 0;
	for (int i = 0; i < m_iDistinctRandMustSetIdxCnt && iResultCnt < iNeedSelectCnt; i++)
	{
		aiRandSetIndex[iResultCnt++] = m_aiDistinctRandMustSetIdx[i];
	}
	
	for (int i = 0; i < m_iDistinctRandSetIdxCnt && iResultCnt < iNeedSelectCnt; i++)
    {        
    	int iRandom = Random(0, m_iMaxAbsoluteRandBase);
        int iRandSetIdx = m_aiDistinctRandSetIdx[i];
        if (iRandom < m_aiRandSet[iRandSetIdx])
        {            
        	aiRandSetIndex[iResultCnt++] = iRandSetIdx;
        }
    }

	return iResultCnt;
}

/**
* 一次性获取需要随机出来的集合
* @param [out] aiRandSetIndex 选取出来的随机对象的索引
* @param [in] iNeedSelectCnt 需要一次性选取出的对象个数
*
* @return 返回实际选取出来的对象个数
*     -1 - 操作失败
*/
int CRandSelect::SelectSet(int aiRandSetIndex[], int iNeedSelectCnt, CRandSelectFilter* pFilter)
{
    if (NULL == aiRandSetIndex || iNeedSelectCnt <= 0)
    {
        return -1;
    }
    
    if (!IsSetReady())
    {
        return -2;
    }
    
    int iSelectCnt = 0;

    Begin(pFilter);

	//如果是绝对概率选取，则优化版本处理,方便和客户端同步
	if (RAND_SELECT_ABSOLUTE_DISTINCT == m_iRandType)
	{
		return SelectAbsoluteSet(aiRandSetIndex, iNeedSelectCnt);
	}
    
    for (int i = 0; i < iNeedSelectCnt; i++)
    {
        aiRandSetIndex[iSelectCnt] = SelectOne();
        if (aiRandSetIndex[iSelectCnt] >= 0)
        {
            iSelectCnt++;
			continue;
        }

		break;
    }

    return iSelectCnt;
}


/**
* 开始随机选取集合
* 每一轮选取多个集合的时候，一定要先调用Begin, 然后再调用多次Select来获取随机集合
* @return 返回函数操作是否成功
* 	0 - 成功
* 	其他 - 失败
* 
* @note Begin;Select();Select(); 这样的调用序列可以重复调用，而不需要重新Init和SetRandSet
*/
int CRandSelect::Begin(CRandSelectFilter* pFilter)
{
    if (!IsSetReady())
    {
        return -1;
    }

    //如果需要唯一的随机序列，这儿需要重新copy一份到当前随机序列中
    //如果配置了-1，则为必选的序列    
    m_iDistinctRandMustSetIdxCnt = 0;
    m_iDistinctRandSetIdxCnt = 0;
    m_iDistinctRandSetBase = 0;
    
    for (int i = 0; i < m_iRandSetCnt; i++)
    {
        if(pFilter != NULL && !pFilter->Filter(i))
        {
            continue;
        }
        
        if (m_aiRandSet[i] >= 0)
        {
            m_aiDistinctRandSetIdx[m_iDistinctRandSetIdxCnt++] = i;
            m_iDistinctRandSetBase += m_aiRandSet[i];
        }
        else
        {
            m_aiDistinctRandMustSetIdx[m_iDistinctRandMustSetIdxCnt++] = i;
        }
    }
    
    return 0;
}


/**
* 用于随机获取集合中值
* @return 返回随机选取到的集合中的索引
*	-1 - 选取失败，主要是前期初始化函数没成功导致的
*   0 - iRandItemCnt-1， 正常返回的索引值
*/
int CRandSelect::SelectOne()
{
    if (!IsSetReady())
    {
        return -1;
    }
    
    switch (m_iRandType)
    {
    case RAND_SELECT_ABSOLUTE_DISTINCT:
        return SelectAbsoluteDistinct();
        
    case RAND_SELECT_RELATIVE_DISTINCT:
        return SelectRelativeDistinct();
        
    default:
        break;
    }

    return -1;
}


int CRandSelect::SelectAbsoluteDistinct()
{    
    int iRandom = Random(1, m_iMaxAbsoluteRandBase);
    for (int i = 0; i < m_iDistinctRandSetIdxCnt; i++)
    {        
        int iRandSetIdx = m_aiDistinctRandSetIdx[i];
        if (iRandom <= m_aiRandSet[iRandSetIdx])
        {            
            m_aiDistinctRandSetIdx[i] = m_aiDistinctRandSetIdx[m_iDistinctRandSetIdxCnt-1];
            m_iDistinctRandSetIdxCnt--;
            return iRandSetIdx;
        }
    }
    
    return -1;
}

int CRandSelect::SelectRelativeDistinct()
{
    int iRandSetIdx;

    //先获取必须获得对象
    if (m_iDistinctRandMustSetIdxCnt > 0)
    {
        iRandSetIdx = m_aiDistinctRandMustSetIdx[0];
        m_aiDistinctRandMustSetIdx[0] = m_aiDistinctRandMustSetIdx[m_iDistinctRandMustSetIdxCnt-1];
        m_iDistinctRandMustSetIdxCnt--;
        
        return iRandSetIdx;
    }

    int iRandomBase = m_iDistinctRandSetBase;    
    int i;
    
    int iRandSetSum = 0;
    int iRandom = Random(0, iRandomBase);
    for (i = 0; i < m_iDistinctRandSetIdxCnt; i++)
    {
        iRandSetIdx = m_aiDistinctRandSetIdx[i];
        iRandSetSum += m_aiRandSet[iRandSetIdx];

        if (iRandom <= iRandSetSum)
        {
            m_iDistinctRandSetBase -= m_aiRandSet[iRandSetIdx];
            m_aiDistinctRandSetIdx[i] = m_aiDistinctRandSetIdx[m_iDistinctRandSetIdxCnt-1];
            m_iDistinctRandSetIdxCnt--;

            return iRandSetIdx;
        }
    }
    
    return -1;
}

bool CRandSelect::IsInit()const
{
    return (m_iRandType > RAND_SELECT_NONE && m_iRandType < RAND_SELECT_TYPE_CNT);
}

bool CRandSelect::IsSetReady()const
{
    if (!IsInit())
    {
        return false;
    }

    return m_iRandSetCnt > 0;
}

int CRandSelect::Random(int a, int b)const
{
	if (m_pRandGen != NULL)
	{
		return m_pRandGen->RandUInt(a, b);
	}
	
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

int CRandSelect::ClearRandSet()
{
    m_iRandSetCnt = 0;

    bzero(&m_aiRandSet,MAX_RAND_SET_ITEM_COUNT);

    return 0;
}



