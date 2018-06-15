/****************************************************************************
 * Copyright (C) 2018 nFore Technology Co. Inc.,                            *
 *                                                                          *
 ****************************************************************************/
/**
 * \file FecView.h declarations of class FecView
 */

#pragma once

#include "ProcessView.h"
#include "FecTool.h"

/**
 * \class FecView
 * \brief FecView is the image viewer for FEC processing image. It handles the video rendering process by the specified process.
 *  It is controlled by its process correspondent FecTool.
 *	It also can display gride lines on the image window.
 *  \sa FecTool.
 */
class FecView : public ProcessView
{
	DECLARE_DYNAMIC(FecView)

public:
	FecView();
	virtual ~FecView();
	/*\brief Sets the original mage. This is a reference image pointer. ProcessView won't change its content 
	* \param pImg the source image which is sued as a source reference image
	*/
	virtual	void SetImgFile(ImgFile* pImg);
	
	/*\brief Sets a reference pointer to FecWrap
	* \param pWrapper pointer to ProcessWrap derrived class
	*/
	virtual	void SetParam(ProcessWrap* pWrapper);
protected:
	//(u, v) destination coordinates
	//(x,y)  source coordinates
	virtual BOOL		PosMap(int u, int v, float&x, float&y);
	//interpolate the color in position x,y in source coordinates
	virtual COLORREF GetColor(BYTE* pSrc, int stride, float x, float y);
private:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FecView)
public:

	//}}AFX_VIRTUAL
	FecWrap* m_pParamRef;//reference pointer to ProcessTool's Parametersettings capulator class
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPreview();

protected:

};


