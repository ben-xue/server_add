#ifndef CSHAIRKVDATA_H_
#define CSHAIRKVDATA_H_

#include "comm_inc.h"
#include "CSingleton.h"
#include "CShmObjPool.h"
#include "CShmHash.h"


union XGKvData
{
	bool bVal;
	uint32_t dwVal;
	int iVal;
	uint64_t ullVal;
	char szVal[128];	
};

enum XGKVKeyType
{
    XG_KEY_WORLD_READY_FLAG  = 1,		//global数据是否ready的标志,bool类型
	XG_KEY_LEGION_READY_FLAG = 2,
	XG_KEY_CUR_MAX_LEGION_ID = 3,
    XG_KEY_OSS_MAIL_VERSION  = 4,
	XG_KEY_MAX
};

class CShareKvData : public CSingleton<CShareKvData>
{
public:	
	int Init(int iShmKey, uint64_t ullKeySize);
	XGKvData* GetData(int iKey);

private:
	class KVWrapClass : public CShmBaseObj
	{
	public:
		virtual int Init();
		virtual void Resume();
		virtual void Free();

		XGKvData m_data;
	};
	
	XGKvData* CreateData(int iKey);
	
protected:
	CShmObjPool<KVWrapClass> m_kvPool;	
	CShmHashTable<int, int> m_kvMap;
};


#endif


