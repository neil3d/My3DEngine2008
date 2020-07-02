
// ndummy_clientView.cpp : implementation of the CDummyClientView class
//

#include "stdafx.h"
#include <Mmsystem.h>
#pragma comment(lib,"Winmm.lib")

#include "ndummy_client.h"

#include "ndummy_clientDoc.h"
#include "ndummy_clientView.h"
#include "player_input.h"
#include "client_session.h"
#include "msg_dispatch.h"
#include "game_app.h"
#include "local_player.h"
#include "ConsoleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDummyClientView

IMPLEMENT_DYNCREATE(CDummyClientView, CFormView)

BEGIN_MESSAGE_MAP(CDummyClientView, CFormView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_STN_CLICKED(IDC_RENDER_WND, &CDummyClientView::OnStnClickedRenderWnd)	
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_POS, &CDummyClientView::OnStnClickedPos)
END_MESSAGE_MAP()

// CDummyClientView construction/destruction

CDummyClientView::CDummyClientView()
	: CFormView(CDummyClientView::IDD)
{
	// TODO: add construction code here

}

CDummyClientView::~CDummyClientView()
{
}

void CDummyClientView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HUD_BOX, m_hud_box);
	DDX_Control(pDX, IDC_RENDER_WND, m_render_wnd);
	DDX_Control(pDX, IDC_PROGRESS_HP, m_hp);
	//DDX_Control(pDX, IDC_PROGRESS_EP, m_ep);
	DDX_Control(pDX, IDC_PROGRESS_MP, m_mp);
}

BOOL CDummyClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CDummyClientView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	game_app::instance()->bind_render_wnd(&m_render_wnd);
}

void CDummyClientView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDummyClientView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CDummyClientView diagnostics

#ifdef _DEBUG
void CDummyClientView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDummyClientView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDummyClientDoc* CDummyClientView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDummyClientDoc)));
	return (CDummyClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CDummyClientView message handlers

void CDummyClientView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	//--
	if( m_hud_box.GetSafeHwnd()
		&& m_render_wnd.GetSafeHwnd() )
	{		
		RECT rc;

		/*m_hud_box.GetWindowRect(&rc);
		ScreenToClient(&rc);		
		m_hud_box.SetWindowPos(NULL, rc.left, rc.top, cx-rc.left*2, rc.bottom-rc.top, SWP_NOZORDER);*/

		m_render_wnd.GetWindowRect(&rc);
		ScreenToClient(&rc);		
		m_render_wnd.SetWindowPos(NULL, rc.left, rc.top, cx-rc.left*2, cy-rc.top-8, SWP_NOZORDER);
	}
}

int CDummyClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	SetTimer(1001, 1000/30, NULL);
	m_lastTickTime = timeGetTime();
	
	return 0;
}

void CDummyClientView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: Add your message handler code here
	game_app::instance()->bind_render_wnd(NULL);
}

void CDummyClientView::OnStnClickedRenderWnd()
{
	// TODO: Add your control notification handler code here
}

BOOL CDummyClientView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->hwnd == m_render_wnd.GetSafeHwnd() )
	{
		switch (pMsg->message)
		{
		case WM_LBUTTONDOWN:
			player_input::instance()->on_mouse_left_down(
				npoint( GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam) )
				);
			break;
		case WM_LBUTTONUP:
			player_input::instance()->on_mouse_left_up();
			break;
		case WM_RBUTTONDOWN:
			player_input::instance()->on_mouse_right_down(
				npoint( GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam) )
				);
			break;
		case WM_RBUTTONUP:
			player_input::instance()->on_mouse_right_up();
			break;
		case WM_MOUSEMOVE:
			player_input::instance()->on_mouse_move(
				npoint( GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam) ),
				pMsg->wParam&MK_CONTROL!=0
				);
			break;
		case WM_MOUSEWHEEL:
			player_input::instance()->on_mouse_wheel( GET_WHEEL_DELTA_WPARAM(pMsg->wParam) );
			break;

		}
	}

	return CFormView::PreTranslateMessage(pMsg);
}


BOOL CDummyClientView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	player_input::instance()->on_mouse_wheel(zDelta);

	return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}

void CDummyClientView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	DWORD now = timeGetTime();
	float delta = (now-m_lastTickTime)/1000.0f;
	m_lastTickTime = now;

	game_app::instance()->update( delta );
	game_app::instance()->render();	


	uint32 lag = client_session::instance()->get_lag();
	uint32 lag2 = client_session::instance()->get_world_lag();
	uint32 lag3 = client_session::instance()->get_gateway_lag();

	if (lag > 500)
	{
		nLog_Info(_T("%u, %u, %u"), lag, lag2, lag3);
	}

	CString strText;
	strText.Format(_T("%u, %u, %u"), lag, lag2, lag3);
	SetDlgItemText(IDC_LAG, strText);

	lag = g_msg_dispatch.get_recv_sec();
	strText.Format(_T("%u"), lag);
	SetDlgItemText(IDC_RECV_SEC, strText);


	vector3 pos = local_player::instance()->get_pos();
	strText.Format(_T("x= %d, y = %d, z = %d"), int(pos.x), int(pos.y), int(pos.z));
	SetDlgItemText(IDC_POS, strText);

	// ÏÔÊ¾level
	uint32 level = local_player::instance()->get_data().get_uint32_value(EUF_UnitLevel);
	strText.Format(_T("µÈ¼¶ %u"), level);
	SetDlgItemText(IDC_PLAYER_LEVEL, strText);

	uint32 max_hp = local_player::instance()->get_data().get_uint32_value(EUF_UnitMaxHealth);
	uint32 hp = local_player::instance()->get_data().get_uint32_value(EUF_UnitHealth);
	m_hp.SetBarColor(RGB(255,0,0));
	m_hp.SetBkColor(RGB(255,255,255));
	m_hp.SetRange32(0, max_hp);
	m_hp.SetPos(hp);

	uint32 max_ep = local_player::instance()->get_data().get_uint32_value(EUF_UnitMaxEnergy);
	uint32 ep = local_player::instance()->get_data().get_uint32_value(EUF_UnitEnergy);
	//m_ep.SetBarColor(RGB(255,255,0));
	//m_ep.SetBkColor(RGB(255,255,255));
	//m_ep.SetRange32(0, max_ep);
	//m_ep.SetPos(ep);

	uint32 max_mp = local_player::instance()->get_data().get_uint32_value(EUF_UnitMaxMana);
	uint32 mp = local_player::instance()->get_data().get_uint32_value(EUF_UnitMana);
	m_mp.SetBarColor(RGB(0,0,255));
	m_mp.SetBkColor(RGB(255,255,255));
	m_mp.SetRange32(0, max_mp);
	m_mp.SetPos(mp);

	CFormView::OnTimer(nIDEvent);
}

void CDummyClientView::OnStnClickedPos()
{
	// TODO: Add your control notification handler code here
}
