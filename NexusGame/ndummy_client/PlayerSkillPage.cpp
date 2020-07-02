// PlayerSkillPage.cpp : implementation file
//

#include "stdafx.h"
#include "ndummy_client.h"
#include "PlayerSkillPage.h"


// CPlayerSkillPage dialog

IMPLEMENT_DYNAMIC(CPlayerSkillPage, CPropertyPage)

CPlayerSkillPage::CPlayerSkillPage()
	: CPropertyPage(CPlayerSkillPage::IDD)
{

}

CPlayerSkillPage::~CPlayerSkillPage()
{
}

void CPlayerSkillPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPlayerSkillPage, CPropertyPage)
END_MESSAGE_MAP()


// CPlayerSkillPage message handlers
