// ontrolBar.cpp : implementation file
//

#include "stdafx.h"
#include "iStitch.h"
#include "controlBar.h"
#include "WorkView.h"
#define PI 3.1415967
// ControlBar dialog

IMPLEMENT_DYNCREATE(ControlBar, CDialog)
PProjectSetting	ControlBar::GetProjectSetting()
{
		if(!m_pParent)
				return NULL;
		CMainDoc* pDoc = m_pParent->GetDocument();
		if(pDoc) {
				return pDoc->GetProjectSetting();
		}
		return NULL;
}
CMainDoc* ControlBar::GetDocument() { return m_pParent->GetDocument();}

ControlBar::ControlBar(CWnd* pParent /*=NULL*/)
	: CDialog(ControlBar::IDD, pParent)
{
		m_pParent = NULL;
}

ControlBar::~ControlBar()
{
}

void ControlBar::DoDataExchange(CDataExchange* pDX)
{
		CDialog::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_COMBO1, m_ImageList);
		DDX_Control(pDX, IDC_X, m_PosX);
		DDX_Control(pDX, IDC_Y, m_PosY);
		DDX_Control(pDX, IDC_SCALE, m_Scale);
		DDX_Control(pDX, IDC_ROTATE, m_Rotate);
}

void	ControlBar::UpdateUI(int nCurSel, BOOL bReset)
{
		PProjectSetting pPs = GetProjectSetting();
		char buff[256];
		int i;
		if(bReset) {
				m_ImageList.ResetContent();
				for (i=0; i< pPs->nImages; i++) {
						_itoa_s(i+1, buff, sizeof(buff), 10);
						m_ImageList.AddString(buff);
				}	

		}


		if(nCurSel >= 0 &&nCurSel< pPs->nImages) {
				m_ImageList.SetCurSel(nCurSel);	
				SetDlgItemInt(IDC_X, pPs->ip[nCurSel].pos.x);
				SetDlgItemInt(IDC_Y, pPs->ip[nCurSel].pos.y);
				sprintf_s(buff, sizeof(buff), "%4.2f", pPs->ip[nCurSel].rotate*180/PI);
				SetDlgItemText(IDC_ROTATE, buff);
				sprintf_s(buff, sizeof(buff), "%4.2f", pPs->ip[nCurSel].scale);
				SetDlgItemText(IDC_SCALE, buff);
		} else {
				m_ImageList.SetCurSel(-1);	
		}
}

BEGIN_MESSAGE_MAP(ControlBar, CDialog)
		ON_WM_CREATE()
		ON_CBN_SELCHANGE(IDC_COMBO1, &ControlBar::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


int ControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
		if (CDialog::OnCreate(lpCreateStruct) == -1)
				return -1;
		m_pParent = (WorkView*) CWnd::FromHandle(lpCreateStruct->hwndParent);

		return 0;
}

BOOL ControlBar::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
		// TODO: Add your specialized code here and/or call the base class

		return CDialog::OnNotify(wParam, lParam, pResult);
}

BOOL ControlBar::OnCommand(WPARAM wParam, LPARAM lParam)
{
		int cmd = (int) wParam;
		if(::IsWindowEnabled(m_ImageList.m_hWnd)){
				if(cmd >= IDC_CONTROL_BEGIN && cmd < IDC_CONTROL_END ) {
						int nSel = m_ImageList.GetCurSel();
						CMainDoc* pDoc = GetDocument();		
						if (nSel <0 && nSel >= pDoc->GetProjectSetting()->nImages)
								return FALSE;
						switch (cmd) {
								case IDC_UP1: pDoc->IncreaseImagePosY(nSel, -1); break;
								case IDC_UP2: pDoc->IncreaseImagePosY(nSel, -10); break;
								case IDC_DOWN1: pDoc->IncreaseImagePosY(nSel, 1); break;
								case IDC_DOWN2: pDoc->IncreaseImagePosY(nSel, 10); break;
								case IDC_LEFT1: pDoc->IncreaseImagePosX(nSel, -1); break;
								case IDC_LEFT2: pDoc->IncreaseImagePosX(nSel, -10); break;
								case IDC_RIGHT1: pDoc->IncreaseImagePosX(nSel, 1); break;
								case IDC_RIGHT2: pDoc->IncreaseImagePosX(nSel, 10); break;
								case IDC_SCALE1: pDoc->IncreaseImageScale(nSel, 0.01f); break;
								case IDC_SCALE2: pDoc->IncreaseImageScale(nSel, -0.01f); break;
								case IDC_ROTATE1: pDoc->IncreaseImageRotate(nSel, 0.1f); break;
								case IDC_ROTATE2: pDoc->IncreaseImageRotate(nSel, -0.1f); break;
								default:
										return FALSE;
						}
						//update UI
						UpdateUI(nSel, FALSE);
						//update WorkView
						m_pParent->InvalidateRect(NULL, FALSE);
				}
		}
		return CDialog::OnCommand(wParam, lParam);
}

void ControlBar::OnCbnSelchangeCombo1()
{
		UpdateUI( m_ImageList.GetCurSel(), FALSE);
		m_pParent->SetCurrentSel(m_ImageList.GetCurSel());
}
