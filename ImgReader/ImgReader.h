// ImgReader.h : main header file for the ImgReader DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CImgReaderApp
// See ImgReader.cpp for the implementation of this class
//

class CImgReaderApp : public CWinApp
{
public:
	CImgReaderApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
