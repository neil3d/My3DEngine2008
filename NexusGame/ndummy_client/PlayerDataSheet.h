#pragma once
#include "PlayerAttPage.h"
#include "PlayerBagPage.h"
#include "PlayerSkillPage.h"
#include "PlayerQuestPage.h"

// CPlayerDataSheet

class CPlayerDataSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CPlayerDataSheet)

public:
	CPlayerDataSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPlayerDataSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CPlayerDataSheet();

	void create(CWnd* parent);
protected:
	CPlayerAttPage	m_playerAttPage;
	CPlayerBagPage	m_playerBagPage;
	CPlayerSkillPage m_playerSkillPage;
	CPlayerQuestPage m_playeQuestPage;

	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);

	bool m_created;
};


