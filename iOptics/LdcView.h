/****************************************************************************
 * Copyright (C) 2018 nFore Technology Co. Inc.,                            *
 *                                                                          *
 ****************************************************************************/
/**
 * \file LdcView.h declarations of class LdcView
 */

#pragma once

#include "ProcessView.h"
#include "LdcTool.h"

/**
 * \class LdcView
 * \brief LdcView is the image viewer for LDC processing image. It handles the video rendering process by the specified process.
 *  It is controlled by its process correspondent LdcTool.
 *	It also can display gride lines on the image window.
 *  \sa LdcTool.
 */
class LdcWrap;
class LdcView : public ProcessView
{
	DECLARE_DYNAMIC(LdcView)

public:
	LdcView();
	virtual ~LdcView();

	/*\brief Sets a reference pointer to LdcWrap
	* \param pWrapper pointer to ProcessWrap derrived class
	*/	
	virtual	void SetParam(ProcessWrap* pWrapper){m_pLdcWrap = (LdcWrap*) pWrapper;};	

protected:
	//(u, v) destination coordinates
	//(x,y)  source coordinates
	virtual BOOL		PosMap(int u, int v, float&x, float&y);
	//interpolate the color in position x,y in source coordinates
	virtual COLORREF GetColor(BYTE* pSrc, int stride, float x, float y);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FecView)
public:

	//}}AFX_VIRTUAL
	LdcWrap* m_pLdcWrap;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPreview();

protected:

};


