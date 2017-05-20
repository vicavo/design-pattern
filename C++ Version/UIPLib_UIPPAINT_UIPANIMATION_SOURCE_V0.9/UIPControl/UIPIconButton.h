#ifndef __UIP_ICON_BUTTON_H
#define __UIP_ICON_BUTTON_H
#include "../UIPLib/Application.h"
#include "../UIPLib/ControlBase.h"
#include "../UIPLib/IRender2D.h"
#include "../UIPLib/EventSystem.h"

//1、定义一个事件
struct CSelectEvent: public CEvent<CControlBase*>
{
	bool IsSelected;
    
	CSelectEvent(bool bSelected):CEvent(NULL,"SelectEvent"),IsSelected(bSelected)
	{
	} 
};

//2、typedef 事件发送者
typedef CEventDispatcher<CSelectEvent>   CSelectEventDispatcher;

//3、typedef 事件监听者
typedef CEventListener<void(CSelectEvent&)> CSelectEventListener;



class CIconButton : public CControlBase
{
public:
	CIconButton(CControlBase* pParent,
		        const char* name,
				const Rect&    localRect,
				const wstring& selectedImgName,
				const wstring& unselectedImgName,
				 wstring  text = L"IConButton",
				bool  bSelected = true,
				bool  bIsToggleButton = true,
				bool  bShowText = true);

   

protected:
	bool OnRender();
	
	void Select();
	
	void Unselect();
	

public:
	//4、声明一个事件发送者
	CSelectEventDispatcher  SelectEventDispatcher;

protected:

	//6、定义事件处理函数
	void HandleMouseButtonEvent(CMouseButtonEvent& e);
	

protected:
   IRenderImage* m_pSelectedImg;
   IRenderImage* m_pUnselectedImg;
   IRenderImage* m_pCurrentImg;
   
   bool          m_bSelected;
   bool          m_bToggleButton;

   bool          m_bDrawText;
   wstring       m_text;
   Color         m_clrText;

public:

	void SetDrawText(bool bDraw)
	{
		m_bDrawText = bDraw;
	}

	const bool IsDrawText() const
	{
		return m_bDrawText;
	}

	void SetText(const wstring& text)
	{
		m_text = text;
	}

	const wstring& GetText() const
	{
		return m_text;
	}

	void SetTextColor(const Color& clr)
	{
		m_clrText = clr;
	}

	const Color& GetTextColor() const
	{
		return m_clrText;
	}

};
#endif