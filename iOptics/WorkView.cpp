// WorkView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "MainDoc.h"
#include "BasicView.h"
#include "WorkView.h"
#include "MainView.h"
#include "LdcView.h"
#include "FecView.h"

#define BACKGND_COLOR	RGB(180,180,180)
IMPLEMENT_DYNAMIC(WorkView, BasicView)

ImgFile* WorkView::GetImage()
{
	CMainDoc* pDoc = GetDocument();
	if (pDoc) {
		return pDoc->GetImage();
	}
	return NULL;
}
CMainDoc* WorkView::GetDocument()
{
	if (m_parent)
		return m_parent->GetDocument();
	return NULL;
}

WorkView::WorkView() :m_gride(RGB(0,255,0))
{

	m_bShowGride = FALSE;
	m_bShowMesh = FALSE;
	m_parent = NULL;
	m_ptCursorPos.x = m_ptCursorPos.y = 0;

	m_pProcessTool = NULL;
	m_pToolGride = NULL;
}

WorkView::~WorkView()
{
	if(m_pToolGride) {
		delete m_pToolGride;
		m_pToolGride = NULL;
	}
}

BEGIN_MESSAGE_MAP(WorkView, CWnd)
		ON_WM_PAINT()
		ON_WM_SIZE()
		ON_COMMAND(ID_VIEW_Gride, &WorkView::OnViewGride)
		ON_UPDATE_COMMAND_UI(ID_VIEW_Gride, &WorkView::OnUpdateViewGride)
		ON_COMMAND(ID_VIEW_Mesh, &WorkView::OnViewMesh)
		ON_UPDATE_COMMAND_UI(ID_VIEW_Mesh, &WorkView::OnUpdateViewMesh)
		ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
		ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMIN, OnUpdateViewZoomin)
		ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
		ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT, OnUpdateViewZoomout)
		ON_UPDATE_COMMAND_UI(ID_INDICATOR_POS, OnUpdateIndicatePos)
		ON_UPDATE_COMMAND_UI(ID_INDICATOR_COLOR, OnUpdateIndicateColor)
		ON_UPDATE_COMMAND_UI(ID_INDICATOR_ZOOM, OnUpdateIndicateZoomFactor)

		ON_WM_MOUSEMOVE()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_COMMAND(ID_PROCESS_FEC, &WorkView::OnProcessFec)
		ON_UPDATE_COMMAND_UI(ID_PROCESS_FEC, &WorkView::OnUpdateProcessFec)
		ON_COMMAND(ID_PROCESS_LDC, &WorkView::OnProcessLdc)
		ON_UPDATE_COMMAND_UI(ID_PROCESS_LDC, &WorkView::OnUpdateProcessLdc)
		ON_COMMAND(ID_PROCESS_HOMO, &WorkView::OnProcessHomo)
		ON_UPDATE_COMMAND_UI(ID_PROCESS_HOMO, &WorkView::OnUpdateProcessHomo)
		ON_COMMAND(ID_PROCESS_EXIT, &WorkView::OnProcessExit)
		ON_COMMAND(ID_PROCESS_PREVIEW, &WorkView::OnProcessPreview)
		ON_COMMAND(ID_TOOLBOX_APPLY, &WorkView::OnToolboxApply)
		ON_UPDATE_COMMAND_UI_RANGE(ID_TOOLBOX_BASE, ID_TOOLBOX_LAST, OnUpdateToolbox)
		ON_COMMAND_RANGE(ID_TOOLBOX_BASE, ID_TOOLBOX_LAST,  OnCommandToolbox) 
		ON_WM_DESTROY()
END_MESSAGE_MAP()

// WorkView message handlers
BOOL	WorkView::Create(CWnd* pParent)
{
	m_parent = (CMainView*) pParent;
	return BasicView::Create("WorkView", pParent, FALSE);
}
void WorkView::OnUpdateIndicatePos(CCmdUI *pCmdUI)
{
	CString szText;
	szText.Format("(%d,%d)", m_ptCursorPos.x, m_ptCursorPos.y);
	pCmdUI->SetText(szText);
}
void WorkView::OnUpdateIndicateColor(CCmdUI *pCmdUI)
{
	CString szText = "";
	ImgFile* pImg; 
	if( (pImg = GetImage())){
		if (m_ptCursorPos.x >=0 && m_ptCursorPos.y >= 0 &&
				m_ptCursorPos.x < (LONG) pImg->Width() &&
				m_ptCursorPos.y < (LONG) pImg->Height())  {
			COLORREF cr;
			pImg->GetPixelColor(m_ptCursorPos, cr);
			szText.Format("(%3d,%3d,%3d)", GetRValue(cr), GetGValue(cr), GetBValue(cr));
		}
	}
	pCmdUI->SetText(szText);
}
void WorkView::OnUpdateIndicateZoomFactor(CCmdUI *pCmdUI)
{
	CString szText;
	int n = ZOOM_FACTOR[m_nZoomFactor];
	float m= ((float)n)/100;
	szText.Format("%4.2fX", m);
	pCmdUI->SetText(szText);
}

// WorkView message handlers
void WorkView::OnSize(UINT nType, int cx, int cy)
{
	BasicView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	m_gride.SetImageArea(m_rcImage);
	m_gride.SetViewPort(m_rcPort);
	if(m_pToolGride) {
		m_pToolGride->SetImageArea(m_rcImage);
		m_pToolGride->SetViewPort(m_rcPort);
	}
	m_mesh.SetLocationOnCanvas(m_rcImage);

}
BOOL WorkView::SetScrollPos(int nX, int nY)
{
	BOOL bOK = FALSE;

	if (nX >= 0){// && nX <= m_rcImage.right - m_rcPort.right + m_rcPort.left) {
		m_rcPort.right += nX - m_rcPort.left;
		m_rcPort.left = nX;
		bOK = TRUE;
	}
	if (nY >= 0){ // && nY <= m_rcImage.bottom - m_rcPort.bottom + m_rcPort.top) {
		m_rcPort.bottom += nY - m_rcPort.top;
		m_rcPort.top = nY;
		bOK = TRUE;
	}
	if (bOK) {
		m_gride.SetViewPort(m_rcPort);
		if(m_pToolGride) {
				m_pToolGride->SetViewPort(m_rcPort);
		}
		InvalidateRect(NULL, FALSE);
	}
	return bOK;
}
void WorkView::OnViewGride()
{
	// TODO: Add your command handler code here
	m_bShowGride = 1 - m_bShowGride;
	InvalidateRect(NULL, FALSE);
}

void WorkView::OnUpdateViewGride(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetDocument() != NULL);
}
void WorkView::OnViewMesh()
{
	m_bShowMesh = 1 - m_bShowMesh;
	InvalidateRect(NULL, FALSE);
}
void WorkView::OnUpdateViewMesh(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetDocument() != NULL);
}
BOOL WorkView::OnCommand(WPARAM wParam, LPARAM lParam)
{
		TRACE("WorkView receive command %x\n", wParam);
		/*
		if(m_pProcessTool) {
				if(LOWORD(wParam) >= ID_TOOLBOX_BASE && LOWORD(wParam) < ID_TOOLBOX_LAST){
						if (::IsWindow(m_pProcessTool->m_hWnd)) {
								return m_pProcessTool->SendMessage(WM_COMMAND, wParam, lParam);
						}
				}
		}*/
		return BasicView::OnCommand(wParam, lParam);
}
//doc has updated image, opened image
void WorkView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	SetImgFile(GetImage());//set doc image to BasicView
	//set image file to other window
	if(m_pProcessTool){
		m_pProcessTool->SetImgFile(GetImage());
	}
	CalculateRects();
	UpdateScrollbarPosition();
	GrideParam gp;
	gp.nInterval = 2;
	gp.nDivider = 8;
	m_gride.SetParam(&gp);
	m_gride.SetImageArea(m_rcImage);
	m_gride.SetViewPort(m_rcPort);
	m_ptCursorPos.x = m_ptCursorPos.y =  -1;

	m_mesh.SetTable(NULL, 9, 9);
	m_mesh.SetLocationOnCanvas(m_rcImage);

	if(m_pToolGride) {
				m_pToolGride->SetParam(m_pProcessTool->GetParam());
				m_pToolGride->SetImageArea(m_rcImage);
				m_pToolGride->SetViewPort(m_rcPort);
	}
	InvalidateRect(NULL, FALSE);
}
void WorkView::UpdateScrollbarPosition()
{
	if(m_rcPort.right - m_rcPort.left < m_szCanvas.cx)
		m_parent->UpdateScrollBar(SB_HORZ, m_rcPort.left, m_rcPort.right - m_rcPort.left, m_szCanvas.cx );
	else
		m_parent->UpdateScrollBar(SB_HORZ, -1, 0,0);
	if(m_rcPort.bottom - m_rcPort.top < m_szCanvas.cy)
		m_parent->UpdateScrollBar(SB_VERT, m_rcPort.top, m_rcPort.bottom - m_rcPort.top, m_szCanvas.cy );
	else
		m_parent->UpdateScrollBar(SB_VERT, -1, 0,0);
}

void WorkView::OnPaint()
{
	CPaintDC dc(this);  
	CDC* pDC = &dc;
	DrawImage(pDC);

	if(m_bShowGride) {
		m_gride.Draw(pDC);
	}
	if(m_pToolGride)
		m_pToolGride->Draw(pDC);

	if(m_bShowMesh)
		m_mesh.Draw(pDC, &m_rcPort, TRUE);
}

void WorkView::OnViewZoomin() 
{
	BasicView::OnViewZoomin();
	UpdateScrollbarPosition();

	m_gride.SetImageArea(m_rcImage);
	m_gride.SetViewPort(m_rcPort);
	if(m_pToolGride){
		m_pToolGride->SetImageArea(m_rcImage);
		m_pToolGride->SetViewPort(m_rcPort);
	}

	m_mesh.SetLocationOnCanvas(m_rcImage);
	InvalidateRect(NULL);

}

void WorkView::OnUpdateViewZoomin(CCmdUI* pCmdUI) 
{
		BasicView::OnUpdateViewZoomin(pCmdUI);	
}

void WorkView::OnViewZoomout() 
{
	BasicView::OnViewZoomout();
	UpdateScrollbarPosition();
	m_gride.SetImageArea(m_rcImage);
	m_gride.SetViewPort(m_rcPort);
	if(m_pToolGride) {
		m_pToolGride->SetImageArea(m_rcImage);
		m_pToolGride->SetViewPort(m_rcPort);
	}
	m_mesh.SetLocationOnCanvas(m_rcImage);
	InvalidateRect(NULL);
}
void WorkView::OnUpdateViewZoomout(CCmdUI* pCmdUI) 
{
	BasicView::OnUpdateViewZoomout(pCmdUI);	
}


void WorkView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//canvas coord
	POINT ptCan;
	ptCan.x = (point.x + m_rcPort.left);
	ptCan.y = (point.y + m_rcPort.top);
	m_ptCursorPos.x = (ptCan.x - m_rcImage.left)* 100 / ZOOM_FACTOR[m_nZoomFactor];
	m_ptCursorPos.y = (ptCan.y - m_rcImage.top)* 100 / ZOOM_FACTOR[m_nZoomFactor];

	if(m_pToolGride) {
		m_pToolGride->OnMouseMove(this, m_ptCursorPos);
	}
	if(m_bShowMesh) {
		m_mesh.OnMouseMove(this, ptCan);
	}
	CWnd::OnMouseMove(nFlags, point);
}

void WorkView::OnLButtonDown(UINT nFlags, CPoint point)
{
	POINT ptCan;
	ptCan.x = (point.x + m_rcPort.left);
	ptCan.y = (point.y + m_rcPort.top);
	m_ptCursorPos.x = (ptCan.x - m_rcImage.left)* 100 / ZOOM_FACTOR[m_nZoomFactor];
	m_ptCursorPos.y = (ptCan.y - m_rcImage.top)* 100 / ZOOM_FACTOR[m_nZoomFactor];
	if(m_bShowMesh) {
		m_mesh.OnLButtonDown(this, ptCan);
	}
	if(m_pToolGride) {
		m_pToolGride->OnLButtonDown(this, m_ptCursorPos);
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void WorkView::OnLButtonUp(UINT nFlags, CPoint point)
{
	POINT ptCan;
	ptCan.x = (point.x + m_rcPort.left);
	ptCan.y = (point.y + m_rcPort.top);
	if(m_pToolGride) {
		m_pToolGride->OnLButtonUp(this, m_ptCursorPos);
	}
	m_mesh.OnLButtonUp(this, ptCan);
	CWnd::OnLButtonUp(nFlags, point);
}

void WorkView::OnProcessLdc()
{
	if(m_pProcessTool && m_pProcessTool->GetProcessToolID() != IDD_LDC ) {
		if (m_pProcessTool) {
			m_pProcessTool->DestroyWindow();
			m_pProcessTool = NULL;
		} 
	}
	if(!m_pProcessTool) {
		m_pProcessTool =  ProcessTool::CreateProcessTool(this, IDD_LDC, GetImage());
		//create 
	}

	m_pProcessTool->ShowWindow(SW_SHOW);
}

void WorkView::OnUpdateProcessLdc(CCmdUI *pCmdUI)
{
	if ( GetImage()){
		pCmdUI->Enable(TRUE);
		if(m_pProcessTool) 
			pCmdUI->SetRadio(m_pProcessTool->GetProcessToolID() == IDD_LDC);
	} else {
		pCmdUI->Enable(FALSE);
	}
}

void WorkView::OnProcessFec()
{
	if(m_pProcessTool && m_pProcessTool->GetProcessToolID() != IDD_FEC ) {
		if (m_pProcessTool) {
			m_pProcessTool->DestroyWindow();
			m_pProcessTool = NULL;
		} 
	}
	if(!m_pProcessTool) {
		m_pProcessTool =  ProcessTool::CreateProcessTool(this, IDD_FEC, GetImage());
		//create 
	}
	
	m_pProcessTool->ShowWindow(SW_SHOW);
}

void WorkView::OnUpdateProcessFec(CCmdUI *pCmdUI)
{
	if ( GetImage()){
		pCmdUI->Enable(TRUE);
		if(m_pProcessTool) 
			pCmdUI->SetRadio(m_pProcessTool->GetProcessToolID() == IDD_FEC);
	} else {
		pCmdUI->Enable(FALSE);
	}
}


void WorkView::OnProcessHomo()
{
	if(m_pProcessTool && m_pProcessTool->GetProcessToolID() != IDD_HOMO) {
		if (m_pProcessTool) {
			m_pProcessTool->DestroyWindow();
			m_pProcessTool = NULL;

			if(m_pToolGride) {
				delete m_pToolGride;
				m_pToolGride = NULL;
			}
		} 
	}
	if(!m_pProcessTool) {
		m_pProcessTool =  ProcessTool::CreateProcessTool(this, IDD_HOMO, GetImage());
	}

	m_pProcessTool->ShowWindow(SW_SHOW);

	if(!m_pToolGride) {
		m_pToolGride = (ToolGride*) new HomoGride(this);
		m_pToolGride->SetParam(m_pProcessTool->GetParam());
		m_pToolGride->SetImageArea(m_rcImage);
		m_pToolGride->SetViewPort(m_rcPort);
		InvalidateRect(NULL, FALSE);
	}
}
/*!< Set current tool settings to WorkView. The content is defined dependently by current ProcessTool */
void	WorkView::SetToolParam(void* pParam)
{
	if(m_pToolGride){
			m_pToolGride->SetParam(pParam);
	}
}
/*!< Get current tool settings on WorkView. The content is defined dependently by current ProcessTool */
void	WorkView::GetToolParam(void* pParam)
{
	if(m_pToolGride){
			m_pToolGride->GetParam(pParam);
	}
}
void WorkView::OnUpdateProcessHomo(CCmdUI *pCmdUI)
{
	if ( GetImage()){
		pCmdUI->Enable(TRUE);
		if(m_pProcessTool) 
			pCmdUI->SetRadio(m_pProcessTool->GetProcessToolID() == IDD_HOMO);
	} else {
		pCmdUI->Enable(FALSE);
	}
}

void WorkView::OnProcessExit()
{
	if(m_pProcessTool) 
		m_pProcessTool = NULL; //process tool will delete by itself
	if (m_pToolGride) {
		delete m_pToolGride;
		m_pToolGride = NULL;
	}
}
//process changed, but not apply yet
void WorkView::OnProcessPreview()
{
	ProcessWrap* pCap =   m_pProcessTool->GetParam();
	//update gride only
	if(m_pToolGride && m_pProcessTool) {
		m_pToolGride->SetParam(pCap);
	}

	m_mesh.SetTable(pCap->GetLutTable(), pCap->LutColumns(), pCap->LutRows());
	m_mesh.SetLocationOnCanvas(m_rcImage);
	InvalidateRect(NULL, FALSE);	
}
//Apply to new image
void WorkView::OnToolboxApply()
{
	ImgFile* pImg = NULL;
	if(m_pProcessTool)
		pImg = m_pProcessTool->GetImgFile();
	GetDocument()->UpdateImage(pImg);
}
void WorkView::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: Add your message handler code here
}
//pass ToolBox command to ProcessTool and ProcessView
void WorkView::OnUpdateToolbox(CCmdUI *pCmdUI)
{
		if(m_pProcessTool) 
				m_pProcessTool->OnUpdateToolbox(pCmdUI);
}
void WorkView::OnCommandToolbox(UINT nCmd)
{
		if(m_pProcessTool) 
				m_pProcessTool->OnCommandToolbox(nCmd);
}