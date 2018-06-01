// ImgView.cpp : implementation of the CMainView class
//

#include "stdafx.h"
#include "resource.h"

#include "MainDoc.h"
#include "MainView.h"
#include "imgfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RULER_SIZE				15
/////////////////////////////////////////////////////////////////////////////
// CMainView

IMPLEMENT_DYNCREATE(CMainView, CView)

BEGIN_MESSAGE_MAP(CMainView, CView)
	//{{AFX_MSG_MAP(CMainView)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_VIEW_Navigater, OnVIEWNavigater)
	ON_UPDATE_COMMAND_UI(ID_VIEW_Navigater, OnUpdateVIEWNavigater)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_Ruler, &CMainView::OnViewRuler)
	ON_UPDATE_COMMAND_UI(ID_VIEW_Ruler, &CMainView::OnUpdateViewRuler)
	ON_COMMAND(ID_INDICATOR_SIZE, &CMainView::OnIndicateSize)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_SIZE, &CMainView::OnUpdateIndicateSize)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainView construction/destruction


CMainView::CMainView()
{
	// TODO: add construction code here

	m_bShowRuler = TRUE;
	m_pNavigator = NULL;

}

CMainView::~CMainView()
{
	if(m_pNavigator)
		delete m_pNavigator;
	m_pNavigator = NULL;
}

BOOL CMainView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= (WS_HSCROLL |WS_VSCROLL );
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainView drawing

void CMainView::OnDraw(CDC* pDC)
{
}

//called when DOC is updated, called UpdateAllViews(NULL);
void CMainView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

}
/////////////////////////////////////////////////////////////////////////////
// CMainView diagnostics

#ifdef _DEBUG
void CMainView::AssertValid() const
{
	CView::AssertValid();
}

void CMainView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMainDoc* CMainView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMainDoc)));
	return (CMainDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainView message handlers
ImgFile*	CMainView::GetImgFile()
{
	CMainDoc* pDoc = GetDocument();
	if(!pDoc) return NULL;
	return pDoc->GetImage();
}


void CMainView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	m_view.OnUpdate(pSender, lHint, pHint);
}//calculate m_rcPort from client size, zoom factor and image size

void CMainView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if(m_bShowRuler){
		m_rulerHorizontal.MoveWindow(RULER_SIZE, 0, cx, RULER_SIZE);
		m_rulerVertical.MoveWindow(0, RULER_SIZE, RULER_SIZE, cy);
		m_view.MoveWindow(RULER_SIZE,RULER_SIZE,cx-RULER_SIZE,cy-RULER_SIZE);
	}else {
		m_view.MoveWindow(0,0,cx,cy);
	}

}


void CMainView::OnVIEWNavigater() 
{
	// TODO: Add your command handler code here
	if(!m_pNavigator)
	{
		m_pNavigator = new Navigator;
		if(m_pNavigator->Create(this,800,100))
		{
			m_pNavigator->SetImage(GetDocument()->GetImage());		
		}
	}
	else
	{
		delete m_pNavigator;
		m_pNavigator =NULL;
	}
}

void CMainView::OnUpdateVIEWNavigater(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetDocument()->GetImage()!=NULL);
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
void CMainView::OnIndicateSize()
{
}
void CMainView::OnUpdateIndicateSize(CCmdUI *pCmdUI)
{
	CMainDoc* pDoc = GetDocument();
	ImgFile* pImg = NULL;
	CString szText = "";
	if(pDoc && (pImg = pDoc->GetImage())){
		szText.Format("%dx%d", pImg->Width(), pImg->Height());
	}
	pCmdUI->SetText(szText);
}
void CMainView::OnViewRuler()
{
	// TODO: Add your command handler code here
	m_bShowRuler = 1 - m_bShowRuler;
	RECT rc;
	GetClientRect(&rc);
	if(m_bShowRuler) {
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
	//CView::OnHScroll(nSBCode, nPos, pScrollBar);
	if (nSBCode == SB_THUMBPOSITION  || nSBCode == SB_THUMBTRACK) {
		if(m_view.SetScrollPos(nPos, -1)){
			SCROLLINFO si;
			si.cbSize = sizeof(si);
			si.nPos = nPos;
			si.fMask = SIF_POS;
			SetScrollInfo(SB_HORZ, &si, TRUE);
		}
			
	} else {
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

