#include "ControlBase.h"
#include "ControlManager.h"
#include "Application.h"
CControlManager* CControlBase::GetManager()
{
	if(!m_pCtrlMgr)
		m_pCtrlMgr =(CControlManager*) GetRoot();

	assert(m_pCtrlMgr);
	return m_pCtrlMgr;

}

void CControlBase::Invalidate()
{
	
	 GetManager()->InvalidateRect(GetWorldRect());
}

Point CControlBase::GetWorldPos()
{
	Point pt;
	CControlBase* pWalk = this;
	while(pWalk)
	{
		pt = pt + pWalk->m_ptLocalPos;
		pWalk = pWalk->GetParent();
	}
	return pt;
}

//只会返回符合深度要求，且包含该点的最后碰到的那个控件
CControlBase* CControlBase::FindHitCtrl (const Point& pt)
{
	if(m_bVisible&&GetWorldRect().Contains(pt))
	{
		//深度优先，先根前序，从右向左递归遍历
		for(int i = GetChildCount()-1; i>=0; i--)
		{
			CControlBase* ret = GetChild(i)->FindHitCtrl(pt);
			if(ret)
				return ret;
		}
		return this;

	}
	return NULL;
}

CControlBase* CControlBase::FindCtrl(const char *name)
{
	return NULL;
}

bool CControlBase::Resize(const Point& pos,const Size& size)
{
	//Resize时最小尺寸检验
	Size newSize;
	newSize.Width  = max(m_szMinSize.Width,size.Width);
	newSize.Height = max(m_szMinSize.Height,size.Height);

	//位置与尺寸没改变，直接退出函数
	bool sizeChanged = !newSize.Equals(m_szSize);
	bool posChanged  = !m_ptLocalPos.Equals(pos);
	if(!sizeChanged && !posChanged)
		return false;

	//记录原来的位置与尺寸
	Rect oldRect = GetLocalRect();

	//设置改变位置和尺寸前的脏区
    Invalidate();

	//位置改变，赋新值
	if(posChanged)
	{
		m_ptLocalPos = pos;

		//应该要触发位置改变的消息
	}

	//尺寸改变，赋新值
	if(sizeChanged)
	{
		m_szSize = newSize;

		//应该要触发大小发生改变的消息
	}
    
	//设置改变位置和尺寸后的脏区
    Invalidate();
	
	return true;
}

void CControlBase::OnBeginRender()
{
	//和脏区求交集
	Rect rc = GetWorldRect();
	Rect::Intersect(rc,rc,App()->GetRender()->GetClipRect());

	//设置Clip区域为刚计算出来的交集
	m_savedRect = App()->GetRender()->GetClipRect();
	App()->GetRender()->SetClipRect(rc);
}

bool CControlBase::OnRender()
{
	//只有控件处于脏区中，才会进行绘制，否则就直接退出
	//这样有非常大的效率提升
	if(!App()->GetRender()->GetClipRect().IntersectsWith(GetWorldRect()))
		return false;

	return true;
}

void CControlBase::OnEndRender()
{
	//恢复为父亲的脏区
	App()->GetRender()->SetClipRect(m_savedRect);

}