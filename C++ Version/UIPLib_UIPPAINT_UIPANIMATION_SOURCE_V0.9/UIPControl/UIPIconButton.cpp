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
	   //7��ע���¼��������,�����¼�Դ�����Ǽ�����
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
					//Ϊ�˼򵥣���������CIconButton�ĸ��׵����ж���
					//����CIconButton����
					//���ǵĻ��໹ȱ��һ�������룬����ת��
					if(m_pParent->GetChild(i) != this)
					{
						//û�취����������ת��
						((CIconButton*)m_pParent->GetChild(i))->Unselect();
					}
				}
			}
		}
        //5������ѡ���¼�
		CSelectEvent se(true);
		se.EventSrc = this;
		SelectEventDispatcher.DispatchEvent(se);

		//�ػ�
		Invalidate();
        
	}

	void CIconButton::Unselect()
	{
		if(m_pCurrentImg == m_pUnselectedImg)
			return;

		m_bSelected = false;

		m_pCurrentImg = m_pUnselectedImg;
		//5������Unselect�¼�
        CSelectEvent se(false);
		se.EventSrc = this;
		//ʹ�����صĺ������ò����������¼�
		SelectEventDispatcher(se);

		//�ػ�
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

