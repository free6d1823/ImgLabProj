/****************************************************************************
 * Copyright (C) 2018 nFore Technology Co. Inc.,                            *
 *                                                                          *
 ****************************************************************************/
/**
 * \file HomoView.h declarations of class HomoView
 */

#pragma once

#include "ProcessView.h"
#include "HomoTool.h"
typedef struct _tagHomoCp{
	POINT pt[4];
}HomoControlPoints;
/**
 * \class HomoView
 * \brief HomoView is the image viewer for Homography matrix processing image. It handles the video rendering process by the specified process.
 *  It is controlled by its process correspondent HomoTool.
 *	It also can display gride lines on the image window.
 *  \sa HomoTool.
 */
class HomoWrap;
class HomoView : public ProcessView
{
	DECLARE_DYNAMIC(HomoView)

public:
	HomoView();
	virtual ~HomoView();
	/*\brief Draw image function for Perspective Transformation view
	* \param pDC	DC of the display
	*/
	virtual void DrawImage(CDC* pDC);
	/*\brief Sets a reference pointer to LdcWrap
	* \param pWrapper pointer to ProcessWrap derrived class
	*/	
	virtual	void SetParam(ProcessWrap* pWrapper);	
	void GetControlPoints(HomoControlPoints&  cp);
protected:
	//(u, v) destination coordinates
	//(x,y)  source coordinates
	virtual BOOL		PosMap(int u, int v, float&x, float&y);
	//interpolate the color in position x,y in source coordinates
	virtual COLORREF GetColor(BYTE* pSrc, int stride, float x, float y);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FecView)
private:
	/*! Check if ptCursor (canvas coordinate) is on a control point
	 *  if return TRUE, index returns the index of control points (0~3)
	 */
	BOOL	IsOnControlPoint(POINT ptCursor, int& index);
	BOOL	m_bModified;
	BOOL m_bDragMode;
	POINT m_ptCursorPos;	/*!< current cursor position on image*/
	POINT	m_ptPrev;			/*!< last drag position, related to focus window*/
	int	m_idSelected;				/*!< current selected point index*/
	POINT	m_ptCp[4]; /*!< 4 control points on image*/
public:

	//}}AFX_VIRTUAL
	HomoWrap* m_pWrapRef;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnPreview();

protected:

};




