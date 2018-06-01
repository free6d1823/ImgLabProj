#include "stdafx.h"
#include "Common.h"
#include "iOptics.h"
#include "ProcessView.h"
#include "ProcessTool.h"
#include "FecTool.h"
#include "LdcTool.h"

#define LUT_SIZE	32+1
ProcessWrap::ProcessWrap()
{
	cxLut = cyLut = LUT_SIZE;
	lut= (PointDouble*) malloc(cxLut* cyLut* sizeof(PointDouble));
};
ProcessWrap::~ProcessWrap()
{if (lut) free(lut);}



ProcessTool* ProcessTool::CreateProcessTool(CWnd* pOwner, int nType, ImgFile* pImage)
{
	ProcessTool* tool = NULL;
	switch (nType) {
		case IDD_FEC:
			tool = (ProcessTool*) new FecTool();
			tool->Create(IDD_FEC, pOwner);
			tool->SetImgFile(pImage);
			break;
		case IDD_LDC:
			tool = (ProcessTool*) new LdcTool();
			tool->Create(IDD_LDC, pOwner);
			tool->SetImgFile(pImage);
			break;
		default:
			break;
	}
	if (tool) {
		CRect rc;
		POINT pt;
		tool->GetWindowRect(&rc);// in screen
		pt.x = AfxGetApp()->GetProfileInt("ProcessTool", "x", 800);
		pt.y = AfxGetApp()->GetProfileInt("ProcessTool", "y", 600);
		rc.MoveToXY(pt);
//		tool->GetParent()->ScreenToClient(&rc);
		tool->MoveWindow(&rc);
		tool->ShowWindow(SW_SHOW);
	}
	return tool;
}


IMPLEMENT_DYNAMIC(ProcessTool, CDialog)

ProcessTool::ProcessTool(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	m_pBasicView = NULL;
	m_pProcessWrap = NULL;
}

ProcessTool::~ProcessTool()
{

}

void ProcessTool::OnDestroy()
{
	CRect rc;
	GetWindowRect(&rc);// in screen
	AfxGetApp()->WriteProfileInt("ProcessTool", "x", rc.left);
	AfxGetApp()->WriteProfileInt("ProcessTool", "y", rc.top);

	CWnd::OnDestroy();

}
void ProcessTool::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	if(m_pBasicView && ::IsWindow(m_pBasicView->m_hWnd)){
		//save position 
		CRect rc;
		m_pBasicView->GetWindowRect(&rc);// in screen
		AfxGetApp()->WriteProfileInt("ProcessView", "x", rc.left);
		AfxGetApp()->WriteProfileInt("ProcessView", "y", rc.top);

		//parent wnd will destroy the window, don't delete it
		m_pBasicView->DestroyWindow();
		m_pBasicView = NULL;
	}

	delete this;
}
ImgFile* ProcessTool::GetImgFile()
{
	if (m_pBasicView)
		return m_pBasicView->GetImgFile();
	return NULL;
}
void ProcessTool::SetImgFile(ImgFile* pImage)
{
	if (m_pBasicView) {
		CRect rc;
		POINT pt;
		m_pBasicView->GetWindowRect(&rc);// in screen
		pt.x = AfxGetApp()->GetProfileInt("ProcessView", "x", 0);
		pt.y = AfxGetApp()->GetProfileInt("ProcessView", "y", 0);
		rc.MoveToXY(pt);
//		m_pBasicView->GetParent()->ScreenToClient(&rc);
		m_pBasicView->MoveWindow(&rc);//popup window, this is screen coord
		m_pBasicView->ShowWindow(SW_SHOW);
	}
}

BEGIN_MESSAGE_MAP(ProcessTool, CDialog)
	ON_BN_CLICKED(IDCANCEL, &ProcessTool::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &ProcessTool::OnBnClickedOk)
		ON_UPDATE_COMMAND_UI_RANGE(ID_TOOLBOX_BASE, ID_TOOLBOX_LAST, OnUpdateToolbox)
		ON_COMMAND_RANGE(ID_TOOLBOX_BASE, ID_TOOLBOX_LAST,  OnCommandToolbox) 
		ON_WM_DESTROY()
END_MESSAGE_MAP()


float ProcessTool::GetDlgItemFloat(int nID)
{
		float value = 0;
		char buff[256];
		if(GetDlgItemText(nID, buff, sizeof(buff))) {
				value = (float) atof(buff);
		}
		return value;
}

void ProcessTool::SetDlgItemFloat(int nID, float fValue)
{
		char buff[256];
		sprintf_s(buff, sizeof(buff), "%6.2f", fValue);
		SetDlgItemText(nID, buff);
}
void ProcessTool::OnBnClickedCancel()
{
	GetParent()->SendMessage(WM_COMMAND, ID_PROCESS_EXIT,0);
	 DestroyWindow();
}

void ProcessTool::OnBnClickedOk()
{
	//inform parent to read data
	GetParent()->SendMessage(WM_COMMAND, ID_PROCESS_PREVIEW,0);
	
}
void ProcessTool::OnUpdateToolbox(CCmdUI *pCmdUI)
{
	switch(pCmdUI->m_nID) {
		case ID_TOOLBOX_TOOGLE: //hide/show this dlg
			if(IsWindowVisible())
				pCmdUI->SetText("Hide tool setting box");
			else
				pCmdUI->SetText("Show tool setting box");
			break;
		case ID_TOOLBOX_APPLY:
				pCmdUI->SetText("Update image");
			break;
		case ID_TOOLBOX_FEC: //toggle tool
				pCmdUI->SetText("FEC"); break;
		case ID_TOOLBOX_LDC:
				pCmdUI->SetText("LDC"); break;
		case ID_TOOLBOX_3:
		case ID_TOOLBOX_4:
		case ID_TOOLBOX_5:
		case ID_TOOLBOX_6:
		case ID_TOOLBOX_7:
				pCmdUI->SetText("TBD"); break;
		default:
			if(m_pBasicView)
					m_pBasicView->OnUpdateToolbox(pCmdUI);

			break;
	}

}
void ProcessTool::OnCommandToolbox(UINT nCmd)
{
	switch(nCmd) {
		case ID_TOOLBOX_TOOGLE: //hide/show this dlg
			ShowWindow(IsWindowVisible()?SW_HIDE:SW_SHOW);
			break;
		case ID_TOOLBOX_APPLY:
			GetParent()->SendMessage(WM_COMMAND, ID_PROCESS_APPLY,0);
			break;
		case ID_TOOLBOX_SAVE: //save parameter
			OnSaveParameter();
			break;
		case ID_TOOLBOX_LOAD: //load parameter
			OnLoadParameter();
			break;
		case ID_TOOLBOX_FEC: //toggle tool
			GetParent()->SendMessage(WM_COMMAND, ID_PROCESS_FEC,0);break;
		case ID_TOOLBOX_LDC:
			GetParent()->SendMessage(WM_COMMAND, ID_PROCESS_LDC,0);break;
		case ID_TOOLBOX_3:
			GetParent()->SendMessage(WM_COMMAND, ID_PROCESS_3,0);break;
		case ID_TOOLBOX_4:
			GetParent()->SendMessage(WM_COMMAND, ID_PROCESS_4,0);break;
		case ID_TOOLBOX_5:
			GetParent()->SendMessage(WM_COMMAND, ID_PROCESS_5,0);break;
		case ID_TOOLBOX_6:
			GetParent()->SendMessage(WM_COMMAND, ID_PROCESS_6,0);break;
		case ID_TOOLBOX_7:
			GetParent()->SendMessage(WM_COMMAND, ID_PROCESS_7,0);break;
			break;
		default:
			//process ID_TOOLBOX_ZOOMIN, ID_TOOLBOX_ZOOMOUT, ID_TOOLBOX_GRIDE
			if(m_pBasicView)
					m_pBasicView->OnCommandToolbox(nCmd);
		break;
	}

}


void ProcessTool::OnSaveParameter()
{
	CString strFileName = AfxGetApp()->GetProfileString("ProcessTool", "LoadParameterFile", "process.ini");
	CString strFilter = "*.ini|*.ini||";
	CFileDialog aFileDialog( FALSE, NULL, strFileName,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
	if( IDOK == aFileDialog.DoModal()) {
		UpdateData(TRUE);
		m_pBasicView->SetParam(m_pProcessWrap); //update data to view
		if( m_pProcessWrap->SaveParameter(aFileDialog.GetPathName())  == TRUE)
			AfxGetApp()->WriteProfileString("ProcessTool", "LoadParameterFile", aFileDialog.GetPathName());

	}
}

void ProcessTool::OnLoadParameter()
{
	CString strFileName = AfxGetApp()->GetProfileString("ProcessTool", "LoadParameterFile", "process.ini");
	CString strFilter = "*.ini|*.ini||";
	CFileDialog aFileDialog( TRUE, NULL, strFileName,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
	if( IDOK == aFileDialog.DoModal()){
		if( m_pProcessWrap->SaveParameter(aFileDialog.GetPathName())== TRUE) {
			AfxGetApp()->WriteProfileString("ProcessTool", "LoadParameterFile", aFileDialog.GetPathName());
			//update to UI
			UpdateData(FALSE);
		}
	}
}
