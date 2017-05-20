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
//����һ���¼�
struct CRenderEvent : public CEvent<CControlBase*>
{
	ERenderTime RenderTime;

	CRenderEvent(ERenderTime time = RENDER_NONE):CEvent(NULL,"RenderEvent"),RenderTime(time)
	{}
};

//typedef �¼�������
typedef CEventDispatcher<CRenderEvent>   CRenderEventDispatcher;

//��������typedefһ���¼�������
typedef CEventListener<void(CRenderEvent&)>   CRenderEventListener;
#endif