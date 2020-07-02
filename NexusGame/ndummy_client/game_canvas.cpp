#include "StdAfx.h"
#include "game_canvas.h"
#include "game_map.h"

game_canvas::game_canvas(void)
{
	m_old_bmp = NULL;
}

game_canvas::~game_canvas(void)
{
}

void game_canvas::init(CWnd* wnd)
{
	int screen_w = ::GetSystemMetrics(SM_CXSCREEN);
	int screen_h = ::GetSystemMetrics(SM_CYSCREEN);

	CClientDC dc(wnd);
	m_mem_dc.CreateCompatibleDC(&dc);
	m_mem_bmp.CreateCompatibleBitmap(&dc, screen_w, screen_h);
	m_old_bmp = m_mem_dc.SelectObject(&m_mem_bmp);

	CRect clientRc;
	wnd->GetClientRect(&clientRc);
	m_view.wnd_w = clientRc.Width();
	m_view.wnd_h = clientRc.Height();
}

void game_canvas::flip(CWnd* wnd)
{
	if( !wnd->GetSafeHwnd() )
		return;

	CRect clientRc;
	wnd->GetClientRect(&clientRc);
	CClientDC targetDC(wnd);
	targetDC.BitBlt(0,0, clientRc.Width(), clientRc.Height(),
		&m_mem_dc, 0, 0, SRCCOPY);

	m_view.wnd_w = clientRc.Width();
	m_view.wnd_h = clientRc.Height();
}

void game_canvas::close()
{
	if(m_old_bmp)
	{
		m_mem_dc.SelectObject(m_old_bmp);
		m_old_bmp = NULL;
	}
	m_mem_dc.DeleteDC();
	m_mem_bmp.DeleteObject();
}

void game_canvas::clear()
{
	m_mem_dc.FillSolidRect( 0, 0, m_view.wnd_w, m_view.wnd_h, 
		m_view.clear_color );
}

void game_canvas::draw_line(int start_x, int start_y, int end_x, int end_y, COLORREF clr)
{
	CPen pn;
	pn.CreatePen(PS_SOLID, 1, clr);
	CPen* oldPen = m_mem_dc.SelectObject(&pn);

	m_mem_dc.MoveTo(start_x, start_y);
	m_mem_dc.LineTo(end_x, end_y);

	m_mem_dc.SelectObject(oldPen);
	pn.DeleteObject();
}

void game_canvas::draw_rect(const nrect& rc, COLORREF clr)
{
	m_mem_dc.FillSolidRect(rc.left, rc.top, rc.get_width(), rc.get_height(), clr);
}

void game_canvas::update_view(const vector3& pos)
{
	m_view.loaction = pos;
}