#ifndef __RENDER_EVENT_H
#define __RENDER_EVENT_H
#include "EventSystem.h"
class CControlBase;

enum ERenderTime
{
	RENDER_NONE,
	RENDER_BEGIN,
	RENDER_ON,
	RENDER_END

};
//定义一个事件
struct CRenderEvent : public CEvent<CControlBase*>
{
	ERenderTime RenderTime;

	CRenderEvent(ERenderTime time = RENDER_NONE):CEvent(NULL,"RenderEvent"),RenderTime(time)
	{}
};

//typedef 事件发送者
typedef CEventDispatcher<CRenderEvent>   CRenderEventDispatcher;

//第三步，typedef一个事件监听者
typedef CEventListener<void(CRenderEvent&)>   CRenderEventListener;
#endif