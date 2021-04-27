/* 
*  共享对象内存池
*/

#ifndef CSHMHASHOBJPOOL
#define CSHMHASHOBJPOOL

#include <new>
#include "CShmObjPool.h"
#include "CShmHash.h"



template<class KeyType, class T>
class CShmHashObjPool
{
public:
	CShmHashObjPool();
	~CShmHashObjPool();
	
	/**
	* 创建内存池
	*/
	int Create(int iShmKey, size_t dwShmSize, size_t dwMaxObjCount);

	/**
	* 分布tick需要设置此值
	*/
	void SetSlowCnt(int iCnt){m_objPool.SetSlowCnt(iCnt);}

	/**
	* 获取指定key的对象
	*/
	T* Find(KeyType key);

	/**
	* 插入对象
	*/
	T* Insert(KeyType key);

	/**
	* 删除对象
	*/
	void Remove(KeyType key);

	/**
	* 当前分配的对象数
	*/
	int GetUsed(){return m_objPool.GetUsed();}

	/**
	* 用于轮询内存池里的所有对象
	*/
	void FirstObj();
	T* NextObj(bool bDebug = false);

	/**
	* 用于轮询内存池里的所有对象 -> 每次N个
	*/
	void GetSlowObj();
	T* NextSlowObj(bool bDebug = false);

	void Clear();
	
private:
	CShmObjPool<T> m_objPool;
	CShmHashTable<KeyType, int> m_hashIndex;
};


template<class KeyType, class T>
CShmHashObjPool<KeyType, T>::CShmHashObjPool()
{
}

template<class KeyType, class T>
CShmHashObjPool<KeyType, T>::~CShmHashObjPool()
{
}

template<class KeyType, class T>
int CShmHashObjPool<KeyType, T>::Create(int iShmKey, size_t dwShmSize, size_t dwMaxObjCount)
{
	int iRet = m_objPool.Create(iShmKey, dwShmSize, dwMaxObjCount);
	if (iRet != 0)
	{
		LOG_ERR("CShmObjPool create failed");
		return -1;
	}

	iRet = m_hashIndex.Create(iShmKey, dwShmSize, dwMaxObjCount);
	if (iRet != 0)
	{
		LOG_ERR("CShmHashTable create failed");
		return -1;
	}
	
	return 0;
}

/**
* 获取指定key的对象
*/
template<class KeyType, class T>
T* CShmHashObjPool<KeyType, T>::Find(KeyType key)
{
	int* pIndex = m_hashIndex.Find(key);
	if (pIndex)
	{
		return m_objPool.FindObj(*pIndex);
	}

	return NULL;
}


/**
* 插入对象
*/
template<class KeyType, class T>
T* CShmHashObjPool<KeyType, T>::Insert(KeyType key)
{
	int* pIndex = m_hashIndex.Insert(key);
	if (!pIndex)
	{
		LOG_ERR("CShmHashObjPool is full now");
		return NULL;
	}

	T* pFree = m_objPool.AllocObj();
	ASSERT(pFree);

	*pIndex = pFree->m_iObjID;
	return pFree;
}


/**
* 删除对象
*/
template<class KeyType, class T>
void CShmHashObjPool<KeyType, T>::Remove(KeyType key)
{
	int* pIndex = m_hashIndex.Find(key);
	if (!pIndex)
	{
		return;
	}
	
	m_objPool.FreeObj(*pIndex);
	m_hashIndex.Remove(key);
}


template<class KeyType, class T>
void CShmHashObjPool<KeyType, T>::FirstObj()
{
	m_objPool.FirstObj();
}

template<class KeyType, class T>
T* CShmHashObjPool<KeyType, T>::NextObj(bool bDebug)
{	
	return m_objPool.NextObj(bDebug);
}

template<class KeyType, class T>
void CShmHashObjPool<KeyType, T>::GetSlowObj()
{
	m_objPool.StartGetSlowObj();
}

template<class KeyType, class T>
T* CShmHashObjPool<KeyType, T>::NextSlowObj(bool bDebug)
{	
	return m_objPool.NextSlowObj(bDebug);
}

template<class KeyType, class T>
void CShmHashObjPool<KeyType, T>::Clear()
{
	m_objPool.FreeAllObject();
	m_hashIndex.Clear();
}



#endif


