// LoginCharPage.cpp : implementation file
//

#include "stdafx.h"
#include "ndummy_client.h"
#include "LoginCharPage.h"
#include "CreateCharDlg.h"

#include "client.h"
#include "timer/ntimestamp.h"
#include "nmsg_char.h"
#include "nmsg_movement.h"

// CLoginCharPage dialog

IMPLEMENT_DYNAMIC(CLoginCharPage, CPropertyPage)

CLoginCharPage::CLoginCharPage()
	: CPropertyPage(CLoginCharPage::IDD)
{

}

CLoginCharPage::~CLoginCharPage()
{
}

void CLoginCharPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoginCharPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_CREATE_CHAR, &CLoginCharPage::OnBnClickedBtnCreateChar)
	ON_BN_CLICKED(IDC_BTN_DEL_CHAR, &CLoginCharPage::OnBnClickedBtnDelChar)
END_MESSAGE_MAP()


// CLoginCharPage message handlers

void CLoginCharPage::OnBnClickedBtnCreateChar()
{
	// TODO: Add your control notification handler code here
	CCreateCharDlg dlg;
	dlg.DoModal();
}

void CLoginCharPage::OnBnClickedBtnDelChar()
{
	// TODO: Add your control notification handler code here
}

BOOL CLoginCharPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   
	psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_FINISH);
	psheet->SetFinishText(_T("进入游戏"));


	return CPropertyPage::OnSetActive();
}

BOOL CLoginCharPage::OnWizardFinish()
{
	tagC2S_CharEnterGame request;
	request.player_id = 125;
	client::instance()->send(&request, sizeof(request));

	// 请求服务器时间
	tagC2S_CheckTime request_time;
	request_time.client_time_stamp = get_time();
	client::instance()->send(&request_time, sizeof(request_time));

	return CPropertyPage::OnWizardFinish();
}
