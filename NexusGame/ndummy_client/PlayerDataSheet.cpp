// PlayDataSheet.cpp : implementation file
//

#include "stdafx.h"
#include "ndummy_client.h"
#include "PlayerDataSheet.h"


// CPlayDataSheet

IMPLEMENT_DYNAMIC(CPlayerDataSheet, CPropertySheet)

CPlayerDataSheet::CPlayerDataSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_created = false;
}

CPlayerDataSheet::CPlayerDataSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_created = false;
}

CPlayerDataSheet::~CPlayerDataSheet()
{
}


BEGIN_MESSAGE_MAP(CPlayerDataSheet, CPropertySheet)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CPlayerDataSheet message handlers
void CPlayerDataSheet::create(CWnd* parent)
{
	AddPage(&m_playerAttPage);
	AddPage(&m_playerBagPage);
	AddPage(&m_playerSkillPage);
	AddPage(&m_playeQuestPage);

	BOOL ok  = Create(parent,  WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_created = true;
}

void CPlayerDataSheet::OnSize(UINT nType, int cx, int cy)
{
	CPropertySheet::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if( !m_created )
		return;

	CRect rc(0,0, cx, cy);
	this->ClientToScreen(&rc);
	int numPage = GetPageCount();
	for (int i=0; i<numPage; i++)
	{
		CPropertyPage* pg = GetPage(i);
		if(pg->GetSafeHwnd())
		{
			pg->MoveWindow(&rc);
		}
	}
}
