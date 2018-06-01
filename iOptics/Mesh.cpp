/****************************************************************************
 * Copyright (C) 2018 nFore Technology Co. Inc.,                            *
 *                                                                          *
 ****************************************************************************/

/**
 * @file mesh.cpp
 * @author CJ Chang
 * @brief Mesh is used to draw look-up table of image transformation, as mesh like,  on an image.
 *
 */

#include "stdafx.h"
#include "Mesh.h"

//Control points
#define CP_SIZE		3
#define CP_COLOR	RGB(255,255,255)
//mesh line
#define LINE_COLOR	RGB(0,0,0)
#define CENTER_COLOR	RGB(255,0,0)
// Mesh
Mesh::Mesh()
{
	memset(&m_cd, 0, sizeof(m_cd));
	//m_cd.pCp = 0

	m_ptListReal = NULL;

	/* mouse events */
	m_bDragMeshMode = FALSE;
}

Mesh::~Mesh()
{
	Release();
}


BOOL Mesh::SetTable(PointDouble* pLut, int column, int row)
{
	if(column != m_cd.column||m_cd.row != row ) {
		Release();
		m_cd.column = column; //number of control points, so add 1
		m_cd.row = row;
		m_ptListReal = (POINT*) malloc( (column)*(row)*sizeof(POINT));
		if (!m_ptListReal)
			return FALSE;
		m_cd.pCp = (PointDouble*) malloc((column)*(row)* sizeof(PointDouble));
		if(!m_cd.pCp) {
			free(m_ptListReal);
			return FALSE;
		}
	}
	double x,y;
	if(pLut == NULL) {
		//init as square
		y=0;
		for (int i=0;i< m_cd.row; i++)  {
			x = 0;
			for(int j=0; j< m_cd.column; j++) {
				int index  = i*m_cd.column+j;
				m_cd.pCp[index].x = x;
				m_cd.pCp[index].y = y;
				x+= 1/(double)(column-1);
			}
			y+= 1/(double)(row-1);
		}
	}else {
		memcpy(m_cd.pCp, pLut, (column)*(row)* sizeof(PointDouble));
	}

	m_bDragMeshMode = FALSE;
	return TRUE;
}
void	Mesh::Release()
{
	if(m_ptListReal) {
		free(m_ptListReal);
		m_ptListReal = NULL;
	}
	if(m_cd.pCp) {
		free(m_cd.pCp);
		memset(&m_cd, 0, sizeof(m_cd));
	}
	m_cd.column = 0; 
	m_cd.row = 0;
}
//remap mesh to real world coordinates
void	Mesh::SetLocationOnCanvas(RECT& rcLocation)
{
	double cx  = (double) (rcLocation.right - rcLocation.left);
	double cy  = (double) (rcLocation.bottom - rcLocation.top);
#ifdef NO_UPSIDE_DOWN	//display order is top down 	
	for (int i=0;i< m_cd.row*m_cd.column; i++)  {
			m_ptListReal[i].x = rcLocation.left + (LONG) (cx*m_cd.pCp[i].x);
			m_ptListReal[i].y = rcLocation.top + (LONG) (cy*m_cd.pCp[i].y);
	}
#else	//for DIB, display order is UP_SIDE_DOWN
	int k;
	int m = 0;
	for (int i=m_cd.row - 1; i>=0; i--)  {
		k = i*m_cd.column;
		for(int j=0;j<m_cd.column; j++){
			m_ptListReal[k+j].x = rcLocation.left + (LONG) (cx*m_cd.pCp[m].x);
			m_ptListReal[k+j].y = rcLocation.top + (LONG) (cy*(1-m_cd.pCp[m].y));
			m++;
		}
	}

#endif
	m_rcLocation = rcLocation;
}
void	Mesh::MoveControlPoint(int m, int n, POINT offset)
{
	int index = n*m_cd.column+m;
	m_ptListReal[index].x += offset.x;
	m_ptListReal[index].y += offset.y;
	double difx = (double) offset.x / (double) (m_rcLocation.right - m_rcLocation.left);
	m_cd.pCp[index].x += difx;
	double dify = (double) offset.y / (double) (m_rcLocation.bottom - m_rcLocation.top);
	m_cd.pCp[index].y += dify;

}
BOOL	Mesh::IsOnControlPoint(POINT ptCursor, POINT& index)
{
	BOOL bFound = FALSE;
	for (int i=0;i< m_cd.row; i++)  {
		int k = i*m_cd.column;
		for(int j=0; j< m_cd.column; j++) {
			if (ptCursor.x <= m_ptListReal[k].x + CP_SIZE 
				&& ptCursor.x >= m_ptListReal[k].x - CP_SIZE
				&& ptCursor.y <= m_ptListReal[k].y + CP_SIZE
				&& ptCursor.y >= m_ptListReal[k].y - CP_SIZE) {
					index.x = j;
					index.y =i;
					bFound = TRUE;
					break;
			}
			k++;
		}
		if (bFound)
			break;
	}
	return bFound;
}

void Mesh::OnMouseMove(CWnd* pWnd, POINT pos)
{
	if(m_bDragMeshMode) {
		//process
		POINT ptOffset;
		ptOffset.x = (pos.x - m_ptPrev.x);
		ptOffset.y = (pos.y - m_ptPrev.y);
		MoveControlPoint(m_idSelected.x, m_idSelected.y, 
			ptOffset);
		m_ptPrev = pos;
		pWnd->InvalidateRect(NULL, FALSE);
	} else {
		POINT index;
		if( IsOnControlPoint(pos, index)) {
			SetCursor(::LoadCursor(NULL, IDC_CROSS));
		}else {
			SetCursor(::LoadCursor(NULL, IDC_ARROW));
		}
	}
}
void Mesh::OnLButtonDown(CWnd* pWnd, POINT pos)
{
	if( !m_bDragMeshMode) {
		POINT index;
		if( IsOnControlPoint(pos, index)) {
			pWnd->SetCapture();
			m_bDragMeshMode = TRUE;
			m_idSelected = index;
			m_ptPrev = pos;
		}
	}
}
void Mesh::OnLButtonUp(CWnd* pWnd, POINT pos)
{
	if(m_bDragMeshMode) {
		ReleaseCapture();
		m_bDragMeshMode = FALSE;
		SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}
}

//Display mesh within rcViewPort, show Control point only if bShowAll is FALSE
void	Mesh::Draw(CDC* pDC, RECT* pViewPort, BOOL bShowAll)
{
	int i,j;
	if(!m_ptListReal)
		return;
	//draw control points
	CPen* pOldPen;
	CPen penCP(PS_SOLID, 1, CP_COLOR);
	pOldPen = pDC->SelectObject(&penCP);

	for ( i=0;i< m_cd.row; i++)  {
		for( j=0; j< m_cd.column; j++) {
			int index = i*m_cd.column+j;
			POINT pt;
			pt.x = m_ptListReal[index].x - pViewPort->left;
			pt.y = m_ptListReal[index].y - pViewPort->top;
			pDC->Ellipse(pt.x-CP_SIZE, pt.y-CP_SIZE, 
				pt.x+CP_SIZE, pt.y+CP_SIZE);
		}
	}
	CPen penLine1(PS_SOLID, 1, LINE_COLOR);
	CPen penLine2(PS_SOLID, 1, CENTER_COLOR);

	if(bShowAll) {
		//draw horizon
		for(i=0; i<m_cd.row; i++)  {
			if ( i == m_cd.row/2 )
				pDC->SelectObject(&penLine2);
			else
				pDC->SelectObject(&penLine1);
			POINT pt;
			int index = i*m_cd.column;
			pt.x = m_ptListReal[index].x - pViewPort->left;
			pt.y = m_ptListReal[index].y - pViewPort->top;
			//draw segment
			pDC->MoveTo(pt);
			for(j=1;j< m_cd.column; j++) {
				index ++;
				pt.x = m_ptListReal[index].x - pViewPort->left;
				pt.y = m_ptListReal[index].y - pViewPort->top;
				pDC->LineTo(pt);
			}
		}
		//draw vertical

		for(i=0; i<m_cd.column; i++)  {
			if ( i == m_cd.column/2 )
				pDC->SelectObject(&penLine2);
			else
				pDC->SelectObject(&penLine1);
			POINT pt;
			int index = i;
			pt.x = m_ptListReal[i].x - pViewPort->left;
			pt.y = m_ptListReal[i].y - pViewPort->top;
			//draw segment
			pDC->MoveTo(pt);
			for(j=1;j< m_cd.row; j++) {
				index += m_cd.column;
				pt.x = m_ptListReal[index].x - pViewPort->left;
				pt.y = m_ptListReal[index].y - pViewPort->top;
				pDC->LineTo(pt);
			}
		}
	}

	pDC->SelectObject(pOldPen);
}
