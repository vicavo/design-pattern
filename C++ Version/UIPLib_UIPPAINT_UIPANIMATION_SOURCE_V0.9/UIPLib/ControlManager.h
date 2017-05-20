#ifndef __CONTROL_MANAGER_H
#define __CONTROL_MANAGER_H
#include "ControlBase.h"
class CControlManager : public CControlBase
{
private:
	CControlManager()
	{}
public:
	CControlManager(HWND hwnd):m_hwnd(hwnd)
	{
		RECT rc;
		::GetClientRect(m_hwnd,&rc);
		SetLocalRect(CUtilHelper::ToRect(rc));
	}

	~CControlManager()
	{
	
	}

public:

	void InvalidateRect(const Rect& rc)
	{

		RECT r = CUtilHelper::FromRect(rc);
		::InvalidateRect(m_hwnd,&r,FALSE);
	}

private:
	HWND          m_hwnd;
};
#endif