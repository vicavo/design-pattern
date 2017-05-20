#ifndef __MOUSE_EVENT_H
#define __MOUSE_EVENT_H
#include "EventSystem.h"

//��չ�¼���ϵ

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


//��һ��:������Ҫʹ�õ��¼�����
//�˴������������¼�
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


//�ڶ�����typedefһ���¼�������
typedef CEventDispatcher<CMouseMoveEvent>   CMouseMoveEventDispatcher;
typedef CEventDispatcher<CMouseButtonEvent> CMouseButtonEventDispatcher;


//��������typedefһ���¼�������
typedef CEventListener<void(CMouseMoveEvent&)>   CMouseMoveEventListener;
typedef CEventListener<void(CMouseButtonEvent&)> CMouseButtonEventListener;



//��ʵ�ϵڶ����͵�����Ҳ���Բ�typedef������Ҫ���������
//ʹ��typedef ���Լ��ٴ������룬��ʹ�����Ķ�������������
#endif