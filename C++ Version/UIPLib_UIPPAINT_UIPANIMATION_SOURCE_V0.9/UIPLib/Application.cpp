#include "Application.h"
#include "GDIRender2D.h"

IRender2D* RenderFactory::CreateRender(ERenderType renderType)
{
	IRender2D* pRender = NULL;
     switch(renderType)
	 {
	 case ERT_GDI:
		 pRender = new GDIRender2D();
		 break;
	 case ERT_DIRECTX:
		 pRender = new GDIRender2D();
		 break;
	 case ERT_OPENGL:
		 pRender = new GDIRender2D();
		 break;
	 default:
		 assert(false);
		 break;
	 }
	 return pRender;
}


IRenderImage* RenderFactory::CreateRenderImage(ERenderType renderType)
{
	
	IRenderImage* pRenderImg = NULL;
     switch(renderType)
	 {
	 case ERT_GDI:
		 pRenderImg = new GDIRenderImage();
		 break;
	 case ERT_DIRECTX:
		 pRenderImg = new GDIRenderImage();
		 break;
	 case ERT_OPENGL:
		 pRenderImg = new GDIRenderImage();
		 break;
	 default:
		 assert(false);
		 break;
	 }
	 return pRenderImg;
}

CApplication* CApplication::m_pGlobalApp = NULL;

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( App() != 0 && App()->GetAppHwnd() != NULL )
		return App()->MsgProc(msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
	return 0;
}

VOID CALLBACK TimerProc(HWND hwnd,
						UINT uMsg,
						UINT_PTR idEvent,
						DWORD dwTime
						)
{
	App()->Update();
}




void CApplication::Init(HINSTANCE hInst, TCHAR *caption,const Rect& rc)
{
	m_hInstance = hInst;
	m_captionName = caption;
	InitWindow(rc);
	InitGDIPlus();
	InitRender();
	InitControlSystem();
	if(!m_bGameMode)
		::SetTimer(m_hwnd,10000,30,&TimerProc);
}

void CApplication::InitGDIPlus()
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

void CApplication::InitWindow(const Rect& rc)
{
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW|CS_OWNDC;
	wc.lpfnWndProc   = MainWndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hInstance;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = 0;
	wc.lpszClassName = TEXT("UIPClassName");

	if( !RegisterClass(&wc) )
	{
		MessageBox(0, TEXT("窗口类注册失败"), 0, 0);
		PostQuitMessage(0);
	}

	
	//AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	m_hwnd = CreateWindow(TEXT("UIPClassName"), m_captionName, 
		WS_OVERLAPPEDWINDOW, rc.X, rc.Y, rc.Width, rc.Height, 
		0, 0, m_hInstance, 0); 

	if( !m_hwnd )
	{
		MessageBox(0, TEXT("窗口创建失败！！！"), 0, 0);
		PostQuitMessage(0);
	}

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

}

void CApplication::InitRender(ERenderType renderType)
{
	m_pRender = RenderFactory::CreateRender(renderType);
	m_pRender->Create(m_hwnd);
}


void CApplication::InitControlSystem()
{
	m_pCtrlMgr = new CControlManager(m_hwnd);
	m_pFocusCtrl = m_pCtrlMgr;
	
}

void CApplication::Render()
{
	if(m_pRender)
	{
		if(!m_bGameMode)
		{
			Rect dirtyRect = CUtilHelper::ToRect(m_dirtyRect);
			m_pRender->SetClipRect(dirtyRect);
		}
		m_pRender->Clear();
		m_pCtrlMgr->Render();
		//下面是debug脏区使用
		//m_pRender->DrawDirtyRect(::GetDC(m_hwnd),dirtyRect);
		m_pRender->Present();
	}
}

void CApplication::Update(float dt)
{
	if(!::IsRectEmpty(&m_dirtyRect))
		Render();

	memset(&m_dirtyRect,0,sizeof(m_dirtyRect));
}


void CApplication::OnSize(const Size& size)
{
	if(m_pRender)
		m_pRender->Resize(size);

	if(m_pCtrlMgr)
		m_pCtrlMgr->SetSize(size);
}

void  CApplication::GetMouseButtonInfo(UINT eventIndex,EButtonType& type,EButtonState& state)
{
	type = NOBUTTON;
    state = NOSTATE;

	switch (eventIndex)
	{
	case WM_LBUTTONUP:
         type = LBUTTON;
		 state = STATE_UP;
		 break;
	case WM_LBUTTONDOWN:
		type = LBUTTON;
		state = STATE_DOWN;
		 break;
	case WM_LBUTTONDBLCLK:
		type = LBUTTON;
		state = STATE_DBCLICK;
		break;

	case WM_RBUTTONUP:
         type = RBUTTON;
		 state = STATE_UP;
		 break;
	case WM_RBUTTONDOWN:
		type = RBUTTON;
		state = STATE_DOWN;
		break;
	case WM_RBUTTONDBLCLK:
		type = RBUTTON;
		state = STATE_DBCLICK;
		break;

	case WM_MBUTTONUP:
		type = MBUTTON;
		state = STATE_UP;
		break;
	case WM_MBUTTONDOWN:
		type = MBUTTON;
		state = STATE_DOWN;
		break;
	case WM_MBUTTONDBLCLK:
		type = MBUTTON;
		state = STATE_DBCLICK;
		break;
	}
}

void CApplication::TriggerMouseButtonEvent(UINT msg,LPARAM lParam)
{
	Point pt;
	pt.X  = LOWORD(lParam);
	pt.Y  = HIWORD(lParam);
	EButtonType type;
	EButtonState state;
	GetMouseButtonInfo(msg,type,state);
	OnMouseButton(CMouseButtonEvent(pt,type,state));
}

void CApplication::OnMouseMove(CMouseMoveEvent& moveEvent)
{
	if(m_pCtrlMgr)
	{
		CControlBase* pHitCtrl = m_pCtrlMgr->FindHitCtrl(moveEvent.MousePt);
		if(pHitCtrl)
		{
			moveEvent.EventSrc = pHitCtrl;
			pHitCtrl->MouseMoveEventDispatcher.DispatchEvent(moveEvent);
		}
	}
}

void CApplication::OnMouseButton(CMouseButtonEvent &mouseEvent)
{
	if(m_pCtrlMgr)
	{
		CControlBase* pHitCtrl = m_pCtrlMgr->FindHitCtrl(mouseEvent.MousePt);
		if(pHitCtrl)
		{
			m_pFocusCtrl = pHitCtrl;
			mouseEvent.EventSrc = pHitCtrl;
			pHitCtrl->MouseButtonEventDispatcher(mouseEvent);
		}
	}
}

void CApplication::OnKeyEvent(CKeyEvent& keyEvent)
{

	if(m_pCtrlMgr)
	{
		if(m_pFocusCtrl)
		{
			keyEvent.EventSrc = m_pFocusCtrl;
			m_pFocusCtrl->KeyEventDispatcher(keyEvent);
		}
	}
}

void CApplication::OnKeyCharEvent(CKeyCharEvent& charEvent)
{
	if(m_pCtrlMgr)
	{
		if(m_pFocusCtrl)
		{
			charEvent.EventSrc = m_pFocusCtrl;
			m_pFocusCtrl->KeyCharEventDispatcher(charEvent);
		}
	}
}


LRESULT CApplication::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool minOrMaxed = false;

	RECT clientRect = {0, 0, 0, 0};
	switch( msg )
	{
	case WM_ACTIVATE:
		if( LOWORD(wParam) == WA_INACTIVE )
			m_bAppPaused = true;
		else
			m_bAppPaused = false;
		return 0;

	case WM_SIZE:
		{
			Size size;
			size.Width  = LOWORD(lParam);
			size.Height = HIWORD(lParam);

			if( wParam == SIZE_MINIMIZED )
			{
				m_bAppPaused = true;
				minOrMaxed = true;
			}
			else if( wParam == SIZE_MAXIMIZED )
			{
				m_bAppPaused = false;
				minOrMaxed = true;
			}
			else if( wParam == SIZE_RESTORED )
			{
				m_bAppPaused = false;

				minOrMaxed = false;
			}

			OnSize(size);

		}
		return 0;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(m_hwnd, &ps);
			m_dirtyRect = ps.rcPaint;
			EndPaint(m_hwnd, &ps);
		}

		break;

	case WM_MOUSEMOVE:
		{
			Point pt;
			pt.X  = LOWORD(lParam);
			pt.Y  = HIWORD(lParam);
			OnMouseMove(CMouseMoveEvent(pt));
		}
		break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		{
			SetCapture(m_hwnd);
			TriggerMouseButtonEvent(msg,lParam);
		}
		break;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		{
			ReleaseCapture();
			TriggerMouseButtonEvent(msg,lParam);
		}
		break;

	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
		{
			TriggerMouseButtonEvent(msg,lParam);
		}
		break;

	case WM_KEYDOWN:
		{
			OnKeyEvent(CKeyEvent(wParam,true));
			break;
		}

	case WM_KEYUP:
		{
			OnKeyEvent(CKeyEvent(wParam,false));
			break;
		}

	case WM_CHAR:
		{
			OnKeyCharEvent(CKeyCharEvent(wParam));
			break;
		}

	case WM_CLOSE:
		DestroyWindow(m_hwnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	}
	return DefWindowProc(m_hwnd, msg, wParam, lParam);
}

int CApplication::Run()
{
	MSG  msg;
	msg.message = WM_NULL;
	if(m_bGameMode)
	{
		while(msg.message != WM_QUIT)
		{
			if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{	
				if( m_bAppPaused )
				{
					Sleep(20);
					continue;
				}

				Render();
				
			}
		}

	}
	//blf 非死循环
	else
	{
		while(msg.message != WM_QUIT)
		{
			if( GetMessage(&msg,NULL,0,0) )
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		return (int)msg.wParam;
	}

	return (int)msg.wParam;
}