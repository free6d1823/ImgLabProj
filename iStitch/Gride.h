#pragma once

typedef struct _GrideParam{
	int			nIntervalH; /*!<number of horz grides between solide gride lines */
	int			nDividerH;	/*!<number of horz grides per row or column*/
	int			nIntervalV; /*!<number of vert grides between solide gride lines*/
	int			nDividerV;	/*!<number of vert grides per row or column*/

}GrideParam;
// Gride
class Gride
{
public:
	Gride(COLORREF color);
	virtual ~Gride();
	virtual void Draw(CDC* pDC);
	virtual void SetParam(void* pParam);
	void SetImageArea(RECT& rcRange);
	void SetViewPort(RECT& rcViewPort);
protected:
	RECT		m_rcViewPort; //display area
	RECT		m_rcRange;		//image location
	COLORREF	m_color;	//gride line color
	GrideParam	m_param;

};
