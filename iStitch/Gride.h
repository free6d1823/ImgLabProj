#pragma once

typedef struct _GrideParam{
	int			nInterval; //number of grides between solide gride lines
	int			nDivider;	//number of grides per row or column
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
	int			m_nInterval; //number of grides between solide gride lines
	int			m_nDivider;	//number of grides per row or column
};
