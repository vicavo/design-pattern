#ifndef __CONTROL_BASE_H
#define __CONTROL_BASE_H
#include "UtilHelper.h"
#include "Node.h"
#include "MouseEvent.h"
#include "KeyEvent.h"
#include "NodeIterator.h"

//最常用的深度优先，前序先根，从左到右迭代器
typedef CNodeForwardIterator<CControlBase> CControlIterator;

class CControlManager;
class CControlBase : public CTypeNode<CControlBase>
{
public:
	CControlBase():m_szSize(8,2),m_bVisible(true),m_pCtrlMgr(NULL)
	{
	}

	CControlBase(CControlBase* pParent,const char* name):CTypeNode(pParent,name),m_bVisible(true),m_szSize(8,2),m_pCtrlMgr(NULL)
	{
	}

	CControlBase(CControlBase* pParent,const char* name,const Rect& localRect) : CTypeNode(pParent,name),m_bVisible(true),m_pCtrlMgr(NULL)
	{
		SetLocalRect(localRect);
	}

	bool SetLocalPos(const Point& pt)
	{
		return Resize(pt,m_szSize);
	}

	const Point&  GetLocalPos() const
	{
		return m_ptLocalPos;
	}

	bool  SetLocalRect(const Rect& rc)
	{
		return Resize(Point(rc.X,rc.Y),Size(rc.Width,rc.Height));
	}

	Rect GetLocalRect() const
	{
		return Rect(m_ptLocalPos,m_szSize);
	}

	Rect GetWorldRect()
	{
		return Rect(GetWorldPos(),GetSize());
	}

	Point GetWorldPos();


	const Size& GetSize() const 
	{
		return m_szSize;
	}

	
	bool  SetSize(const Size& size)
	{
		return Resize(m_ptLocalPos,size);
	}

	void SetMinSize(const Size& size)
	{
		m_szMinSize = size;
	}

	const Size& GetMinSzie() const
	{
		return m_szMinSize;
	}

	CControlBase* FindHitCtrl (const Point& pt);
	CControlBase* FindCtrl(const char* name);


	bool IsVisible() const
	{
		return m_bVisible;
	}

	void SetVisible(bool bVisible)
	{
		if(m_bVisible != bVisible)
		{
		   m_bVisible = bVisible;
		   Invalidate();
		}
	}

	
	virtual bool Resize(const Point& pos,const Size& size);

	void Render()
	{
		/*
		//深度优先，前续先根，从左到右递归遍历
		OnBeginRender();
		OnRender();
		OnEndRender();
		for(int i= 0; i < GetChildCount(); i++)
			GetChild(i)->Render();
		*/

        //深度优先，前序先根，从左到右非递归迭代器
		CControlIterator iter(this);
		while(iter.MoveNext())
		{
			iter.GetCurrent().OnBeginRender();
			iter.GetCurrent().OnRender();
			iter.GetCurrent().OnEndRender();
		}
	}

	//模板方法设计模式
	//如果子类重载下面三个渲染函数
	//必须要调用基类的同名方法
	virtual void OnBeginRender();
	
	virtual bool OnRender();

	virtual void OnEndRender();
	

	void Invalidate();

	CControlManager* GetManager();


public:
	//事件
	CMouseButtonEventDispatcher MouseButtonEventDispatcher;
	CMouseMoveEventDispatcher   MouseMoveEventDispatcher;
	CKeyEventDispatcher         KeyEventDispatcher;
	CKeyCharEventDispatcher     KeyCharEventDispatcher;

protected:
	Size  m_szMinSize;
	Size  m_szSize;
	Point m_ptLocalPos;
	bool  m_bVisible;
	CControlManager* m_pCtrlMgr;
	Rect  m_savedRect;
};

#endif