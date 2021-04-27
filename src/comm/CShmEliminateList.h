#ifndef CSHMELIMINATELIST_H__
#define CSHMELIMINATELIST_H__

#include "CShmObjPool.h"

//支持淘汰的内存池的共享内存池节点基类
class CEliminatePoolObjBase :public CShmBaseObj
{
public: 
	virtual int Init()
	{
		m_iNextObjID = -1;
		m_iPrevObjID = -1;
		return 0;
	};
	virtual void Resume(){};
	virtual void Free(){};

	virtual bool CanFree() { return true; };
public:
	int m_iNextObjID;
	int m_iPrevObjID;

	template<class NodeT>
	friend class CShmEliminatePool;
};

//支持淘汰的共享内存池
//NodeValueT必须继承CEliminatePoolObjBase
template<class NodeT>
class CShmEliminatePool
{
public:
	CShmEliminatePool() : m_pFnDeleteCallBackFunc(NULL)
	{

	}

	typedef void(*DeleteCallBackFunc)(NodeT *);

	int Create(int iShmKey, size_t dwShmSize, int iMaxCnt, DeleteCallBackFunc pFnFunc = NULL);

	/**
	* 分布tick需要设置此值
	*/
	void SetSlowCnt(int iCnt){m_NodePool.SetSlowCnt(iCnt);}

	//获得已经使用的链表节点个数
	int GetUsed();

	NodeT * AllocObj();

	NodeT * FindObj(int iObjID);
	//NodeT * GetObj(int iObjID);
// 	NodeT * FindObj(int iObjID) 
// 	{ 
// 		return GetObj(iObjID);
// 	};

	void FirstObj()
	{
		m_NodePool.FirstObj();
	}
	NodeT * NextObj()
	{
		return m_NodePool.NextObj();
	}

	void GetSlowObj()
	{
		m_NodePool.StartGetSlowObj();
	}
	NodeT * NextSlowObj()
	{
		return m_NodePool.NextSlowObj();
	}

	//int Free(NodeT * pNode);

	void DelObj(int iObjID);

	typedef void (*DumpFunc)(NodeT *) ;
	void DumpObj(DumpFunc);

	//将链表中某元素放到表尾
	void MoveToTail(int iObjID);

public:

	//链表表头节点的类型
	class HeadNode : public CShmBaseObj
	{
	public:

		virtual int Init()
		{
			m_iHeadObjID = -1;
			m_iTailObjID = -1;
			m_iMaxNum = 0;
			return 0;
		}

		virtual void Resume()
		{
			//这里为了保持数据不变,不能做任何操作
			//blank
			LOG_DBG("in CShmEliminatePool HeadNode Resume");
		}
		virtual void Free(){};

		void SetMaxNum(int iMaxNum)
		{
			m_iMaxNum = iMaxNum;
		}
		int GetMaxNum()
		{
			return m_iMaxNum;
		}
	
		int m_iMaxNum;
		int m_iHeadObjID;
		int m_iTailObjID;
	};

public:
	HeadNode * GetHeadNode();

private:
	
	CShmObjPool<HeadNode> m_Head;
	CShmObjPool<NodeT> m_NodePool;
	DeleteCallBackFunc m_pFnDeleteCallBackFunc;
};

template<class NodeT>
int CShmEliminatePool<NodeT>::Create(int iShmKey, size_t dwShmSize, int iMaxCnt, DeleteCallBackFunc pFnFunc)
{
	int iRet = m_Head.Create(iShmKey, dwShmSize, 1);
	if (0 != iRet)
	{
		return iRet;
	}
	if (m_Head.GetUsed() == 1)
	{
		//说明是热启动建立的内存池
		
		iRet = m_NodePool.Create(iShmKey, dwShmSize, iMaxCnt);
		if (0 != iRet)
		{
			LOG_ERR("hot start m_NodePool.Create ret(%d) invalid", iRet);
			return iRet;
		}

		m_pFnDeleteCallBackFunc = pFnFunc;

		return 0;
	}
	else if(m_Head.GetUsed() > 1)
	{
		//不应该出现的错误状态
		LOG_ERR("m_Head.GetUsed()(%d) > 1", m_Head.GetUsed());
		return -1;
	}
	else //冷启动,需要初始化
	{

		//初始化头节点
		HeadNode * headNode = m_Head.AllocObj();
		LOG_DBG("headNode->m_iObjID(%d)", headNode->m_iObjID);
		//headNode->Init();
		HeadNode * pHeadNode = GetHeadNode();
		ASSERT_RET(pHeadNode, -1);

		pHeadNode->SetMaxNum(iMaxCnt);

		iRet = m_NodePool.Create(iShmKey, dwShmSize, iMaxCnt);
		if (0 != iRet)
		{
			LOG_ERR("init start m_NodePool.Create ret(%d) invalid", iRet);
			return iRet;
		}

		m_pFnDeleteCallBackFunc = pFnFunc;
	}

	return 0;
}

template<class NodeT>
int CShmEliminatePool<NodeT>::GetUsed()
{
	return m_NodePool.GetUsed();
}

template<typename NodeT>
typename CShmEliminatePool<NodeT>::HeadNode * CShmEliminatePool<NodeT>::GetHeadNode()
{
	m_Head.FirstObj();
	return m_Head.NextObj();
}

template<class NodeT>
NodeT * CShmEliminatePool<NodeT>::AllocObj()
{

	HeadNode * pHeadNode = GetHeadNode();
	ASSERT_RET(pHeadNode, NULL);

	if (0 >= pHeadNode->GetMaxNum())
	{
		LOG_ERR("pHeadNode.GetMaxNum ret (%d) invalid", pHeadNode->GetMaxNum());
		return NULL;
	}

	int iUsedNum = m_NodePool.GetUsed();
	if (0 == iUsedNum)
	{
		ASSERT_RET(-1 == pHeadNode->m_iHeadObjID, NULL);
		ASSERT_RET(-1 == pHeadNode->m_iTailObjID, NULL);
	}


	if (iUsedNum > pHeadNode->GetMaxNum())
	{
		//出现错误的情况
		LOG_ERR("m_NodePool.GetUsed()(%d) > m_Head.GetMaxNum()(%d) invalid", 
			m_NodePool.GetUsed(), pHeadNode->GetMaxNum());
		return NULL;
	}

	if (iUsedNum == pHeadNode->GetMaxNum())
	{
		int iMaxLoopNum = m_NodePool.GetUsed();
		bool bDeleteOneElem = false;  //标记是否找到一个可以删除的元素
		int iLoopNum = 0;
		for (int i = 0; i < iMaxLoopNum; i++)
		{
			iLoopNum++;
			//遍历链表, 删除第一个可以删除的节点, 如果不可以删除,放置到链表尾
			NodeT * pToCheckNode = m_NodePool.FindObj(pHeadNode->m_iHeadObjID);
			if (!pToCheckNode->CanFree())
			{
				//检查不通过, 不能释放节点, 将该节点放置在链表尾
				MoveToTail(pToCheckNode->m_iObjID);
				
			}
			else
			{
				bDeleteOneElem = true;
				break;
			}

		}

		if (!bDeleteOneElem)
		{
			//一个元素都不能删除, 不能再继续分配节点
			LOG_ERR("bDeleteOneElem ret false, can not alloc elem any more, iLoopNum(%d)", iLoopNum);
			//##import ## 重要的中断流程
			return NULL;
		}

		//如果有限的链表空间已经用完,删除表头的元素
		NodeT * pToDelNode = m_NodePool.FindObj(pHeadNode->m_iHeadObjID);
		if (!pToDelNode)
		{
			LOG_ERR("pHeadNode->m_iHeadObjID(%d) m_NodePool.FindObj ret NULL", pHeadNode->m_iHeadObjID);
			return NULL;
		}
		
		//先确认通过链表头索引能找到对应的头元素再执行删除操作(保证一定能删除)
		DelObj(pHeadNode->m_iHeadObjID);
		iUsedNum = m_NodePool.GetUsed();
	}

	if (iUsedNum < pHeadNode->GetMaxNum())
	{
		//还有空间的情况
		NodeT * pNode = m_NodePool.AllocObj();
		if (!pNode)
		{
			LOG_ERR("m_NodePool.AllocObj() ret NULL");
			return NULL;
		}

		pNode->m_iNextObjID = -1;
		pNode->m_iPrevObjID = -1;


		{

			if (-1 == pHeadNode->m_iHeadObjID)
			{
				//链表为空的状态,当前分配出来的对象就是第一个节点
				pHeadNode->m_iHeadObjID = pNode->m_iObjID;
				pHeadNode->m_iTailObjID = pNode->m_iObjID;
			}
			else
			{
				//新分配的节点插入链表尾
				//取原有的表头
				int iOldTailID = pHeadNode->m_iTailObjID;
				NodeT * pOldTailNode = m_NodePool.FindObj(iOldTailID);
				ASSERT_RET(pOldTailNode, NULL);
				pOldTailNode->m_iNextObjID = pNode->m_iObjID;
				pNode->m_iPrevObjID = pOldTailNode->m_iObjID;
				pHeadNode->m_iTailObjID = pNode->m_iObjID;

// 				int iOldHeadID = pHeadNode->m_iHeadObjID;
// 				NodeT * pOldHeadNode = m_NodePool.FindObj(iOldHeadID);
// 				ASSERT_RET(pOldHeadNode, NULL);
// 
// 				pOldHeadNode->m_iPrevObjID = pNode->m_iObjID;
// 				pNode->m_iNextObjID = pOldHeadNode->m_iObjID;
// 
// 				//更新表头的索引
// 				pHeadNode->m_iHeadObjID = pNode->m_iObjID;
			}

		}

		return pNode;
	}
	else //m_NodePool.GetUsed() == pHeadNode.GetMaxNum()
	{
		//这种情况不应该会出现,因为已经尝试清理了数据
		LOG_ERR("iUsedNum(%d) >= pHeadNode->GetMaxNum(%d)", iUsedNum, pHeadNode->GetMaxNum());
		return NULL;
		//空间满需要淘汰第一个节点的情况
		//取第一个待淘汰的节点
// 		NodeT * pOldFistNode = m_NodePool.GetObj(pHeadNode.m_iHeadObjID);
// 		ASSERT_RET(pOldHeadNode, NULL);
// 
// 		pNode->m_iNexObjID = pOldFistNode->m_iNextObjID;
// 		NodeT * pNextNode = m_NodePool.GetObj(pNode->m_iNexObjID);

	}

	return NULL;
}


//在链表中删除特定的节点
template<class NodeT>
void CShmEliminatePool<NodeT>::DelObj(int iObjID)
{
	LOG_DBG("CShmEliminatePool<NodeT>::DelObj delete Obj(%d)", iObjID);
	HeadNode * pHeadNode = GetHeadNode();
	ASSERT_RET(pHeadNode); //头节点必须存在

	int iOldUsedNum = m_NodePool.GetUsed();
	if (0 >= iOldUsedNum)
	{
		LOG_ERR("iUsedNum(%d) invalid when del objID(%d)", iOldUsedNum, iObjID);
		return;
	}

	//有元素存在的情况下,头尾指针肯定都存在
	ASSERT_RET(0 < pHeadNode->m_iHeadObjID);
	ASSERT_RET(0 < pHeadNode->m_iTailObjID);

	//取待删除的节点
	NodeT * pToDelNode = m_NodePool.FindObj(iObjID);
	if (!pToDelNode)
	{
		LOG_ERR("iObjID(%d) no in m_NodePool", iObjID);
		return;
	}

	//修改链表头指针
	if (pHeadNode->m_iHeadObjID == iObjID)
	{
		pHeadNode->m_iHeadObjID = pToDelNode->m_iNextObjID;
	}

	if (pHeadNode->m_iTailObjID == iObjID)
	{
		pHeadNode->m_iTailObjID = pToDelNode->m_iPrevObjID;
	}

	//修改链表的引用关系
	{
		NodeT * pPrevNode = NULL;
		NodeT * pNextNode = NULL;

		if (0 < pToDelNode->m_iPrevObjID)
		{
			pPrevNode = m_NodePool.FindObj(pToDelNode->m_iPrevObjID);
		}

		if (0 < pToDelNode->m_iNextObjID)
		{
			pNextNode = m_NodePool.FindObj(pToDelNode->m_iNextObjID);
		}

		if (pPrevNode)
		{
			pPrevNode->m_iNextObjID = pToDelNode->m_iNextObjID;
		}
		if (pNextNode)
		{
			pNextNode->m_iPrevObjID = pToDelNode->m_iPrevObjID;
		}

	}

	//从内存池中删除对应的节点
	if (m_pFnDeleteCallBackFunc)
	{
		m_pFnDeleteCallBackFunc(pToDelNode);
	}
	m_NodePool.FreeObj(pToDelNode);

	return;
}

//查找接口会引起最近被访问的元素被放置在链表末尾
template<class NodeT>
NodeT * CShmEliminatePool<NodeT>::FindObj(int iObjID)
{
	ASSERT_RET(0 < iObjID, NULL);
	HeadNode * pHeadNode = GetHeadNode();
	ASSERT_RET(pHeadNode, NULL); //头节点必须存在

	//内存池中找到对应的元素
	NodeT * pRetNode = m_NodePool.FindObj(iObjID);
	ASSERT_RET(pRetNode, NULL);
	if (pHeadNode->m_iHeadObjID == pHeadNode->m_iTailObjID
		&& pHeadNode->m_iHeadObjID == pRetNode->m_iObjID)
	{
		//只有一个节点的情况, 不需要做处理,直接返回
		return pRetNode;
	}
	if (pHeadNode->m_iTailObjID == pRetNode->m_iObjID
		&& pRetNode->m_iNextObjID == -1)
	{
		//待返回的节点是最后一个节点的情况,直接返回
		return pRetNode;
	}

	//从链表中删除
	{
		//更新头节点中的头尾索引
		if (pHeadNode->m_iHeadObjID == pRetNode->m_iObjID)
		{
			//头索引需要更新的情况
			pHeadNode->m_iHeadObjID = pRetNode->m_iNextObjID;
		}
		if (pHeadNode->m_iTailObjID == pRetNode->m_iObjID)
		{
			//尾索引
			pHeadNode->m_iTailObjID = pRetNode->m_iPrevObjID;
		}

		NodeT * pPrevNode = NULL;
		NodeT * pNextNode = NULL;
		if (0 < pRetNode->m_iPrevObjID)
		 {
			pPrevNode = m_NodePool.FindObj(pRetNode->m_iPrevObjID);
			ASSERT_RET(pPrevNode, NULL);
		}
		if (0 < pRetNode->m_iNextObjID)
		{
			pNextNode = m_NodePool.FindObj(pRetNode->m_iNextObjID);
			ASSERT_RET(pNextNode, NULL);
		}

		if (pPrevNode)
		{
			pPrevNode->m_iNextObjID = pRetNode->m_iNextObjID;
			
		}

		if (pNextNode)
		{
			pNextNode->m_iPrevObjID = pRetNode->m_iPrevObjID;
			
		}
		pRetNode->m_iPrevObjID = -1;
		pRetNode->m_iNextObjID = -1;

	}

	//需要返回的节点放置到链表尾
	{

		ASSERT_RET(0 < pHeadNode->m_iTailObjID, NULL);
		NodeT * pTailNode = m_NodePool.FindObj(pHeadNode->m_iTailObjID);
		ASSERT_RET(pTailNode, NULL);
		pTailNode->m_iNextObjID = pRetNode->m_iObjID;
		pRetNode->m_iPrevObjID = pTailNode->m_iObjID;
		pHeadNode->m_iTailObjID = pRetNode->m_iObjID;

	}

	return pRetNode;
}

template<class NodeT>
//void CShmEliminatePool<NodeT>::DumpObj(typename CShmEliminatePool<NodeT>::DumpFunc pDumpFunc)
void CShmEliminatePool<NodeT>::DumpObj(DumpFunc pDumpFunc)
{
	if (NULL == pDumpFunc)
	{
		LOG_ERR("pDumpFunc is NULL");
		return;
	}

	//按链表顺序输出各元素
	HeadNode * pHeadNode = GetHeadNode();
	if (!pHeadNode)
	{
		LOG_DBG("pHeadNode is NULL");
		return;
	}
	LOG_DBG("pHeadNode HeadId(%d), TailId(%d), ListSize(%d)", pHeadNode->m_iHeadObjID, pHeadNode->m_iTailObjID, m_NodePool.GetUsed());


	NodeT * node = m_NodePool.FindObj(pHeadNode->m_iHeadObjID);
	if (!node)
	{
		LOG_ERR("first node is NULL 4 HeadObjID(%d)", pHeadNode->m_iHeadObjID);
		return;
	}
	while (node)
	{
		pDumpFunc(node);
		if (-1 == node->m_iNextObjID)
		{
			//已经是最后一个节点
			break;
		}

		node = m_NodePool.FindObj(node->m_iNextObjID);
	}

	return;
}

//将链表中某元素放到表尾
template<class NodeT>
void CShmEliminatePool<NodeT>::MoveToTail(int iObjID)
{
	ASSERT_RET(0 < iObjID);

	HeadNode * pHeadNode = GetHeadNode();
	ASSERT_RET(NULL != pHeadNode);

	if (pHeadNode->m_iTailObjID == iObjID)
	{
		//已经是链表尾的元素,不用再做处理
		return;
	}

	NodeT * pNode = m_NodePool.FindObj(iObjID);
	if (!pNode)
	{
		LOG_ERR("pNode NULL 4 ObjID(%d)", iObjID);
		return;
	}

	ASSERT_RET(pNode->m_iObjID == iObjID);

	ASSERT_RET(0 < pHeadNode->m_iHeadObjID);
	ASSERT_RET(0 < pHeadNode->m_iTailObjID);
	NodeT * pTailNode = m_NodePool.FindObj(pHeadNode->m_iTailObjID);
	ASSERT_RET(NULL != pTailNode);

	//取当前节点的前一个节点
	NodeT * pPrevNode = NULL;
	if (0 < pNode->m_iPrevObjID)
	{
		pPrevNode = m_NodePool.FindObj(pNode->m_iPrevObjID);
		ASSERT_RET(pPrevNode);
	}

	NodeT * pNextNode = NULL;
	if (0 < pNode->m_iNextObjID)
	{
		pNextNode = m_NodePool.FindObj(pNode->m_iNextObjID);

		ASSERT_RET(pNextNode);
	}

	//修改待移动节点前后节点的链表引用关系
	if (pPrevNode)
	{
		pPrevNode->m_iNextObjID = pNextNode ? pNextNode->m_iObjID : -1;
	}

	if (pNextNode)
	{
		pNextNode->m_iPrevObjID = pPrevNode ? pPrevNode->m_iObjID : -1;
	}

	//todo:
	//修改头节点的m_iHeadObjID
	if (pHeadNode->m_iHeadObjID == iObjID)
	{
		pHeadNode->m_iHeadObjID = pNode->m_iNextObjID;
	}

	pNode->m_iPrevObjID = -1;
	pNode->m_iNextObjID = -1;

	//取链表尾的节点
// 	NodeT * pTailNode = m_NodePool->FindObj(pHeadNode->m_iTailObjID);
// 	if (!pTailNode)
// 	{
// 		LOG_ERR("pTailNode NULL 4 Tail ObjID(%d)", pHeadNode->m_iTailObjID);
// 		return;
// 	}

	pTailNode->m_iNextObjID = iObjID;
	pNode->m_iPrevObjID = pTailNode->m_iObjID;
	pNode->m_iNextObjID = -1;
	pHeadNode->m_iTailObjID = iObjID;
	return;
}

#endif