#ifndef __UIP_PAINT_AREA_H
#define __UIP_PAINT_AREA_H
#include "../UIPLib/Application.h"
#include "../UIPLib/ControlBase.h"
#include "../UIPLib/IRender2D.h"
#include "../UIPLib/EventSystem.h"
#include "../UIPLib/RenderEvent.h"
class CPaintArea : public CControlBase
{
public:
	CPaintArea(CControlBase* pParent,
		        const char* name,
				const Rect&    localRect) : CControlBase(pParent,name,localRect),
				                            m_clrBoard(Color(255,255,0,0)),
											m_clrFill(Color(255,0,0,255))
	{
	}	

	void SetBoardColor(const Color& clr)
	{
		m_clrBoard = clr;
		Invalidate();
	}

	const Color& GetBoardColor() const
	{
		return m_clrBoard;
	}

	void SetFillColor(const Color& clr)
	{
		m_clrFill = clr;
		Invalidate();
	}

	const Color& GetFillColor() const
	{
		return m_clrFill;
	}

protected:
	bool OnRender();
	
protected:
    Color  m_clrBoard;
	Color  m_clrFill;

public:
	//Ϊ��������ܹ�������ͼ�λ�����PaintArea�ؼ���
	//���Ƕ���һ��RenderEvent������
	CRenderEventDispatcher  RenderEventDispatcher;
};
#endif