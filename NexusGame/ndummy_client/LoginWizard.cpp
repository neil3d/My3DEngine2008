// LoginWizard.cpp : implementation file
//

#include "stdafx.h"
#include "ndummy_client.h"
#include "LoginWizard.h"


// CLoginWizard

IMPLEMENT_DYNAMIC(CLoginWizard, CPropertySheet)

CLoginWizard::CLoginWizard(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CLoginWizard::CLoginWizard(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

CLoginWizard::~CLoginWizard()
{
}


BEGIN_MESSAGE_MAP(CLoginWizard, CPropertySheet)
END_MESSAGE_MAP()


// CLoginWizard message handlers

void CLoginWizard::create(CWnd* parent)
{
	AddPage(&m_accountPage);
	AddPage(&m_charPage);

	SetWizardMode();	
}