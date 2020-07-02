// PlayerBagPage.cpp : implementation file
//

#include "stdafx.h"
#include "ndummy_client.h"
#include "PlayerBagPage.h"


// CPlayerBagPage dialog

IMPLEMENT_DYNAMIC(CPlayerBagPage, CPropertyPage)

CPlayerBagPage::CPlayerBagPage()
	: CPropertyPage(CPlayerBagPage::IDD)
{

}

CPlayerBagPage::~CPlayerBagPage()
{
}

void CPlayerBagPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BAG_LIST, m_listBox);
}


BEGIN_MESSAGE_MAP(CPlayerBagPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_BAG_DROP, &CPlayerBagPage::OnBnClickedBtnBagDrop)
END_MESSAGE_MAP()


// CPlayerBagPage message handlers

void CPlayerBagPage::OnBnClickedBtnBagDrop()
{
	// TODO: Add your control notification handler code here
	int sel = m_listBox.GetSelCount();
}

BOOL CPlayerBagPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	m_listBox.AddString(_T("Test Item"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
