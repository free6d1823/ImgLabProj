
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "iStitch.h"

#include "MainFrm.h"
#include "MainDoc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CMainFrame::OnUpdateFileSave)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_POS,
	ID_INDICATOR_ZOOM,
	ID_INDICATOR_SIZE,
};

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
	m_wndToolBar.SetButtons(NULL, 12);
	m_wndToolBar.SetButtonInfo(0, ID_FILE_NEW,TBBS_BUTTON, 0);
	m_wndToolBar.SetButtonInfo(1, ID_FILE_OPEN,TBBS_BUTTON, 1);
	m_wndToolBar.SetButtonInfo(2, ID_FILE_SAVE,TBBS_BUTTON, 2);
	m_wndToolBar.SetButtonInfo(3, ID_FILE_EXPORT,TBBS_BUTTON, 3);
	m_wndToolBar.SetButtonInfo(4, ID_SEPARATOR,TBBS_SEPARATOR, 12);
	m_wndToolBar.SetButtonInfo(5, ID_VIEW_GRIDE,TBBS_BUTTON, 4);
	m_wndToolBar.SetButtonInfo(6, ID_VIEW_ZOOMIN,TBBS_BUTTON, 5);
	m_wndToolBar.SetButtonInfo(7, ID_VIEW_ZOOMOUT,TBBS_BUTTON, 6);
	m_wndToolBar.SetButtonInfo(8, ID_VIEW_CONTROLLER,TBBS_BUTTON, 7);
	m_wndToolBar.SetButtonInfo(9, ID_SEPARATOR,TBBS_SEPARATOR, 12);
	m_wndToolBar.SetButtonInfo(10, ID_FILE_PROPERTIES,TBBS_BUTTON, 8);
	m_wndToolBar.SetButtonInfo(11, ID_APP_ABOUT,TBBS_BUTTON, 9);

	// invalidate the call update handlers before painting
	m_wndToolBar.Invalidate();

}
void CMainFrame::OnUpdateFileSave(CCmdUI *pCmdUI)
{
		CMainDoc* pDoc = (CMainDoc*) GetActiveDocument();
		pCmdUI->Enable(pDoc->GetProjectSetting()->nImages > 0);
}
