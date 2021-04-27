#include	<errno.h>
#include	<stdlib.h>
#include	<sys/ipc.h>
#include	<sys/shm.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	"CShmemMgr.h"
#include 	"klog.h"


int CShmemMgr::ms_iKeyToAddrNum = 0;    //当前已经存在的共享内存个数
ShmKeyToAddress CShmemMgr::ms_stKeyToAddr[MAX_KEY_TO_ADDRESS_NUM];    //共享内存到地址数组

void* CShmemMgr::GetShmem( const int aiKey, const size_t aiNeedSize, ENMShmemStartMode& enmMode )
{
    //查找指定Key的共享内存是否存在
    void* pShmem = NULL;
    ShmKeyToAddress *pstKeyToAddr = FindAddrOfKey(aiKey);
    if (NULL == pstKeyToAddr)
    {
        //不存在，直接创建NeedSize大小的共享内存
        if (0 != CreateShmem (aiKey, aiNeedSize, aiNeedSize))
        {
            //创建失败，返回NULL
            LOG_ERR("CreateShmem(%d, %zu) failed!", aiKey, aiNeedSize);
            return NULL;
        }
        //再次查找
        pstKeyToAddr = FindAddrOfKey(aiKey);
        if (NULL == pstKeyToAddr)
        {
            LOG_ERR("FindAddrOfKey(%d) failed!", aiKey);
            return NULL;
        }
    }

    //返回参数中带回共享内存启动类型
    enmMode = pstKeyToAddr->m_enmStartMode;
    LOG_INFO("Start Mode[%d]", enmMode);

    //根据偏移位置计算分配内存的首地址
    pShmem  = (char*)pstKeyToAddr->m_vpMemAddr + pstKeyToAddr->m_iOffset;
    LOG_INFO("Offset[%" PRIu64 "]", pstKeyToAddr->m_iOffset);

    //判断当前请求是否超出size范围
    if (pstKeyToAddr->m_iSize < pstKeyToAddr->m_iOffset + aiNeedSize)
    {
        //申请溢出，返回NULL
        LOG_ERR("Size[%zu] Offset[%" PRIu64 "] aiNeedSize[%zu] too large!", \
            pstKeyToAddr->m_iSize, pstKeyToAddr->m_iOffset, aiNeedSize);
        return NULL;
    }
    else
    {
        //重置偏移量
        pstKeyToAddr->m_iOffset += aiNeedSize;
        LOG_INFO("Reset Offset[%"PRIu64"]", pstKeyToAddr->m_iOffset);
    }

    //返回分配内存的首地址
    return pShmem;
}

void* CShmemMgr::CreateShmem( const int aiKey, const size_t aiTotalSize, const size_t aiNeedSize, int aiMode /*= 0660*/ )
{
    ENMShmemStartMode enmMode = ESSM_Init;
    int     iHandle = -1;
    void*   pvMem   = NULL;

	//查找是否已经存在本地
	ShmKeyToAddress *pstKeyToAddr = FindAddrOfKey(aiKey);
	if (NULL != pstKeyToAddr)
	{
		if (pstKeyToAddr->m_iOffset + aiNeedSize > pstKeyToAddr->m_iSize)
		{
			LOG_ERR("CreatShmem fail. offset+needsize:%"PRIu64" > keysize:%zu",
										pstKeyToAddr->m_iOffset + aiNeedSize, 
										pstKeyToAddr->m_iSize);
			return NULL;
		}
		void *pvMem = (void *)(((char *)pstKeyToAddr->m_vpMemAddr) + pstKeyToAddr->m_iOffset);
		pstKeyToAddr->m_iOffset += aiNeedSize;

		LOG_INFO("[exist]share memory key:%u, needsize:%zu, total size:%"PRIu64".",
			aiKey, aiNeedSize, pstKeyToAddr->m_iOffset);
		return pvMem;
	}

    LOG_INFO("Key[%d] Size[%zu]", aiKey, aiTotalSize);

    iHandle = shmget(aiKey, aiTotalSize, aiMode|IPC_CREAT|IPC_EXCL);
    if (0 > iHandle)
    {
        //创建失败，判断是否是已经存在
        if ( errno == EEXIST)
        {
            LOG_INFO("Start ESSM_Resume");
            //共享内存已经存在，设置启动模式
            enmMode = ESSM_Resume;
            
            //将Key与Shm关联
            iHandle = shmget (aiKey, aiTotalSize, aiMode);
            if (0 > iHandle)
            {
                //关联失败
                LOG_ERR("EEXIST shmget(%d,%zu,%x) failed[%d]! error(%d)(%s)", \
                    aiKey, aiTotalSize, aiMode, iHandle, errno, strerror(errno));
                return NULL;
            }
            else
            {
                //关联成功，获取地址
                pvMem = (char*)shmat(iHandle, 0, 0);
                if (pvMem == NULL)
                {
                    LOG_ERR("Resume shmat(%d) failed!", iHandle);
                    return NULL;
                }
            }
        }
        else
        {
            //创建失败
            LOG_ERR("shmget(%d,%zu,%x) failed! error(%d)(%s)", \
                aiKey, aiTotalSize, aiMode|IPC_CREAT|IPC_EXCL,
                errno, strerror(errno));
            return NULL;
        }
    }
    else
    {
        //共享内存是重新创建的，设置启动模式
        LOG_INFO("Start ESSM_Init");
        enmMode = ESSM_Init;
        pvMem   = (char*)shmat(iHandle, 0, 0);
        if (pvMem == NULL)
        {
            LOG_ERR("Init shmat(%d) failed!", iHandle);
            return NULL;
        }
        //因为这样会导致程序启动必然使用全部分配的空间
        //所以去掉，由分配者自行初始化
        //初始创建置为0
        //memset(pvMem, 0, aiTotalSize);
    }

    //保存共享内存信息
    ShmKeyToAddress stKeyToAddr;
    stKeyToAddr.m_iKey          = aiKey;
    stKeyToAddr.m_iSize         = aiTotalSize;
    stKeyToAddr.m_iOffset       = 0;
    stKeyToAddr.m_vpMemAddr     = pvMem;
    stKeyToAddr.m_enmStartMode  = enmMode;

    if (aiNeedSize > aiTotalSize)
    {
        stKeyToAddr.m_iOffset = 0;
        pvMem = NULL;
        LOG_ERR("%s Error NULL aiTotalSize:%zu But aiNeedSize:%zu, Pls Check", __FUNCTION__, aiTotalSize, aiNeedSize);
    }
    else
    {
       stKeyToAddr.m_iOffset = aiNeedSize;   

       LOG_INFO("[first]share memory key:%u, needsize:%zu, total size:%zu.",
            aiKey, aiNeedSize, stKeyToAddr.m_iOffset); 
    }
	
    AddOneKeyToAddress(stKeyToAddr);

    return pvMem;
}

int CShmemMgr::DestroyShmem (const int aiKey, const size_t aiTotalSize)
{
    int iHandle = shmget (aiKey, aiTotalSize, 0640|IPC_EXCL);

    if (iHandle >= 0)
    {
        DelOneKeyToAddress(aiKey);
        shmctl(iHandle, IPC_RMID, 0);
    }

    return 0; 
}

ENMShmemStartMode CShmemMgr::GetStartFlag (const int aiKey)
{
    //查找是否已经存在
    ShmKeyToAddress *pstKeyToAddr = FindAddrOfKey(aiKey);
    if (NULL != pstKeyToAddr)
    {
        return pstKeyToAddr->m_enmStartMode;
    }

    return ESSM_Init;
}

ShmKeyToAddress* CShmemMgr::FindAddrOfKey(const int aiKey)
{
    for(int i = 0; i < ms_iKeyToAddrNum; ++i)
    {
        if (ms_stKeyToAddr[i].m_iKey == aiKey)
        {
            return &ms_stKeyToAddr[i];
        }
    }
    return NULL;
}

int CShmemMgr::AddOneKeyToAddress(ShmKeyToAddress &stKeyToAddr)
{
    if (ms_iKeyToAddrNum >= MAX_KEY_TO_ADDRESS_NUM)
    {
        printf ("key to address is full");
        return -1;
    }

    ms_stKeyToAddr[ms_iKeyToAddrNum] = stKeyToAddr;

    ++ms_iKeyToAddrNum;

    return 0;
}

int CShmemMgr::DelOneKeyToAddress(const int aiKey)
{
    int i = 0;
    for(i = 0; i < ms_iKeyToAddrNum; ++i)
    {
        if (ms_stKeyToAddr[i].m_iKey == aiKey)
        {
            break;
        }
    }
    if (i < ms_iKeyToAddrNum)
    {
        for (; i < ms_iKeyToAddrNum-1; ++i)
        {
            ms_stKeyToAddr[i] = ms_stKeyToAddr[i+1];
        }
        --ms_iKeyToAddrNum;
        return 0;
    }
    return -1;
}

//创建共享内存，如果首次创建，则初始化zero
void* CShmemMgr::CreateShmemWithZero(const int aiKey, const size_t aiTotalSize, const size_t aiNeedSize)
{	
	size_t needSize = aiNeedSize;
	void* pShmMem = CShmemMgr::CreateShmem(aiKey, aiTotalSize, needSize);
	if (NULL == pShmMem)
	{
		LOG_ERR("CreateShmem failed, ShmKey[%d] ShmSize[%zu] NeedSize[%zu]",
			aiKey, aiTotalSize, needSize);
		return NULL;
	}
	
	int iRet = 0;
	if (ESSM_Init == CShmemMgr::GetStartFlag(aiKey))
	{
		memset(pShmMem, 0, needSize);
	}

	return pShmMem;
}

int CShmemMgr::GetOffset(const int aiKey, void* pAddr)
{
	ShmKeyToAddress *pstKeyToAddr = FindAddrOfKey(aiKey);
	if (pstKeyToAddr)
	{
		return (intptr_t)pAddr - (intptr_t)pstKeyToAddr->m_vpMemAddr;
	}

	return -1;
}

