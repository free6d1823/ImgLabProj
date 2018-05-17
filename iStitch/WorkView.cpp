#include "StdAfx.h"
#include "resource.h"
#include "MainView.h"
#include "MainDoc.h"
#include "WorkView.h"

#define DEFAULT_VISIBLE_WIDTH	1280
#define DEFAULT_VISIBLE_HEIGHT	740
#define FORGND_COLOR			RGB(255,255,255)
#define BACKGND_COLOR	RGB(130,130,130)
#define SELECTED_COLOR		RGB(0,60,200)
#define BORDER_SIZE		20
int WorkView::ZOOM_FACTOR[MAX_ZOOM] = { 10, 25, 33, 50, 75, 100,150, 200, 300, 400};

// WorkView

IMPLEMENT_DYNAMIC(WorkView, CWnd)

WorkView::WorkView(void) : m_gride(RGB(128,100,0))
{
	m_nZoomFactor = DEFAULT_ZOOM_FACTOR;
	m_rcPort = CRect(0,0,800,600);
	m_rcCanvas = m_rcPort;
	m_bShowGride = FALSE;
	m_parent = NULL;
	m_ptCursorPos = CPoint(0,0);
	m_nCurrentSelectedImage = -1;
}

WorkView::~WorkView(void)
{
}
void WorkView::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: Add your message handler code here
}

CMainDoc* WorkView::GetDocument()
{
	if (m_parent)
		return m_parent->GetDocument();
	return NULL;
}

void WorkView::DocToViewPort(SIZE& size)
{
	size.cx = size.cx * ZOOM_FACTOR[m_nZoomFactor] /100;
	size.cy = size.cy * ZOOM_FACTOR[m_nZoomFactor] /100;
}
void WorkView::DocToViewPort(RECT& rect)
{
	rect.left = rect.left  * ZOOM_FACTOR[m_nZoomFactor] /100;
	rect.right = rect.right * ZOOM_FACTOR[m_nZoomFactor] /100;
	rect.top = rect.top * ZOOM_FACTOR[m_nZoomFactor] /100;
	rect.bottom = rect.bottom * ZOOM_FACTOR[m_nZoomFactor] /100;
}
void WorkView::ViewPortToDoc(SIZE& size)
{
	ASSERT(ZOOM_FACTOR[m_nZoomFactor] > 0);
	size.cx = size.cx * 100 / ZOOM_FACTOR[m_nZoomFactor];
	size.cy = size.cy * 100 / ZOOM_FACTOR[m_nZoomFactor];
}
void WorkView::ViewPortToDoc(RECT& rect)
{
	ASSERT(ZOOM_FACTOR[m_nZoomFactor] > 0);
	rect.left = rect.left * 100 / ZOOM_FACTOR[m_nZoomFactor];
	rect.right = rect.right * 100 / ZOOM_FACTOR[m_nZoomFactor];
	rect.top = rect.top * 100 / ZOOM_FACTOR[m_nZoomFactor];
	rect.bottom = rect.bottom * 100 / ZOOM_FACTOR[m_nZoomFactor];
}

BEGIN_MESSAGE_MAP(WorkView, CWnd)
		ON_WM_PAINT()
		ON_WM_SIZE()
		ON_COMMAND(ID_VIEW_GRIDE, OnViewGride)
		ON_UPDATE_COMMAND_UI(ID_VIEW_GRIDE, OnUpdateViewGride)
		ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
		ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMIN, OnUpdateViewZoomin)
		ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
		ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT, OnUpdateViewZoomout)
		ON_UPDATE_COMMAND_UI(ID_INDICATOR_POS, OnUpdateIndicatePos)
		ON_UPDATE_COMMAND_UI(ID_INDICATOR_SIZE, OnUpdateIndicateSize)
		ON_UPDATE_COMMAND_UI(ID_INDICATOR_ZOOM, OnUpdateIndicateZoomFactor)
		ON_WM_MOUSEMOVE()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_CREATE()
		ON_WM_DESTROY()
		ON_COMMAND(ID_VIEW_CONTROLLER, &WorkView::OnViewController)
		ON_UPDATE_COMMAND_UI(ID_VIEW_CONTROLLER, &WorkView::OnUpdateViewController)
		ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// WorkView message handlers
BOOL	WorkView::Create(CWnd* pParent)
{
	m_parent = (CMainView*) pParent;
	LPCTSTR lpszClass = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW|CS_DBLCLKS);
	return CreateEx(0, lpszClass, _T("WorkView"),WS_CHILD|WS_VISIBLE,
			CRect(0, 0, 800, 600), pParent, NULL,NULL);
}
int WorkView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_ControlBar.Create(IDD_CONTROLLER, this))
			return -1;
	m_ControlBar.ShowWindow(SW_HIDE);
	return 0;
}
// WorkView message handlers
void WorkView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	CalculateRects();
	UpdateScrollbarPosition();
	m_gride.SetImageArea(m_rcCanvas);
	m_gride.SetViewPort(m_rcPort);
}
BOOL WorkView::SetScrollPos(int nX, int nY)
{
	BOOL bOK = FALSE;

	if (nX >= 0){// && nX <= m_rcCanvas.right - m_rcPort.right + m_rcPort.left) {
		m_rcPort.right += nX - m_rcPort.left;
		m_rcPort.left = nX;
		bOK = TRUE;
	}
	if (nY >= 0){ // && nY <= m_rcCanvas.bottom - m_rcPort.bottom + m_rcPort.top) {
		m_rcPort.bottom += nY - m_rcPort.top;
		m_rcPort.top = nY;
		bOK = TRUE;
	}
	if (bOK) {
		m_gride.SetViewPort(m_rcPort);
		InvalidateRect(NULL, FALSE);
	}
	return bOK;
}
void WorkView::OnViewGride()
{
	// TODO: Add your command handler code here
	m_bShowGride = 1 - m_bShowGride;
	InvalidateRect(NULL, FALSE);
}

void WorkView::OnUpdateViewGride(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetDocument() != NULL);
}
BOOL WorkView::OnCommand(WPARAM wParam, LPARAM lParam)
{
		TRACE("WorkView receive command %x\n", wParam);

		return CWnd::OnCommand(wParam, lParam);
}
void WorkView::CalculateRects()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	PProjectSetting pPs = GetDocument()->GetProjectSetting();
	if(!pPs) {
		m_rcPort = rcClient;			//the view port on the canvas, top-left is scroll poition
		m_szWorkspace.cx = rcClient.right;	//size of total area, the scroll area
		m_szWorkspace.cy = rcClient.bottom;
		m_rcCanvas.left = m_rcCanvas.right= 0;//location of zoomed image n canvas
		m_rcCanvas.top = m_rcCanvas.bottom= 0;
	} else {
		CRect rcBound;
		GetDocument()->GetImagesBoundary(rcBound);
		m_rcCanvas.left =  - min(0, rcBound.left) + BORDER_SIZE;
		m_rcCanvas.right = max(pPs->canvas.cx, rcBound.right) ;
		m_rcCanvas.top=  - min(0, rcBound.top) + BORDER_SIZE;
		m_rcCanvas.bottom = max(pPs->canvas.cy, rcBound.bottom);
		m_szWorkspace.cx = m_rcCanvas.left  +  m_rcCanvas.right  + BORDER_SIZE;
		m_szWorkspace.cy = m_rcCanvas.top + m_rcCanvas.bottom +BORDER_SIZE;
		m_rcCanvas.right = m_rcCanvas.left + pPs->canvas.cx ;
		m_rcCanvas.bottom = m_rcCanvas.top + pPs->canvas.cy;

		DocToViewPort(m_szWorkspace);
		DocToViewPort(m_rcCanvas);//the area of pPs->canvas on ViewPort 

	}
		//convert to zoomed size
	if( rcClient.right > m_szWorkspace.cx) {
		m_rcPort.left = 0; m_rcPort.right = rcClient.right;
		m_rcCanvas.left = (rcClient.right - m_szWorkspace.cx)/2;
		m_rcCanvas.right = m_rcCanvas.left + m_szWorkspace.cx;
		m_szWorkspace.cx = rcClient.right;
	} else {
		m_rcPort.right = m_rcPort.left + rcClient.right;
		if(m_rcPort.right > m_szWorkspace.cx) {
			m_rcPort.right = m_szWorkspace.cx;
			m_rcPort.left = m_szWorkspace.cx - rcClient.right;
		}
	}
	if( rcClient.bottom > m_szWorkspace.cy) {
		//canvas equals t client
		m_rcPort.top = 0; m_rcPort.bottom = rcClient.bottom;
		m_rcCanvas.top = (rcClient.bottom - m_szWorkspace.cy)/2;
		m_rcCanvas.bottom = m_rcCanvas.top + m_szWorkspace.cy;
		m_szWorkspace.cy = rcClient.bottom;
	} else {
		//image equals to canvas
		m_rcPort.bottom = m_rcPort.top + rcClient.bottom;
		if(m_rcPort.bottom > m_szWorkspace.cy) {
			m_rcPort.bottom = m_szWorkspace.cy;
			m_rcPort.top = m_szWorkspace.cy - rcClient.bottom;
		}
	}

}
// called first time after doc is new or opened
void WorkView::OnInitialUpdate() 
{
	CMainDoc* pDoc = GetDocument();
	PProjectSetting pPs = pDoc->GetProjectSetting();
	m_nZoomFactor = DEFAULT_ZOOM_FACTOR;
	CRect rcCanvas;
	pDoc->GetImagesBoundary(rcCanvas);

	if(rcCanvas.Width() > 0 && rcCanvas.Height() >0) {
		while(m_nZoomFactor > 0 && 
				( rcCanvas.Width() > DEFAULT_VISIBLE_WIDTH || rcCanvas.Width() > DEFAULT_VISIBLE_HEIGHT)) {
				m_nZoomFactor -- ;
				pDoc->GetImagesBoundary(rcCanvas);
				DocToViewPort(rcCanvas);
		}
	} else {
			rcCanvas = CRect(0,0,800,600);
	}
	//move window to fit work area
	CRect rc1, rc2;
	AfxGetMainWnd()->GetWindowRect(&rc1);
	GetClientRect(&rc2);
	rc1.right += (rcCanvas.Width()-rc2.right);
	rc1.bottom += (rcCanvas.Height()-rc2.bottom);
	AfxGetMainWnd()->MoveWindow(rc1);//move nbottom-right to fit rcCanvas

}
//doc has updated image, opened image
void WorkView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{

	CalculateRects();
	UpdateScrollbarPosition();
	GrideParam gp;
	gp.nInterval = 5;
	gp.nDivider = 20;
	m_gride.SetParam(&gp);
	m_gride.SetImageArea(m_rcCanvas);
	m_gride.SetViewPort(m_rcPort);
	m_ptCursorPos.x = m_ptCursorPos.y =  -1;
	m_nCurrentSelectedImage = -1;
	InvalidateRect(NULL, FALSE);
	m_ControlBar.UpdateUI(m_nCurrentSelectedImage, TRUE);
}
void WorkView::UpdateScrollbarPosition()
{
	if(m_rcPort.right - m_rcPort.left < m_szWorkspace.cx)
		m_parent->UpdateScrollBar(SB_HORZ, m_rcPort.left, m_rcPort.right - m_rcPort.left, m_szWorkspace.cx );
	else
		m_parent->UpdateScrollBar(SB_HORZ, -1, 0,0);
	if(m_rcPort.bottom - m_rcPort.top < m_szWorkspace.cy)
		m_parent->UpdateScrollBar(SB_VERT, m_rcPort.top, m_rcPort.bottom - m_rcPort.top, m_szWorkspace.cy );
	else
		m_parent->UpdateScrollBar(SB_VERT, -1, 0,0);
}
void WorkView::OnPaint()
{
	CPaintDC dc(this);  
	CDC* pDC = &dc;
	CRect rcClient;
	GetClientRect(&rcClient);
	if((m_rcCanvas.right - m_rcCanvas.left > 0) || (m_rcCanvas.bottom - m_rcCanvas.top> 0))	{
		//draw background
		CRect rcDest;
		rcDest.IntersectRect(&m_rcPort, &m_rcCanvas);
		rcDest.OffsetRect(-m_rcPort.left, -m_rcPort.top);
		CRect rcBkgnd = CRect(0,0, rcClient.right, rcDest.top);
		pDC->FillSolidRect(&rcBkgnd,BACKGND_COLOR);
		rcBkgnd = CRect(0, rcDest.bottom, rcClient.right, rcClient.bottom);
		pDC->FillSolidRect(&rcBkgnd,BACKGND_COLOR);
		rcBkgnd = CRect(0, rcDest.top, rcDest.left, rcDest.bottom);
		pDC->FillSolidRect(&rcBkgnd,BACKGND_COLOR);
		rcBkgnd = CRect(rcDest.right, rcDest.top, rcClient.right, rcDest.bottom);
		pDC->FillSolidRect(&rcBkgnd,BACKGND_COLOR);
		pDC->FillSolidRect(&rcDest,FORGND_COLOR);
		//shift to m_rcWord space
		CRect rc = m_rcCanvas;
		CRect vp = m_rcCanvas; //shift to relative to ViewPort
		vp.OffsetRect(-m_rcPort.left, -m_rcPort.top);
		ViewPortToDoc(rc); 
		int nMode = pDC->SetMapMode(MM_ANISOTROPIC);
		CSize szWinExt = pDC->SetWindowExt(rc.Width(), rc.Height());
		CSize szViewExt = pDC->SetViewportExt(vp.Width(), vp.Height());
		CPoint ptV = pDC->SetViewportOrg(m_rcCanvas.left-m_rcPort.left,m_rcCanvas.top-m_rcPort.top);
		CPoint ptW = pDC->SetWindowOrg(0,0);
		DrawImages(pDC);
		pDC->SetMapMode(nMode);
		pDC->SetWindowExt(szWinExt);
		pDC->SetViewportExt(szViewExt);
		pDC->SetViewportOrg(ptV);
		pDC->SetWindowOrg(ptW);
		if(m_bShowGride) {
			m_gride.Draw(pDC);
		}
	} else {
		pDC->FillSolidRect(&rcClient,BACKGND_COLOR);
	}
}
void WorkView::SetCurrentSel(int nSel)
{
		if(m_nCurrentSelectedImage != nSel) {
				m_nCurrentSelectedImage = nSel;
				InvalidateRect(NULL, FALSE);
		}
}
void WorkView::DrawImages(CDC* pDC)
{
		PProjectSetting pPs = GetDocument()->GetProjectSetting();

	for(int i=0;i< pPs->nImages; i++ )
	{
			if ( i == m_nCurrentSelectedImage)
					continue;
			CRect rcDest = pPs->ip[i].rcBound;
			DocToViewPort(rcDest);
			if (pPs->ip[i].pImg) {
					CRect rcSrc= CRect(0,0,pPs->ip[i].pImg->Width(), pPs->ip[i].pImg->Height());
					pPs->ip[i].pImg->StretchToDC(*pDC, rcSrc, rcDest, SRCCOPY); 
			}
	}
	if (m_nCurrentSelectedImage >= 0 && m_nCurrentSelectedImage <pPs->nImages) {
			CRect rcDest = pPs->ip[m_nCurrentSelectedImage].rcBound;
			DocToViewPort(rcDest);
			if (pPs->ip[m_nCurrentSelectedImage].pImg) {
					CRect rcSrc= CRect(0,0,pPs->ip[m_nCurrentSelectedImage].pImg->Width(), 
							pPs->ip[m_nCurrentSelectedImage].pImg->Height());
					pPs->ip[m_nCurrentSelectedImage].pImg->StretchToDC(*pDC, rcSrc, rcDest, SRCCOPY); 
			}
			CPen pen(PS_SOLID, 1, SELECTED_COLOR);
			CBrush brush; 
			brush.CreateStockObject(HOLLOW_BRUSH);

			CPen* pOld = pDC->SelectObject(&pen);
			CBrush* pOldBrush = pDC->SelectObject(&brush);
			pDC->Rectangle(&rcDest);
			pDC->SelectObject(pOld);
			pDC->SelectObject(pOldBrush);
	}
}
void WorkView::OnViewZoomin() 
{
	m_nZoomFactor++;
	//shift origin
	m_rcPort.left = m_rcPort.left * ZOOM_FACTOR[m_nZoomFactor]/ZOOM_FACTOR[m_nZoomFactor-1] ;
	m_rcPort.top = m_rcPort.top * ZOOM_FACTOR[m_nZoomFactor]/ZOOM_FACTOR[m_nZoomFactor-1] ;
	CalculateRects();
	InvalidateRect(NULL);
	UpdateScrollbarPosition();

	m_gride.SetImageArea(m_rcCanvas);
	m_gride.SetViewPort(m_rcPort);
	InvalidateRect(NULL);
}

void WorkView::OnUpdateViewZoomin(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( (BOOL)(m_nZoomFactor < MAX_ZOOM-1));
}

void WorkView::OnViewZoomout() 
{
	if(	m_nZoomFactor > 0){
		m_nZoomFactor --;
		//shift origin
		m_rcPort.left = m_rcPort.left * ZOOM_FACTOR[m_nZoomFactor]/ZOOM_FACTOR[m_nZoomFactor+1]; 
		m_rcPort.top = m_rcPort.top * ZOOM_FACTOR[m_nZoomFactor]/ZOOM_FACTOR[m_nZoomFactor+1]; 
		CalculateRects();
		InvalidateRect(NULL);
	}
	UpdateScrollbarPosition();
	m_gride.SetImageArea(m_rcCanvas);
	m_gride.SetViewPort(m_rcPort);
	InvalidateRect(NULL);
}
void WorkView::OnUpdateIndicatePos(CCmdUI *pCmdUI)
{
	CString szText;
	szText.Format("(%4d,%4d)", m_ptCursorPos.x, m_ptCursorPos.y);
	pCmdUI->SetText(szText);
}
void WorkView::OnUpdateIndicateZoomFactor(CCmdUI *pCmdUI)
{
	CString szText;
	int n = ZOOM_FACTOR[m_nZoomFactor];
	float m= ((float)n)/100;
	szText.Format("%4.2fX", m);
	pCmdUI->SetText(szText);
}
void WorkView::OnUpdateIndicateSize(CCmdUI *pCmdUI)
{
	CString szText="";
	CMainDoc* pDoc = GetDocument();
	if( pDoc ) {
		CRect rc = m_rcCanvas;
		ViewPortToDoc(rc);
		szText.Format("(%4dx%4d)", rc.Width(), rc.Height());
	}
	pCmdUI->SetText(szText);
}
void WorkView::OnUpdateViewZoomout(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( (BOOL)(m_nZoomFactor > 0));	
}

void WorkView::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint ptCan;
	ptCan = point + m_rcPort.TopLeft();
	m_ptCursorPos.x = (ptCan.x - m_rcCanvas.left)* 100 / ZOOM_FACTOR[m_nZoomFactor];
	m_ptCursorPos.y = (ptCan.y - m_rcCanvas.top)* 100 / ZOOM_FACTOR[m_nZoomFactor];


	CWnd::OnMouseMove(nFlags, point);
}

void WorkView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
}
inline BOOL IsPtInRegion(POINT p, POINT q, SIZE s)
{
		if (p.x <q.x || p.x > (q.x + s.cx)) return FALSE;
		if (p.y <q.y || p.y > (q.y + s.cy)) return FALSE;
		return TRUE;
}
void WorkView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
		CPoint ptCan;
		ptCan = point + m_rcPort.TopLeft();
		m_ptCursorPos.x = (ptCan.x - m_rcCanvas.left)* 100 / ZOOM_FACTOR[m_nZoomFactor];
		m_ptCursorPos.y = (ptCan.y - m_rcCanvas.top)* 100 / ZOOM_FACTOR[m_nZoomFactor];
		PProjectSetting pPs = GetDocument()->GetProjectSetting();		
		int nSel = -1;
		int i;
		//find region by display order
		if (m_nCurrentSelectedImage >=0 && m_nCurrentSelectedImage < pPs->nImages &&
						IsPtInRegion(m_ptCursorPos, pPs->ip[m_nCurrentSelectedImage].pos, 
										pPs->ip[m_nCurrentSelectedImage].size)) {
				m_nCurrentSelectedImage = -1;
		} else {
				for (i =pPs->nImages-1 ; i>= 0; i--) {
						if (i == m_nCurrentSelectedImage)
								continue;
						if(IsPtInRegion(m_ptCursorPos, pPs->ip[i].pos, pPs->ip[i].size)) {
								m_nCurrentSelectedImage = i;
								break;
						}
				}
		}

		m_ControlBar.UpdateUI(m_nCurrentSelectedImage, FALSE);
		InvalidateRect(NULL, FALSE);
}

void WorkView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);
	
}

void WorkView::OnViewController()
{
		if(m_ControlBar.IsWindowVisible()) 
				m_ControlBar.ShowWindow(SW_HIDE);
		else
				m_ControlBar.ShowWindow(SW_SHOW);
}

void WorkView::OnUpdateViewController(CCmdUI *pCmdUI)
{
		CMainDoc* pDoc = GetDocument();
		pCmdUI->Enable(pDoc && pDoc->GetProjectSetting()->nImages >0);
}

