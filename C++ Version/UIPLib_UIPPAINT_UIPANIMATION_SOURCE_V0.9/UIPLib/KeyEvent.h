#ifndef __KEY_EVENT_H
#define __KEY_EVENT_H
#include "EventSystem.h"
class CControlBase;

//��һ��:������Ҫʹ�õ��¼�����
struct CKeyEvent : public CEvent<CControlBase*>
{
	int  KeyCode;//VK_��ֵ��Ϊwin32�����
    bool IsKeyDown;

	CKeyEvent(int key,bool bKeyDown) : CEvent(NULL,"KeyEvent"),KeyCode(key),IsKeyDown(bKeyDown)
	{}
};

//�ڶ���,typedef �¼�������
typedef CEventDispatcher<CKeyEvent>           CKeyEventDispatcher;


//��������typedefһ���¼�������
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