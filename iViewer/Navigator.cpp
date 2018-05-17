// Navigator.cpp : implementation file
//

#include "stdafx.h"
#include "iViewer.h"
#include "MainView.h"
#include "Navigator.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Navigator

#define WINDOW_WIDTH	150
#define WINDOW_HEIGHT	170
Navigator::Navigator()
{
}

Navigator::~Navigator()
{
}


BEGIN_MESSAGE_MAP(Navigator, CWnd)
	//{{AFX_MSG_MAP(Navigator)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Navigator message handlers

BOOL Navigator::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void Navigator::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void Navigator::OnPaint() 
{
	ImgFile*	pImg = m_pParent->GetImgFile();
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if(pImg)
	{
		CPen pen(PS_SOLID,2,0xff0000);
		CRect rcBorder = m_rcImg;
		CRect rcSrc = CRect(0,0,pImg->Width(), pImg->Height());
		dc.SelectObject(&pen);
		rcBorder.InflateRect(1,1);
		dc.Rectangle(rcBorder);
		pImg->StretchToDC(dc,rcSrc, m_rcImg);
	}
	// Do not call CWnd::OnPaint() for painting messages
}

void Navigator::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnLButtonDown(nFlags, point);
}

void Navigator::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnLButtonUp(nFlags, point);
}

void Navigator::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnMouseMove(nFlags, point);
}

BOOL Navigator::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL Navigator::Create(CWnd *pWnd,  int x, int y)
{
	CString szClassName = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW,
      ::LoadCursor(NULL, IDC_ARROW),
      (HBRUSH) ::GetStockObject(LTGRAY_BRUSH));

	m_pParent = (CMainView*) pWnd;
	return CWnd::CreateEx(
		WS_EX_TOPMOST|WS_EX_DLGMODALFRAME ,//WS_EX_TOOLWINDOW|WS_EX_TOPMOST,
		szClassName, "Navigator", 
		WS_POPUPWINDOW |WS_VISIBLE|WS_CAPTION,
		x,y, WINDOW_WIDTH, WINDOW_HEIGHT, 
		pWnd->GetSafeHwnd(), 
		NULL, NULL);

}

BOOL Navigator::SetImage(CDib* pDib)
{
	ImgFile*	pImg = m_pParent->GetImgFile();
	if(!pImg)
		return FALSE;
	CRect rcBound;
	GetClientRect(&rcBound);
	rcBound.DeflateRect(2,2);

	if(pImg->Width()*rcBound.Height() > pImg->Height()* rcBound.Width())
	{
		m_rcImg.left = rcBound.left;
		m_rcImg.right = rcBound.right;
		int h = (rcBound.Height()-rcBound.Width()*pImg->Height()/pImg->Width() )/2;
		m_rcImg.top = rcBound.top + h;
		m_rcImg.bottom = rcBound.bottom - h;
	}
	else
	{
		m_rcImg.top = rcBound.top;
		m_rcImg.bottom = rcBound.bottom;
		int w = (rcBound.Width()-rcBound.Height()*pDib->Width()/pDib->Height() )/2;
		m_rcImg.left = rcBound.left + w;
		m_rcImg.bottom = rcBound.bottom - w;

	}

	ShowWindow(SW_SHOW);
	return TRUE;
}

BOOL Navigator::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CWnd::OnEraseBkgnd(pDC);
}
