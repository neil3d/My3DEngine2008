// ConsoleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ndummy_client.h"
#include "ConsoleDlg.h"
#include "commands.h"
#include <boost/tokenizer.hpp>

console_cmd::console_cmd(const nstring& cmd_line)
{
	std::string str = conv_string( cmd_line );

	typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
	boost::char_separator<char> sep(",");
	tokenizer tok(str, sep);

	for(tokenizer::iterator beg=tok.begin(); beg!=tok.end();++beg)
	{
		std::string tok_str = *beg;
		m_tokens.push_back( conv_string(tok_str) );
	}
}

nstring console_cmd::get_command() const
{
	nstring ret;
	if( !m_tokens.empty() )
		ret = m_tokens[0];
	return ret;
}

int console_cmd::get_num_arg() const
{
	if(m_tokens.empty())
		return 0;
	return
		m_tokens.size()-1;
}

nstring console_cmd::get_arg_string(int i) const
{
	nstring ret;
	if (m_tokens.size() > 0)
	{
		ret = m_tokens[i+1];
	}
	return ret;
}

// CConsoleDlg dialog
CConsoleDlg* g_console_dlg = NULL;
IMPLEMENT_DYNAMIC(CConsoleDlg, CDialog)

CConsoleDlg::CConsoleDlg()
	: CDialog(CConsoleDlg::IDD)
{
	g_console_dlg = this;
}

CConsoleDlg::~CConsoleDlg()
{
	g_console_dlg = NULL;
}

void CConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONSOLE_TEXT, m_textEdit);
	DDX_Control(pDX, IDC_CMD_INPUT, m_inputEdit);
	DDX_Control(pDX, IDC_BTN_INPUT, m_btnEnter);
}


BEGIN_MESSAGE_MAP(CConsoleDlg, CDialog)
	ON_WM_CREATE()	
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_INPUT, &CConsoleDlg::OnBnClickedBtnInput)
END_MESSAGE_MAP()


// CConsoleDlg message handlers

CConsoleDlg* CConsoleDlg::instance()
{
	return g_console_dlg;
}

int CConsoleDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	
	return 0;
}

BOOL CConsoleDlg::OnInitDialog( )
{
	register_cmd( _T("help"), _T("控制台帮助命令"), boost::bind(&CConsoleDlg::CmdHelp, this, _1) );
	register_cmd( _T("chat"), _T("chat,<type>,<msg>"), boost::bind(&commands::handle_chat, commands::instance(), _1) );
	register_cmd( _T("cast"), _T("cast,<spell_id>"), boost::bind(&commands::handle_cast_spell, commands::instance(), _1) );

	return CDialog::OnInitDialog();
}
void CConsoleDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if( !m_textEdit.GetSafeHwnd())
		return;

	const int INPUT_H = 20;
	const int BTN_W = 32;
	CRect rc;
	GetClientRect(&rc);

	m_textEdit.MoveWindow(2, 2, rc.Width()-2, rc.Height()-INPUT_H-4);
	m_inputEdit.MoveWindow(2, rc.Height()-INPUT_H-2, rc.Width()-BTN_W-6, INPUT_H);
	m_btnEnter.MoveWindow(rc.Width()-BTN_W-2, rc.Height()-INPUT_H-2, BTN_W, INPUT_H);
}

void CConsoleDlg::register_cmd(const nstring& cmd, const nstring& help_str, console_cmd_handler handler)
{
	handler_info h={cmd, help_str, handler};
	m_handlers.push_back(h);	
}

void CConsoleDlg::CmdHelp(const console_cmd& cmd)
{
	print_line(_T("已注册的控制台命令："));
	
	for(st_handler_array::iterator iter = m_handlers.begin();
		iter != m_handlers.end();
		++iter)
	{
		const handler_info& h = *iter;
		std::wostringstream ss;
		ss << h.cmd << _T(" : ") << h.help;
		print_line( ss.str().c_str() );
	}
}

void CConsoleDlg::OnBnClickedBtnInput()
{
	// TODO: Add your control notification handler code here
	CString inputStr;
	m_inputEdit.GetWindowText(inputStr);
	m_inputEdit.SetWindowText(_T(""));

	CString outputStr;
	outputStr.Format(_T(">%s"), inputStr.GetString());
	print_line(outputStr.GetString());

	//-- 执行命令
	console_cmd cmd(inputStr.GetString());
	nstring cmd_key = cmd.get_command();
	if( !cmd_key.empty() )
	{
		int cmd_index = find_handler(cmd_key);
		if( cmd_index!=-1 )
		{
			console_cmd_handler hdl = m_handlers[cmd_index].func;
			hdl(cmd);
		}
		else
		{
			print_line(_T("无效的控制台命令"));
		}
	}
}

void CConsoleDlg::print_line(const TCHAR* str)
{
	if (m_textEdit.GetLineCount() > 500)
	{
		m_textEdit.SetWindowText(_T(""));
	}

	CString txt;
	m_textEdit.GetWindowText(txt);
	txt += str;
	txt += _T("\r\n");
	m_textEdit.SetWindowText(txt);
	m_textEdit.LineScroll( m_textEdit.GetLineCount() );
}

int CConsoleDlg::find_handler(const nstring& cmd)
{
	for (size_t i=0; i<m_handlers.size(); i++)
	{
		if( cmd == m_handlers[i].cmd )
			return i;
	}
	return -1;
}