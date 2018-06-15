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
TRACE("Output %dx%d\n", FEC_PARAM().szOutput.cx, FEC_PARAM().szOutput.cy);
	if(m_pImage) {
		if( m_pImage->Width() == FEC_PARAM().szOutput.cx &&  
				m_pImage->Height() == FEC_PARAM().szOutput.cy)
			return;
		delete m_pImage;
	}
	m_pImage = new ImgFile();
	m_pImage->Create(FEC_PARAM().szOutput.cx,FEC_PARAM().szOutput.cy,24,0);
TRACE("Image %dx%d\n", m_pImage->Width() , m_pImage->Height());

	UpdateView();
}
void FecView::SetImgFile(ImgFile* pImg)
{
	ProcessView::SetImgFile(pImg);
}
/*\brief Position mapping from processed image to original image
* \param[in] u [0~szOutput.cx) in processed image
* \param[in] v [0~szOutput.cy) in processed image
* \param[out] x [0~szInput.cx) in source image
* \param[out] y [0~szInput.cy) in source image
*/
BOOL		FecView::PosMap(int u, int v, float&x, float&y)
{
	return m_pParamRef->PosMap(u,v,x,y);
}


//interpolate the color in position x,y in source coordinates
COLORREF FecView::GetColor(BYTE* pSrc, int stride, float x, float y)
{

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
