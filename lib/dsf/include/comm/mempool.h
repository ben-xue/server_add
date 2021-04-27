
#ifndef MEMPOOL_H
#define MEMPOOL_H

#include <stddef.h>


#ifdef __cplusplus
extern "C"
{
#endif

#define MEMPOOL_MAGIC			0x4324
#define MEMPOOL_BUILD			0x01

#define MEMPOOL_FIFO			0x00
#define MEMPOOL_LIFO			0x01

struct tagMemBlock
{
	int fValid;   
	int iIdx; //此memblock在内存池中的索引
	int iNext; // 空闲链表下一个指针
	int iUsedPrev;	//使用链表前一个指针
	int iUsedNext;	//使用链表下一个指针
	int iCount; //暂时没有使用 
	char szData[1]; //应用数据起始位置
};

typedef struct tagMemBlock		MEMBLOCK;
typedef struct tagMemBlock		*LPMEMBLOCK;

#define MEMBLOCK_IS_VALID(p)	((p)->fValid)
#define MEMBLOCK_GET_DATA(p)	((p)->szData)

struct tagMemPool
{
	int iMagic; //用于检查mempool内存
	int iBuild; //用于检查mempool内存
	int iMax;//mempool的容量
	int iUnit;//应用数据的大小

	int iUsed;//有多少memblock已经被使用
	int iRealUnit;//在mempool中考虑圆整后数据占用内存的大小
	unsigned long int iSize;//mempool占用内存的大小
	unsigned long int iRealSize;//与iSize相同, 目前没有使用

	int iLastIdx;//最后分配的memblock索引
	int iInited;//mempool是否经过了初始化
	int iIsCalloc;//mempool是否分配过内存
	int iMethod;//TMEMPOOL_FIFO  memblock分配采用先进先出,是默认的方法; TMEMPOOL_LIFO 

	int iFreeHead; //空闲链表头指针
	int iFreeTail;//空闲链表尾指针
	int iStart;//内部使用，与iEnd配合检验tmempool_alloc和tmempool_free操作的原子性
	int iEnd;

	int iUsedHead;	//使用链表头指针
	int iUsedTail;  //使用链表尾指针

	char szRes[56];//预留

	char szBlocks[1];//数据区起始位置
};

typedef struct tagMemPool		MEMPOOL;
typedef struct tagMemPool		*LPMEMPOOL;


#define MEMPOOL_GET_PTR(pool, idx)	(MEMBLOCK*)((pool)->szBlocks + (unsigned long int)((pool)->iRealUnit)*((idx) % (pool)->iMax) )
#define MEMPOOL_GET_PTR_BYPOS(pool, idx)   (MEMBLOCK*)((pool)->szBlocks + (unsigned long int)((pool)->iRealUnit)*(idx))
#define MEMPOOL_GET_CAP(pool)		((pool)->iMax)

#define MEMPOOL_GET_BLOCK_IDX(blk)	((blk)->iIdx)

#define MEMPOOL_MIN_SIZE()			( offsetof(MEMPOOL, szBlocks) )

#define MEMPOOL_ROUND(unit, round)	( ( ( (unit) + (round) - 1 ) / (round) ) * (round) )

#define MEMPOOL_ROUND_UNIT(unit)	MEMPOOL_ROUND(unit, 64)

#define MEMPOOL_ROUND_POOL(unit)	MEMPOOL_ROUND(unit, 128)

#define MEMPOOL_REAL_UNIT(unit)	MEMPOOL_ROUND_UNIT( ( (unit) + offsetof(MEMBLOCK, szData) ) )

#define MEMPOOL_CALC(max, unit)	( offsetof(MEMPOOL, szBlocks) + (max)*MEMPOOL_REAL_UNIT(unit) )

/**初始化mempool内存，形成空闲链表
*@param[in,out]	 ppstPool  mempool指针的指针
*@param[in]      iMax      mempool的容量
*@param[in]      iUnit     应用数据的大小
*@param[in]      pvBase    用于形成mempool的内存的指针
*@param[in]      iSize     pvBase 内存的大小
		
*@retval  0  成功
         -1  失败
*/
int mempool_init(MEMPOOL** ppstPool, int iMax, int iUnit, void* pvBase, unsigned long int iSize);

/**分配mempool内存并初始化
*@param[in,out]	 ppstPool  mempool指针的指针
*@param[in]      iMax      mempool的容量
*@param[in]      iUnit     应用数据的大小
		
*@retval  0  成功
         -1  失败

*@note   与tmempool_destroy配对使用         
*/
int mempool_new(MEMPOOL** ppstPool, int iMax, int iUnit);

/**attach 一块已存在的mempool内存，并检查内存格式
*@param[in,out]	 ppstPool  mempool指针的指针
*@param[in]      iMax      mempool的容量
*@param[in]      iUnit     应用数据的大小
*@param[in]      pvBase    用于形成mempool的内存的指针
*@param[in]      iSize     pvBase 内存的大小
		
*@retval  0  成功
         -1  失败
*/
int mempool_attach(MEMPOOL** ppstPool, int iMax, int iUnit, void* pvBase, unsigned long int iSize);

/**销毁由tmempool_new创建的内存池*/
int mempool_destroy(MEMPOOL** ppstPool);

/**设置mempool的memblock分配方式
*@param[in]	     pstPool   mempool指针
*@param[in]      iMethod   TMEMPOOL_FIFO  先进先出,默认方式; TMEMPOOL_LIFO 后进先出
		
*@retval  0  成功
         -1  失败
*/
int mempool_set_method(MEMPOOL* pstPool, int iMethod);

/**根据索引获取应用数据
*@param[in]	     pstPool   mempool指针
*@param[in]      iIdx      memblock的索引
		
*@retval  not NULL  成功
          NULL      失败
*/
void* mempool_get(MEMPOOL* pstPool, int iIdx);

/**根据索引获取应用数据
*@param[in]	     pstPool   mempool指针
*@param[in]      iPos      memblock 数组索引0~max-1
		
*@retval  not NULL  成功
          NULL      失败
*/
void* mempool_get_bypos(MEMPOOL* pstPool, int iPos);


/**分配一块memblock
*@param[in]	     pstPool   mempool指针
		
*@retval  >=0     成功,   返回memblock的索引
          -1      失败
*/
int mempool_alloc(MEMPOOL* pstPool);

/**释放一块索引为iIdx的memblock */
int mempool_free(MEMPOOL* pstPool, int iIdx);

/**重新生成空闲链表，一般在iStart!=iEnd时使用*/
int mempool_fix(MEMPOOL* pstPool);

/**从索引0开始找到第一个使用中的memblock
*@param[in]	     pstPool   mempool指针
*@param[out]     piPos     存储memblock index
		
*@retval   0      成功
          -1      失败
*/
int mempool_find_used_first(MEMPOOL* pstPool, int* piPos);

/**从输入的一个索引的下一个索引开始找到一个使用中的memblock
*@param[in]	     pstPool   mempool指针
*@param[in,out]  piPos     
                       in  输入的索引
                      out  >=0  找到的索引
                           -1   已经没有下一个使用中的memblock了
                      
*@retval  >=0     成功
          -1      失败
*/
int mempool_find_used_next(MEMPOOL* pstPool, int* piPos);

#ifdef __cplusplus
}
#endif

#endif /* TMEMPOOL_H */
