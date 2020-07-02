// CreateCharDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ndummy_client.h"
#include "CreateCharDlg.h"

#include "client.h"
#include "nmsg_char.h"

// CCreateCharDlg dialog

IMPLEMENT_DYNAMIC(CCreateCharDlg, CDialog)

CCreateCharDlg::CCreateCharDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateCharDlg::IDD, pParent)
{

}

CCreateCharDlg::~CCreateCharDlg()
{
}

void CCreateCharDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCreateCharDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CCreateCharDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCreateCharDlg message handlers

void CCreateCharDlg::OnBnClickedOk()
{
	tagC2S_CharCreate request;
	
	// 获取设置的角色基本信息
	client::instance()->send(&request, sizeof(request));

	OnOK();
}
