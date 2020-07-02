#pragma once
#include "resource.h"
#include "afxwin.h"
#include <boost/lexical_cast.hpp>

class console_cmd
{
public:
	console_cmd(const nstring& cmd_line);

	nstring get_command() const;
	int get_num_arg() const;
	nstring get_arg_string(int i) const;
	
	template<typename T>
	bool get_arg(int i, T& arg) const
	{
		try
		{
			arg = boost::lexical_cast<T>( get_arg_string(i) );
			return true;
		}
		catch (boost::bad_lexical_cast&)
		{
			arg = T();
			return false;
		}		
	}

private:
	std::vector<nstring> m_tokens;
};

typedef boost::function<void(const console_cmd&)> console_cmd_handler;

// CConsoleDlg dialog

class CConsoleDlg : public CDialog
{
	DECLARE_DYNAMIC(CConsoleDlg)

public:
	CConsoleDlg();
	virtual ~CConsoleDlg();

// Dialog Data
	enum { IDD = IDD_CONSOLE_DLG };

	static CConsoleDlg* instance();
	virtual BOOL OnInitDialog( );

	void register_cmd(const nstring& cmd, const nstring& help_str, console_cmd_handler handler);
	void print_line(const TCHAR* str);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void CmdHelp(const console_cmd& cmd);
	
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
private:
	CEdit m_textEdit;
	CEdit m_inputEdit;
	CButton m_btnEnter;

	struct handler_info
	{
		nstring cmd;
		nstring help;
		console_cmd_handler func;
	};

	int find_handler(const nstring& cmd);
	typedef std::vector<handler_info> st_handler_array;
	st_handler_array	m_handlers;
public:
	afx_msg void OnBnClickedBtnInput();
};
