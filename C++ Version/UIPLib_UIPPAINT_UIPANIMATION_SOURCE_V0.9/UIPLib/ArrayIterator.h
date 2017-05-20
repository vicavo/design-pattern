#ifndef __ARRAY_LIST_ITERATOR_H
#define __ARRAY_LIST_ITERATOR_H
#include "ArrayList.h"
#include "Iterator.h"
template<typename T,typename Indexer= CGetIndexLR>
class CArrayIterator : public IIterable<T>
{
public:

	CArrayIterator():m_pArrayList(NULL),m_nIdx(-1)
	{
	}

	CArrayIterator(CArrayList<T>* arrList)
	{
		assert(arrList);
		m_pArrayList = arrList;
		Reset();
	}

	bool MoveNext()
	{
		m_nIdx++;
		return m_nIdx < m_pArrayList->GetSize();
	}

	T& GetCurrent()
	{
		return m_pArrayList->GetAt(getIndex(m_pArrayList->GetSize(),m_nIdx));
	}

	void Reset()
	{
		m_nIdx = -1;
	}

	
private:

	CArrayList<T>*  m_pArrayList;
	int             m_nIdx;
    Indexer         getIndex;

public:

	void SetContainer(CArrayList<T>* arrList)
	{
		assert(arrList);
		m_pArrayList = arrList;
		Reset();
	}

	CArrayList<T>* GetContainer()
	{
		return m_pArrayList;
	}

};
#endif