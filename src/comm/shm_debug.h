/* 
*  共享对象内存池
*/

#ifndef CSHMDBG_H__
#define CSHMDBG_H__

#include <new>
#include "comm_inc.h"
#include "comm/mempool.h"
#include "CShmemMgr.h"

enum ShmDbgErrType
{
	SHM_OBJ_POOL_ID_INVALD = 1,
};


/*
* 模版类调试很麻烦，所以统一调用该接口，在该接口里打断点
*/
int ShmDebugErr(int iErrType);


#endif


