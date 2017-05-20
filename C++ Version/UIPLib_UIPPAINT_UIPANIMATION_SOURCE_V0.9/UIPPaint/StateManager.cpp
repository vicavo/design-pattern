#include "StateManager.h"
#include "Mediator.h"
#include "ShapeOperatorCommand.h"

void CRectState::MouseDown(const Point& pt)
{
	Size size(40,30);
	Point center = pt - size / 2;
	CFacadeMediator* pMediator = m_pStateMgr->GetMediator();
	CRectShape*  pShape = pMediator->CreateShape<CRectShape>(Rect(center,size));
	pMediator->PushCommand(pMediator->CreateShapeCommand<CShapeCreateCommand>(pShape));
}

void CCircleState::MouseDown(const Point& pt)
{
	Size size(40,40);
	Point center = pt - size / 2;
    CFacadeMediator* pMediator = m_pStateMgr->GetMediator();
    CCircleShape* pShape = pMediator->CreateShape<CCircleShape>(Rect(center,size));
	pMediator->PushCommand(pMediator->CreateShapeCommand<CShapeCreateCommand>(pShape));
}

void CTriangleState::MouseDown(const Point& pt)
{
	Size size(40,30);
	Point center = pt - size / 2;
    CFacadeMediator* pMediator = m_pStateMgr->GetMediator();
	CTriangleShape* pShape = pMediator->CreateShape<CTriangleShape>(Rect(center,size));
	pMediator->PushCommand(pMediator->CreateShapeCommand<CShapeCreateCommand>(pShape));
}

void CFillState::MouseDown(const Point& pt)
{
	CFacadeMediator* pMediator = m_pStateMgr->GetMediator();
	CShapeBase* pShape = pMediator->FindShape(pt);
	if(pShape)
	{
		pShape->SetFill(true);
	}
}

