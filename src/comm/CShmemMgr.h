
#ifndef _SHMEMMGR_H_
#define _SHMEMMGR_H_

#include "comm_inc.h"


//共享内存启动模式
enum ENMShmemStartMode
{
    ESSM_Init   = 1,    //全新，即原来没有共享内存新创建
    ESSM_Resume = 2,    //恢复，即共享原来已经存在，恢复使用
};

#define MAX_KEY_TO_ADDRESS_NUM      50     //最大共享内存块个数

//共享内存到进程空间地址的映射
struct ShmKeyToAddress
{
    int                 m_iKey;             //标识key
    size_t              m_iSize;            //共享内存大小
    uint64_t            m_iOffset;          //当前已分配的偏移
    void*               m_vpMemAddr;       //对应的地址
    ENMShmemStartMode   m_enmStartMode;     //启动模式
};

class CShmemMgr
{
public:
    //申请共享内存，不存在时直接创建
    static void*    GetShmem(const int aiKey, const size_t aiNeedSize, ENMShmemStartMode& enmMode);

    //创建共享内存
    static void* CreateShmem (const int aiKey, const size_t aiTotalSize, const size_t aiNeedSize, int aiMode = 0660);    

    static int  DestroyShmem (const int aiKey, const size_t aiTotalSize);

    static ENMShmemStartMode GetStartFlag (const int aiKey);

	//创建共享内存，如果首次创建，则初始化zero
	static void* CreateShmemWithZero(const int aiKey, const size_t aiTotalSize, const size_t aiNeedSize);

	static int GetOffset(const int aiKey, void* pAddr);

private:

    //通过共享内存的key和大小获得其进程空间指针
    static ShmKeyToAddress* FindAddrOfKey(const int aiKey);

    //增加一个共享内存的key和大小与其进程空间指针的对应关系
    static int AddOneKeyToAddress(ShmKeyToAddress &stKeyToAddr);

    //删除一个共享内存的key和大小与其进程空间指针的对应关系
    static int DelOneKeyToAddress(const int aiKey);

private:

    //当前已经存在的共享内存个数
    static int ms_iKeyToAddrNum;   
    //共享内存到地址数组
    static ShmKeyToAddress ms_stKeyToAddr[MAX_KEY_TO_ADDRESS_NUM];

};

#endif
