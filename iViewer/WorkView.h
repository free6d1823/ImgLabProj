#pragma once


// WorkView
#include "resource.h"
#include "Gride.h"
#include "Mesh.h"
#include "MainDoc.h"
#include "MainView.h"
#include "BasicView.h"
#include "FecTool.h"
class CMainView;
class BasicView;

typedef enum PROCESS_MODE {
	PM_NONE = 0,
	PM_FEC = ID_PROCESS_FEC,
	PM_LDC=ID_PROCESS_LDC
};

class WorkView : public BasicView
{
	DECLARE_DYNAMIC(WorkView)

public:
	WorkView();
	virtual ~WorkView();
	BOOL	Create(CWnd* pParent);
	//if nX or nY negative, don't move picture  in that direction
	//return TRUE if scroll successfully
	BOOL SetScrollPos(int nX, int nY);
	virtual void UpdateScrollbarPosition();
	CMainDoc* GetDocument();
	ImgFile* GetImage();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	PROCESS_MODE m_eCurrentProcessMode;
	POINT	m_ptCursorPos;//current cursor position on original image
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainView)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
protected:

protected:
	CMainView*	m_parent;
	ProcessTool*		m_pProcessTool;

	//gride
	BOOL			m_bShowGride;
	Gride			m_gride;
	Gride*	m_pToolGride;	//gride line for the selected processing tool

	//mesh
	BOOL			m_bShowMesh;
	Mesh			m_mesh;

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
	afx_msg void OnProcessExit();
	afx_msg void  OnToolboxApply();
	afx_msg void OnProcessPreview();
	afx_msg void OnUpdateToolbox(CCmdUI *pCmdUI);
	afx_msg void OnCommandToolbox(UINT nCmd);
};


