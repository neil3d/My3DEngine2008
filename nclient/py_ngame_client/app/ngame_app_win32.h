#ifndef _NEXUS_GAME_APP_WIN32_H_
#define _NEXUS_GAME_APP_WIN32_H_
#include "ncore.h"
#include "ngame_app.h"

namespace nexus
{
	class nClientAPI ngame_app_win32
		: public ngame_app
	{
	public:
		ngame_app_win32(void);
		virtual ~ngame_app_win32(void);

		virtual void create(int wnd_w, int wnd_h, const nstring& title, bool full_scr_style);
		virtual void close();
		virtual void main_loop();

		HWND get_wnd_handle() const	{	return m_hWnd; }

		virtual LRESULT CALLBACK _WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	protected:
		bool	m_runing;
		HWND	m_hWnd;

		nDECLARE_VIRTUAL_CLASS(ngame_app_win32)
	};
}//namespace nexus

#endif //_NEXUS_GAME_APP_WIN32_H_