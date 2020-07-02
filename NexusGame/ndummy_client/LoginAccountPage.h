#pragma once


// CLoginAccountPage dialog

class CLoginAccountPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CLoginAccountPage)

public:
	CLoginAccountPage();
	virtual ~CLoginAccountPage();

// Dialog Data
	enum { IDD = IDD_LOGIN_ACCOUNT_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual LRESULT OnWizardNext();

	// add by leo
	afx_msg void OnBnClickedBatchLogin();

private:
	// add by leo
	bool	m_batch_login;
	CWnd*	m_num_static;
	CWnd*	m_num_edit;


private:
	// add by leo
	void init_num_wnd(void);
	void show_num_wnd(bool bshow);
	void get_num_wnd(void);

	// 发送验证登陆消息
	void send_login_proof_msg(char* name, char* psd);

	// 单独登陆
	void single_login(void);

	// 批量登陆
	void batch_login(void);
 
};
