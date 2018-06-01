// iOptics.h : main header file for the iOptics application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMainApp:
// See iOptics.cpp for the implementation of this class
//

class CMainApp : public CWinApp
{
public:
	CMainApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMainApp theApp;
class CMainDoc;
CMainDoc* AfxGetDocument();
