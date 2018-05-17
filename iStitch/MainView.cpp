#include "StdAfx.h"
#include "resource.h"
#include "WorkView.h"
#include "MainView.h"
///////////////////////////////////////////////
#define RULER_SIZE 15
IMPLEMENT_DYNCREATE(CMainView, CView)

BEGIN_MESSAGE_MAP(CMainView, CView)
	//{{AFX_MSG_MAP(CImgView)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_RULER, OnViewRuler)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RULER, OnUpdateViewRuler)
END_MESSAGE_MAP()

CMainView::CMainView(void)
{
}

CMainView::~CMainView(void)
{
}
BOOL CMainView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= (WS_HSCROLL |WS_VSCROLL );
	return CView::PreCreateWindow(cs);
}
void CMainView::OnDraw(CDC* pDC)
{
}
void CMainView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	m_view.OnInitialUpdate();
}
void CMainView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	m_view.OnUpdate(pSender, lHint, pHint);
}
void CMainView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if(m_rulerHorizontal.IsWindowVisible()){
		m_rulerHorizontal.MoveWindow(RULER_SIZE, 0, cx, RULER_SIZE);
		m_rulerVertical.MoveWindow(0, RULER_SIZE, RULER_SIZE, cy);
		m_view.MoveWindow(RULER_SIZE,RULER_SIZE,cx-RULER_SIZE,cy-RULER_SIZE);
	}else {
		m_view.MoveWindow(0,0,cx,cy);
	}

}

int CMainView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_rulerCorner.Create(this);
	m_rulerHorizontal.Create(this, TRUE);
	m_rulerVertical.Create(this, FALSE);
	m_view.Create(this);
	return 0;
}
void CMainView::OnViewRuler()
{
	// TODO: Add your command handler code here
	BOOL bShowRuler = m_rulerHorizontal.IsWindowVisible();
	bShowRuler = 1 - bShowRuler;
	RECT rc;
	GetClientRect(&rc);
	if(bShowRuler) {
		m_rulerCorner.ShowWindow(SW_SHOW);
		m_rulerHorizontal.ShowWindow( SW_SHOW);
		m_rulerVertical.ShowWindow( SW_SHOW);
		m_view.MoveWindow(RULER_SIZE,RULER_SIZE,rc.right-RULER_SIZE,rc.bottom-RULER_SIZE);

	} else {
		m_rulerCorner.ShowWindow(SW_HIDE);
		m_rulerHorizontal.ShowWindow( SW_HIDE);
		m_rulerVertical.ShowWindow( SW_HIDE);
		m_view.MoveWindow(0,0, rc.right,rc.bottom);
	}
}

void CMainView::OnUpdateViewRuler(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

}
void CMainView::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_view.SetFocus();
}
BOOL CMainView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_view.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	{
		return TRUE;
	}
	// otherwise, do default handling
	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
//nBar = SB_VERT or SB_HORZ
void CMainView::UpdateScrollBar(int nBar, int pos, int page, int range)
{
	if (pos >= 0) {
			SCROLLINFO si;
			si.cbSize = sizeof(si);
			si.nMin = 0;
			si.nMax = range;
			si.nPos = pos;
			si.nPage = page;
			si.fMask = SIF_PAGE|SIF_POS|SIF_RANGE;
			SetScrollInfo(nBar, &si, TRUE);

	}else {
			ShowScrollBar(nBar, FALSE);
	}
}
void CMainView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (nSBCode == SB_THUMBPOSITION  || nSBCode == SB_THUMBTRACK) {
			if(m_view.SetScrollPos(nPos, -1)){
				SCROLLINFO si;
				si.cbSize = sizeof(si);
				si.nPos = nPos;
				si.fMask = SIF_POS;
				SetScrollInfo(SB_HORZ, &si, TRUE);
			}
	}
	else {
				SCROLLINFO si;
				si.cbSize = sizeof(si);
				si.fMask = SIF_POS;
				GetScrollInfo(SB_HORZ, &si);
				switch(nSBCode)
				{
				case SB_LINELEFT:  si.nPos -= 1; break;
				case SB_LINERIGHT:  si.nPos += 1; break;
				case SB_PAGELEFT: si.nPos -= si.nPage;break;
				case SB_PAGERIGHT: si.nPos += si.nPage;break;
				default: //no support
					return;
				}
				if(si.nPos < si.nMin) si.nPos  = si.nMin;
				if(si.nPos> si.nMax - (int)si.nPage) si.nPos  =si.nMax - (int)si.nPage+1;
				SetScrollInfo(SB_HORZ, &si, TRUE);
				m_view.SetScrollPos(si.nPos, -1);
	}
}

void CMainView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (nSBCode == SB_THUMBPOSITION  || nSBCode == SB_THUMBTRACK) {
		if(m_view.SetScrollPos(-1, nPos)){
			SCROLLINFO si;
			si.cbSize = sizeof(si);
			si.nPos = nPos;
			si.fMask = SIF_POS;
			SetScrollInfo(SB_VERT, &si, TRUE);
		}

	}else {
				SCROLLINFO si;
				si.cbSize = sizeof(si);
				si.fMask = SIF_POS;
				GetScrollInfo(SB_VERT, &si);
				switch(nSBCode)
				{
				case SB_LINELEFT:  si.nPos -= 1; break;
				case SB_LINERIGHT:  si.nPos += 1; break;
				case SB_PAGELEFT: si.nPos -= si.nPage;break;
				case SB_PAGERIGHT: si.nPos += si.nPage;break;
				default: //no support
					return;
				}
				if(si.nPos < si.nMin) si.nPos  = si.nMin;
				if(si.nPos> si.nMax - (int)si.nPage) si.nPos  =si.nMax - (int)si.nPage+1;
				SetScrollInfo(SB_VERT, &si, TRUE);
				m_view.SetScrollPos(-1, si.nPos);
	}
}