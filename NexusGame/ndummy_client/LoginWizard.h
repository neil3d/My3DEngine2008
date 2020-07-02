#pragma once
#include "LoginAccountPage.h"
#include "LoginCharPage.h"

// CLoginWizard

class CLoginWizard : public CPropertySheet
{
	DECLARE_DYNAMIC(CLoginWizard)

public:
	CLoginWizard(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CLoginWizard(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CLoginWizard();

	void create(CWnd* parent);

protected:
	CLoginAccountPage	m_accountPage;
	CLoginCharPage		m_charPage;

	DECLARE_MESSAGE_MAP()
};


