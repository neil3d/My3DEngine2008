
#pragma once
#include "ConsoleDlg.h"

/////////////////////////////////////////////////////////////////////////////
// COutputList window

class COutputList : public CListBox
{
// Construction
public:
	COutputList();

// Implementation
public:
	virtual ~COutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};

class CCoreLogRedirect : public nlog_output
{
public:
	CCoreLogRedirect(void)	{}
	virtual ~CCoreLogRedirect(void)	{}

	virtual void open(const nstring& param){}
	virtual void write_string(const TCHAR* txt);
	virtual void write_string(enum ELogType t, const TCHAR* txt);
	virtual void close(){}
};

class COutputWnd : public CDockablePane
{
// Construction
public:
	COutputWnd();

// Attributes
protected:
	CFont m_Font;

	CMFCTabCtrl	m_wndTabs;
	COutputList m_wndOutputDebug;
	CConsoleDlg	m_consoleDlg;

protected:
	void AdjustHorzScroll(CListBox& wndListBox);

// Implementation
public:
	virtual ~COutputWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

