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
        
		//当我们绘制好PaintArea后，发送Render事件
		//这样外界可以接受到该事件，从而绘制自己想要的内容
		CRenderEvent e(RENDER_END);
		e.EventSrc = this;

		RenderEventDispatcher.DispatchEvent(e);
		
		return true;
	}