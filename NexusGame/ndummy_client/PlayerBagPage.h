#pragma once
#include "afxwin.h"


// CPlayerBagPage dialog

class CPlayerBagPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CPlayerBagPage)

public:
	CPlayerBagPage();
	virtual ~CPlayerBagPage();

// Dialog Data
	enum { IDD = IDD_PLAYER_BAG_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnBagDrop();
private:
	CListBox m_listBox;
public:
	virtual BOOL OnInitDialog();
};
