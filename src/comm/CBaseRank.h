#ifndef CBASERANK_H__
#define CBASERANK_H__

#include <map>
#include <vector>
//#include "rank_comm.h"
#include "CShmObjPool.h"
#include "CSingleton.h"
#include "CEventMgr.h"
#include "CShmArrayList.h"



/**
* 统一的排行榜模块，除了PVP全服排行榜之外的top排行榜
* 全部由这儿来统一实现
*/
class CBaseRank
{
public:
	virtual void OnTick(){}
	virtual void OnZeroDay() { }

	/**
	* 是否榜单数据都ready了
	*/
	virtual bool IsReady(){return false;}

	/**
	* 强制保存
	*/
	virtual void ForceSave(){}

	// 统一导出排行榜数据接口
//	virtual uint32_t GetRankID () =0;
//	virtual int GetDataCount() = 0 ;
	// 转换第 i+1 名的数据至 CS 结构体
	//virtual int ConvertCsAtRank(int i, tagCSPlayerRankData& stCsData) { return 0; }
	// 获取 key 标记的排名位置所在信息
	//virtual int ConvertCsRankByKey(uint64_t ullKeyID, tagCSSelfInfo& stCsData) { return 0; }
	// 获取 key 标记的排名位置，按自然数意义，1表示排名第1
	virtual int SaveasXml() = 0;
};


#endif


