#pragma once
#include "basicview.h"
#include "Gride.h"
#include "FecTool.h"

/**
 * \class ProcessView
 * \brief ProcessView is the basic image viewer for processing image viewing. It handles the video rendering process by the specified process.
 *  It is controlled by its process correspondent ProcessTool.
 *	It also can display gride lines on the image window.
 *  \sa ProcessTool.
 */

class ProcessWrap;
class ProcessView : public BasicView
{
	DECLARE_DYNAMIC(ProcessView)

public:
	ProcessView();
	virtual ~ProcessView();
	/*\brief Sets the original mage. This is a reference image pointer. ProcessView won't change its content 
	* \param pImg the source image which is sued as a source reference image
	*/
	virtual	void SetImgFile(ImgFile* pImg);
	
	/*\brief Sets a reference pointer to the process parameters wrapper class. 
	* \param pWrapper pointer to ProcessWrap derrived class
	*/	
	virtual	void SetParam(ProcessWrap* pWrapper){};

protected:
	/*\brief Position mapping from processed image to original image
	* \param (u, v) destination coordinates
	* \param (x, y) the ideal position in source image
	*/
	virtual BOOL		PosMap(int u, int v, float&x, float&y);

	/*\brief Gets the color value of the ideal position in source image.This implys the color is interpolated by its neighborhood pixels.
	* \param pSrc source image to be referenced
	* \param stride byes-per-line of Src
	* \param (x,y) the ideal position in source image
	*/
	virtual COLORREF GetColor(BYTE* pSrc, int stride, float x, float y);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(BasicView)
public:
	BOOL SetScrollPos(int nX, int nY);
	BOOL			m_bShowGride;
	Gride m_gride;
	//}}AFX_VIRTUAL
	ImgFile*	m_pOriginalImg;	/*<! the reference pointer to original image */

	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnPreview();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnViewGride();
	afx_msg void OnUpdateViewGride(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolbox(CCmdUI *pCmdUI);
	afx_msg void OnCommandToolbox(UINT nCmd);
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
protected:

};
