#ifndef __KEY_EVENT_H
#define __KEY_EVENT_H
#include "EventSystem.h"
class CControlBase;

//第一步:定义需要使用的事件类型
struct CKeyEvent : public CEvent<CControlBase*>
{
	int  KeyCode;//VK_键值，为win32虚拟键
    bool IsKeyDown;

	CKeyEvent(int key,bool bKeyDown) : CEvent(NULL,"KeyEvent"),KeyCode(key),IsKeyDown(bKeyDown)
	{}
};

//第二步,typedef 事件发送者
typedef CEventDispatcher<CKeyEvent>           CKeyEventDispatcher;


//第三步，typedef一个事件监听者
typedef CEventListener<void(CKeyEvent&)>       CKeyEventListener;


struct CKeyCharEvent : public CEvent<CControlBase*>
{
	char  KeyChar;

	CKeyCharEvent(char keyChar) : CEvent(NULL,"KeyCharEvent"),KeyChar(keyChar)
	{}

};

typedef CEventDispatcher<CKeyCharEvent>         CKeyCharEventDispatcher;
typedef CEventListener<void(CKeyCharEvent&)>    CKeyCharEventListener;


#endif