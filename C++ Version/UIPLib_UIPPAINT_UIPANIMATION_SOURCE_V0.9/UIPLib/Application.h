#ifndef __APPLICATION_H
#define __APPLICATION_H
#include "Base.h"
#include "IRender2D.h"
#include "ControlManager.h"
class CApplication
{
protected:
	CApplication() : m_hInstance(NULL),m_hwnd(NULL),m_bAppPaused(false),m_bGameMode(false),m_captionName(NULL),m_pRender(NULL),m_gdiplusToken(NULL),m_pCtrlMgr(NULL),m_pFocusCtrl(NULL)
	{
		memset(&m_dirtyRect,0,sizeof(m_dirtyRect));
		m_pGlobalApp = this;
	}

public:
	virtual ~CApplication()
	{
		if(!m_bGameMode)
			::KillTimer(m_hwnd,10000);
		if(m_pCtrlMgr)
		{
			delete m_pCtrlMgr;
			m_pCtrlMgr = NULL;
		}

		if(m_pRender)
		{
			delete m_pRender;
			m_pRender = NULL;
		}
		GdiplusShutdown(m_gdiplusToken);
	}

public:
	HINSTANCE GetAppInst() const
	{
		return m_hInstance;
	}

	HWND GetAppHwnd() const
	{
		return m_hwnd;
	}



	virtual void Init(HINSTANCE hInst,TCHAR* caption,const Rect& rc = Rect(100,100,800,600));

	virtual int Run();

	virtual LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void Update(float dt = 0.0F);

	CControlBase*  GetFocusCtrl()
	{
		return m_pFocusCtrl;
	}

	IRender2D*  GetRender()
	{
		return m_pRender;
	}

	void Invalidate()
	{
		assert(m_hwnd);
		::InvalidateRect(m_hwnd,NULL,false);
	}

protected:

	//初始化模板方法
	//全部被Init函数调用
	virtual void InitGDIPlus();
	virtual void InitWindow(const Rect& rc);
	virtual void InitRender(ERenderType renderType = ERT_GDI);
	virtual void InitControlSystem();

	virtual void Render();

protected:
	virtual void OnSize(const Size& size);

	virtual void OnMouseMove(CMouseMoveEvent& moveEvent);
	virtual void OnMouseButton(CMouseButtonEvent& mouseEvent);

	virtual void OnKeyEvent(CKeyEvent& keyEvent);

	virtual void OnKeyCharEvent(CKeyCharEvent& charEvent);

	static void  GetMouseButtonInfo(UINT eventIndex,EButtonType& type,EButtonState& state);
	void         TriggerMouseButtonEvent(UINT eventIndex,LPARAM lParam);
	
protected:
    HINSTANCE        m_hInstance;
	HWND             m_hwnd;
    TCHAR*           m_captionName;
	bool             m_bAppPaused;

	IRender2D*        m_pRender;
	CControlManager*  m_pCtrlMgr;
	CControlBase*     m_pFocusCtrl;

	bool             m_bGameMode;
    RECT             m_dirtyRect;
 
public:

	static CApplication* Get() 
	{ 
		return m_pGlobalApp;
	}

protected:
	static CApplication*	m_pGlobalApp;

private:
	ULONG_PTR           m_gdiplusToken; 
    
};


//减少输入
inline CApplication* App()
{
	return CApplication::Get();
}

//继承自CApplication的App初始化
//必须由客户端调用
template<typename TApp>
void CreateApplication()
{
	static TApp tApp;
}

#endif