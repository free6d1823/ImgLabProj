#include "stdafx.h"
#include "resource.h"
#include "ImgFile.h"
#include "DistMeasure.h"
#include "ProcessView.h"

#define GRIDE_COLOR	RGB(0,0,255)

#include <ImgLab.h>

IMPLEMENT_DYNAMIC(ProcessView, BasicView)
ProcessView::ProcessView(void): m_gride(RGB(0,255,0))
{

	m_pOriginalImg = NULL;
	m_bShowGride = m_bShowMeasure = FALSE;
	m_pDistMeasure = NULL;

}

ProcessView::~ProcessView(void)
{
	if(m_pImage)
		delete m_pImage;
	m_pImage = NULL;
	if(m_pDistMeasure) {
		delete m_pDistMeasure;
	}
}

BEGIN_MESSAGE_MAP(ProcessView, BasicView)
		ON_WM_CREATE()
		ON_WM_PAINT()
		ON_COMMAND(ID_PROCESS_PREVIEW, OnPreview)
		ON_WM_SIZE()
		ON_WM_HSCROLL()
		ON_WM_VSCROLL()
		ON_COMMAND_RANGE(ID_TOOLBOX_BASE, ID_TOOLBOX_LAST,  OnCommandToolbox) 
		ON_WM_MOUSEMOVE()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void ProcessView::OnUpdateToolbox(CCmdUI *pCmdUI)
{
		switch( pCmdUI->m_nID){
				case ID_TOOLBOX_GRIDE: OnUpdateViewGride(pCmdUI); break;
				case ID_TOOLBOX_ZOOMIN: BasicView::OnUpdateViewZoomin(pCmdUI); break;
				case ID_TOOLBOX_ZOOMOUT: BasicView::OnUpdateViewZoomout(pCmdUI); break;
				case ID_TOOLBOX_MEASURE: OnUpdateViewMeasure(pCmdUI); break;
		}
}
void ProcessView::OnCommandToolbox(UINT nCmd)
{
		switch( nCmd){
				case ID_TOOLBOX_GRIDE: OnViewGride();break;
				case ID_TOOLBOX_ZOOMIN: OnViewZoomin(); break;
				case ID_TOOLBOX_ZOOMOUT: OnViewZoomout(); break;
				case ID_TOOLBOX_MEASURE: OnViewMeasure(); break;
		}
}

// ProcessView message handlers

void ProcessView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (nSBCode == SB_THUMBPOSITION  || nSBCode == SB_THUMBTRACK) {
			m_rcPort.right += nPos - m_rcPort.left;
			m_rcPort.left = nPos;

			SCROLLINFO si;
			si.cbSize = sizeof(si);
			si.nPos = nPos;
			si.fMask = SIF_POS;
			SetScrollInfo(SB_HORZ, &si, TRUE);
	}else {
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
		//m_view.SetScrollPos(-1, si.nPos);
		m_rcPort.bottom += si.nPos - m_rcPort.top;
		m_rcPort.top = si.nPos;
	}	
	m_gride.SetViewPort(m_rcPort);
	if(m_pDistMeasure){
		m_pDistMeasure->SetViewPort(m_rcPort);
	}
	InvalidateRect(NULL, FALSE);
}

void ProcessView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (nSBCode == SB_THUMBPOSITION  || nSBCode == SB_THUMBTRACK) {
			m_rcPort.bottom += nPos - m_rcPort.top;
			m_rcPort.top = nPos;

			SCROLLINFO si;
			si.cbSize = sizeof(si);
			si.nPos = nPos;
			si.fMask = SIF_POS;
			SetScrollInfo(SB_VERT, &si, TRUE);
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
		//m_view.SetScrollPos(-1, si.nPos);
		m_rcPort.bottom += si.nPos - m_rcPort.top;
		m_rcPort.top = si.nPos;
	}	
	m_gride.SetViewPort(m_rcPort);
	if(m_pDistMeasure){
		m_pDistMeasure->SetViewPort(m_rcPort);
	}
	InvalidateRect(NULL, FALSE);

}

void ProcessView::OnViewZoomin()
{
	BasicView::OnViewZoomin();
	UpdateScrollbarPosition();

	m_gride.SetImageArea(m_rcImage);
	m_gride.SetViewPort(m_rcPort);
	if(m_pDistMeasure){
		m_pDistMeasure->SetImageArea(m_rcImage);
		m_pDistMeasure->SetViewPort(m_rcPort);
	}
}
void ProcessView::OnViewZoomout()
{
	BasicView::OnViewZoomout();
	UpdateScrollbarPosition();


	m_gride.SetImageArea(m_rcImage);
	m_gride.SetViewPort(m_rcPort);
	if(m_pDistMeasure){
		m_pDistMeasure->SetImageArea(m_rcImage);
		m_pDistMeasure->SetViewPort(m_rcPort);
	}
}

void ProcessView::UpdateView()
{
	if(m_pImage == NULL)
		return;
	//update rect
	CRect rcClient;
	GetClientRect(&rcClient);
	//find a suitable zoom factor
	m_nZoomFactor = 0;
	while (m_nZoomFactor <MAX_ZOOM) {
		m_nZoomFactor++;
		CSize szTemp = CSize(m_pImage->Width(), m_pImage->Height());
		DocToViewPort(szTemp);
		if (szTemp.cx > rcClient.right || szTemp.cy > rcClient.bottom) {
			m_nZoomFactor--;
			break;
		}
	}
TRACE("------------UpdateView() %dx%d\n", m_pImage->Width() , m_pImage->Height());
	m_rcImage.top = m_rcImage.left = 0;
	m_rcImage.right = m_pImage->Width();
	m_rcImage.bottom = m_pImage->Height();
	DocToViewPort(m_rcImage);
	m_szCanvas.cx = max(m_rcImage.right, rcClient.right);
	m_szCanvas.cy = max (m_rcImage.bottom, rcClient.bottom);
	POINT offset;
	offset.x = (m_szCanvas.cx - m_rcImage.right)/2;
	offset.y = (m_szCanvas.cy - m_rcImage.bottom)/2;
	m_rcImage.top += offset.y;
	m_rcImage.bottom += offset.y;
	m_rcImage.left += offset.x;
	m_rcImage.right += offset.x;
	m_rcPort.top = 0; m_rcPort.bottom = rcClient.bottom;
	m_rcPort.left = 0; m_rcPort.right = rcClient.right;
	UpdateScrollbarPosition();
	InvalidateRect(NULL);


	GrideParam gp;
	gp.nInterval = 2;
	gp.nDivider = 8;
	m_gride.SetParam(&gp);
	m_gride.SetImageArea(m_rcImage);
	m_gride.SetViewPort(m_rcPort);
	if(m_pDistMeasure){
		m_pDistMeasure->SetImageArea(m_rcImage);
		m_pDistMeasure->SetViewPort(m_rcPort);
	}
}



void ProcessView::SetParam(ProcessWrap* pWrapper)
{	
	//default copy a image
	if(m_pOriginalImg) {
		if(m_pImage) {
			if( m_pImage->Width() == m_pOriginalImg->Width() &&  
					m_pImage->Height() == m_pOriginalImg->Height())
				return;
			delete m_pImage;
		}
		m_pImage = new ImgFile();
		m_pImage->CopyFrom(m_pOriginalImg);
	}
}

void ProcessView::SetImgFile(ImgFile* pImg)
{

	m_pOriginalImg = pImg;	//pointer to original mage
	UpdateView();

}

int ProcessView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (BasicView::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_pDistMeasure = new DistMeasure(this);
	return 0;
}
void ProcessView::PostNcDestroy()
{
	CWnd::PostNcDestroy();
	//delete this when it is destryed by parent wnd
	delete this;
}

void ProcessView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	DrawImage(&dc);
	if(m_bShowGride) {
		m_gride.Draw(&dc);
	}
	if(m_bShowMeasure && m_pDistMeasure)
		m_pDistMeasure->Draw(&dc);
}
BOOL		ProcessView::PosMap(int u, int v, float&x, float&y)
{
	x	= (float)u; y=(float)v;
	return TRUE;
}

//interpolate the color in position x,y in source coordinates
COLORREF ProcessView::GetColor(BYTE* pSrc, int stride, float x, float y)
{
	COLORREF cr= 0;

	int new_x = (int)x;
	int new_y = (int)y;
	int x1 = new_x < (int)m_pImage->Width() - 1 ? new_x + 1: new_x; 
	int y1 = new_y < (int)m_pImage->Height() - 1 ? new_y + 1: new_y;
	float f1 = x-(float)new_x;
	float f2 = y-(float)new_y;
	int m0,m1,m2,m3;
	m0 = new_y * stride + new_x*3;
	m1 = new_y * stride + x1*3;
	m2 = y1 * stride + new_x*3;
	m3 = y1 * stride + x1*3;
	float p0,p1,p2,p3,p01,p23;
	p0 = (float) pSrc[m0];
	p1 = (float) pSrc[m1];
	p2 = (float) pSrc[m2];
	p3 = (float) pSrc[m3];
	p01 = p0 + f1*(p1-p0);
	p23 = p2 + f1*(p3-p2);
	cr = cr |( ((int) (p01 + f2*(p23-p01)))<<16);// 


	p0 = (float) pSrc[m0+1];
	p1 = (float) pSrc[m1+1];
	p2 = (float) pSrc[m2+1];
	p3 = (float) pSrc[m3+1];
	p01 = p0 + f1*(p1-p0);
	p23 = p2 + f1*(p3-p2);
	cr = cr |( ((int) (p01 + f2*(p23-p01)))<<8);//G 

	p0 = (float) pSrc[m0+2];
	p1 = (float) pSrc[m1+2];
	p2 = (float) pSrc[m2+2];
	p3 = (float) pSrc[m3+2];
	p01 = p0 + f1*(p1-p0);
	p23 = p2 + f1*(p3-p2);
	cr = cr | ((int) (p01 + f2*(p23-p01)));//R 

	return cr;
}

void ProcessView::OnPreview()
{
		if(!m_pImage || !m_pOriginalImg)
		return;
		//copy m_pOriginalImg to m_pImage
		//check if video output size is changed

		BYTE* pSrc = (BYTE*)m_pOriginalImg->GetBits();
		BYTE* pDest = (BYTE*) m_pImage->GetBits();
		if(0) {
			//gImgLab->MappingImage();
		}else {

			BYTE* pLineDest = pDest;
			BYTE* pData;
			float x=-1, y=-1;
			COLORREF color;
			int i,j;

			for(i=0;i<(int)m_pImage->Height(); i++ ) {
					pData = pLineDest;
					for(j=0;j< (int)m_pImage->Width(); j++) {

							PosMap(j,i, x,y);
							if(x<0 || y<0 || x>= (float)m_pOriginalImg->Width() ||y>= (float)m_pOriginalImg->Height())
								continue;
							color = GetColor(pSrc, m_pOriginalImg->BytesPerLine(), x,y);
							*pData = GetBValue(color);
							*(pData+1) = GetGValue(color);
							*(pData+2) = GetRValue(color);	
							pData += 3; 
					}
					pLineDest += m_pImage->BytesPerLine();
			}
		}

		InvalidateRect(NULL, FALSE);
}

void ProcessView::OnSize(UINT nType, int cx, int cy)
{
		BasicView::OnSize(nType, cx, cy);
		m_gride.SetImageArea(m_rcImage);
		m_gride.SetViewPort(m_rcPort);
		if(m_bShowMeasure && m_pDistMeasure){
			m_pDistMeasure->SetViewPort(m_rcPort);
			m_pDistMeasure->SetImageArea(m_rcImage);
		}
}

void ProcessView::OnViewGride()
{
	// TODO: Add your command handler code here
	m_bShowGride = 1 - m_bShowGride;
	InvalidateRect(NULL, FALSE);
}

void ProcessView::OnUpdateViewGride(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( m_pImage != NULL);
}
void ProcessView::OnUpdateViewMeasure(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( m_pImage != NULL);
}
void ProcessView::OnViewMeasure()
{
	// TODO: Add your command handler code here
	m_bShowMeasure = 1 - m_bShowMeasure;
}
BOOL ProcessView::SetScrollPos(int nX, int nY)
{
	BOOL bOK = FALSE;

	if (nX >= 0){// && nX <= m_rcImage.right - m_rcPort.right + m_rcPort.left) {
		m_rcPort.right += nX - m_rcPort.left;
		m_rcPort.left = nX;
		bOK = TRUE;
	}
	if (nY >= 0){ // && nY <= m_rcImage.bottom - m_rcPort.bottom + m_rcPort.top) {
		m_rcPort.bottom += nY - m_rcPort.top;
		m_rcPort.top = nY;
		bOK = TRUE;
	}
	if (bOK) {
		m_gride.SetViewPort(m_rcPort);
		if(m_bShowMeasure && m_pDistMeasure)
			m_pDistMeasure->SetViewPort(m_rcPort);
		InvalidateRect(NULL, FALSE);
	}
	return bOK;
}
void ProcessView::OnMouseMove(UINT nFlags, CPoint point)
{
	point.x = point.x + m_rcPort.left - m_rcImage.left; //view port related to image
	point.y = point.y + m_rcPort.top- m_rcImage.top;
	//convert to canvas
	ViewPortToDoc(point);
	if(m_bShowMeasure && m_pDistMeasure)
			m_pDistMeasure->OnMouseMove(this, point);
 
	CWnd::OnMouseMove(nFlags, point);
}
void ProcessView::OnLButtonDown(UINT nFlags, CPoint point)
{
	point.x = point.x + m_rcPort.left - m_rcImage.left; //view port related to image
	point.y = point.y + m_rcPort.top- m_rcImage.top;
	//convert to canvas
	ViewPortToDoc(point);
	if(m_bShowMeasure && m_pDistMeasure)
			m_pDistMeasure->OnLButtonDown(this, point);
	CWnd::OnLButtonDown(nFlags, point);
}
void ProcessView::OnLButtonUp(UINT nFlags, CPoint point)
{
		if(m_bShowMeasure && m_pDistMeasure)
			m_pDistMeasure->OnLButtonUp(this, point);
		CWnd::OnLButtonUp(nFlags, point);
}