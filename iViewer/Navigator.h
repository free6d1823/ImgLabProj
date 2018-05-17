#if !defined(AFX_NAVIGATOR_H__F13FCD3B_D2B9_4FF9_B420_D7D6431CD30D__INCLUDED_)
#define AFX_NAVIGATOR_H__F13FCD3B_D2B9_4FF9_B420_D7D6431CD30D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Navigator.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Navigator window
#include "Dib.h"

class CMainView;
class Navigator : public CWnd
{
// Construction
public:
	Navigator();

	CRect m_rcImg;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Navigator)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL SetImage(CDib* pDib);
	BOOL Create(CWnd* pWnd, int x, int y);
	virtual ~Navigator();
	// Generated message map functions
protected:
	CMainView* m_pParent;
	//{{AFX_MSG(Navigator)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAVIGATOR_H__F13FCD3B_D2B9_4FF9_B420_D7D6431CD30D__INCLUDED_)
