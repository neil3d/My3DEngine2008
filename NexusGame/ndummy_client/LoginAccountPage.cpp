// LoginAccountPage.cpp : implementation file
//

#include "stdafx.h"
#include "ndummy_client.h"
#include "LoginAccountPage.h"
#include "client.h"
#include "nmsg_login.h"
#include "nutil.h"

// CLoginAccountPage dialog

IMPLEMENT_DYNAMIC(CLoginAccountPage, CPropertyPage)

CLoginAccountPage::CLoginAccountPage()
	: CPropertyPage(CLoginAccountPage::IDD)
{
	init_num_wnd();
}

CLoginAccountPage::~CLoginAccountPage()
{
}

void CLoginAccountPage::DoDataExchange(CDataExchange* pDX)
{
	// add by leo
	get_num_wnd();
	show_num_wnd(false);

	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoginAccountPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BATCH_LOGIN, &CLoginAccountPage::OnBnClickedBatchLogin)
END_MESSAGE_MAP()


// CLoginAccountPage message handlers

LRESULT CLoginAccountPage::OnWizardNext()
{
	//add by Leo
	if( !m_batch_login )
	{//单独登陆
		single_login();
	}
	else
	{//批量登陆
		batch_login();
	}

	return CPropertyPage::OnWizardNext();
}

void CLoginAccountPage::OnBnClickedBatchLogin()
{
	// TODO: Add your control notification handler code here
	m_batch_login = (1==IsDlgButtonChecked(IDC_BATCH_LOGIN));
	if( m_batch_login )
	{
		SetDlgItemText(IDC_STATIC_PSD,_T("开始序号"));
		show_num_wnd(true);
	}
	else
	{
		SetDlgItemText(IDC_STATIC_PSD,_T("密码"));
		show_num_wnd(false);
	}
}


void CLoginAccountPage::init_num_wnd(void)
{
	m_batch_login	= false;
	m_num_static	= NULL;
	m_num_edit		= NULL;
}


void CLoginAccountPage::show_num_wnd(bool bshow)
{
	if( m_num_static )	m_num_static->ShowWindow(bshow);
	if( m_num_edit )	m_num_edit->ShowWindow(bshow);
}


void CLoginAccountPage::get_num_wnd(void)
{
	if( !m_num_static )	m_num_static = GetDlgItem(IDC_STATIC_NUM);
	if( !m_num_edit )	m_num_edit = GetDlgItem(IDC_EDIT_NUM);
}


// 发送验证登陆消息
void CLoginAccountPage::send_login_proof_msg(char* name, char* psd)
{
	if( !name || !psd )
		return;

	//登陆验证消息
	tagC2S_LoginProof request;
	request.cur_version	= 1;
	request.account_id	= s_util.crc32(name, (uint32)strlen(name));
	memcpy(request.psd, psd, strlen(psd));
	request.ip			= 1;
	memset(request.mac, 1, sizeof(request.mac));
	client::instance()->send(&request, sizeof(request));
}

// 单独登陆
void CLoginAccountPage::single_login(void)
{
	//获取玩家名称
	CString	name;
	GetDlgItemText(IDC_EDIT_USERNAME, name);
	uint32 len_name = name.GetLength();


	char szName[MAX_ACCOUNT_LEN];
	memset(szName, 0, sizeof(szName));
	for(uint32 index=0; index<len_name; ++index)
	{
		szName[index] = (char)(name.GetAt(index));
	}

	//获取玩家密码
	CString	psd;
	GetDlgItemText(IDC_EDIT_PWD, psd);
	uint32 len_psd = psd.GetLength();
	char szPsd[MAX_MD5_SIZE];
	memset(szPsd, 0, sizeof(szPsd));
	for(uint32 index=0; index<len_psd; ++index)
	{
		szPsd[index] = (char)(psd.GetAt(index));
	}

	send_login_proof_msg(szName, szPsd);
}

// 批量登陆
void CLoginAccountPage::batch_login(void)
{
	CString	name;
	GetDlgItemText(IDC_EDIT_USERNAME, name);
	uint32 len_name = name.GetLength();


	char szName[MAX_ACCOUNT_LEN];
	char szPsd[MAX_MD5_SIZE];
	memset(szName,	0,	sizeof(szName));
	memset(szPsd,	0,	sizeof(szPsd));
	for(uint32 index=0; index<len_name; ++index)
	{
		szName[index]	= (char)(name.GetAt(index));
		szPsd[index]	= (char)(name.GetAt(index));
	}

	//获取批量登陆相关数量信息
	int32 index_start	= GetDlgItemInt(IDC_EDIT_PWD);
	int32 num			= GetDlgItemInt(IDC_EDIT_NUM);

	for(int32 index=0; index<num; ++index)
	{
		char cur_name[MAX_ACCOUNT_LEN];
		char cur_psd[MAX_MD5_SIZE];
		sprintf_s(cur_name, MAX_ACCOUNT_LEN,	"%s_%d",	szName,	(index_start+index));
		sprintf_s(cur_psd,	MAX_MD5_SIZE,		"%s_%d",	szPsd,	(index_start+index));

		send_login_proof_msg(cur_name, cur_psd);
}
}