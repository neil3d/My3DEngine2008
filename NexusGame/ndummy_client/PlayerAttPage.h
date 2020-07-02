#pragma once
#include "resource.h"

// CPlayerAttPage dialog

class CPlayerAttPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CPlayerAttPage)

public:
	CPlayerAttPage();
	virtual ~CPlayerAttPage();

// Dialog Data
	enum { IDD = IDD_PLAYER_ATT_PAGE };

	void update_local_player_att();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CMFCPropertyGridCtrl m_attGrid;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnSetActive();
};
