// Mesh.cpp : implementation file
//

#include "stdafx.h"
#include "Mesh.h"

//Control points
#define CP_SIZE		3
#define CP_COLOR	RGB(255,255,255)
//mesh line
#define LINE_COLOR	RGB(0,0,0)

// Mesh
Mesh::Mesh()
{
	memset(&m_cd, 0, sizeof(m_cd));
	m_ptListReal = NULL;

		/* mouse events */
	m_bDragMeshMode = FALSE;
}

Mesh::~Mesh()
{
	Release();
}
BOOL Mesh::Init(int column, int row)
{
	Release();
	m_cd.column = column+1; //number of control points, so add 1
	m_cd.row = row+1;
	m_ptListReal = (POINT*) malloc( (column+1)*(row+1)*sizeof(POINT));
	if (!m_ptListReal)
		return FALSE;
	m_cd.pCp = (PMeshControlPoint) malloc((column+1)*(row+1)* sizeof(MeshControlPoint));
	if(!m_cd.pCp) {
		free(m_ptListReal);
		return FALSE;
	}
	float x,y;
	y=0;
	for (int i=0;i< m_cd.row; i++)  {
		x = 0;
		for(int j=0; j< m_cd.column; j++) {
			int index  = i*m_cd.column+j;
			m_cd.pCp[index].u = x;
			m_cd.pCp[index].v = y;
			x+= 1/(float)column;
		}
		y+= 1/(float)row;
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
}
//remap mesh to real world coordinates
void	Mesh::SetLocationOnCanvas(RECT& rcLocation)
{
	float cx  = (float) (rcLocation.right - rcLocation.left);
	float cy  = (float) (rcLocation.bottom - rcLocation.top);

	for (int i=0;i< m_cd.row; i++)  {
		for(int j=0; j< m_cd.column; j++) {
			int index = i*m_cd.column+j;
			m_ptListReal[index].x = rcLocation.left + (LONG) (cx*m_cd.pCp[index].u);
			m_ptListReal[index].y = rcLocation.top + (LONG) (cy*m_cd.pCp[index].v);
		}
	}
	m_rcLocation = rcLocation ;
}
void	Mesh::MoveControlPoint(int m, int n, POINT offset)
{
		int index = n*m_cd.column+m;
		m_ptListReal[index].x += offset.x;
		m_ptListReal[index].y += offset.y;
		float difx = (float) offset.x / (float) (m_rcLocation.right - m_rcLocation.left);
		m_cd.pCp[index].u += difx;
		float dify = (float) offset.y / (float) (m_rcLocation.bottom - m_rcLocation.top);
		m_cd.pCp[index].v += dify;

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
					index.x = j; index.y =i;
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
	CPen penLine(PS_SOLID, 1, LINE_COLOR);
	pDC->SelectObject(&penLine);
	if(bShowAll) {
		//draw horizon
		for(i=0; i<m_cd.row; i++)  {
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

//apply the mesh to pSrc
//return the ew ImgFile and its top-left position (doc coordinate) relative to pSrc top-left
ImgFile* Mesh::ApplyImage(ImgFile* pSrc, POINT* ptNewOrigin)
{
	ImgFile* pImg = NULL;
	float umin=100, umax= -100, vmin= 100,vmax=-100;
	int i,j,k;
	k=0;
	for(i=0; i<m_cd.row; i++)  {
			for(j=0;j< m_cd.column; j++, k++) {
				if(m_cd.pCp[k].u > umax) umax=  m_cd.pCp[k].u;
				if(m_cd.pCp[k].v > vmax) vmax=  m_cd.pCp[k].v;			
				if(m_cd.pCp[k].u < umin) umin=  m_cd.pCp[k].u;
				if(m_cd.pCp[k].v < vmin) vmin=  m_cd.pCp[k].v;
			}
	}
	float w1 = (float)pSrc->Width();
	float h1 = (float)pSrc->Height();
	float w2 = w1 * (umax - umin);
	float h2 = h1 * (vmax - vmin);
	

	return pImg;
}