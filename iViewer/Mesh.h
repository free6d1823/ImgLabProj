#pragma once

#include "ImgFile.h"

// Mesh
typedef struct meshControlPoint{
	float u;
	float v;
}MeshControlPoint, *PMeshControlPoint;

typedef struct meshControlData{
	int	column; //number of column
	int   row;		//number of row
	MeshControlPoint* pCp;
}MeshControlData;

class ImgFile;
class Mesh 
{
public:
	Mesh();
	virtual ~Mesh();
	BOOL Init(int column, int row);
	void	Release();
	//remap mesh to real world coordinates
	void	SetLocationOnCanvas(RECT& rcLocation);
	//Display mesh within rcViewPort, show Control point only if bShowAll is FALSE
	void	Draw(CDC* pDC, RECT* pViewPort, BOOL bShowAll);
	//Move indexed control point by offset with real world coordinates
	void	MoveControlPoint(int m, int n, POINT offset);
	//Check if ptCursor (canvas coordinate) is on a control point
	//if return TRUE, CP index is write tp index
	BOOL	IsOnControlPoint(POINT ptCursor, POINT& index);
	//process mouse event
	//pos is Canvas coordinate
	void OnMouseMove(CWnd* pWnd, POINT pos);
	void OnLButtonDown(CWnd* pWnd, POINT pos);
	void OnLButtonUp(CWnd* pWnd, POINT pos);

	//mesh operation
	ImgFile* ApplyImage(ImgFile* pSrc, POINT* ptNewOrigin);

private:
	/* mouse events */
	BOOL	m_bDragMeshMode;
	POINT	m_ptPrev;	
	POINT	m_idSelected; //current selected index
	//
	RECT		m_rcLocation; //clocation on canvas
	MeshControlData	m_cd;
	POINT*						m_ptListReal;
};


