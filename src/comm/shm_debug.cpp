/* 
*  共享对象内存池
*/

#include "shm_debug.h"

/*
* 模版类调试很麻烦，所以统一调用该接口，在该接口里打断点
*/
int ShmDebugErr(int iErrType)
{
    LOG_ERR("ShmDebugErr: %d", iErrType);
    return 0;
}
