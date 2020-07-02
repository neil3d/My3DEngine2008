
// ndummy_clientView.h : interface of the CDummyClientView class
//


#pragma once
#include "afxwin.h"
#include "afxcmn.h"

class CDummyClientView : public CFormView
{
protected: // create from serialization only
	CDummyClientView();
	DECLARE_DYNCREATE(CDummyClientView)

public:
	enum{ IDD = IDD_NDUMMY_CLIENT_FORM };

// Attributes
public:
	CDummyClientDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CDummyClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
private:
	CStatic m_hud_box;
	CStatic m_render_wnd;
	DWORD	m_lastTickTime;
	
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnStnClickedRenderWnd();
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStnClickedPos();
	CProgressCtrl m_hp;
	CProgressCtrl m_ep;
	CProgressCtrl m_mp;
};

#ifndef _DEBUG  // debug version in ndummy_clientView.cpp
inline CDummyClientDoc* CDummyClientView::GetDocument() const
   { return reinterpret_cast<CDummyClientDoc*>(m_pDocument); }
#endif