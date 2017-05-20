#include "UIPPaintArea.h"
bool CPaintArea::OnRender()
	{
		if(!CControlBase::OnRender())
			return false;
     
		IRender2D* pRender = App()->GetRender();
		pRender->SetLineWidth(5.0F);
		Rect rc = GetWorldRect();
		pRender->DrawRect(m_clrFill,rc);
		pRender->DrawRect(m_clrBoard,rc,false);
		pRender->SetLineWidth(1.0F);
        
		//�����ǻ��ƺ�PaintArea�󣬷���Render�¼�
		//���������Խ��ܵ����¼����Ӷ������Լ���Ҫ������
		CRenderEvent e(RENDER_END);
		e.EventSrc = this;

		RenderEventDispatcher.DispatchEvent(e);
		
		return true;
	}