/* 
*  带排名状态的共享对象内存池
*/

#ifndef CSHMSORTSTATSARRAY_H__
#define CSHMSORTSTATSARRAY_H__

#include "CShmSortArray.h"

//T必须拥有RANKINFO类型命名为RankInfoData的成员
template<class KeyT, class T>
class CShmSortStatsArray : public CShmSortArray<KeyT, T>
{
public:
	CShmSortStatsArray(){};
	virtual ~CShmSortStatsArray(){};

	int Update(T* t);
};

template <class KeyT, class T>
int CShmSortStatsArray<KeyT, T>::Update(T* t)
{
	ASSERT_RET(t, -1);

 	KeyT key = GetKey(t);
 	if (CShmSortArray<KeyT, T>::m_hashCheck.Find(key) != NULL)
	{
		//这里必须用旧的排行榜数据
		T* pT = NULL;
		int* piIndex = GetArrayIndex(t);
		if (piIndex)
		{
			int i = *piIndex;
			pT = CShmSortArray<KeyT, T>::m_array.Get(i);
		}

		if (pT)
		{
			t->RankInfoData = pT->RankInfoData;
		}		

		DoRemove(t);
		DoInsert(t,-1,false);
 	}
	else
 	{		
		int iRet = DoInsert(t,-1,false);
		ASSERT_RET(0 == iRet, -1);	
 	}

	return 0;
}


#endif //CSHMSORTSTATSARRAY_H__


