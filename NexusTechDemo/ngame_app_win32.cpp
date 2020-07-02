#include "StdAfx.h"
#include "ngame_app_win32.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(ngame_app, nobject)
	ngame_app* ngame_app::s_instance = NULL;
	ngame_app* ngame_app::instance()
	{
		return s_instance;
	}

	nDEFINE_VIRTUAL_CLASS(ngame_app_win32, ngame_app)

	ngame_app_win32::ngame_app_win32(void)
		: m_runing(true), m_hWnd(NULL)
	{
	}

	ngame_app_win32::~ngame_app_win32(void)
	{
	}

	LRESULT CALLBACK GWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return ( (ngame_app_win32*)ngame_app_win32::instance() )->_WndProc(hWnd, message, wParam, lParam);
	}

	void ngame_app_win32::create(int wnd_w, int wnd_h, const nstring& title, bool full_scr_style)
	{
		nASSERT(wnd_w > 0);
		nASSERT(wnd_h > 0);

		HINSTANCE hInstance = ::GetModuleHandle(NULL);

		const TCHAR* szWindowClass = _T("nexus_game_app_wnd");

		//-- 注册window class
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wcex.lpfnWndProc	= GWndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInstance;
		wcex.hIcon			= NULL;
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= szWindowClass;
		wcex.hIconSm		= NULL;

		ATOM wnd_class_atom = ::RegisterClassEx(&wcex);

		//-- 创建一个窗口，并在屏幕上居中
		DWORD style = 0;
		DWORD styleEx = 0;

		if( full_scr_style )
		{
			style = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
			styleEx = WS_EX_TOPMOST;
		}
		else
		{
			style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_THICKFRAME;
		}

		RECT rc = {0, 0, 0, 0};
		
		int scr_w = ::GetSystemMetrics(SM_CXSCREEN);
		int scr_h = ::GetSystemMetrics(SM_CYSCREEN);

		if( !full_scr_style )
		{
			if( scr_w > wnd_w )
				rc.left = (scr_w-wnd_w)/2;
			if( scr_h > wnd_h )
				rc.top = (scr_h-wnd_h)/2;			
		}
		rc.right = rc.left+wnd_w;
		rc.bottom = rc.top+wnd_h;

		::AdjustWindowRect(&rc, style, FALSE);

		m_hWnd = ::CreateWindow(szWindowClass, title.c_str(), style,
			rc.left, rc.top, 
			rc.right-rc.left, rc.bottom-rc.top,
			NULL, NULL, hInstance, NULL);

		if( m_hWnd == NULL )
			nthrow(_T("create win32 window failed."));

		::ShowWindow(m_hWnd, SW_SHOWNORMAL);
		::UpdateWindow(m_hWnd);
	}

	void ngame_app_win32::close()
	{
		m_runing = false;
	}

	void ngame_app_win32::main_loop()
	{
		MSG msg;

		// Main message loop:
		while (m_runing)
		{
			if( ::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
			{
				if( !::GetMessage(&msg, NULL, 0, 0) )
				{
					m_runing = false;
					break;
				}

				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else	// 只有所有win32消息处理完，才跑游戏更新--优先处理win32 message
			{
				tick();
			}
		}
	}

	LRESULT CALLBACK ngame_app_win32::_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch(message)
		{
		case WM_ERASEBKGND:
			return TRUE;
		case WM_LBUTTONDOWN:
			::SetCapture(hWnd);	//	为了防止鼠标拖动时，滑出窗口后无法受到鼠标移动消息
			break;
		case WM_LBUTTONUP:
			::ReleaseCapture();
			break;
		case WM_DESTROY:
			close();
			break;
		case WM_SYSKEYUP:
			// 防止产生SC_KEYMENU系统命令
			if( wParam == VK_MENU
				|| wParam == VK_F10 )
				return FALSE;
			break;
		}

		return ::DefWindowProc(hWnd, message, wParam, lParam);
	}
}//namespace nexus