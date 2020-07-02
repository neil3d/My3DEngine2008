#pragma once


// CPlayerQuestPage dialog

class CPlayerQuestPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CPlayerQuestPage)

public:
	CPlayerQuestPage();
	virtual ~CPlayerQuestPage();

// Dialog Data
	enum { IDD = IDD_PLAYER_QUEST_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
