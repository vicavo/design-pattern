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
//我们来扩展键盘事件
public:
	CAppKeyEventDispatcher  AppKeyEventDispatcher;//声明一个事件发送器

	
protected:
	//override基类的OnKeyEvent方法
    void OnKeyEvent(CKeyEvent& keyEvent)
	{
		//先发送全局键盘事件
		CAppKeyEvent e(this,keyEvent);
		AppKeyEventDispatcher(e);

		//然后再调用基类同名方法
		//基类方法从获得焦点的控件发送键盘事件
		CApplication::OnKeyEvent(keyEvent);

		//这样我们增添了全局键盘事件
		//同时又发送聚焦控件的键盘事件
		//那么我们就可以监听全局的键盘事件了，例如undo/redo
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
		//override本函数时，必须要调用基类方法
		CApplication::InitControlSystem();


        //初始化控件系统以及中间者
		Rect rc = m_pCtrlMgr->GetWorldRect();

		Rect paintRect(95,0,rc.Width-98,rc.Height-3);

		m_pDrawer = new CDrawerVisitor();
        CPaintArea* pPaintArea = new CPaintArea(m_pCtrlMgr,"PaintArea",paintRect);
		m_pInOutCtrlMgr = new CInOutCtrlManager(pPaintArea);
	    m_pMediator = new CFacadeMediator(m_pInOutCtrlMgr,m_pDrawer);

        CPaintArea* pGroup = new CPaintArea(m_pCtrlMgr,"IconGroup",Rect(0,0,90,rc.Height - 3));

		 m_pInOutCtrlMgr->AddIconButton(new CIconButton(pGroup,"DrawCircle",Rect(20,90,50,50),L"pic/Selected.png",L"pic/Circle.png",L"圆形",false));
         m_pMediator->AddState(m_pMediator->CreateState<CCircleState>());

		 m_pInOutCtrlMgr->AddIconButton(new CIconButton(pGroup,"DrawTriangle",Rect(20,160,50,50),L"pic/Selected.png",L"pic/Triangle.png",L"三角形",false));
		 m_pMediator->AddState(m_pMediator->CreateState<CTriangleState>());

		m_pInOutCtrlMgr->AddIconButton(new CIconButton(pGroup,"Fill",Rect(20,230,50,50),L"pic/Selected.png",L"pic/Fill.png",L"填充",false));
        m_pMediator->AddState(m_pMediator->CreateState<CFillState>());

		m_pInOutCtrlMgr->AddIconButton(new CIconButton(pGroup,"DrawRect",Rect(20,20,50,50),L"pic/Selected.png",L"pic/Rect.png",L"矩形",true));
		m_pMediator->AddState(m_pMediator->CreateState<CRectState>());
		

		//注册感兴趣的事件
		m_pMediator->RegisterEventListeners();

		
		//创建帮助系统,HelpChain会监听F1keyEvent事件
		//你可以将需要提供帮助的控件添加到HelpChain中去
		//哪些需要提供帮助，哪些不需要，依赖与你的选择
		m_pHelpChain = new CControlHelpChain(m_pCtrlMgr,L"这是控件管理器");
		m_pHelpChain
			->AddToChain(new CControlHelpChain((CControlBase*)m_pInOutCtrlMgr->GetButton(0),L"用于绘制Rect形状"))
			->AddToChain(new CControlHelpChain((CControlBase*)m_pInOutCtrlMgr->GetButton(1),L"用于绘制圆形"))
			->AddToChain(new CControlHelpChain((CControlBase*)m_pInOutCtrlMgr->GetButton(2),L"用于绘制三角形"))
			->AddToChain(new CControlHelpChain((CControlBase*)m_pInOutCtrlMgr->GetButton(3),L"用于填充选中图形"))
			->AddToChain(new CControlHelpChain(pGroup,L"IconButton控件的容器"))
			->AddToChain(new CControlHelpChain(pPaintArea,L"用于绘制图形的显示区域控件"))
			->AddToChain(new CEndChain());//结尾标记

		//注册undo/redo键盘事件
		AppKeyEventDispatcher.RegisterListener(CAppKeyEventListener(m_pMediator,&CFacadeMediator::HandleUndoRedoKeyEvent));
	}


protected:
	CFacadeMediator*       m_pMediator;
    CDrawerVisitor*        m_pDrawer;
	CControlHelpChain*     m_pHelpChain;
	CInOutCtrlManager*     m_pInOutCtrlMgr;
};


#endif