#pragma once


// CLoginCharPage dialog

class CLoginCharPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CLoginCharPage)

public:
	CLoginCharPage();
	virtual ~CLoginCharPage();

// Dialog Data
	enum { IDD = IDD_LOGIN_CHAR_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCreateChar();
	afx_msg void OnBnClickedBtnDelChar();
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
};
