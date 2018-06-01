 // MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "resource.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_COLOR,
	ID_INDICATOR_POS,
	ID_INDICATOR_ZOOM,
	ID_INDICATOR_SIZE
};

static UINT BASED_CODE processtool[] =
{
	ID_TOOLBOX_TOOGLE,
	ID_TOOLBOX_APPLY,
	ID_TOOLBOX_FEC,
	ID_TOOLBOX_LDC,
	ID_TOOLBOX_3,
	ID_TOOLBOX_4,
	ID_TOOLBOX_5,
	ID_TOOLBOX_6,
	ID_TOOLBOX_7,
	ID_TOOLBOX_ZOOMIN,
	ID_TOOLBOX_ZOOMOUT,
	ID_TOOLBOX_GRIDE,
	ID_TOOLBOX_SAVE,
	ID_TOOLBOX_LOAD
};
/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	EnableDocking(CBRS_ALIGN_ANY);

	if(!CreateToolBar())
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	if (!CreatePaletteBar())
		return -1;

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

BOOL CMainFrame::CreatePaletteBar()
{
	if (!m_wndToolBox.Create(this, WS_CHILD | WS_VISIBLE | CBRS_SIZE_FIXED |
		CBRS_TOP | CBRS_TOOLTIPS, ID_TOOLBOX) ||
		!m_wndToolBox.LoadBitmap(IDB_TOOLBOX) ||
		!m_wndToolBox.SetButtons(processtool,
		  sizeof(processtool)/sizeof(UINT)))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;       // fail to create
	}
	m_wndToolBox.SetWindowText(_T("Tool Box"));
	m_wndToolBox.EnableDocking(0);

	// Create the Palette.  We are using hardcoded numbers for ease here
	// normally the location would be read in from an ini file.
	CPoint pt(GetSystemMetrics(SM_CXSCREEN) - 100,
		GetSystemMetrics(SM_CYSCREEN) / 3);

	m_wndToolBox.SetColumns(2);
	FloatControlBar(&m_wndToolBox, pt);

	return TRUE;
}

BOOL CMainFrame::CreateToolBar()
{
	if (!m_wndToolBar.Create(this,
			CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|WS_VISIBLE) ||
		!m_wndToolBar.LoadBitmap(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;       // fail to create
	}

	OnViewShort();
	return TRUE;
}

void CMainFrame::OnViewShort()
{
	// Set the toolbar to show only partial commmand list
	m_wndToolBar.SetButtons(NULL, 8);
	m_wndToolBar.SetButtonInfo(0, ID_FILE_OPEN,TBBS_BUTTON, 0);
	m_wndToolBar.SetButtonInfo(1, ID_FILE_SAVE,TBBS_BUTTON, 1);
	m_wndToolBar.SetButtonInfo(2, ID_SEPARATOR,TBBS_SEPARATOR, 6);
	m_wndToolBar.SetButtonInfo(3, ID_VIEW_ZOOMIN,TBBS_BUTTON, 2);
	m_wndToolBar.SetButtonInfo(4, ID_VIEW_ZOOMOUT,TBBS_BUTTON, 3);
	m_wndToolBar.SetButtonInfo(5, ID_VIEW_Gride,TBBS_BUTTON, 4);
	m_wndToolBar.SetButtonInfo(6, ID_SEPARATOR,TBBS_SEPARATOR, 6);
	m_wndToolBar.SetButtonInfo(7, ID_APP_ABOUT,TBBS_BUTTON, 5);

	// invalidate the call update handlers before painting
	m_wndToolBar.Invalidate();

}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers