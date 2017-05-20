#ifndef __OBSERVER_H
#define __OBSERVER_H
#include "../UIPLib/Base.h"
#include "../UIPLib/ArrayList.h"
#include "../UIPLib/ArrayIterator.h"
struct IObserver
{
	virtual void Update() = 0;
};

struct ISubject
{
	virtual void RegisterObserver(IObserver* observer) = 0;
	virtual void RemoveObserver(IObserver* observer) = 0;
	virtual void Notify() = 0;
};



struct CChangeText : public ISubject
{
	void RegisterObserver(IObserver* observer)
	{
		if(m_arrObservers.IndexOf(observer) != -1)
			return;

		m_arrObservers.Add(observer);
	}

	void RemoveObserver(IObserver* observer)
	{
		int idx = m_arrObservers.IndexOf(observer);
		if(idx == -1)
			return;

		m_arrObservers.Remove(idx);
	}

	void Notify()
	{
		CArrayIterator<IObserver*> iter(&m_arrObservers);
		while(iter.MoveNext())
			iter.GetCurrent()->Update();
	}

	const string& GetText()
	{
		return m_strText;
	}

	void SetText(const string& str)
	{
		if(str != m_strText)
		{
			m_strText = str;
			Notify();
		}
	}

protected:
	CArrayList<IObserver*> m_arrObservers;
	string                 m_strText;
};

struct CTextChangeHandler : public IObserver
{
	void Update()
	{
		printf("文字发生改变了\n");
	}
};

void TestObserver()
{
	shared_ptr<IObserver> pChangeHandler(new CTextChangeHandler);

    CChangeText changeText;
	changeText.RegisterObserver(pChangeHandler.get());
	changeText.SetText("I love you");
    changeText.SetText("I hate you");

}

/*
//空接口，用于类型标记
struct IEventListener
{
};

class AdjustmentEvent;
struct IAdjustmentListener : public IEventListener
{
     virtual void AdjustmentValueChanged(AdjustmentEvent& e) = 0
};

struct IAdjustable
{
	virtual void AddAdjustmentListener      (IAdjustmentListener* l) = 0;
    virtual void RemoveAdjustmentListener   (IAdjustmentListener* l) = 0;
};
*/

#endif