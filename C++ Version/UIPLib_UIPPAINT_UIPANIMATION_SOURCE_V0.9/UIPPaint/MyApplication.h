#ifndef __MY_APPLICATION_H
#define __MY_APPLICATION_H
#include "../UIPLib/Application.h"
#include "../UIPLib/ArrayList.h"
#include "../UIPControl/UIPIconButton.h"
#include "../UIPControl/UIPPaintArea.h"
#include "AppKeyEvent.h"
#include "StateManager.h"
#include "Mediator.h"
#include "Visitor.h"
#include "ResponsibleChain.h"
#include "InOutCtrlManager.h"

class MyApp : public CApplication
{
//��������չ�����¼�
public:
	CAppKeyEventDispatcher  AppKeyEventDispatcher;//����һ���¼�������

	
protected:
	//override�����OnKeyEvent����
    void OnKeyEvent(CKeyEvent& keyEvent)
	{
		//�ȷ���ȫ�ּ����¼�
		CAppKeyEvent e(this,keyEvent);
		AppKeyEventDispatcher(e);

		//Ȼ���ٵ��û���ͬ������
		//���෽���ӻ�ý���Ŀؼ����ͼ����¼�
		CApplication::OnKeyEvent(keyEvent);

		//��������������ȫ�ּ����¼�
		//ͬʱ�ַ��;۽��ؼ��ļ����¼�
		//��ô���ǾͿ��Լ���ȫ�ֵļ����¼��ˣ�����undo/redo
	}


public:
	MyApp():m_pMediator(NULL),m_pDrawer(NULL),m_pHelpChain(NULL)
	{}

	~MyApp()
	{
		delete m_pHelpChain;
		delete m_pDrawer;
		delete m_pMediator;
		delete m_pInOutCtrlMgr;
		m_pHelpChain = NULL;
		m_pDrawer    = NULL;
		m_pMediator  = NULL;
		m_pInOutCtrlMgr = NULL;
	}

	CChainBase* GetHelpChain()
	{
		return m_pHelpChain;
	}

protected:
	void InitControlSystem()
	{
		//override������ʱ������Ҫ���û��෽��
		CApplication::InitControlSystem();


        //��ʼ���ؼ�ϵͳ�Լ��м���
		Rect rc = m_pCtrlMgr->GetWorldRect();

		Rect paintRect(95,0,rc.Width-98,rc.Height-3);

		m_pDrawer = new CDrawerVisitor();
        CPaintArea* pPaintArea = new CPaintArea(m_pCtrlMgr,"PaintArea",paintRect);
		m_pInOutCtrlMgr = new CInOutCtrlManager(pPaintArea);
	    m_pMediator = new CFacadeMediator(m_pInOutCtrlMgr,m_pDrawer);

        CPaintArea* pGroup = new CPaintArea(m_pCtrlMgr,"IconGroup",Rect(0,0,90,rc.Height - 3));

		 m_pInOutCtrlMgr->AddIconButton(new CIconButton(pGroup,"DrawCircle",Rect(20,90,50,50),L"pic/Selected.png",L"pic/Circle.png",L"Բ��",false));
         m_pMediator->AddState(m_pMediator->CreateState<CCircleState>());

		 m_pInOutCtrlMgr->AddIconButton(new CIconButton(pGroup,"DrawTriangle",Rect(20,160,50,50),L"pic/Selected.png",L"pic/Triangle.png",L"������",false));
		 m_pMediator->AddState(m_pMediator->CreateState<CTriangleState>());

		m_pInOutCtrlMgr->AddIconButton(new CIconButton(pGroup,"Fill",Rect(20,230,50,50),L"pic/Selected.png",L"pic/Fill.png",L"���",false));
        m_pMediator->AddState(m_pMediator->CreateState<CFillState>());

		m_pInOutCtrlMgr->AddIconButton(new CIconButton(pGroup,"DrawRect",Rect(20,20,50,50),L"pic/Selected.png",L"pic/Rect.png",L"����",true));
		m_pMediator->AddState(m_pMediator->CreateState<CRectState>());
		

		//ע�����Ȥ���¼�
		m_pMediator->RegisterEventListeners();

		
		//��������ϵͳ,HelpChain�����F1keyEvent�¼�
		//����Խ���Ҫ�ṩ�����Ŀؼ���ӵ�HelpChain��ȥ
		//��Щ��Ҫ�ṩ��������Щ����Ҫ�����������ѡ��
		m_pHelpChain = new CControlHelpChain(m_pCtrlMgr,L"���ǿؼ�������");
		m_pHelpChain
			->AddToChain(new CControlHelpChain((CControlBase*)m_pInOutCtrlMgr->GetButton(0),L"���ڻ���Rect��״"))
			->AddToChain(new CControlHelpChain((CControlBase*)m_pInOutCtrlMgr->GetButton(1),L"���ڻ���Բ��"))
			->AddToChain(new CControlHelpChain((CControlBase*)m_pInOutCtrlMgr->GetButton(2),L"���ڻ���������"))
			->AddToChain(new CControlHelpChain((CControlBase*)m_pInOutCtrlMgr->GetButton(3),L"�������ѡ��ͼ��"))
			->AddToChain(new CControlHelpChain(pGroup,L"IconButton�ؼ�������"))
			->AddToChain(new CControlHelpChain(pPaintArea,L"���ڻ���ͼ�ε���ʾ����ؼ�"))
			->AddToChain(new CEndChain());//��β���

		//ע��undo/redo�����¼�
		AppKeyEventDispatcher.RegisterListener(CAppKeyEventListener(m_pMediator,&CFacadeMediator::HandleUndoRedoKeyEvent));
	}


protected:
	CFacadeMediator*       m_pMediator;
    CDrawerVisitor*        m_pDrawer;
	CControlHelpChain*     m_pHelpChain;
	CInOutCtrlManager*     m_pInOutCtrlMgr;
};


#endif