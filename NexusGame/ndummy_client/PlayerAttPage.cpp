// PlayerAttPage.cpp : implementation file
//

#include "stdafx.h"
#include "ndummy_client.h"
#include "PlayerAttPage.h"
#include "local_player.h"

// CPlayerAttPage dialog

IMPLEMENT_DYNAMIC(CPlayerAttPage, CPropertyPage)

CPlayerAttPage::CPlayerAttPage()
	: CPropertyPage(CPlayerAttPage::IDD)
{

}

CPlayerAttPage::~CPlayerAttPage()
{
}

void CPlayerAttPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPlayerAttPage, CPropertyPage)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CPlayerAttPage message handlers
void CPlayerAttPage::update_local_player_att()
{	
	m_attGrid.RemoveAll();

	//-- 基础属性组
	CMFCPropertyGridProperty* pBaseAttGroup = new CMFCPropertyGridProperty(_T("基础属性"));

	CMFCPropertyGridProperty* curHP = new CMFCPropertyGridProperty(_T("当前HP"), _variant_t(120), _T("主角当前的HP值"));
	curHP->AllowEdit(FALSE);
	pBaseAttGroup->AddSubItem(curHP);

	m_attGrid.AddProperty(pBaseAttGroup);
	m_attGrid.ExpandAll();
}

BOOL CPlayerAttPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rc;
	GetClientRect(&rc);
	if (!m_attGrid.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, rc, this, (UINT)-1) )
	{
		TRACE0("Failed to create Properties Grid \n");
		return FALSE;      // fail to create
	}

	m_attGrid.EnableHeaderCtrl(TRUE);
	m_attGrid.EnableDescriptionArea();
	m_attGrid.SetVSDotNetLook();
	m_attGrid.MarkModifiedProperties();

	update_local_player_att();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPlayerAttPage::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if( m_attGrid.GetSafeHwnd() )
	{
		m_attGrid.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOACTIVATE | SWP_NOZORDER);	
		m_attGrid.RedrawWindow();
	}
}

BOOL CPlayerAttPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	if( m_attGrid.GetSafeHwnd() )
	{
		CRect rc;
		GetClientRect(&rc);
		m_attGrid.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOACTIVATE | SWP_NOZORDER);	
		m_attGrid.RedrawWindow();
	}

	return CPropertyPage::OnSetActive();
}
