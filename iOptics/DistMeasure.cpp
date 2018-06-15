#include "StdAfx.h"
#include "BasicView.h"
#include "DistMeasure.h"
#include <math.h>

#define COLOR_NORMAL	RGB(0,255,0)
DistMeasure::DistMeasure(BasicView* pOwner): ToolGride(pOwner)
{
		RECT rc;
		pOwner->GetClientRect(&rc);
		m_bDragMode = FALSE;
		m_ToolTipCtrl.Create(pOwner, TTS_ALWAYSTIP);
		m_ToolTipCtrl.Activate(TRUE);
		m_ToolTipCtrl.SetDelayTime(10);
}

DistMeasure::~DistMeasure(void)
{
}

void DistMeasure::Draw(CDC* pDC)
{
	if(! m_bDragMode)
		return;
		CPen pen(PS_SOLID, 1, COLOR_NORMAL);
		CPen* pOld = pDC->SelectObject(&pen);
		POINT pt1 = m_ptStart;
		POINT pt2 = m_ptEnd;
		m_pOwner->DocToViewPort(pt1);
		m_pOwner->DocToViewPort(pt2);
		pt1.x += (m_rcRange.left- m_rcViewPort.left);
		pt1.y += (m_rcRange.top - m_rcViewPort.top);
		pt2.x += (m_rcRange.left- m_rcViewPort.left);
		pt2.y += (m_rcRange.top - m_rcViewPort.top);
		pDC->MoveTo(pt1);
		pDC->LineTo(pt2);
		pDC->SelectObject(pOld);
}

/*! Mouse event handling. 
*   pos is Canvas coordinate.
*/
void DistMeasure::OnMouseMove(CWnd* pWnd, POINT pos)
{
	if(m_bDragMode) {
		//convert canvas to doc
		m_ptEnd = pos;
		SIZE sz;
		sz.cx = m_ptEnd.x - m_ptStart.x;
		sz.cy = m_ptEnd.y - m_ptStart.y;


		CString szInfo;
		szInfo.Format("(%d,%d)->(%d,%d)\n size= [%dx%d]\ndistance=%8.1f\n",
			  m_ptStart.x, m_ptStart.y, m_ptEnd.x, m_ptEnd.y,
				sz.cx, sz.cy, sqrt((double)(sz.cx*sz.cx+ sz.cy*sz.cy) ));
		//
		m_pOwner->DocToViewPort(pos);
		pos.x +=  (m_rcRange.left- m_rcViewPort.left)+10;
		pos.y += (m_rcRange.top - m_rcViewPort.top);
		m_ToolTipCtrl.ShowText(pos, szInfo);

		pWnd->InvalidateRect(NULL, FALSE);
	}
}
void DistMeasure::OnLButtonDown(CWnd* pWnd, POINT pos)
{
	if( !m_bDragMode) {
			pWnd->SetCapture();
			m_bDragMode = TRUE;
			m_ptStart = pos;
			m_ptEnd = m_ptStart;
			SetCursor(::LoadCursor(NULL, IDC_CROSS));	
	}
}
void DistMeasure::OnLButtonUp(CWnd* pWnd, POINT pos)
{
	if(m_bDragMode) {
		ReleaseCapture();
		m_bDragMode = FALSE;
		SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}
}