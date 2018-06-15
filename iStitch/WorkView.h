/****************************************************************************
 * Copyright (C) 2018 nFore Technology Co. Inc.,                                                                              *
 *                                                                                                                                                                       *
 ****************************************************************************/
/**
 * \file WorkView.h : interface of the WorkView class
 */

#pragma once

#include "Gride.h"
#include "ControlBar.h"

#define MAX_ZOOM		18


class CMainView;
class CMainDoc;
/**
 * \class WorkView 
 * \brief WorkView is a workspace for operating stitching and viewing images
  */
class WorkView : public CWnd
{
		DECLARE_DYNAMIC(WorkView)
public:
	WorkView(void);
	~WorkView(void);
	BOOL	Create(CWnd* pParent);
	BOOL SetScrollPos(int nX, int nY);
	virtual void UpdateScrollbarPosition();
	void SetCurrentSel(int nSel);
	CMainDoc* GetDocument();
	void DrawImages(CDC* pDC);
virtual void OnInitialUpdate(); // called first time after construct
virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(WorkView)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
protected:
	CMainView* m_parent;
//convert image size to Scren coord
	void DocToViewPort(SIZE& size); /*!<convert position on original to zoomed, related to m_szWorkspace*/
	void DocToViewPort(RECT& rect);  /*!<convert position on original to zoomed, related to m_szWorkspace*/
	void ViewPortToDoc(SIZE& size); /*!<convert position on zoomed workspace to original doc*/
	void ViewPortToDoc(RECT& rect);/*!<convert position on zoomed workspace to original doc*/
	void CalculateRects();		/*!<recalculate m_szWorkspace, m_rcCanvas, m_rcPort, scrollbar position, when Window size changed, image changed*/

	ControlBar		m_ControlBar;		/*!<The stitching controller*/
	static int ZOOM_FACTOR[MAX_ZOOM];
	CPoint	m_ptCursorPos;/*!<current cursor position on original image*/
	BOOL			m_bShowGride;		/*!< TRUE to display gride */
	Gride			m_gride;
	int m_nZoomFactor;
	CRect	m_rcPort;			/*!< the display area of window (client area of WorkView) mapped to workspace. top-left is the scroll poition on window 	*/
	CSize	m_szWorkspace;	/*!<  size of total area, including border (gray ) */
	CRect	m_rcCanvas;			/*!< The zoomed canvas area on Workspace */
	int				m_nCurrentSelectedImage; /*!< Current image id under controlling */
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnViewZoomin();
	afx_msg void OnUpdateViewZoomin(CCmdUI* pCmdUI);
	afx_msg void OnViewZoomout();
	afx_msg void OnUpdateViewZoomout(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnViewGride();
	afx_msg void OnUpdateViewGride(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicatePos(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicateSize(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicateZoomFactor(CCmdUI *pCmdUI);
	afx_msg void OnViewController();
	afx_msg void OnUpdateViewController(CCmdUI *pCmdUI);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
