#ifndef __EVENT_LISTENER_H
#define __EVENT_LISTENER_H
#include "Base.h"
//实现一个范型观察者模式
//封装FastDelegate库并实现多播系统。

//为了命名一致性，我们宏定义
#define CEventListener FastDelegate

template<typename T>
struct CListenerLink
{
	CListenerLink<T> *m_pNext,*m_pPrev;//双链表实现，前驱或后驱指针

	float m_fOrder;//链接顺序，意味着回调函数触发的顺序

	//监听的回调函数原型void (T&) 
	//实际上模板参数T可以是任意类型。
	//但是在UIPLib事件系统中T人为限定为CEvent或其子类
	CEventListener<void(T&)> m_listener;

	CListenerLink(){}

	CListenerLink(const CEventListener<void(T&)>& dlg) : m_listener(dlg) {}

	void Link(CListenerLink<T>* node,float m_fOrder)
	{
		CListenerLink<T> * walk = m_pNext;
		while (m_fOrder >= walk->m_fOrder && walk->m_pNext != this)
			walk = walk->m_pNext;

		if (m_fOrder >= walk->m_fOrder)
		{
			// 链接到walk后面
			node->m_pPrev = walk;
			node->m_pNext = walk->m_pNext;
			walk->m_pNext->m_pPrev = node;
			walk->m_pNext = node;
		}
		else
		{
			// 链接到walk前面
			node->m_pPrev = walk->m_pPrev;
			node->m_pNext = walk;
			walk->m_pPrev->m_pNext = node;
			walk->m_pPrev = node;
		}
		node->m_fOrder = m_fOrder;
	}

	void Unlink()
	{
		m_pNext->m_pPrev = m_pPrev;
		m_pPrev->m_pNext = m_pNext;
	}
};

//支持多播以及触发顺序调整
template<typename T>
class CEventDispatcher
{
public:
	CEventDispatcher()
	{
		m_list.m_pNext = m_list.m_pPrev = &m_list;
		m_list.m_fOrder = 0.5f;
	}

    ~CEventDispatcher()
	{
		while (m_list.m_pNext != &m_list)
		{
			CListenerLink<T>* ptr = m_list.m_pNext;
			ptr->Unlink();
			delete ptr;
			ptr = NULL;
		}
	}

	bool IsEmpty() const{ return m_list.m_pNext == &m_list; }

	bool Contains(const  CEventListener<void(T&)>& dlg,float m_fOrder) const
	{
		for(CListenerLink<T> *ptr = m_list.m_pNext;ptr != &m_list;ptr = ptr->m_pNext)
		{
			if(ptr->m_listener == dlg &&  ptr->m_fOrder == m_fOrder)
				return true;
		}
		return false;
	}

	bool  RegisterListener(const  CEventListener<void(T&)>& dlg, float order = 0.5F)
	{
		if(Contains(dlg,order))
			return false;

	    m_list.Link(new CListenerLink<T>(dlg),order);
		return true;
	}

	bool UnregisterListener(const  CEventListener<void(T&)>& dlg,float order)
	{
		for(CListenerLink<T> *ptr = m_list.m_pNext;ptr != &m_list;ptr = ptr->m_pNext)
		{
			if(ptr->m_listener == dlg && ptr->m_fOrder == order)
			{
				ptr->Unlink();
				delete ptr;
				ptr = NULL;
				return true;
			}
		}

		return false;
	}

	void DispatchEvent(T& e)
	{
		for(CListenerLink<T> *ptr = this->m_list.m_pNext;ptr != &this->m_list;ptr = ptr->m_pNext)
		{
			this->GetDelegate(ptr)(e);
		}
	}

	//重载函数调用操作符
	void operator () (T& e)
	{
		DispatchEvent(e);
	}

private:
	CListenerLink<T> m_list;
	 CEventListener<void(T&)>& GetDelegate(CListenerLink<T> * link)
	{
		return link->m_listener;
	}
};

template<typename TPointer>
struct CEvent
{
	TPointer    EventSrc;//事件源指针(由谁触发了本事件)
	const char* EventName;//事件名称

	CEvent():EventSrc(NULL),EventName(NULL)
	{
	}

	CEvent(TPointer src,const char* name)
	{
		EventSrc = src;
		EventName = name;
	}

	virtual ~CEvent()
	{
		EventSrc = NULL;
		EventName = NULL;
	}
};


#endif
