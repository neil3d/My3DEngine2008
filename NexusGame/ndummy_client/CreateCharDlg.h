#pragma once


// CCreateCharDlg dialog

class CCreateCharDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateCharDlg)

public:
	CCreateCharDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCreateCharDlg();

// Dialog Data
	enum { IDD = IDD_DLG_CREATE_CHAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
