#include "UIPIconButton.h"
CIconButton::CIconButton(CControlBase* pParent,
		        const char* name,
				const Rect&    localRect,
				const wstring& selectedImgName,
				const wstring& unselectedImgName,
				 wstring  text,
				bool  bSelected,
				bool  bIsToggleButton,
				bool  bShowText) :CControlBase(pParent,name,localRect),
				                                  m_pSelectedImg(NULL),
												  m_pUnselectedImg(NULL),
												  m_bSelected(bSelected),
												  m_bToggleButton(bIsToggleButton),
												  m_bDrawText(bShowText),
												  m_text(text),
												  m_clrText(Color(255,255,0,0))

   {
	   //7、注册事件处理程序,即是事件源，又是监听者
	   MouseButtonEventDispatcher.RegisterListener(CMouseButtonEventListener(this,&CIconButton::HandleMouseButtonEvent));

	   m_pSelectedImg = App()->GetRender()->FindImage(selectedImgName);
	   m_pUnselectedImg = App()->GetRender()->FindImage(unselectedImgName);

	   if(m_bSelected)
		   m_pCurrentImg = m_pSelectedImg;
	   else
		   m_pCurrentImg = m_pUnselectedImg;
	 
   }

bool CIconButton::OnRender()
	{
		if(!CControlBase::OnRender())
			return false;
        Rect rect = GetWorldRect();
		App()->GetRender()->DrawImage(m_pCurrentImg,rect);

		if(m_bDrawText)
			App()->GetRender()->DrawText(m_text.c_str(),rect,m_clrText);

		return true;
	}

	void CIconButton::Select()
	{
		if(m_pCurrentImg == m_pSelectedImg)
			return;

		m_bSelected = true;
		m_pCurrentImg = m_pSelectedImg;

		if(m_pParent)
		{
			if(m_bToggleButton)
			{
				for(int i = 0; i < m_pParent->GetChildCount(); i++)
				{
					//为了简单，我们限制CIconButton的父亲的所有儿子
					//都是CIconButton类型
					//我们的基类还缺少一个类型码，用于转换
					if(m_pParent->GetChild(i) != this)
					{
						//没办法消除掉向下转型
						((CIconButton*)m_pParent->GetChild(i))->Unselect();
					}
				}
			}
		}
        //5、触发选中事件
		CSelectEvent se(true);
		se.EventSrc = this;
		SelectEventDispatcher.DispatchEvent(se);

		//重绘
		Invalidate();
        
	}

	void CIconButton::Unselect()
	{
		if(m_pCurrentImg == m_pUnselectedImg)
			return;

		m_bSelected = false;

		m_pCurrentImg = m_pUnselectedImg;
		//5、触发Unselect事件
        CSelectEvent se(false);
		se.EventSrc = this;
		//使用重载的函数调用操作符触发事件
		SelectEventDispatcher(se);

		//重绘
		Invalidate();
	}

	void CIconButton::HandleMouseButtonEvent(CMouseButtonEvent& e)
	{
		if(e.ButtonType == LBUTTON && e.ButtonState == STATE_DOWN)
			Select();
		if(e.ButtonType == LBUTTON && e.ButtonState == STATE_UP)
		{
			if(!m_bToggleButton)
				Unselect();
		}
	}

