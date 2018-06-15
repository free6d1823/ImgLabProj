#pragma once
#include "EasyTip.h"
#include "HomoGride.h"

class DistMeasure: public ToolGride
{
public:
	DistMeasure(BasicView* pOwner);
	~DistMeasure(void);
	virtual void Draw(CDC* pDC);
	/*! Mouse event handling. 
	*   pos is Canvas coordinate.
	*/
	virtual void OnMouseMove(CWnd* pWnd, POINT pos);
	virtual void OnLButtonDown(CWnd* pWnd, POINT pos);
	virtual void OnLButtonUp(CWnd* pWnd, POINT pos);
private:
	BOOL m_bDragMode;
    Easytip    m_ToolTipCtrl;
	POINT	m_ptStart;
	POINT	m_ptEnd;
};
