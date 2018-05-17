// FecView.cpp : implementation file
//

#include "stdafx.h"
#include "iViewer.h"
#include "FecView.h"
#include "ImgFile.h"
#include "math.h"
#include "MeasurePerformance.h"

// FecView


IMPLEMENT_DYNAMIC(FecView, ProcessView)

FecView::FecView() : ProcessView()
{
}

FecView::~FecView()
{

}


BEGIN_MESSAGE_MAP(FecView, ProcessView)

	ON_COMMAND(ID_PROCESS_PREVIEW, OnPreview)
END_MESSAGE_MAP()

void FecView::SetParam(void* pParam)
{
	memcpy(&m_param, pParam, sizeof(FecParam));
}


//(u, v) destination coordinates
//(x,y)  source coordinates
#define PI	3.1415967f
#define PI_2 (PI/2)
BOOL		FecView::PosMap(int u, int v, float&x, float&y)
{
	float uu, vv; //shift to center
	uu = (float)u-(float)(m_param.szOutput.cx)/2;
	vv = (float)v-(float)(m_param.szOutput.cy)/2;
	float ro2x, ro2y;
	float phi;
	float theda;
	switch(m_param.lt) {
		case LensType_1://tan(angle/2)
			ro2x = ((float)m_param.szOutput.cx/2)*1.0f; //1/tan(45)
			ro2y = ((float)m_param.szOutput.cy/2)*1.0f; //1/tan(45)
			phi = 2*atan(vv/ro2y);
			theda = 2*atan(uu/ro2x);
			break;
		case LensType_2: //sin(angle/2)
			ro2x = ((float)m_param.szOutput.cx/2)*1.414f; //1/sin(45)
			ro2y = ((float)m_param.szOutput.cy/2)*1.414f; //1/sin(45)
			phi = 2*asin(vv/ro2y);
			theda = 2*asin(uu/ro2x);
			break;
		case LensType_4: //sin(angle)
			ro2x =  ((float)m_param.szOutput.cx/2);
			ro2y =  ((float)m_param.szOutput.cy/2);
			phi = asin(vv/ro2y);
			theda = asin(uu/ro2x);
			break;
		case LensType_3: //angle
			 ro2x = ((float)m_param.szOutput.cx)/PI;
			 ro2y = ((float)m_param.szOutput.cy)/PI;
			phi = vv/ro2y;
			theda = uu/ro2x;
			break;
		default: //	LensType_0 or 5
				//just scale image 
				x = uu*2*m_param.szRadius.cx/(m_param.szOutput.cx) +  (float) m_param.ptCenter.x;
				y=  vv*2*m_param.szRadius.cy/(m_param.szOutput.cy) + (float)m_param.ptCenter.y;
			return TRUE;
	}
	float rox = (float)(m_param.szRadius.cx);
	float roy = (float)(m_param.szRadius.cy);
	x = rox* cos(phi) * sin(theda) +  (float) m_param.ptCenter.x;
	y = roy*sin(phi) + (float)m_param.ptCenter.y;

	if(x >= (float)m_param.szInput.cx)
		x = (float)m_param.szInput.cx-1;
	if( x < 0) x=0;
	if(y >= (float)m_param.szInput.cy)
		y = (float)m_param.szInput.cy -1;
	if(y<0) y=0;
	return TRUE;
}

//interpolate the color in position x,y in source coordinates
COLORREF FecView::GetColor(BYTE* pSrc, int stride, float x, float y)
{
	COLORREF cr;
	BYTE* p;
	int x0 = (int) (x+0.5);
	int y0 = (int)(y+0.5);
	int offset = (int)(y0*stride+ x0*3);
	p = pSrc + offset;
	cr = RGB(*(p+2), *(p+1), *p);
	return cr;
}

void FecView::OnPreview()
{
MeasurePerformance mp;
mp.start();
		ProcessView::OnPreview();
mp.end();
TRACE("FEC one frame takes %f ms\n", mp.duration());
}
