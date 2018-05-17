#pragma once
#include "afxwin.h"
#include "MainDoc.h"
#include "RulerBar.h"
#include "WorkView.h"

class CMainView : 	public CView
{
protected: // create from serialization only
	DECLARE_DYNCREATE(CMainView)
	CMainView(void);
public:
	void UpdateScrollBar(int nBar, int pos, int page, int range);
	CMainDoc* GetDocument() const;
private:
	WorkView	m_view;
	RulerCorner	m_rulerCorner;
	RulerBar	m_rulerHorizontal;
	RulerBar	m_rulerVertical;
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgView)
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
protected:
	//{{AFX_MSG(CImgView)

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnViewRuler();
	afx_msg void OnUpdateViewRuler(CCmdUI *pCmdUI);
};
inline CMainDoc* CMainView::GetDocument()const
{ return reinterpret_cast<CMainDoc*>(m_pDocument); }

