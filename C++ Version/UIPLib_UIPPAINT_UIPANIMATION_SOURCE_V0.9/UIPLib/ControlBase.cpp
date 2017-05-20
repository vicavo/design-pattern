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

//ֻ�᷵�ط������Ҫ���Ұ����õ������������Ǹ��ؼ�
CControlBase* CControlBase::FindHitCtrl (const Point& pt)
{
	if(m_bVisible&&GetWorldRect().Contains(pt))
	{
		//������ȣ��ȸ�ǰ�򣬴�������ݹ����
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
	//Resizeʱ��С�ߴ����
	Size newSize;
	newSize.Width  = max(m_szMinSize.Width,size.Width);
	newSize.Height = max(m_szMinSize.Height,size.Height);

	//λ����ߴ�û�ı䣬ֱ���˳�����
	bool sizeChanged = !newSize.Equals(m_szSize);
	bool posChanged  = !m_ptLocalPos.Equals(pos);
	if(!sizeChanged && !posChanged)
		return false;

	//��¼ԭ����λ����ߴ�
	Rect oldRect = GetLocalRect();

	//���øı�λ�úͳߴ�ǰ������
    Invalidate();

	//λ�øı䣬����ֵ
	if(posChanged)
	{
		m_ptLocalPos = pos;

		//Ӧ��Ҫ����λ�øı����Ϣ
	}

	//�ߴ�ı䣬����ֵ
	if(sizeChanged)
	{
		m_szSize = newSize;

		//Ӧ��Ҫ������С�����ı����Ϣ
	}
    
	//���øı�λ�úͳߴ�������
    Invalidate();
	
	return true;
}

void CControlBase::OnBeginRender()
{
	//�������󽻼�
	Rect rc = GetWorldRect();
	Rect::Intersect(rc,rc,App()->GetRender()->GetClipRect());

	//����Clip����Ϊ�ռ�������Ľ���
	m_savedRect = App()->GetRender()->GetClipRect();
	App()->GetRender()->SetClipRect(rc);
}

bool CControlBase::OnRender()
{
	//ֻ�пؼ����������У��Ż���л��ƣ������ֱ���˳�
	//�����зǳ����Ч������
	if(!App()->GetRender()->GetClipRect().IntersectsWith(GetWorldRect()))
		return false;

	return true;
}

void CControlBase::OnEndRender()
{
	//�ָ�Ϊ���׵�����
	App()->GetRender()->SetClipRect(m_savedRect);

}