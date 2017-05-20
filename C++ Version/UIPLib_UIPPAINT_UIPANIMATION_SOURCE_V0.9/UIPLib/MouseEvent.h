#ifndef __MOUSE_EVENT_H
#define __MOUSE_EVENT_H
#include "EventSystem.h"

//扩展事件体系

enum EButtonType
{
  NOBUTTON,
  LBUTTON,   
  RBUTTON, 
  MBUTTON
};

enum EButtonState
{
	NOSTATE,
	STATE_DOWN,
	STATE_UP,
	STATE_DBCLICK
};


//第一步:定义需要使用的事件类型
//此处定义鼠标相关事件
class CControlBase;

struct CMouseMoveEvent : public CEvent<CControlBase*>
{
	Point                 MousePt;
	CMouseMoveEvent(const Point& pt = Point(0,0)) : CEvent(NULL,"MouseMoveEvent"),MousePt(pt)
	{

	}
};

struct CMouseButtonEvent:public CEvent<CControlBase*>
{
    Point                  MousePt;
	EButtonType            ButtonType;
	EButtonState           ButtonState;

	CMouseButtonEvent(const Point& pt = Point(0,0)):CEvent(NULL,"MouseButtonEvent"), MousePt(pt),ButtonType(NOBUTTON),ButtonState(NOSTATE)
	{
	}

	CMouseButtonEvent(const Point& pt,EButtonType type,EButtonState state):CEvent(NULL,"MouseButtonEvent")
	{
		MousePt = pt;
		ButtonType = type;
		ButtonState = state;
	}
};


//第二步，typedef一个事件发送者
typedef CEventDispatcher<CMouseMoveEvent>   CMouseMoveEventDispatcher;
typedef CEventDispatcher<CMouseButtonEvent> CMouseButtonEventDispatcher;


//第三步，typedef一个事件监听者
typedef CEventListener<void(CMouseMoveEvent&)>   CMouseMoveEventListener;
typedef CEventListener<void(CMouseButtonEvent&)> CMouseButtonEventListener;



//事实上第二步和第三步也可以不typedef，但是要多输入代码
//使用typedef 可以减少代码输入，并使代码阅读起来更加清晰
#endif