#ifndef CITEMSELECTOR_H_ND68JH0S
#define CITEMSELECTOR_H_ND68JH0S

// use fixed size array
#define MAX_COLLECTION_COUNT 128

#define CUT_FLOOR(arg, val) do{ if((arg) < (val)) (arg) = (val); }while(0)
#define CUT_ROOF(arg, val) do{ if((arg) > (val)) (arg) = (val); }while(0)

template <typename T> 
class CPrimarySelector
{
public:
	CPrimarySelector () { m_size = 0; };
	virtual ~CPrimarySelector () {};

	int AddElement(T item, int iWeight);

	int GetSize() { return m_size; }

	T GetFirst(int idx = 0){
		CUT_FLOOR(idx, 0);
		CUT_ROOF(idx, m_size-1);
		// if (idx < 0) idx = 0;
		// if (idx > m_size) idx = m_size;
		return m_aElement[idx];
	}

	T GetWeight(int idx = 0){
		CUT_FLOOR(idx, 0);
		CUT_ROOF(idx, m_size-1);
		return m_aiWeight[idx];
	}

private:
	// the element is sorted by weight, the larger is in front
	T m_aElement[MAX_COLLECTION_COUNT];
	int m_aiWeight[MAX_COLLECTION_COUNT];
	int m_size;
};

template <class T> 
int CPrimarySelector<T>::AddElement(T item, int iWeight)
{
	if (iWeight < 0)
	{
		return -1;
	}

	int idx = m_size++;

	// find the same item already in the array
	for (int i = 0; i < m_size && i < MAX_COLLECTION_COUNT; ++i)
	{
		if (m_aElement[i] == item)
		{
			idx = i;
			iWeight += m_aiWeight[i];
			m_size--; // found repeated item
			break;
		}
	}

	if (m_size > MAX_COLLECTION_COUNT)
	{
		m_size = MAX_COLLECTION_COUNT;
		idx = m_size-1;

		if (iWeight <= m_aiWeight[idx])
		{
			return -1;
		}
	}

	// shift ahead by weight
	while (idx > 0)
	{
		int preidx = idx-1;
		if (iWeight <= m_aiWeight[preidx])
		{
			break;
		}

		m_aElement[idx] = m_aElement[preidx];
		m_aiWeight[idx] = m_aiWeight[preidx];
		idx = preidx;
	}

	// insert the item
	m_aElement[idx] = item;
	m_aiWeight[idx] = iWeight;

	return idx;
}

#endif /* end of include guard: CITEMSELECTOR_H_ND68JH0S */
