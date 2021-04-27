/* 
*  共享对象内存池
*/

#ifndef CSHMOBJPOOL
#define CSHMOBJPOOL

#include <new>
#include "comm_inc.h"
#include "comm/mempool.h"
#include "CShmemMgr.h"

#include "shm_debug.h"
#include <typeinfo>

class CShmBaseObj
{
public:
	virtual int Init() = 0;
	virtual void Resume() = 0;
	virtual void Free() = 0;

	int m_iObjID;
};

//用来存储int数据
//通用的对象
class CShmIntObj : public CShmBaseObj
{
public:
	int m_iVal;

public:
	virtual int Init(){m_iVal = 0;return 0;};
	virtual void Resume(){};
	virtual void Free(){};
};

//用来存储uint32_t数据
//通用的对象 
class CShmUInt32Obj : public CShmBaseObj
{
public:
	uint32_t m_uVal;

public:
	virtual int Init() { m_uVal = 0; return 0; }
	virtual void Resume() {}
	virtual void Free() {}
};

//用来存储uint64_t数据
//通用的对象
class CShmUInt64Obj : public CShmBaseObj
{
public:
	uint64_t m_ullVal;

public:
	virtual int Init(){ m_ullVal = 0; return 0; };
	virtual void Resume(){};
	virtual void Free(){};
};

template<class T>
class CShmObjPool
{
public:
	CShmObjPool();
	~CShmObjPool();
	
	/**
	* 创建内存池
	*/
	int Create(int iShmKey, size_t dwShmSize, size_t dwMaxObjCount);
    /**
	* 分布tick需要设置此值
	*/
	void SetSlowCnt(int iCnt){m_iSlowCnt=iCnt;}

	/**
	* 从内存池里分配新的对象
	*/
	T* AllocObj();

	/**
	* 释放内存池里的对象
	*/
	void FreeObj(T* pObj);
	void FreeObj(int iObjID);	

	/**
	* 查找对象
	*/
	T* FindObj(int iObjID);

	int GetUsed(){return m_pMemPool->iUsed;}

	/**
	* 用于轮询内存池里的所有对象
	*/
	void FirstObj();
	T* NextObj(bool bDebug = false);

	/**
	* 用于轮询内存池里的所有对象 -> 每次N个
	*/
	void StartGetSlowObj();
	T* NextSlowObj(bool bDebug = false);

	void FreeAllObject()
	{
		FirstObj();
		for (T* curr = NextObj(); curr != NULL; curr = NextObj())
		{
			FreeObj(curr);
		}
	}
private:	
	void FreeObj(int iObjID, bool bFromInitFail);
	
private:
	MEMPOOL* m_pMemPool;
	int		  m_iFindIndex;
	int		  m_iFindSlowStartIndex;
	int		  m_iFindSlowCnt;
	int       m_iSlowCnt;

	int m_iShmKey;
	void* m_pMemPtr;
};


template <class	T>
CShmObjPool<T>::CShmObjPool()
{
	m_pMemPool = NULL;
	m_iFindIndex = -1;
	m_iShmKey = 0;
	m_pMemPtr = NULL;
	m_iFindSlowCnt = 0;
	m_iFindSlowStartIndex = -1;
	m_iSlowCnt = 100; // 默认100
}

template <class	T>
CShmObjPool<T>::~CShmObjPool()
{
	if (m_pMemPool != NULL)
	{
		mempool_destroy(&m_pMemPool);
	}

	if (m_pMemPtr != NULL)
	{
		free(m_pMemPtr);
		m_pMemPtr = NULL;
	}
}

template <class	T>
int CShmObjPool<T>::Create(int iShmKey, size_t dwShmSize, size_t dwMaxObjCount)
{
	if (dwMaxObjCount <= 0)
	{
		LOG_ERR("CShmObjPool<T>::Create failed, MaxObjCount[%uz]", dwMaxObjCount);
		return -1;
	}
	
	size_t needSize = MEMPOOL_CALC(dwMaxObjCount, sizeof(T));	
	LOG_ERR("shm pool obj type[%s] count[%d] needsize[%.2fM]", typeid(*this).name(), dwMaxObjCount, needSize * 1.0 / 1000000);
		
	void* pShmMem = NULL;
	int iRet = 0;

	if (iShmKey != 0)
	{
		pShmMem = CShmemMgr::CreateShmem(iShmKey, dwShmSize, needSize);

		if (NULL == pShmMem)
		{
			LOG_ERR("CreateShmem failed, ShmKey[%d] ShmSize[%zu] NeedSize[%zu]",			 
				iShmKey, dwShmSize, needSize);
			return -2;
		}
		
		if (ESSM_Init == CShmemMgr::GetStartFlag(iShmKey))
		{
			iRet = mempool_init(&m_pMemPool, dwMaxObjCount, sizeof(T), pShmMem, needSize);
			ASSERT_RET(0 == iRet, iRet);
		}
		else
		{
			iRet = mempool_attach(&m_pMemPool, dwMaxObjCount, sizeof(T), pShmMem, needSize);
			if (iRet != 0)
			{
				LOG_ERR("mempool_attach failed, ShmKey[%d] ShmSize[%zu] NeedSize[%zu]", 
					iShmKey, dwShmSize, needSize);
				return -3;
			}

			//循环调用所有对象的resume
			int iPos = 0;
			if (0 == mempool_find_used_first(m_pMemPool, &iPos) && iPos >= 0)
			{
				void* pUnitMem = mempool_get_bypos(m_pMemPool, iPos);
				ASSERT_RET(pUnitMem, -4);
				T* pT = new(pUnitMem)T;
				ASSERT(pT);

				pT->Resume();
				
				while(mempool_find_used_next(m_pMemPool, &iPos) >= 0 && iPos >= 0)
				{
					pUnitMem = mempool_get_bypos(m_pMemPool, iPos);
					if (NULL == pUnitMem)
					{
						LOG_ERR("mempool_get pos[%d] failed", iPos);
						ASSERT(pUnitMem);
					}
					
					pT = new(pUnitMem)T;
					ASSERT(pT);

					pT->Resume();
				}
			}
		}
	}
	else
	{
		pShmMem = malloc(needSize);
		m_pMemPtr = pShmMem;

		iRet = mempool_init(&m_pMemPool, dwMaxObjCount, sizeof(T), pShmMem, needSize);
		ASSERT_RET(0 == iRet, iRet);
	}
		
	return 0;
}

template <class	T>
T* CShmObjPool<T>::AllocObj()
{
	ASSERT(m_pMemPool);
	
	int idx = mempool_alloc(m_pMemPool);
	ASSERT_RET(idx >= 0, NULL);

	void* pUnitMem = mempool_get(m_pMemPool, idx);
	ASSERT(pUnitMem);
	
	T* pT = new(pUnitMem)T;
	ASSERT(pT);

	//为了防止objid为0，这儿统一加1
	pT->m_iObjID = idx+1;
	int iRet = pT->Init();
	ASSERT(0 == iRet);

	if (iRet != 0)
	{
		FreeObj(pT->m_iObjID, true);
		pT = NULL;
	}
	
	return pT;
}


template <class	T>
void CShmObjPool<T>::FreeObj(T* pObj)
{
	ASSERT(m_pMemPool);
	
	int iObjID = pObj->m_iObjID;
	FreeObj(iObjID);
}

template <class	T>
void CShmObjPool<T>::FreeObj(int iObjID)
{
	FreeObj(iObjID, false);
}

template <class	T>
void CShmObjPool<T>::FreeObj(int iObjID, bool bFromInitFail)
{
	ASSERT(m_pMemPool);
	if (iObjID <= 0)
	{
		ShmDebugErr(SHM_OBJ_POOL_ID_INVALD);
		ASSERT_RET(iObjID > 0);
	}	
	
	T* pT = (T*)mempool_get(m_pMemPool, iObjID-1);
	ASSERT_RET(pT);

	if (!bFromInitFail)
	{
		pT->Free();
	}
	
	pT->~T();
	mempool_free(m_pMemPool, iObjID-1);
}


template <class	T>
T* CShmObjPool<T>::FindObj(int iObjID)
{
	ASSERT(m_pMemPool);	
	if (iObjID <= 0)
	{
		//ShmDebugErr(SHM_OBJ_POOL_ID_INVALD);
		return NULL;
	}	
	
	return (T*)mempool_get(m_pMemPool, iObjID-1);;
}

template <class	T>
void CShmObjPool<T>::FirstObj()
{
	if (mempool_find_used_first(m_pMemPool, &m_iFindIndex) < 0)
	{
		m_iFindIndex = -1;
	}
}

template <class	T>
T* CShmObjPool<T>::NextObj(bool bDebug)
{	
	int iPrevPos = m_iFindIndex;
	if (m_iFindIndex < 0)
	{
		if (bDebug)
		{
			LOG_DBG("FindIndex: %d null", m_iFindIndex);
		}
		
		return NULL;
	}

	T* obj = (T*)mempool_get_bypos(m_pMemPool, m_iFindIndex);
	if (obj == NULL)
	{
		if (bDebug)
		{
			LOG_DBG("FindIndex: %d null", m_iFindIndex);
		}
		
		return obj;
	}
	
	if (mempool_find_used_next(m_pMemPool, &m_iFindIndex) < 0)
	{
		if (bDebug)
		{
			LOG_DBG("FindIndex: %d next is null", m_iFindIndex);
		}
		
		m_iFindIndex = -1;
	}

	if (bDebug)
	{
		LOG_DBG("FindIndex: %d hit, next: %d", iPrevPos, m_iFindIndex);
	}
	return obj;
}

template <class	T>
void CShmObjPool<T>::StartGetSlowObj()
{
	if( m_iFindSlowStartIndex < 0 )
	{
	    if (mempool_find_used_first(m_pMemPool, &m_iFindSlowStartIndex) < 0)
    	{
    		m_iFindSlowStartIndex = -1;
    	}
	}
	m_iFindSlowCnt = 0;
}

template <class	T>
T* CShmObjPool<T>::NextSlowObj(bool bDebug)
{	
    if( m_iFindSlowCnt >= m_iSlowCnt || m_iFindSlowStartIndex < 0 )
    {   
        return NULL;
    }
    
	int iPrevPos = m_iFindSlowStartIndex;
	if (m_iFindSlowStartIndex < 0)
	{
		if (bDebug)
		{
			LOG_DBG("FindIndex: %d null", m_iFindSlowStartIndex);
		}
		
		return NULL;
	}

    T* obj = (T*)mempool_get_bypos(m_pMemPool, m_iFindSlowStartIndex);
	if (obj == NULL)
	{
		if (bDebug)
		{
			LOG_DBG("FindIndex: %d null", m_iFindSlowStartIndex);
		}
		m_iFindSlowStartIndex = -1;
		return obj;
	}
    
	if (mempool_find_used_next(m_pMemPool, &m_iFindSlowStartIndex) < 0)
	{
		if (bDebug)
		{
			LOG_DBG("FindIndex: %d next is null", m_iFindSlowStartIndex);
		}
		
		m_iFindSlowStartIndex = -1;
	}

	if (bDebug)
	{
		LOG_DBG("FindIndex: %d hit, next: %d", iPrevPos, m_iFindSlowStartIndex);
	}
	m_iFindSlowCnt++;
	return obj;
}

#endif


