// iViewer.h : main header file for the IVIEWER application
//

#if !defined(AFX_IVIEWER_H__AFAD197E_B3A0_4F1B_8711_4A90C6D104C2__INCLUDED_)
#define AFX_IVIEWER_H__AFAD197E_B3A0_4F1B_8711_4A90C6D104C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMainApp:
// See iViewer.cpp for the implementation of this class
//

class CMainApp : public CWinApp
{
public:
	CMainApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMainApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IVIEWER_H__AFAD197E_B3A0_4F1B_8711_4A90C6D104C2__INCLUDED_)
