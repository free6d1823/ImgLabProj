#if !defined(AFX_EASYTIP_H__2EACC17B_19EA_4E23_B3F7_672DC4389E46__INCLUDED_)
#define AFX_EASYTIP_H__2EACC17B_19EA_4E23_B3F7_672DC4389E46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Easytip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Easytip window

class Easytip : public CWnd
{
// Construction
public:
	Easytip();
    BOOL Create( CWnd* pParentWnd, DWORD dwStyle = 0 );
    BOOL AddTool( CWnd* pWnd, LPCTSTR lpszText = LPSTR_TEXTCALLBACK, LPCRECT lpRectTool = NULL, UINT nIDTool = 0 );
    void Activate( BOOL bActivate );
    void RelayEvent( LPMSG lpMsg );
    void SetDelayTime( UINT nDelay ){m_nDelay = nDelay;};
	void ShowText(POINT pt, LPCTSTR szText);
// Attributes

public:

protected:
    UINT    m_nState;
    CString m_szText;
    UINT m_nDelay;
    UINT    m_nShowTime;
    UINT m_nTick;
    CRect m_rcTool;
    UINT  m_nIDTool;
    CWnd*   m_pOwner;
    CFont   m_font;
    int     m_nDelayMove;
	POINT	m_ptLast;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Easytip)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~Easytip();

	// Generated message map functions
protected:
	BOOL ShowTip();
	//{{AFX_MSG(Easytip)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EASYTIP_H__2EACC17B_19EA_4E23_B3F7_672DC4389E46__INCLUDED_)
