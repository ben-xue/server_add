
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
	int iIdx; //��memblock���ڴ���е�����
	int iNext; // ����������һ��ָ��
	int iUsedPrev;	//ʹ������ǰһ��ָ��
	int iUsedNext;	//ʹ��������һ��ָ��
	int iCount; //��ʱû��ʹ�� 
	char szData[1]; //Ӧ��������ʼλ��
};

typedef struct tagMemBlock		MEMBLOCK;
typedef struct tagMemBlock		*LPMEMBLOCK;

#define MEMBLOCK_IS_VALID(p)	((p)->fValid)
#define MEMBLOCK_GET_DATA(p)	((p)->szData)

struct tagMemPool
{
	int iMagic; //���ڼ��mempool�ڴ�
	int iBuild; //���ڼ��mempool�ڴ�
	int iMax;//mempool������
	int iUnit;//Ӧ�����ݵĴ�С

	int iUsed;//�ж���memblock�Ѿ���ʹ��
	int iRealUnit;//��mempool�п���Բ��������ռ���ڴ�Ĵ�С
	unsigned long int iSize;//mempoolռ���ڴ�Ĵ�С
	unsigned long int iRealSize;//��iSize��ͬ, Ŀǰû��ʹ��

	int iLastIdx;//�������memblock����
	int iInited;//mempool�Ƿ񾭹��˳�ʼ��
	int iIsCalloc;//mempool�Ƿ������ڴ�
	int iMethod;//TMEMPOOL_FIFO  memblock��������Ƚ��ȳ�,��Ĭ�ϵķ���; TMEMPOOL_LIFO 

	int iFreeHead; //��������ͷָ��
	int iFreeTail;//��������βָ��
	int iStart;//�ڲ�ʹ�ã���iEnd��ϼ���tmempool_alloc��tmempool_free������ԭ����
	int iEnd;

	int iUsedHead;	//ʹ������ͷָ��
	int iUsedTail;  //ʹ������βָ��

	char szRes[56];//Ԥ��

	char szBlocks[1];//��������ʼλ��
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

/**��ʼ��mempool�ڴ棬�γɿ�������
*@param[in,out]	 ppstPool  mempoolָ���ָ��
*@param[in]      iMax      mempool������
*@param[in]      iUnit     Ӧ�����ݵĴ�С
*@param[in]      pvBase    �����γ�mempool���ڴ��ָ��
*@param[in]      iSize     pvBase �ڴ�Ĵ�С
		
*@retval  0  �ɹ�
         -1  ʧ��
*/
int mempool_init(MEMPOOL** ppstPool, int iMax, int iUnit, void* pvBase, unsigned long int iSize);

/**����mempool�ڴ沢��ʼ��
*@param[in,out]	 ppstPool  mempoolָ���ָ��
*@param[in]      iMax      mempool������
*@param[in]      iUnit     Ӧ�����ݵĴ�С
		
*@retval  0  �ɹ�
         -1  ʧ��

*@note   ��tmempool_destroy���ʹ��         
*/
int mempool_new(MEMPOOL** ppstPool, int iMax, int iUnit);

/**attach һ���Ѵ��ڵ�mempool�ڴ棬������ڴ��ʽ
*@param[in,out]	 ppstPool  mempoolָ���ָ��
*@param[in]      iMax      mempool������
*@param[in]      iUnit     Ӧ�����ݵĴ�С
*@param[in]      pvBase    �����γ�mempool���ڴ��ָ��
*@param[in]      iSize     pvBase �ڴ�Ĵ�С
		
*@retval  0  �ɹ�
         -1  ʧ��
*/
int mempool_attach(MEMPOOL** ppstPool, int iMax, int iUnit, void* pvBase, unsigned long int iSize);

/**������tmempool_new�������ڴ��*/
int mempool_destroy(MEMPOOL** ppstPool);

/**����mempool��memblock���䷽ʽ
*@param[in]	     pstPool   mempoolָ��
*@param[in]      iMethod   TMEMPOOL_FIFO  �Ƚ��ȳ�,Ĭ�Ϸ�ʽ; TMEMPOOL_LIFO ����ȳ�
		
*@retval  0  �ɹ�
         -1  ʧ��
*/
int mempool_set_method(MEMPOOL* pstPool, int iMethod);

/**����������ȡӦ������
*@param[in]	     pstPool   mempoolָ��
*@param[in]      iIdx      memblock������
		
*@retval  not NULL  �ɹ�
          NULL      ʧ��
*/
void* mempool_get(MEMPOOL* pstPool, int iIdx);

/**����������ȡӦ������
*@param[in]	     pstPool   mempoolָ��
*@param[in]      iPos      memblock ��������0~max-1
		
*@retval  not NULL  �ɹ�
          NULL      ʧ��
*/
void* mempool_get_bypos(MEMPOOL* pstPool, int iPos);


/**����һ��memblock
*@param[in]	     pstPool   mempoolָ��
		
*@retval  >=0     �ɹ�,   ����memblock������
          -1      ʧ��
*/
int mempool_alloc(MEMPOOL* pstPool);

/**�ͷ�һ������ΪiIdx��memblock */
int mempool_free(MEMPOOL* pstPool, int iIdx);

/**�������ɿ�������һ����iStart!=iEndʱʹ��*/
int mempool_fix(MEMPOOL* pstPool);

/**������0��ʼ�ҵ���һ��ʹ���е�memblock
*@param[in]	     pstPool   mempoolָ��
*@param[out]     piPos     �洢memblock index
		
*@retval   0      �ɹ�
          -1      ʧ��
*/
int mempool_find_used_first(MEMPOOL* pstPool, int* piPos);

/**�������һ����������һ��������ʼ�ҵ�һ��ʹ���е�memblock
*@param[in]	     pstPool   mempoolָ��
*@param[in,out]  piPos     
                       in  ���������
                      out  >=0  �ҵ�������
                           -1   �Ѿ�û����һ��ʹ���е�memblock��
                      
*@retval  >=0     �ɹ�
          -1      ʧ��
*/
int mempool_find_used_next(MEMPOOL* pstPool, int* piPos);

#ifdef __cplusplus
}
#endif

#endif /* TMEMPOOL_H */
