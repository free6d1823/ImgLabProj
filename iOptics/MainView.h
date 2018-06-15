// ImgView.h : interface of the CMainView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MAINVIEW_H_)
#define MAINVIEW_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Navigator.h"
#include "RulerBar.h"

class WorkView;
class CMainView : public CView
{
protected: // create from serialization only
	CMainView();
	DECLARE_DYNCREATE(CMainView)

// Attributes
public:
	void UpdateScrollBar(int nBar, int pos, int page, int range);
	CMainDoc* GetDocument();
	ImgFile*	GetImgFile();
	Navigator* m_pNavigator;

private:
	WorkView	m_view;
	BOOL			m_bShowRuler;

	RulerCorner	m_rulerCorner;
	RulerBar	m_rulerHorizontal;
	RulerBar	m_rulerVertical;
// Operations
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMainView)

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVIEWNavigater();
	afx_msg void OnUpdateVIEWNavigater(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnViewRuler();
	afx_msg void OnUpdateViewRuler(CCmdUI *pCmdUI);
	afx_msg void OnIndicateSize();
	afx_msg void OnUpdateIndicateSize(CCmdUI *pCmdUI);
protected:
};

#ifndef _DEBUG  // debug version in ImgView.cpp
inline CMainDoc* CMainView::GetDocument()
   { return (CMainDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(MAINVIEW_H_)
