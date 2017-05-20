#ifndef __APP_KEY_EVENT_H
#define __APP_KEY_EVENT_H
/*
在做Undo/Redo测试的时候，我们使用的是鼠标中键undo,鼠标右键redo，现在我们要符合win32标准，ctrl+Z为undo,
ctrl+Y为redo，此时发现CApplication的键盘事件是从获得焦点的控件触发的，而undo/redo需要全局的键盘事件。
我们原来的CApplication的键盘事件不符合我们的需求。

有两种方案：
1) 在UIPLib中的CApplication中增加一个CAppKeyEvent事件触发机制。但是需要我们更改UIPLib中的源代码。
2) 我们在UIPPaint中进行扩展实现该事件触发机制。这种情况下，不需要修改UIPLib的源代码，很符合开闭原则。
那么我们以第二种方式进行扩展。
这个例子很好的演示了我们的UIPLib的扩展性
*/
#include "../UIPLib/EventSystem.h"
class CApplication;

struct CAppKeyEvent : public CEvent<CApplication*>
{
	int  KeyCode;//VK_键值，为win32虚拟键
    bool IsKeyDown;

	CAppKeyEvent(CApplication* pApp,CKeyEvent& e) : CEvent(pApp,"AppKeyEvent"),KeyCode(e.KeyCode),IsKeyDown(e.IsKeyDown)
	{}
};

typedef CEventDispatcher<CAppKeyEvent>         CAppKeyEventDispatcher;
typedef CEventListener<void(CAppKeyEvent&)>    CAppKeyEventListener;

#endif