/* 
*  共享对象内存池
*/

#ifndef CSHMDATAPOOL_H__
#define CSHMDATAPOOL_H__

#include <new>
#include "comm_inc.h"
#include "comm/mempool.h"
#include "CShmemMgr.h"

#include "shm_debug.h"
#include <typeinfo>



///固定内存大小的内存池
class CShmDataPool
{
public:
	CShmDataPool();
	~CShmDataPool();
	
	/**
	* 创建内存池
	*/
	int Create(int iShmKey, size_t dwShmSize, size_t dataUnitSize, size_t maxDataCount);

	size_t GetDataUnitSize() {return m_unitSize;}
	
	/**
	* 从内存池里分配新的对象
	*/
	void* Alloc();
	
	/**
	* 从内存池中取对象
	*/
	void * GetObj(int iMemId);

	/**
	* 从内存地址中得到对象id
	*/
	int GetObjId(void * pMem);

	/**
	* 释放内存池里的对象
	*/
	void Free(void* pData);
	
	int GetUsed(){return m_pMemPool->iUsed;}
	
	
private:
	MEMPOOL* m_pMemPool;
	int		  m_iFindIndex;

	int m_iShmKey;
	void* m_pMemPtr;

	size_t m_unitSize;
};


#endif


