/**
*   @file   RandSelect.h
*   @brief  封装的统一随机选择模块，比如类似于100个物品，随机选择几个类似的需求
*   @author arrowgu
*   @date   2013-1-30
*/

#ifndef CRANDSELECT_H__
#define CRANDSELECT_H__

//默认的绝对概率模式下，计算概率的基数，同礼包随机概率相等，便于使用
#define DEF_MAX_ABSOLUTE_RANDOM_BASE 10000

//支持最大选取的集合个数
#define MAX_RAND_SET_ITEM_COUNT 2048

enum ERandSelectType
{
    RAND_SELECT_NONE = 0,                   //无效的模式类型
	RAND_SELECT_ABSOLUTE_DISTINCT,			//按照绝对概率的模式来选取对象, 同时选取出的对象要完全不同
	RAND_SELECT_RELATIVE_DISTINCT,			//按照相对概率的模式来选取对象, 同时选取出的对象要完全不同
	RAND_SELECT_TYPE_CNT,                   //最大个数
};

class CRandSelectFilter
{
public:
	/**
	* 过滤满足条件的掉落
	*/
	virtual bool Filter(int iIndex) = 0;
};

class CRandGenerator;

class CRandSelect
{
public:
    CRandSelect();
    
	/**
	* 初始化函数
	* @parma [in] iRandType 随机模式，参考ERandSelectType定义
	* @param [in] iMaxAbsoluteRandBase 绝对概率情况下，计算随机数的最大计数，默认为DEF_MAX_ABSOLUTE_RANDOM_BASE
	* 
	* @return 返回初始化是否成功
	*	0 - 成功
	*	其他-失败
	*/
	int Init(int iRandType, int iMaxAbsoluteRandBase = 0);

	void SetRandGenerator(CRandGenerator* pRandGen);
	
	/**
	* 设置需要随机选取的数据集合
	* @param [in] aiRandSet 所有待选取项的概率值
	* @param [in] iRandItemCnt 待选取项集合的个数
	* @return 返回设置是否成功
	*	0 - 成功
	* 	其他 - 失败
	*/
	int SetRandSet(int aiRandSet[], int iRandItemCnt);

	//增量增加数据集合
	void ResetRandSet();
	int AddRandSet(int iRandParam);
	int ClearRandSet();
	int GetSetSize() {return m_iRandSetCnt;}
	int GetWight(int iIndex) {return m_aiRandSet[iIndex]; }
	
    /**
    * 一次性获取需要随机出来的集合
    * @param [out] aiRandSetIndex 选取出来的随机对象的索引
    * @param [in] iNeedSelectCnt 需要一次性选取出的对象个数
    *
    * @return 返回实际选取出来的对象个数
    *     < 0  - 操作失败
    */
    int SelectSet(int aiRandSetIndex[], int iNeedSelectCnt, CRandSelectFilter* pFilter = NULL);
	
	int Random(int iStart, int iEnd)const;

	int Begin(CRandSelectFilter* pFilter);
	int SelectOne();

private:
    bool IsInit()const;
    bool IsSetReady()const;
    
    int SelectAbsoluteDistinct();
    int SelectRelativeDistinct();

	//优化版本绝对概率
	int SelectAbsoluteSet(int aiRandSetIndex[], int iNeedSelectCnt);

private:
    int m_iRandType;
    int m_iMaxAbsoluteRandBase;

    int m_aiRandSet[MAX_RAND_SET_ITEM_COUNT];
    int m_iRandSetCnt;

    int m_aiDistinctRandSetIdx[MAX_RAND_SET_ITEM_COUNT];
    int m_iDistinctRandSetIdxCnt;
    int m_iDistinctRandSetBase;

    int m_aiDistinctRandMustSetIdx[MAX_RAND_SET_ITEM_COUNT];
    int m_iDistinctRandMustSetIdxCnt;

	CRandGenerator* m_pRandGen;
    
};

#endif

