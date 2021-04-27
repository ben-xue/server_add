
#ifndef MEMPOOL_SHM_H
#define MEMPOOL_SHM_H

#include "comm/mempool.h"

#ifdef __cplusplus
extern "C"
{
#endif

int mempool_new_shm(MEMPOOL** ppstPool, unsigned int uiKey, int iMax, int iUnit);

#ifdef __cplusplus
}
#endif

#endif  /* TMEMPOOL_SHM_H */

