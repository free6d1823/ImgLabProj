// LdcView.cpp : implementation file
//

#include "stdafx.h"
#include "iViewer.h"
#include "LdcView.h"


// LdcView

IMPLEMENT_DYNAMIC(LdcView, ProcessView)

LdcView::LdcView()
{

}

LdcView::~LdcView()
{
}


BEGIN_MESSAGE_MAP(LdcView, BasicView)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// LdcView message handlers



int LdcView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (BasicView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_tool.Create(IDD_LDC, this);

	return 0;
}

void LdcView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	DrawImage(&dc);
}
