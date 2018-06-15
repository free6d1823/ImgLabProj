#include "StdAfx.h"
#include "resource.h"
#include "BasicView.h"
#include "HomoGride.h"
#include "HomoTool.h"

#define COLOR_NORMAL		RGB(255,255,0)
#define COLOR_MODIFIED		RGB(255,0,0)

ToolGride::ToolGride(BasicView* pOwner)
{
	m_pOwner = pOwner;

}
ToolGride::~ToolGride(void)
{
}
void ToolGride::SetImageArea(RECT& rcRange)
{
	m_rcRange = rcRange;
}
void ToolGride::SetViewPort(RECT& rcViewPort)
{
	m_rcViewPort = rcViewPort;
}
////////////////////////////////////////////////////////////////////////////
#define CP_SIZE		4
HomoGride::HomoGride(BasicView* pOwner): ToolGride(pOwner)
{
	m_bModified = FALSE;
	m_bDragMode = FALSE;
}

HomoGride::~HomoGride(void)
{
}
void HomoGride::Draw(CDC* pDC)
{
		CPen pen(PS_SOLID, 1, (m_bModified)? COLOR_MODIFIED:
			COLOR_NORMAL);
		POINT pt[4] ;
		CPen* pOld = pDC->SelectObject(&pen);
		for(int i=0;i<4;i++){
			pt[i] = m_ptCp[i];//on canvas
			m_pOwner->DocToViewPort(pt[i]);
			pt[i].x += (m_rcRange.left- m_rcViewPort.left);
			pt[i].y += (m_rcRange.top - m_rcViewPort.top);
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

/*!< Control Pointers coordinate on Canvas space */
void HomoGride::SetParam(void* pParam)
{
	HomoWrap* pWrap = (HomoWrap*) pParam; 
	for (int i=0; i< sizeof(m_ptCp)/sizeof(POINT); i++) {
		//doc to canvas
		m_ptCp[i].x = (LONG) (pWrap->param.q[i].x + 0.5);
		m_ptCp[i].y = (LONG) (pWrap->param.q[i].y + 0.5);
	}
}

/*!< Control Pointers coordinate on Canvas space */
void HomoGride::GetParam(void* pParam)
{
	HomoWrap* pWrap = (HomoWrap*) pParam; 
	for (int i=0; i< sizeof(m_ptCp)/sizeof(POINT); i++) {
		pWrap->param.q[i].x  = (double) m_ptCp[i].x;
		pWrap->param.q[i].y  = (double) m_ptCp[i].y;


	}
}
/*! Mouse event handling. 
*   pos is Canvas coordinate.
*/
void HomoGride::OnMouseMove(CWnd* pWnd, POINT pos)
{
	if(m_bDragMode) {
		//convert canvas to doc
		m_ptCp[m_idSelected] = pos;
		pWnd->InvalidateRect(NULL, FALSE);
	} else {
		int index;
		if( IsOnControlPoint(pos, index)) {
			SetCursor(::LoadCursor(NULL, IDC_CROSS));
		}else {
			SetCursor(::LoadCursor(NULL, IDC_ARROW));
		}
	}
}
void HomoGride::OnLButtonDown(CWnd* pWnd, POINT pos)
{
	if( !m_bDragMode) {
		int index;

		if( IsOnControlPoint(pos, index)) {
			pWnd->SetCapture();
			m_bDragMode = TRUE;
			m_idSelected = index;
		}
	}
}
void HomoGride::OnLButtonUp(CWnd* pWnd, POINT pos)
{
	if(m_bDragMode) {
		ReleaseCapture();
		m_bDragMode = FALSE;
		SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}
}

/*! Check if ptCursor (canvas coordinate) is on a control point
 *  if return TRUE, index returns the index of control points (0~3)
 */
BOOL	HomoGride::IsOnControlPoint(POINT ptCursor, int& index)
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
