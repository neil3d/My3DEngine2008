#pragma once


// CPlayerSkillPage dialog

class CPlayerSkillPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CPlayerSkillPage)

public:
	CPlayerSkillPage();
	virtual ~CPlayerSkillPage();

// Dialog Data
	enum { IDD = IDD_PLAYER_SKILL_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
