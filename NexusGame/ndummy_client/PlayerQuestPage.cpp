// PlayerQuestPage.cpp : implementation file
//

#include "stdafx.h"
#include "ndummy_client.h"
#include "PlayerQuestPage.h"


// CPlayerQuestPage dialog

IMPLEMENT_DYNAMIC(CPlayerQuestPage, CPropertyPage)

CPlayerQuestPage::CPlayerQuestPage()
	: CPropertyPage(CPlayerQuestPage::IDD)
{

}

CPlayerQuestPage::~CPlayerQuestPage()
{
}

void CPlayerQuestPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPlayerQuestPage, CPropertyPage)
END_MESSAGE_MAP()


// CPlayerQuestPage message handlers
