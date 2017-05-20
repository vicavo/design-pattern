#include "Mediator.h"
#include "StateManager.h"

CFacadeMediator::CFacadeMediator(CInOutCtrlManager* pCtrlMgr,CDrawerVisitor* pVisitor):m_pCtrlMgr(pCtrlMgr),m_pDrawerVistor(pVisitor)
{
	assert(pCtrlMgr);
	m_pStateMgr = new CStateManager(this);
	m_pShapeMgr = new CShapeManager(this);
	m_pUndoMgr  = new CUndoManager ();
}

CFacadeMediator::~CFacadeMediator()
{
	delete m_pStateMgr;
	m_pStateMgr = NULL;

	delete m_pShapeMgr;
	m_pShapeMgr = NULL;

	delete m_pUndoMgr;
	m_pUndoMgr  = NULL;
}

int CFacadeMediator::GetStateCount() const
{
	assert(m_pStateMgr);
	return m_pStateMgr->GetStateCount();
}

int CFacadeMediator::AddState(CStateBase* pState)
{
	assert(m_pStateMgr);
	return m_pStateMgr->AddState(pState);
}

bool CFacadeMediator::RemoveState(int idx)
{
	assert(m_pStateMgr);
	return m_pStateMgr->RemoveState(idx);
}

int CFacadeMediator::GetStateIndex(CStateBase* pState)
{
	assert(m_pStateMgr);
	return m_pStateMgr->GetStateIndex(pState);
}

CStateBase* CFacadeMediator::GetState(int idx)
{
	assert(m_pStateMgr);
	return m_pStateMgr->GetState(idx);
}

int CFacadeMediator::GetShapeCount() const
{
	assert(m_pShapeMgr);
	return m_pShapeMgr->GetShapeCount();
}

int CFacadeMediator::AddShape(CShapeBase* pShape)
{
	assert(m_pShapeMgr);
	return m_pShapeMgr->AddShape(pShape);
}

bool CFacadeMediator::RemoveShape(int idx)
{
	assert(m_pShapeMgr);
	return m_pShapeMgr->RemoveShape(idx);
}

int CFacadeMediator::GetShapeIndex(CShapeBase* pShape)
{
	assert(m_pShapeMgr);
	return m_pShapeMgr->GetShapeIndex(pShape);
}

CShapeBase* CFacadeMediator::GetShape(int idx)
{
	assert(m_pShapeMgr);
	return m_pShapeMgr->GetShape(idx);
}

CShapeBase* CFacadeMediator::FindShape(const Point& pt)
{
	assert(m_pShapeMgr);
	return m_pShapeMgr->FindShape(pt);
}

void CFacadeMediator::DrawShapes()
{
	assert(m_pShapeMgr);
	for(int i = 0; i < m_pShapeMgr->GetShapeCount(); i++)
	{
		m_pShapeMgr->GetShape(i)->VisitDrawer(m_pDrawerVistor);
	}
}

void CFacadeMediator::SetMaxCmdCount(int nMax)
{
	assert(m_pUndoMgr);
	m_pUndoMgr->SetMaxCount(nMax);
}

int CFacadeMediator::GetMaxCmdCount() const
{
	assert(m_pUndoMgr);
	return m_pUndoMgr->GetMaxCount();
}

bool CFacadeMediator::CanUndo() const
{
	assert(m_pUndoMgr);
	return m_pUndoMgr->CanUndo();
}

bool CFacadeMediator::CanRedo() const
{
	assert(m_pUndoMgr);
	return m_pUndoMgr->CanRedo();
}

void CFacadeMediator::PushCommand(ICommandPtr pCmd,bool bExecute)
{
	assert(m_pUndoMgr);
    m_pUndoMgr->PushCommand(pCmd,bExecute);
}

bool CFacadeMediator::Undo()
{
	assert(m_pUndoMgr);
	return m_pUndoMgr->Undo();
}

bool CFacadeMediator::Redo()
{
	assert(m_pUndoMgr);
	return m_pUndoMgr->Redo();
}

void CFacadeMediator::HandleIconButtonsSelectEvent(CSelectEvent& e)
{
	assert(m_pCtrlMgr->GetButtonCount() == m_pStateMgr->GetStateCount());
	for(int i = 0; i < m_pCtrlMgr->GetButtonCount(); i++)
	{
		if(e.EventSrc == m_pCtrlMgr->GetButton(i))
		{
			m_pStateMgr->SetCurrentState(m_pStateMgr->GetState(i));
			return;
		}
	}
}

void CFacadeMediator::HandlePaintAreaMouseButtonEvent(CMouseButtonEvent& e)
{
	if(e.EventSrc == m_pCtrlMgr->GetPaintArea())
	{
		if(e.ButtonType == LBUTTON && e.ButtonState == STATE_DOWN)
		{
			m_pStateMgr->MouseDown(e.MousePt);
			e.EventSrc->Invalidate();
		}
		//下面代码要去除掉，现在存在是为了测试undo/redo以及鼠标中键和右键功能
		else if(e.ButtonType == MBUTTON && e.ButtonState == STATE_DOWN)
		{
			m_pUndoMgr->Undo();
			e.EventSrc->Invalidate();
			
		}
		else if(e.ButtonType == RBUTTON && e.ButtonState == STATE_DOWN)
		{
			m_pUndoMgr->Redo();
			e.EventSrc->Invalidate();
		}
	}
}

void CFacadeMediator::HandleUndoRedoKeyEvent(CAppKeyEvent& e)
{

	if(CUtilHelper::IsCtrlDown())
	{
		if(e.KeyCode == 90 && e.IsKeyDown)
		{
            m_pUndoMgr->Undo();
			e.EventSrc->Invalidate();	
		}
		else if(e.KeyCode == 89 && e.IsKeyDown)
		{
			m_pUndoMgr->Redo();
			e.EventSrc->Invalidate();
		}
	}
}



void CFacadeMediator::HandlePaintEvent(CRenderEvent& e)
{
	if(e.EventSrc == m_pCtrlMgr->GetPaintArea())
	{
		if(e.RenderTime == RENDER_END)
		{
			DrawShapes();
		}
	}
}

void CFacadeMediator::RegisterEventListeners()
{
	if(m_pCtrlMgr && m_pCtrlMgr->GetPaintArea())
	{
		m_pCtrlMgr->GetPaintArea()->MouseButtonEventDispatcher.RegisterListener(CMouseButtonEventListener(this,&CFacadeMediator::HandlePaintAreaMouseButtonEvent));

		m_pCtrlMgr->GetPaintArea()->RenderEventDispatcher.RegisterListener(CRenderEventListener(this,&CFacadeMediator::HandlePaintEvent));

	}


	if(m_pCtrlMgr)
	{
		for(int i = 0; i < m_pCtrlMgr->GetButtonCount(); i++)
		{
			m_pCtrlMgr->GetButton(i)->SelectEventDispatcher.RegisterListener(CSelectEventListener(this,&CFacadeMediator::HandleIconButtonsSelectEvent));
		}
	}
    
}

