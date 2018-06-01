/****************************************************************************
 * Copyright (C) 2018 nFore Technology Co. Inc.,                            *
 *                                                                          *
 ****************************************************************************/
/**
 * \file Mesh.h declarations of class Mesh
 */
#pragma once
#include "common.h"
#include "ImgFile.h"

/**
 * \struct _MeshControlData
 */
typedef struct _MeshControlData{
	int	column;			/*!< number of column */
	int   row;			/*!< number of row*/
	PointDouble* pCp;	/*!< start address of the control data*/
}MeshControlData;

/**
 * \class Mesh
 * \brief Draw mesh on the specified area
 *  working flow
 @dot
  digraph mesh_flow {
		edge [fontname="Helvetica",fontsize="10",labelfontname="Helvetica",labelfontsize="10"];
		node [fontname="Helvetica",color="black", height=0.2,width=0.4,fontsize="10",shape=record];
		event1 [label="LUT dataupdated", shape= "plaintext"];
		event1 -> SetTable [ arrowhead="open", style="dashed" ];
		event2 [label="Image position changed or LUT table updated", shape= "plaintext"];
		event2 ->SetLocationOnCanvas[ arrowhead="open", style="dashed" ];
		event3 [label="On drawing",  shape= "plaintext"];
		event3 -> Draw[ arrowhead="open", style="dashed" ];
		event4 [label="Free resource", shape = "plaintext"]
		event4 ->Release[ arrowhead="open", style="dashed" ];
	}
 @enddot
 */
class ImgFile;
class Mesh 
{
public:
	Mesh();
	virtual ~Mesh();

	/*! Sets the LUT data and specify its dimension.
	* this function is called when content of table is changed
	* \param[in] pLut LUT contents to set. If pLut != NULL, copy pLut to pCp, else makes pCp as rectangle grides 
	* \param[in] column numbers of vertical lines, including border
	* \param[in] row numbers of horizontal lines, including border
	* \return FALSE if failed to allocat internal LUT memory.
	*/
	BOOL SetTable(PointDouble* pLut, int column, int row);

	/*! Clears all resource. 
	*/
	void	Release();

	/*! Remap mesh to real world coordinates
	*/
	void	SetLocationOnCanvas(RECT& rcLocation);

	/*! Display mesh within rcViewPort, show Control point only if bShowAll is FALSE
	*/
	void	Draw(CDC* pDC, RECT* pViewPort, BOOL bShowAll);

	/*! Move indexed control point by offset with real world coordinates
	*/
	void	MoveControlPoint(int m, int n, POINT offset);

	/*! Check if ptCursor (canvas coordinate) is on a control point
	 *  if return TRUE, CP index is write tp index
	 *  Display mesh within rcViewPort, show Control point only if bShowAll is FALSE
	 */
	BOOL	IsOnControlPoint(POINT ptCursor, POINT& index);
	
	/*! Mouse event handling. 
	*   pos is Canvas coordinate.
	*/
	void OnMouseMove(CWnd* pWnd, POINT pos);
	void OnLButtonDown(CWnd* pWnd, POINT pos);
	void OnLButtonUp(CWnd* pWnd, POINT pos);

private:
	BOOL	m_bDragMeshMode;	/*!< TRUE if Control Points are movable*/
	POINT	m_ptPrev;			/*!< last drag position, related to focus window*/
	POINT	m_idSelected;		/*!< current selected index of X and Y */
	RECT	m_rcLocation;		/*!< location of the image on canvas */ 
	MeshControlData	m_cd;		/*!< the CD buffers */
	POINT*	m_ptListReal;		/*!< the coordings of CP in current wiondow */
};


