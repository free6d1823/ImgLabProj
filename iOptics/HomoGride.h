#pragma once

class BasicView;
class ToolGride
{
public:
	ToolGride(BasicView* pOwner);
	~ToolGride();
	virtual void Draw(CDC* pDC){};
	/*!< Sets CP coordinates in Canvas space*/
	virtual void SetParam(void* pParam){};
	/*!< Read back  CP coordinates in Canvas space. 
		\param pParam	pointer to HomoWrap which will receive CP values
	*/
	virtual void GetParam(void* pParam){};
	void SetImageArea(RECT& rcRange);
public:
	void SetViewPort(RECT& rcViewPort);
	/*! Mouse event handling. 
	*   pos is Canvas coordinate.
	*/
	virtual void OnMouseMove(CWnd* pWnd, POINT pos){};
	virtual void OnLButtonDown(CWnd* pWnd, POINT pos){};
	virtual void OnLButtonUp(CWnd* pWnd, POINT pos){};
protected:
	BasicView*	m_pOwner;
	RECT		m_rcViewPort; //display area
	RECT		m_rcRange;		//image location
	
};

class HomoGride :
	public ToolGride
{
public:
	HomoGride(BasicView* pOwner);
	~HomoGride();
	virtual void Draw(CDC* pDC);
	/*!< Sets CP coordinates in Canvas space*/
	virtual void SetParam(void* pParam);
	/*!< Read back  CP coordinates in Canvas space. 
		\param pParam	pointer to HomoWrap which will receive CP values
	*/
	virtual void GetParam(void* pParam);

	/*! Mouse event handling. 
	*   pos is position on image related to op-left corner
	*/
	virtual void OnMouseMove(CWnd* pWnd, POINT pos);
	virtual void OnLButtonDown(CWnd* pWnd, POINT pos);
	virtual void OnLButtonUp(CWnd* pWnd, POINT pos);
protected:
	/*! Check if ptCursor (canvas coordinate) is on a control point
	 *  if return TRUE, index returns the index of control points (0~3)
	 */
	BOOL	IsOnControlPoint(POINT ptCursor, int& index);
	BOOL	m_bModified;
	BOOL m_bDragMode;
	int	m_idSelected;				/*!< current selected point index*/
	POINT	m_ptCp[4];			/*!< 4 control points on original image coordinates*/

};
