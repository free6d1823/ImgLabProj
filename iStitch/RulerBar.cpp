// RulerBar.cpp : implementation file
//

#include "stdafx.h"
#include "RulerBar.h"

// RulerCorner
#define ID_RULERBAR	0x1225
#define RULER_COLOR	RGB(200,210,250)
#define RULERCORNER_COLOR	RGB(180,190,230)

#define RULER_SIZE				15

IMPLEMENT_DYNAMIC(RulerCorner, CWnd)

RulerCorner::RulerCorner()
{
}

RulerCorner::~RulerCorner()
{
}

BEGIN_MESSAGE_MAP(RulerCorner, CWnd)
		ON_WM_PAINT()
END_MESSAGE_MAP()

// RulerBar message handlers
BOOL	RulerCorner::Create(CWnd* pParent)
{
	return CreateEx(0, _T("STATIC"), _T(""), WS_CHILD|WS_VISIBLE, 
		0,0,RULER_SIZE,RULER_SIZE, pParent->m_hWnd, (HMENU)ID_RULERBAR+3 );

}
BOOL RulerCorner::PreCreateWindow(CREATESTRUCT& cs) 
{
	// create a brush using the same color as the background
	if (cs.lpszClass == NULL)
	{
		HBRUSH hBr=CreateSolidBrush(GetSysColor(COLOR_MENU));
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS|CS_BYTEALIGNWINDOW, NULL, hBr);
	}

	return CWnd::PreCreateWindow(cs);
}
void RulerCorner::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	RECT rc;
	GetClientRect(&rc);
	dc.FillSolidRect(&rc, RULERCORNER_COLOR);
}

// RulerBar

IMPLEMENT_DYNAMIC(RulerBar, CWnd)

RulerBar::RulerBar()
{

}

RulerBar::~RulerBar()
{
}


BEGIN_MESSAGE_MAP(RulerBar, CWnd)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// RulerBar message handlers


BOOL	RulerBar::Create(CWnd* pParent, BOOL bTypeHoriz)
{
	m_bTypeHoriz = bTypeHoriz;
	return CreateEx(0, _T("STATIC"), _T(""), WS_CHILD|WS_VISIBLE, 
		0,0,RULER_SIZE,RULER_SIZE, pParent->m_hWnd, (HMENU)ID_RULERBAR+bTypeHoriz );

}
void RulerBar::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}

BOOL RulerBar::PreCreateWindow(CREATESTRUCT& cs) 
{
	// create a brush using the same color as the background
	if (cs.lpszClass == NULL)
	{
		HBRUSH hBr=CreateSolidBrush(GetSysColor(COLOR_MENU));
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS|CS_BYTEALIGNWINDOW, NULL, hBr);
	}

	return CWnd::PreCreateWindow(cs);
}
int RulerBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

void RulerBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	RECT rc;
	GetClientRect(&rc);
	dc.FillSolidRect(&rc, RULER_COLOR);
	if(m_bTypeHoriz) {
		dc.MoveTo(0, rc.bottom/2);
		dc.LineTo(rc.right, rc.bottom/2);

	} else {
		dc.MoveTo(rc.right/2, 0);
		dc.LineTo(rc.right/2, rc.bottom);

	}
}
