// Gride.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "Gride.h"
#include "common.h"

// Gride
Gride::Gride(COLORREF color)
{
	m_rcViewPort = CRect(0,0,800,600); //display area
	m_color = color;	//gride line color
	m_rcRange  = m_rcViewPort;		//image location
	m_nInterval = 2;
	m_nDivider = 8;

}

Gride::~Gride()
{
}
void Gride::SetParam(void* pParam)
{
	GrideParam* pPar = (GrideParam*) pParam;
	m_nInterval = pPar->nInterval;
	m_nDivider= pPar->nDivider;
}
void Gride::SetImageArea(RECT& rcRange)
{
	m_rcRange = rcRange;
}

void Gride::SetViewPort(RECT& rcViewPort)
{
	m_rcViewPort = rcViewPort;
}


void Gride::Draw(CDC* pDC)
{
	CRect rect ;
	BOOL bInt = rect.IntersectRect(&m_rcRange, &m_rcViewPort);
	if(!bInt) 
		return;
	POINT ptStart, ptStart2,ptEnd;

	CPen penCenter(PS_SOLID, 1, m_color);
	CPen penMain(PS_DASH, 1, m_color);
	CPen penMinor(PS_DOT, 1, m_color);
	CPen* pOldPen;
	float p1, p2;
	ptStart.y = rect.top;
	ptStart.y -= m_rcViewPort.top;
	ptStart2.y = ptStart.y;
	ptEnd.y = rect.bottom - m_rcViewPort.top;

	//draw center vertical
	p1 = (float) (m_rcRange.left + m_rcRange.right)/2;
	ptStart.x = (LONG) p1;

	pOldPen = pDC->SelectObject(&penCenter);
	if(ptStart.x >= rect.left && ptStart.x<= rect.right) {
		//shift to origin
		ptStart.x -= m_rcViewPort.left;
		ptEnd.x = ptStart.x;
		pDC->MoveTo(ptStart);
		pDC->LineTo(ptEnd);
	}
	//draw vertical
	p2 = p1;
	float  interval = (float)(m_rcRange.right - m_rcRange.left)/(float)m_nDivider;
	for(int i=1;i<= m_nDivider/2; i++) {

		if (i%m_nInterval == 0)
			pDC->SelectObject(&penMain);
		else
			pDC->SelectObject(&penMinor);

		p2-= interval;
		ptStart2.x = (LONG) p2;
		if(ptStart2.x  >= rect.left && ptStart2.x<= rect.right) {
			ptStart2.x -= m_rcViewPort.left;
			ptEnd.x = ptStart2.x;
			pDC->MoveTo(ptStart2);
			pDC->LineTo(ptEnd);
		}
		p1 += interval;
		ptStart.x = (LONG) p1;
		if(ptStart.x  >= rect.left && ptStart.x<= rect.right) {
			ptStart.x -= m_rcViewPort.left;
			ptEnd.x = ptStart.x;
			pDC->MoveTo(ptStart);
			pDC->LineTo(ptEnd);
		}
	}
	//----draw horizontal lines
	ptStart.x =  rect.left - m_rcViewPort.left;
	ptStart2.x = ptStart.x ;
	ptEnd.x = rect.right - m_rcViewPort.left;

	//draw center 
	p1 = (float) (m_rcRange.top + m_rcRange.bottom)/2;
	ptStart.y = (LONG) p1;

	pOldPen = pDC->SelectObject(&penCenter);
	if(ptStart.y >= rect.top && ptStart.y<= rect.bottom) {
		 ptStart.y -= m_rcViewPort.top;
		ptEnd.y = ptStart.y;
		pDC->MoveTo(ptStart);
		pDC->LineTo(ptEnd);
	}
//draw vertical
	p2 = p1;
	interval = (float)(m_rcRange.bottom - m_rcRange.top)/(float) m_nDivider;
	for(int i=1;i<= m_nDivider/2; i++) {

		if (i%m_nInterval == 0)
			pDC->SelectObject(&penMain);
		else
			pDC->SelectObject(&penMinor);
		p2 -= interval;
		ptStart2.y = (LONG) p2;
		if(ptStart2.y  >= rect.top&& ptStart2.y<= rect.bottom) {
			ptStart2.y -= m_rcViewPort.top;
			ptEnd.y = ptStart2.y;
			pDC->MoveTo(ptStart2);
			pDC->LineTo(ptEnd);
		}
		p1 += interval;
		ptStart.y = (LONG) p1;
		if(ptStart.y  >= rect.top && ptStart.y <= rect.bottom) {
			ptStart.y -= m_rcViewPort.top;
			ptEnd.y = ptStart.y;
			pDC->MoveTo(ptStart);
			pDC->LineTo(ptEnd);
		}
	}

}
