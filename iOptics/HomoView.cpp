// LdcView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "HomoView.h"
#include "MeasurePerformance.h"
#include "ImgFile.h"


#define CONTROLLER_COLOR1	RGB(255,0,0)
#define CONTROLLER_COLOR2	RGB(255,255,0)
#define CP_SIZE			4
IMPLEMENT_DYNAMIC(HomoView, ProcessView)

HomoView::HomoView() : ProcessView()
{
	m_bModified = FALSE;
	m_bDragMode = FALSE;
	m_ptCursorPos.x = m_ptCursorPos.y = 0;
}

HomoView::~HomoView()
{

}


BEGIN_MESSAGE_MAP(HomoView, ProcessView)

	ON_COMMAND(ID_PROCESS_PREVIEW, OnPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()
#define HOMO_PARAM() ((HomoWrap*)m_pWrapRef)->param
void HomoView::SetParam(ProcessWrap* pWrapper)
{
	m_pWrapRef = (HomoWrap*) pWrapper;
	memcpy(&m_ptCp, &m_pWrapRef->param.p, sizeof(m_ptCp));
	m_bModified = FALSE;

	if(m_pImage) {
		if( m_pImage->Width() == HOMO_PARAM().szOutput.cx &&  
				m_pImage->Height() == HOMO_PARAM().szOutput.cy)
			return;
		delete m_pImage;
	}
	m_pImage = new ImgFile();
	m_pImage->Create(HOMO_PARAM().szOutput.cx,HOMO_PARAM().szOutput.cy,24,0);

	UpdateView();
}

/*\brief Position mapping from processed image to original image
* \param[in] u [0~szOutput.cx) in corrected image
* \param[in] v [0~szOutput.cy) in corrected image
* \param[out] x [0~szInput.cx) in source image
* \param[out] y [0~szInput.cy) in source image
*/
BOOL		HomoView::PosMap(int u, int v, float&x, float&y)
{
	return m_pWrapRef->PosMap(u,v,x,y);
}

//interpolate the color in position x,y in source coordinates
COLORREF HomoView::GetColor(BYTE* pSrc, int stride, float x, float y)
{
	if(x<0 || y<0 || x>= (float)HOMO_PARAM().szInput.cx ||y>= (float)HOMO_PARAM().szInput.cy)
		return 0;
	return ProcessView::GetColor(pSrc, stride, x, y);
}

void HomoView::OnPreview()
{
MeasurePerformance mp;
mp.start();
		ProcessView::OnPreview();
mp.end();
TRACE("HomoView one frame takes %f ms\n", mp.duration());
}	
BOOL	HomoView::IsOnControlPoint(POINT ptCursor, int& index)
{
	BOOL bFound = FALSE;
	for (int k=0;k< 4; k++)  {
			if (ptCursor.x <= m_ptCp[k].x + CP_SIZE 
				&& ptCursor.x >= m_ptCp[k].x - CP_SIZE
				&& ptCursor.y <= m_ptCp[k].y + CP_SIZE
				&& ptCursor.y >= m_ptCp[k].y - CP_SIZE) {
					index = k;
					bFound = TRUE;
					break;
			}
	}
	return bFound;
}
void HomoView::OnMouseMove(UINT nFlags, CPoint point)
{
	m_ptCursorPos.x = point.x + m_rcPort.left - m_rcImage.left; //view port related to image
	m_ptCursorPos.y = point.y + m_rcPort.top- m_rcImage.top;
	//convert to canvas
	ViewPortToDoc(m_ptCursorPos);
	if(m_bDragMode) {
			POINT ptOffset;
			ptOffset.x = (m_ptCursorPos.x - m_ptPrev.x);
			ptOffset.y = (m_ptCursorPos.y - m_ptPrev.y);
			m_ptCp[m_idSelected] = m_ptCursorPos;
			m_ptPrev = m_ptCursorPos;
			InvalidateRect(NULL, FALSE);
	}else {
		int index;
		if( IsOnControlPoint(m_ptCursorPos, index)) {
			SetCursor(::LoadCursor(NULL, IDC_CROSS));
		}else {
			SetCursor(::LoadCursor(NULL, IDC_ARROW));
		}
	}
	//chec control lines
	ProcessView::OnMouseMove(nFlags, point);
}
void HomoView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_ptCursorPos.x = point.x + m_rcPort.left - m_rcImage.left; //view port related to image
	m_ptCursorPos.y = point.y + m_rcPort.top- m_rcImage.top;
	//convert to canvas
	ViewPortToDoc(m_ptCursorPos);
	if( !m_bDragMode) {
		int index;
		if( IsOnControlPoint(m_ptCursorPos, index)) {
			SetCapture();
			m_bDragMode = TRUE;
			m_idSelected = index;
			m_ptPrev = m_ptCursorPos;
		}
	}
	ProcessView::OnLButtonDown(nFlags, point);
}
void HomoView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bDragMode) {
		ReleaseCapture();
		m_bDragMode = FALSE;
		m_bModified = TRUE;
		SetCursor(::LoadCursor(NULL, IDC_ARROW));

	}
		ProcessView::OnLButtonUp(nFlags, point);
}
void HomoView::GetControlPoints(HomoControlPoints&  cp)
{
	memcpy(&cp.pt, &m_ptCp, sizeof(m_ptCp));
}

void HomoView::DrawImage(CDC* pDC)
{
	BasicView::DrawImage(pDC);
	//draw 
	if(m_pWrapRef) {
		
		CPen pen(PS_SOLID, 1, (m_bModified)? CONTROLLER_COLOR1:
			CONTROLLER_COLOR2);
		POINT pt[4] ;
		CPen* pOld = pDC->SelectObject(&pen);
		for(int i=0;i<4;i++){
			pt[i] = m_ptCp[i];
			DocToViewPort(pt[i]);
			pt[i].x += (m_rcImage.left- m_rcPort.left);
			pt[i].y += (m_rcImage.top - m_rcPort.top);
			if(i==0)
				pDC->MoveTo(pt[0]);
			else
				pDC->LineTo(pt[i]);
			//draw control pointer
			pDC->Ellipse(pt[i].x - CP_SIZE, pt[i].y - CP_SIZE, pt[i].x + CP_SIZE, pt[i].y + CP_SIZE);
		}
		pDC->LineTo(pt[0]);

		pDC->SelectObject(pOld);
	}
}