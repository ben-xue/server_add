#ifndef SHTABLE_H
#define SHTABLE_H

/*************************************************************
#FUNCTION:	static hash table defintion.

*************************************************************/
#include <stdio.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define SHT_MIN_HEAD			0x100
#define SHT_BUCKET_ALIGN		0x100
#define SHT_MIN_ALIGN			0x08
#define SHT_VERSION				0x0101

#define SHTF_NEEDFREE			0x01

struct tagSHitem
{
	int iPrev;
	int iNext;
	unsigned uCode;
	int fValid;
	char szData[1];
};

typedef struct tagSHitem		SHITEM;
typedef struct tagSHitem		*LPSHITEM;

struct tagSHbucket
{
	int iCount;
	int iHead;
};

typedef struct tagSHbucket		SHBUCKET;
typedef struct tagSHbucket		*LPSHBUCKET;

typedef int (*SHT_CMP)(const void* pvData1, const void* pvData2);
typedef unsigned int (*SHT_HASHCODE)(const void* pvData);
typedef int (*SHT_PRINT)(FILE* fp, const void* pvData);

struct tagSHtable
{
	int cbSize;		/* the size of this struct. */
	unsigned int uFlags;		/* some flags. */
	int iVersion;				/* version number. */
	unsigned long int iBuff;					/* the size of the buff. */

	int iBucket;				/* bucket number used. */
	int iMax;					/* maximum items can store. */
	int iItem;					/* current item number. */
	int iHeadSize;

	int iBucketOff;
	int iBucketSize;

	int iDataOff;
	unsigned long int iDataSize;
	int iDataUnitMin;			/* the data-unit's real size. */
	int iDataUnitMax;			/* the data-unit's occupy size.*/

	int iFreeHead;
	int iRes;					/* reserved. */
};

typedef struct tagSHtable			SHTABLE;
typedef struct tagSHtable			*LPSHTABLE;

/*�ڴ����*/
#define SHT_ROUND(size)					( ( (size) + SHT_MIN_ALIGN - 1) /SHT_MIN_ALIGN*SHT_MIN_ALIGN )

#define SHT_HEADSIZE()					( SHT_MIN_HEAD < sizeof(SHTABLE) ? sizeof(SHTABLE) : SHT_MIN_HEAD )

#define SHT_BUCKETSIZE(buck)			( (buck) * sizeof(SHBUCKET) )

#define SHT_DATAUNIT(data)				SHT_ROUND((data) + offsetof(SHITEM, szData))

#define SHT_DATASIZE(max, unit)			( (max) * SHT_DATAUNIT(unit) )

#define SHT_SIZE(buck, max, unit)		( SHT_HEADSIZE() + SHT_BUCKETSIZE(buck) + SHT_DATASIZE(max, unit) )

#define SHT_GET_BUCKET(pstTab, i)		( (LPSHBUCKET) ( ((char *)(pstTab)) + pstTab->iBucketOff + i*sizeof(SHBUCKET) ) )

#define SHT_GET_ITEM(pstTab, i)			( (LPSHITEM) ( ((char *)(pstTab)) + pstTab->iDataOff + (unsigned long int)i*pstTab->iDataUnitMax ) )

#define SHT_DATA2ITEM(pvData)			( (SHITEM*) ( ((char *)(pvData)) - offsetof(SHITEM, szData)) )
#define SHT_ITEM2DATA(pvItem)			( (pvItem)->szData )

/**�ṩһ��Ĭ�ϵ��ַ���hash code����
*@param[in]	 szKey  һ����0��β���ַ���
		
*@retval  hash code
*/
unsigned int sht_get_code(const char* szKey);

/**����ڴ��Ƿ����shtable�ĸ�ʽ��һ����   ����attach shtable shm ʱʹ��
*@param[in]	 pvBuff  �ڴ��ַָ��
*@param[in]  iBuff   �ڴ��С
*@param[in]  iBucket  Bucket num
*@param[in]  iMax    Max item num
*@param[in]  iUnit   ÿ��item�Ĵ�С
		
*@retval  0  �ɹ�
         -1  ʧ��
*/
int sht_check(void* pvBuff, unsigned long int iBuff, int iBucket, int iMax, int iUnit);

/**���ڴ�������һ��shtable
*@param[in]      iBucket  Bucket num
*@param[in]      iMax    Max item num
*@param[in]      iUnit   ÿ��item�Ĵ�С
*@param[in,out]  piBuff  ָ�����ɵ�shtable�Ĵ�С��ָ��
		
*@retval  not NULL  �ɹ�shtable ָ��
         NULL  ʧ��

*@note  �˺�����sht_destroy ���ʹ��         
*/
LPSHTABLE sht_create(int iBucket, int iMax, int iUnit, unsigned long int* piBuff);

/**��ʼ��shtable �ڴ�
*@param[in]	 pvBuff  �ڴ��ַָ��
*@param[in]  iBuff   �ڴ��С
*@param[in]  iBucket  Bucket num
*@param[in]  iMax    Max item num
*@param[in]  iUnit   ÿ��item�Ĵ�С
		
*@retval  not NULL  �ɹ�shtable ָ��
         NULL  ʧ��
*/
LPSHTABLE sht_init(void* pvBuff, unsigned long int iBuff, int iBucket, int iMax, int iUnit);

/**attach shtable �ڴ棬ʵ������ִ��sht_check����ڴ��ٷ���shtable ָ��
*@param[in]	 pvBuff  �ڴ��ַָ��
*@param[in]  iBuff   �ڴ��С
*@param[in]  iBucket  Bucket num
*@param[in]  iMax    Max item num
*@param[in]  iUnit   ÿ��item�Ĵ�С
		
*@retval  not NULL  �ɹ�shtable ָ��
         NULL  ʧ��
*/
LPSHTABLE sht_attach(void* pvBuff, unsigned long int iBuff, int iBucket, int iMax, int iUnit);

/**insert unique item into shtable
*@param[in]	 pstTab  shtable ָ��
*@param[in]  pvData  insert item data ָ��
*@param[in]  pfnCmp  �Ƚ�����item data �ĺ���ָ��, �˱ȽϺ�����
                     ����ֵΪ=0 ��item data��ͬ, <0 ��>0 ��item data����ͬ
*@param[in]  pfnHashCode  ����item data ����hash code �ĺ���ָ��
		
*@retval  not NULL  �ɹ���shtable �ڴ��data�ĵ�ַָ��
          NULL      ʧ��
*@note    �˺������óɹ���ֻ����shtable�ڷ�����һ���ڴ棬��Ҫ�ⲿ
          ��Ҫ�����������copy��shtable�У��˷�ʽ��Ϊ����������ԣ�
          �ɲμ��ĵ��еĴ���ʾ��
*/
void* sht_insert_unique(LPSHTABLE pstTab, const void* pvData, SHT_CMP pfnCmp, SHT_HASHCODE pfnHashCode); /* insert a item into hashtable, keep it unique.*/

/** insert a item into hashtable, do not keep unique.*/
void* sht_insert_multi(LPSHTABLE pstTab, const void* pvData, SHT_HASHCODE pfnHashCode); 

/**find item in shtable
*@param[in]	 pstTab  shtable ָ��
*@param[in]  pvData  find item data ָ��
*@param[in]  pfnCmp  �Ƚ�����item data �ĺ���ָ��, �˱ȽϺ�����
                     ����ֵΪ=0 ��item data��ͬ, <0 ��>0 ��item data����ͬ
*@param[in]  pfnHashCode  ����item data ����hash code �ĺ���ָ��
		
*@retval  not NULL  �ɹ���shtable �ڴ��data�ĵ�ַָ��
          NULL      ʧ��

*@note   pvDataֻ��Ҫ������pfnCmp��pfnHashCode ��صĲ��ּ���          
*/
void* sht_find(LPSHTABLE pstTab, const void* pvData, SHT_CMP pfnCmp, SHT_HASHCODE pfnHashCode);

/**remove item in shtable
*@param[in]	 pstTab  shtable ָ��
*@param[in]  pvData  remove item data ָ��
*@param[in]  pfnCmp  �Ƚ�����item data �ĺ���ָ��, �˱ȽϺ�����
                     ����ֵΪ=0 ��item data��ͬ, <0 ��>0 ��item data����ͬ
*@param[in]  pfnHashCode  ����item data ����hash code �ĺ���ָ��
		
*@retval  not NULL  �ɹ���shtable �ڴ��data�ĵ�ַָ��
          NULL      ʧ��

*@note   pvDataֻ��Ҫ������pfnCmp��pfnHashCode ��صĲ��ּ���          
*/
void* sht_remove(LPSHTABLE pstTab, const void* pvData, SHT_CMP pfnCmp, SHT_HASHCODE pfnHashCode);

/**remove item in shtable by pos
*@param[in]	 pstTab  shtable ָ��
*@param[in]  iPos    remove item data ��shtable ��item list �е�����λ��

*@retval  not NULL  �ɹ���shtable �ڴ��data�ĵ�ַָ��
          NULL      ʧ��

*@note   item list ��һ�����飬������>=0 , <pstTab->iMax          
*/
void* sht_remove_by_pos(LPSHTABLE pstTab, int iPos);

/**remove item in shtable by addr
*@param[in]	 pstTab  shtable ָ��
*@param[in]  pstItem  item ��shtable�еĵ�ַָ��

*@retval  not NULL  �ɹ���shtable �ڴ��data�ĵ�ַָ��
          NULL      ʧ��
       
*/
void* sht_remove_by_addr(LPSHTABLE pstTab, LPSHITEM pstItem);


/**get item in shtable by iPos
*@param[in]	 pstTab  shtable ָ��
*@param[in]  iPos    item data ��shtable ��item list �е�����λ��
*@param[in,out] pfValid  ��ʶ��item data �Ƿ���Ч, *pfValid = 1 ��Ч��=0 ��Ч

*@retval  not NULL  �ɹ���shtable �ڴ��data�ĵ�ַָ��
          NULL      ʧ��
          
*@note   item list ��һ�����飬������>=0 , <pstTab->iMax          
  
*/
void* sht_pos(LPSHTABLE pstTab, int iPos, int* pfValid);	

/**��sht_create���ʹ�ã��ͷ���Դ*/
int sht_destroy(LPSHTABLE* ppstTab);

/**�ؽ�item ���������bucket ��������ʹ��*/
int sht_rebuild(LPSHTABLE pstTab);

/**�ж�shtable item data�ռ��Ƿ�Ϊ��, ����1 Ϊ��, 0 ����*/
int sht_is_empty(LPSHTABLE pstTab);

/**�ж�shtable item data�ռ��Ƿ�����, ����1 ����, 0 δ��*/
int sht_is_full(LPSHTABLE pstTab);

/**����shtable��item list �е��������ÿһ��valid item data ����pfnPrint(fp, pvData)*/
int sht_dump_all(LPSHTABLE pstTab, FILE* fp, SHT_PRINT pfnPrint);

/**����shtable��item list �е�����valid item���ÿһ��item data ����pfnPrint(fp, pvData)*/
int sht_dump_valid(LPSHTABLE pstTab, FILE* fp, SHT_PRINT pfnPrint);

/**��֪item hashֵǿ�Ʋ��룬���������ʹ�ã�by kent 2008.1.18*/
void *sht_insert_force(LPSHTABLE pstTab, unsigned int uCode);

#ifdef __cplusplus
}
#endif

#endif /* SHTABLE_H */
