// BasicView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BasicView.h"
#include "ImgFile.h"

#define BACKGND_COLOR	RGB(120,120,120)
int BasicView::ZOOM_FACTOR[MAX_ZOOM] = { 10, 25, 33, 50,60,75,80,90, 100,110, 120, 125, 150, 175, 200, 250, 300, 400};


// BasicView

IMPLEMENT_DYNAMIC(BasicView, CWnd)

BasicView::BasicView()
{
	m_nZoomFactor = DEFAULT_ZOOM_FACTOR;
	m_pImage = NULL;
}

BasicView::~BasicView()
{
	//reference image m_pImage
	//if(m_pImage) delete m_pImage;
}
void BasicView::PostNcDestroy()
{
	CWnd::PostNcDestroy();
}

//convert image size to Scren coord
void BasicView::DocToViewPort(SIZE& size)
{
	size.cx = size.cx * ZOOM_FACTOR[m_nZoomFactor] /100;
	size.cy = size.cy * ZOOM_FACTOR[m_nZoomFactor] /100;

}
void BasicView::DocToViewPort(POINT& pt)
{
	pt.x = pt.x * ZOOM_FACTOR[m_nZoomFactor] /100;
	pt.y = pt.y * ZOOM_FACTOR[m_nZoomFactor] /100;

}
void BasicView::DocToViewPort(RECT& rect)
{
	rect.left = rect.left  * ZOOM_FACTOR[m_nZoomFactor] /100;
	rect.right = rect.right * ZOOM_FACTOR[m_nZoomFactor] /100;
	rect.top = rect.top * ZOOM_FACTOR[m_nZoomFactor] /100;
	rect.bottom = rect.bottom * ZOOM_FACTOR[m_nZoomFactor] /100;
}
//convert Screen size to image
void BasicView::ViewPortToDoc(SIZE& size)
{
	ASSERT(ZOOM_FACTOR[m_nZoomFactor] > 0);
	size.cx = size.cx * 100 / ZOOM_FACTOR[m_nZoomFactor];
	size.cy = size.cy * 100 / ZOOM_FACTOR[m_nZoomFactor];
}
void BasicView::ViewPortToDoc(POINT& pt)
{
	ASSERT(ZOOM_FACTOR[m_nZoomFactor] > 0);
	pt.x = pt.x * 100 / ZOOM_FACTOR[m_nZoomFactor];
	pt.y = pt.y * 100 / ZOOM_FACTOR[m_nZoomFactor];
}
void BasicView::ViewPortToDoc(RECT& rect)
{
	ASSERT(ZOOM_FACTOR[m_nZoomFactor] > 0);
	rect.left = rect.left * 100 / ZOOM_FACTOR[m_nZoomFactor];
	rect.right = rect.right * 100 / ZOOM_FACTOR[m_nZoomFactor];
	rect.top = rect.top * 100 / ZOOM_FACTOR[m_nZoomFactor];
	rect.bottom = rect.bottom * 100 / ZOOM_FACTOR[m_nZoomFactor];
}
void BasicView::CalculateRects()
{
	ImgFile* pImg = m_pImage;
	CRect rcClient;
	GetClientRect(&rcClient);
	if (!pImg) {
		m_rcPort = rcClient;			//the view port on the canvas, top-left is scroll poition
		m_szCanvas.cx = rcClient.right;	//size of total area, the scroll area
		m_szCanvas.cy = rcClient.bottom;
		m_rcImage.left = m_rcImage.right= 0;//location of zoomed image n canvas
		m_rcImage.top = m_rcImage.bottom= 0;
	} else {
		m_szCanvas.cx = m_pImage->Width();
		m_szCanvas.cy = m_pImage->Height();
		DocToViewPort(m_szCanvas);
	}
	//convert to zoomed size
	if( rcClient.right > m_szCanvas.cx) {
		m_rcPort.left = 0; m_rcPort.right = rcClient.right;
		m_rcImage.left = (rcClient.right - m_szCanvas.cx)/2;
		m_rcImage.right = m_rcImage.left + m_szCanvas.cx;
		m_szCanvas.cx = rcClient.right;
	} else {
		m_rcImage.left = 	0;
		m_rcImage.right = m_szCanvas.cx; 
		m_rcPort.right = m_rcPort.left + rcClient.right;
		if(m_rcPort.right > m_szCanvas.cx) {
			m_rcPort.right = m_szCanvas.cx;
			m_rcPort.left = m_szCanvas.cx - rcClient.right;
		}
	}
	if( rcClient.bottom > m_szCanvas.cy) {
		//canvas equals t client
		m_rcPort.top = 0; m_rcPort.bottom = rcClient.bottom;
		m_rcImage.top = (rcClient.bottom - m_szCanvas.cy)/2;
		m_rcImage.bottom = m_rcImage.top + m_szCanvas.cy;
		m_szCanvas.cy = rcClient.bottom;
	} else {
		//image equals to canvas
		m_rcImage.top = 	0;
		m_rcImage.bottom = m_szCanvas.cy; 
		m_rcPort.bottom = m_rcPort.top + rcClient.bottom;
		if(m_rcPort.bottom > m_szCanvas.cy) {
			m_rcPort.bottom = m_szCanvas.cy;
			m_rcPort.top = m_szCanvas.cy - rcClient.bottom;
		}
	}

}


BEGIN_MESSAGE_MAP(BasicView, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMIN, OnUpdateViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT, OnUpdateViewZoomout)
END_MESSAGE_MAP()

BOOL BasicView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}
BOOL BasicView::Create(LPCTSTR szName,CWnd* pParentWnd, BOOL bPopup)
{
	DWORD style;
	DWORD exstyle;
	if(bPopup) {
		style = WS_POPUP|WS_CAPTION|WS_THICKFRAME|WS_SYSMENU
					|WS_HSCROLL |WS_VSCROLL;
		exstyle = WS_EX_CLIENTEDGE|WS_EX_TOOLWINDOW;
	} else {
		style = WS_CHILD|WS_VISIBLE;
		exstyle = 0;
	}
	//child = WS_CHIL|WS_VISIBLE
	LPCTSTR lpszClass = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW|CS_DBLCLKS);
	return CreateEx(exstyle, lpszClass, szName,style,
			CRect(0, 0, 800, 600), pParentWnd, NULL,NULL);
}


void BasicView::SetImgFile(ImgFile* pImg)
{

m_pImage = pImg;
		CalculateRects();
		InvalidateRect(NULL);
}
// BasicView message handlers



void BasicView::OnClose()
{
	ShowWindow(SW_HIDE);
	//CWnd::OnClose();
}
void BasicView::DrawImage(CDC* pDC)
{
	CRect  rcClient;
	CRect rcSource, rcDest;
	GetClientRect(&rcClient);
	if(m_pImage)
	{
		//
		rcDest.IntersectRect(&m_rcPort, &m_rcImage);
		rcSource = rcDest;
		//
		rcDest.OffsetRect(-m_rcPort.left, -m_rcPort.top);
		rcSource.OffsetRect(-m_rcImage.left, -m_rcImage.top);
		ViewPortToDoc(rcSource);
		//fill background
		CRect rcBkgnd = CRect(0,0, rcClient.right, rcDest.top);
		pDC->FillSolidRect(&rcBkgnd,BACKGND_COLOR);
		rcBkgnd = CRect(0, rcDest.bottom, rcClient.right, rcClient.bottom);
		pDC->FillSolidRect(&rcBkgnd,BACKGND_COLOR);

		rcBkgnd = CRect(0, rcDest.top, rcDest.left, rcDest.bottom);
		pDC->FillSolidRect(&rcBkgnd,BACKGND_COLOR);
		rcBkgnd = CRect(rcDest.right, rcDest.top, rcClient.right, rcDest.bottom);
		pDC->FillSolidRect(&rcBkgnd,BACKGND_COLOR);

		//convert to unzoomed coordinates
		m_pImage->StretchToDC(*pDC,rcSource, rcDest);
	} else {
		pDC->FillSolidRect(&rcClient,BACKGND_COLOR);
	}
}
void BasicView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	DrawImage(&dc);
}

void BasicView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	CalculateRects();
	UpdateScrollbarPosition();
	InvalidateRect(NULL, FALSE);
}
void BasicView::OnViewZoomin() 
{
	// TODO: Add your command handler code here
	m_nZoomFactor++;
		//shift origin
		m_rcPort.left = m_rcPort.left * ZOOM_FACTOR[m_nZoomFactor]/ZOOM_FACTOR[m_nZoomFactor-1] ;
		m_rcPort.top = m_rcPort.top * ZOOM_FACTOR[m_nZoomFactor]/ZOOM_FACTOR[m_nZoomFactor-1] ;
	CalculateRects();
	InvalidateRect(NULL);

}

void BasicView::OnUpdateViewZoomin(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( (BOOL)(m_pImage && m_nZoomFactor < MAX_ZOOM-1));
}

void BasicView::OnViewZoomout() 
{
	if(	m_nZoomFactor > 0){
		m_nZoomFactor --;
		//shift origin
		m_rcPort.left = m_rcPort.left * ZOOM_FACTOR[m_nZoomFactor]/ZOOM_FACTOR[m_nZoomFactor+1]; 
		m_rcPort.top = m_rcPort.top * ZOOM_FACTOR[m_nZoomFactor]/ZOOM_FACTOR[m_nZoomFactor+1]; 
		CalculateRects();
		InvalidateRect(NULL);
	}
}
void BasicView::OnUpdateViewZoomout(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( (BOOL)(m_pImage&& m_nZoomFactor > 0));	
}

void BasicView::UpdateScrollbarPosition()
{
	SCROLLINFO si;
	if(m_rcPort.right - m_rcPort.left < m_szCanvas.cx){
			ShowScrollBar(SB_HORZ, TRUE);
			si.cbSize = sizeof(si);
			si.nMin = 0;
			si.nMax = m_szCanvas.cx;
			si.nPos = m_rcPort.left;
			si.nPage = m_rcPort.right - m_rcPort.left;
			si.fMask = SIF_PAGE|SIF_POS|SIF_RANGE;
			SetScrollInfo(SB_HORZ, &si, TRUE);
	}
	else
		ShowScrollBar(SB_HORZ, FALSE);

	if(m_rcPort.bottom - m_rcPort.top < m_szCanvas.cy){
			si.cbSize = sizeof(si);
			si.nMin = 0;
			si.nMax = m_szCanvas.cy;
			si.nPos = m_rcPort.top;
			si.nPage = m_rcPort.bottom - m_rcPort.top;
			si.fMask = SIF_PAGE|SIF_POS|SIF_RANGE;
			SetScrollInfo(SB_VERT, &si, TRUE);
			ShowScrollBar(SB_VERT, TRUE);
	}
	else
			ShowScrollBar(SB_VERT, FALSE);

}


void BasicView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (nSBCode == SB_THUMBPOSITION  || nSBCode == SB_THUMBTRACK) {
			m_rcPort.right += nPos - m_rcPort.left;
			m_rcPort.left = nPos;
			InvalidateRect(NULL, FALSE);

			SCROLLINFO si;
			si.cbSize = sizeof(si);
			si.nPos = nPos;
			si.fMask = SIF_POS;
			SetScrollInfo(SB_HORZ, &si, TRUE);
		
	}
}

void BasicView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (nSBCode == SB_THUMBPOSITION  || nSBCode == SB_THUMBTRACK) {
			m_rcPort.bottom += nPos - m_rcPort.top;
			m_rcPort.top = nPos;
			InvalidateRect(NULL, FALSE);

			SCROLLINFO si;
			si.cbSize = sizeof(si);
			si.nPos = nPos;
			si.fMask = SIF_POS;
			SetScrollInfo(SB_VERT, &si, TRUE);
	}
}
