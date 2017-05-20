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
	/*--------------状态子系统相关操作开始-------------*/
    
    template<typename TState>
	TState*           CreateState();

	int               GetStateCount() const;

	int               AddState(CStateBase* pState);

	bool              RemoveState(int idx);

	int               GetStateIndex(CStateBase* pState);

	CStateBase*       GetState(int idx);

	/*--------------状态子系统相关操作开始-------------*/

public:

	/*--------------图元子系统相关操作开始-------------*/
    template<typename TShape>
	TShape*            CreateShape(const Rect& rc);

	int                GetShapeCount() const;
	
	int                AddShape(CShapeBase* pShape);
	
	bool               RemoveShape(int idx);

	int                GetShapeIndex(CShapeBase* pShape);

	CShapeBase*        GetShape(int idx);
	
	CShapeBase*        FindShape(const Point& pt);
	
	void               DrawShapes();
	
	/*--------------图元子系统相关操作结束-------------*/



public:
	/*--------------Undo/Redo子系统相关操作开始-------------*/

	template<typename TShapeCmd>
    ICommandPtr        CreateShapeCommand(CShapeBase* pShape);

	void               SetMaxCmdCount(int nMax);
	
	int                GetMaxCmdCount() const;
	
	bool               CanUndo() const;
	
	bool               CanRedo() const;
	
    void               PushCommand(ICommandPtr pCmd,bool bExecute = true);
	
	bool               Undo();
	
	bool               Redo();
	
	/*--------------Undo/Redo子系统相关操作结束-------------*/



public:

	/*--------------事件处理相关操作开始-------------*/

    //注册要监听的所有事件
	virtual void       RegisterEventListeners();

	//处理全局的键盘事件,Ctrl-Z和Ctrl-Y
	void               HandleUndoRedoKeyEvent(CAppKeyEvent& e);

protected:

	//处理所有的IconButton的select事件
	void               HandleIconButtonsSelectEvent(CSelectEvent& e);
	
    //处理PaintArea的mouseButton事件
	void               HandlePaintAreaMouseButtonEvent(CMouseButtonEvent& e);

	//处理PaintArea的Render事件
	void               HandlePaintEvent(CRenderEvent& e);

    /*--------------事件处理相关操作结束-------------*/
	
protected:
	
	//组合关系指针，掌管声明周期
	CStateManager*            m_pStateMgr;
	CShapeManager*            m_pShapeMgr;
	CUndoManager*             m_pUndoMgr;

protected://由构造函数中设置聚合指针关系

	//聚合关系指针
	CDrawerVisitor*           m_pDrawerVistor; //用于图元访问绘制

	//中间者类处理Input控件的事件，进行相关操作后，通过output控件输出显示
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