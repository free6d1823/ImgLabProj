#pragma once

#include "Gride.h"
#include "ControlBar.h"

#define MAX_ZOOM		10
#define DEFAULT_ZOOM_FACTOR  	5

class CMainView;
class CMainDoc;

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
	void DocToViewPort(SIZE& size);
	void DocToViewPort(RECT& rect);
	void ViewPortToDoc(SIZE& size);
	void ViewPortToDoc(RECT& rect);

	ControlBar		m_ControlBar;
	static int ZOOM_FACTOR[MAX_ZOOM];
	CPoint	m_ptCursorPos;//current cursor position on original image
	void CalculateRects();
	BOOL			m_bShowGride;
	Gride			m_gride;
	int m_nZoomFactor;
	CRect	m_rcPort;			//顯示試窗對應到Workspace的作標, top-left is scroll poition
											//Shif t 到top-left 即為client Window 
	CSize	m_szWorkspace;	//size of total area, the scroll area
	CRect	m_rcCanvas;		//canvas location, related to Workspace
	int				m_nCurrentSelectedImage;
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
