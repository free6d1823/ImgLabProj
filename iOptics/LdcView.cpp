// LdcView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "LdcView.h"
#include "MeasurePerformance.h"

// LdcView

IMPLEMENT_DYNAMIC(LdcView, ProcessView)

LdcView::LdcView() : ProcessView()
{
}

LdcView::~LdcView()
{

}


BEGIN_MESSAGE_MAP(LdcView, ProcessView)

	ON_COMMAND(ID_PROCESS_PREVIEW, OnPreview)
END_MESSAGE_MAP()


//(u, v) destination coordinates
//(x,y)  source coordinates
BOOL		LdcView::PosMap(int u, int v, float&x, float&y)
{
	return ProcessView::PosMap(u,v,x,y);
}

//interpolate the color in position x,y in source coordinates
COLORREF LdcView::GetColor(BYTE* pSrc, int stride, float x, float y)
{
	return ProcessView::GetColor(pSrc, stride, x, y);
}

void LdcView::OnPreview()
{
MeasurePerformance mp;
mp.start();
		ProcessView::OnPreview();
mp.end();
TRACE("LdcView one frame takes %f ms\n", mp.duration());
}
