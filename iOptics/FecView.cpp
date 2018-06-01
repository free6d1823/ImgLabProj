// FecView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "FecView.h"
#include "ImgFile.h"
#include "math.h"
#include "MeasurePerformance.h"

// FecView


IMPLEMENT_DYNAMIC(FecView, ProcessView)

FecView::FecView() : ProcessView()
{
	m_pParamRef = NULL;
}

FecView::~FecView()
{

}


BEGIN_MESSAGE_MAP(FecView, ProcessView)

	ON_COMMAND(ID_PROCESS_PREVIEW, OnPreview)
END_MESSAGE_MAP()


#define FEC_PARAM() ((FecWrap*)m_pParamRef)->param
void FecView::SetParam(ProcessWrap* pParam)
{
	m_pParamRef = (FecWrap*) pParam;
}

/*\brief Position mapping from processed image to original image
* \param[in] u [0~szOutput.cx) in rocessed image
* \param[in] v [0~szOutput.cy) in rocessed image
* \param[out] x [0~szInput.cx) in rocessed image
* \param[out] y [0~szInput.cy) in rocessed image
*/
BOOL		FecView::PosMap(int u, int v, float&x, float&y)
{
	return m_pParamRef->PosMap(u,v,x,y);
}


//interpolate the color in position x,y in source coordinates
COLORREF FecView::GetColor(BYTE* pSrc, int stride, float x, float y)
{
	if(x<0 || y<0 || x>= (float)FEC_PARAM().szInput.cx ||y>= (float)FEC_PARAM().szInput.cy)
		return 0;
	return ProcessView::GetColor(pSrc, stride, x,y);

}

void FecView::OnPreview()
{
MeasurePerformance mp;
	if(!m_pParamRef) {
		TRACE("should call SetParam first!!\n");
		ASSERT(0);
		return; 
	}
mp.start();
		ProcessView::OnPreview();
mp.end();
TRACE("FEC one frame takes %f ms\n", mp.duration());
}
