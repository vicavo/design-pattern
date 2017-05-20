#ifndef __PROXY_COUNT_REF
#define __PROXY_COUNT_REF
#include "../UIPLib/Base.h"
//代理模式的一个经典实现：侵入式智能指针
class CRefCounter
{
public:
	virtual ~CRefCounter ()
	{
		assert(m_iRefCount == 0);
	}

protected:
	CRefCounter ()
	{
		m_iRefCount = 0;
	}

public:

	void AddRef ()
	{
		 m_iRefCount++;
	}

	void Release ()
	{
		if (--m_iRefCount == 0)
		{
			delete this;
		}
	}

	int GetRefCount () const
	{
		 return m_iRefCount;
	}


private:

	int m_iRefCount;
};

template <class T>
class CSmartPointer
{
public:

    CSmartPointer (T* pRef = 0);
    CSmartPointer (const CSmartPointer& rkPointer);
    ~CSmartPointer ();

    
    operator T* () const;
    T& operator* () const;
    T* operator-> () const;

 
    CSmartPointer& operator= (T* pRef);
    CSmartPointer& operator= (const CSmartPointer& rkReference);

 
    bool operator== (T* pRef) const;
    bool operator!= (T* pRef) const;
    bool operator== (const CSmartPointer& rkReference) const;
    bool operator!= (const CSmartPointer& rkReference) const;

protected:
    T* m_pRef;
};

template <class T>
CSmartPointer<T>::CSmartPointer (T* pRef)
{
    m_pRef = pRef;
    if (m_pRef)
    {
        m_pRef->AddRef();
    }
}

template <class T>
CSmartPointer<T>::CSmartPointer (const CSmartPointer& rkPointer)
{
    m_pRef = rkPointer.m_pRef;
    if (m_pRef)
    {
        m_pRef->AddRef();
    }
}

template <class T>
CSmartPointer<T>::~CSmartPointer ()
{
    if (m_pRef)
    {
        m_pRef->Release();
    }
}

template <class T>
CSmartPointer<T>::operator T* () const
{
    return m_pRef;
}

template <class T>
T& CSmartPointer<T>::operator* () const
{
    return *m_pRef;
}

template <class T>
T* CSmartPointer<T>::operator-> () const
{
    return m_pRef;
}

template <class T>
CSmartPointer<T>& CSmartPointer<T>::operator= (T* pRef)
{
    if (m_pRef != pRef)
    {
        if (pRef)
        {
            pRef->AddRef();
        }

        if (m_pRef)
        {
            m_pRef->Release();
        }

        m_pRef = pRef;
    }
    return *this;
}

template <class T>
CSmartPointer<T>& CSmartPointer<T>::operator= (const CSmartPointer& rkPointer)
{
    if (m_pRef != rkPointer.m_pRef)
    {
        if (rkPointer.m_pRef)
        {
            rkPointer.m_pRef->AddRef();
        }

        if (m_pRef)
        {
            m_pRef->Release();
        }

        m_pRef = rkPointer.m_pRef;
    }
    return *this;
}

template <class T>
bool CSmartPointer<T>::operator== (T* pRef) const
{
    return m_pRef == pRef;
}

template <class T>
bool CSmartPointer<T>::operator!= (T* pRef) const
{
    return m_pRef != pRef;
}

template <class T>
bool CSmartPointer<T>::operator== (const CSmartPointer& rkPointer) const
{
    return m_pRef == rkPointer.m_pRef;
}

template <class T>
bool CSmartPointer<T>::operator!= (const CSmartPointer& rkPointer) const
{
    return m_pRef != rkPointer.m_pRef;
}
#endif