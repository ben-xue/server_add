#ifndef _ARRAY_OPER_H_
#define _ARRAY_OPER_H_

#include <vector>

template<typename T, typename NumT>
void ArrayAdd(T* pT, NumT & iNum, int iMaxNum, const T & elem)
{
	if (!pT)
	{
		return;
	}
	if (iNum >= iMaxNum)
	{
		return;
	}

	pT[iNum] = elem;
	iNum++;

}

template<typename T, typename NumT>
void ArrayInsert(T* pT, NumT & iNum, int iMaxNum, const T & elem, int iDstPos)
{
	if (!pT)
	{
		return;
	}
	if (iNum >= iMaxNum)
	{
		return;
	}

	if (iDstPos > iNum)
	{
		iDstPos = iNum;
	}

	if (iNum >= iDstPos + 1)
	{
		memmove(pT + iDstPos + 1, pT + iDstPos, sizeof(T)* (iNum - iDstPos - 1));
	}

	pT[iDstPos] = elem;
	iNum++;
}



//从数组中删除pToDelete指向的元素
template<typename T, typename N>
int ArrayDelete(T* pT, N& iNum, T* pToDelete)
{
	static int iErrNo = -1;
	if (!pT)
	{
		return iErrNo--;
	}

	if (!pToDelete)
	{
		return iErrNo--;
	}

	if (0 >= iNum)
	{
		return iErrNo--;
	}

	if (pToDelete < pT)
	{
		return iErrNo--;
	}

	int iPos = (pToDelete - pT);
	if (iPos >= iNum)
	{
		return iErrNo--;
	}

	if (iPos == iNum - 1)
	{
		//no need move data
	}
	else
	{
		//need move data
		memmove(pToDelete, pToDelete + 1, sizeof(T)* (pT + iNum - pToDelete - 1));

	}

	iNum--;

	return 0;
}

//从平坦数组中删除满足断言的元素
template<typename T, typename N>
void ArrayDelete(T * pT, N& iNum, bool(*ArrayAssertFp) (T*))
{
	if (!pT)
	{
		return;
	}

	if (!ArrayAssertFp)
	{
		return;
	}

	int iCurIndex = 0;
	T* pElem = NULL;
	while (iCurIndex < iNum)
	{
		pElem = pT + iCurIndex;
		if (ArrayAssertFp(pElem))
		{
			//需要删除数组元素
			T* pEnd = pT + iNum;
			if ((pEnd - (pElem + 1)) > 0)
			{
				memmove(pElem, pElem + 1, (pEnd - (pElem + 1)) * sizeof(T));
			}
			iNum--;
		}
		else
		{
			iCurIndex++;
		}

	}

}

template<typename Obj, typename T, typename N>
void ArrayDelete(T * pT, N& iNum, Obj * pObj, bool (Obj::*ArrayAssertFp) (T*))
{
	if (!pT)
	{
		return;
	}
	if (!pObj)
	{
		return;
	}
	if (!ArrayAssertFp)
	{
		return;
	}



	int iCurIndex = 0;
	T* pElem = NULL;
	while (iCurIndex < iNum)
	{
		pElem = pT + iCurIndex;
		if ((pObj->*ArrayAssertFp)(pElem))
		{
			//需要删除数组元素
			T* pEnd = pT + iNum;
			if ((pEnd - (pElem + 1)) > 0)
			{
				memmove(pElem, pElem + 1, (pEnd - (pElem + 1)) * sizeof(T));
			}
			iNum--;
		}
		else
		{
			iCurIndex++;
		}

	}
}

//删除一个vector中满足pFunc的元素
template<typename T>
void VectorDelete(std::vector<T>& v, bool(*pFunc) (const T&))
{
	if (!pFunc)
	{
		return;
	}

	for (auto it = v.begin(); it != v.end();)
	{
		if (pFunc(*it))
		{
			it = v.erase(it);
		}
		else
		{
			it++;
		}
	}
}

#endif