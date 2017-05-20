#ifndef __MY_APP_ANIMATION_H
#define __MY_APP_ANIMATION_H
#include "../UIPLib/Application.h"
#include "AnimationControler.h"
class MyApp : public CApplication
{
public:
	MyApp()
	{
		m_bGameMode = true;
	}

	~MyApp()
	{
	}

protected:

	void InitControlSystem()
	{
		//override������ʱ������Ҫ���û��෽��
		CApplication::InitControlSystem(); 

		Rect rc = m_pCtrlMgr->GetWorldRect();
		m_pAnimPtr.reset(new CAnimationControler(new CPaintArea(m_pCtrlMgr,"PaintArea",rc)));

	}

protected:
	CAnimPtr         m_pAnimPtr;
};
#endif