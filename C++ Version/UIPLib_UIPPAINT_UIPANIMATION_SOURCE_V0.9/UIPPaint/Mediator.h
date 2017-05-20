#ifndef __MEDIATOR_H
#define __MEDIATOR_H
#include "../UIPLib/Base.h"
#include "../UIPLib/ArrayList.h"
#include "../UIPLib/ControlBase.h"
#include "../UIPControl/UIPIconButton.h"
#include "../UIPControl/UIPPaintArea.h"
#include "Visitor.h"
#include "UndoManager.h"
#include "AppKeyEvent.h"
#include "InOutCtrlManager.h"
class  CStateManager;
class  CStateBase;
class  CShapeBase;
class  CShapeManager;


class CFacadeMediator
{
   
public:
	CFacadeMediator(CInOutCtrlManager* pCtrlMgr,CDrawerVisitor* pVisitor);
	
	~CFacadeMediator();

public:
	/*--------------״̬��ϵͳ��ز�����ʼ-------------*/
    
    template<typename TState>
	TState*           CreateState();

	int               GetStateCount() const;

	int               AddState(CStateBase* pState);

	bool              RemoveState(int idx);

	int               GetStateIndex(CStateBase* pState);

	CStateBase*       GetState(int idx);

	/*--------------״̬��ϵͳ��ز�����ʼ-------------*/

public:

	/*--------------ͼԪ��ϵͳ��ز�����ʼ-------------*/
    template<typename TShape>
	TShape*            CreateShape(const Rect& rc);

	int                GetShapeCount() const;
	
	int                AddShape(CShapeBase* pShape);
	
	bool               RemoveShape(int idx);

	int                GetShapeIndex(CShapeBase* pShape);

	CShapeBase*        GetShape(int idx);
	
	CShapeBase*        FindShape(const Point& pt);
	
	void               DrawShapes();
	
	/*--------------ͼԪ��ϵͳ��ز�������-------------*/



public:
	/*--------------Undo/Redo��ϵͳ��ز�����ʼ-------------*/

	template<typename TShapeCmd>
    ICommandPtr        CreateShapeCommand(CShapeBase* pShape);

	void               SetMaxCmdCount(int nMax);
	
	int                GetMaxCmdCount() const;
	
	bool               CanUndo() const;
	
	bool               CanRedo() const;
	
    void               PushCommand(ICommandPtr pCmd,bool bExecute = true);
	
	bool               Undo();
	
	bool               Redo();
	
	/*--------------Undo/Redo��ϵͳ��ز�������-------------*/



public:

	/*--------------�¼�������ز�����ʼ-------------*/

    //ע��Ҫ�����������¼�
	virtual void       RegisterEventListeners();

	//����ȫ�ֵļ����¼�,Ctrl-Z��Ctrl-Y
	void               HandleUndoRedoKeyEvent(CAppKeyEvent& e);

protected:

	//�������е�IconButton��select�¼�
	void               HandleIconButtonsSelectEvent(CSelectEvent& e);
	
    //����PaintArea��mouseButton�¼�
	void               HandlePaintAreaMouseButtonEvent(CMouseButtonEvent& e);

	//����PaintArea��Render�¼�
	void               HandlePaintEvent(CRenderEvent& e);

    /*--------------�¼�������ز�������-------------*/
	
protected:
	
	//��Ϲ�ϵָ�룬�ƹ���������
	CStateManager*            m_pStateMgr;
	CShapeManager*            m_pShapeMgr;
	CUndoManager*             m_pUndoMgr;

protected://�ɹ��캯�������þۺ�ָ���ϵ

	//�ۺϹ�ϵָ��
	CDrawerVisitor*           m_pDrawerVistor; //����ͼԪ���ʻ���

	//�м����ദ��Input�ؼ����¼���������ز�����ͨ��output�ؼ������ʾ
    CInOutCtrlManager*        m_pCtrlMgr;
};

template<typename TShape>  
TShape*       CFacadeMediator::CreateShape(const Rect& rc)
{
	return new TShape(rc);
}

template<typename TShapeCmd>
ICommandPtr   CFacadeMediator::CreateShapeCommand(CShapeBase* pShape)
{
	ICommandPtr ptr(new TShapeCmd(pShape,this));
	return ptr;
}

template<typename TState>
TState*       CFacadeMediator::CreateState()
{
	return new TState(m_pStateMgr);
}



#endif