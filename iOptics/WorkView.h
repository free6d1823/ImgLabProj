/****************************************************************************
 * Copyright (C) 2018 nFore Technology Co. Inc.,                            *
 *                                                                          *
 ****************************************************************************/
/**
 * \file WorkView.h declarations of class WorkView
 */
#pragma once


// WorkView
#include "resource.h"
#include "Gride.h"
#include "Mesh.h"
#include "FecTool.h"
#include "HomoGride.h"

class CMainDoc;
class CMainView;
class BasicView;

/**
 * \class WorkView
 * \brief WorkView is the current opened image display window. It can display gride and mesh for comparision with processed image.
 *	It also provides zooming and scrolling functions for viewing.
 */
class WorkView : public BasicView
{
	DECLARE_DYNAMIC(WorkView)

public:
	WorkView();
	virtual ~WorkView();
	BOOL	Create(CWnd* pParent);
	BOOL SetScrollPos(int nX, int nY);
	virtual void UpdateScrollbarPosition();
	CMainDoc* GetDocument();
	ImgFile* GetImage();
	/*!< Set current tool settings to WorkView. The content is defined dependently by current ProcessTool */
	void	SetToolParam(void* pParam);

	/*!< Get current tool settings on WorkView. The content is defined dependently by current ProcessTool */
	void	GetToolParam(void* pParam);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	POINT	m_ptCursorPos;/*!< current cursor position on original image */
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainView)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
protected:

protected:
	CMainView*	m_parent;
	ProcessTool*		m_pProcessTool;/*!< pointer to current process control dialog box*/

	//gride
	BOOL			m_bShowGride;
	Gride		m_gride;
	ToolGride*	m_pToolGride;	/*!< facility line for the selected processing tool*/

	//mesh
	BOOL			m_bShowMesh;
	Mesh			m_mesh;/*!< mesh control class */

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
	afx_msg void OnViewMesh();
	afx_msg void OnUpdateViewMesh(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicatePos(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicateColor(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicateZoomFactor(CCmdUI *pCmdUI);
	afx_msg void OnProcessFec();
	afx_msg void OnUpdateProcessFec(CCmdUI *pCmdUI);
	afx_msg void OnProcessLdc();
	afx_msg void OnUpdateProcessLdc(CCmdUI *pCmdUI);
	afx_msg void OnProcessHomo();
	afx_msg void OnUpdateProcessHomo(CCmdUI *pCmdUI);
	afx_msg void OnProcessExit();
	afx_msg void  OnToolboxApply();
	afx_msg void OnProcessPreview();
	afx_msg void OnUpdateToolbox(CCmdUI *pCmdUI);
	afx_msg void OnCommandToolbox(UINT nCmd);
};


